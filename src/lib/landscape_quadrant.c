/**
 * This file is part of Walker, a game.

Copyright (c) 2023, Kees-Jan Hermans <kees.jan.hermans@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the organization nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL Kees-Jan Hermans BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 *
 * \file
 * \brief
 */

#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <fcntl.h>

#include "walker.h"

/**
 * Optimizes the quadrant, that is: 
 */
void landscape_quadrant_optimize
  (wlandscape_t* ls, wquadrant_t* q)
{
  int qtx = q->qx * WQUADRANT_DIMENSION;
  int qtz = q->qz * WQUADRANT_DIMENSION;
  
  for (unsigned i=0; i < WQUADRANT_DIMENSION; i++) { 
    for (unsigned j=0; j < WQUADRANT_DIMENSION; j++) {
      wtile_t* tiles[9] = {
        0, 0, 0, 0, &(q->tiles[ i ][ j ]), 0, 0, 0, 0
      };
      if (i && i < WQUADRANT_DIMENSION-1) { 
        if (j && j < WQUADRANT_DIMENSION-1) { 
          tiles[ 0 ] = &(q->tiles[ i-1 ][ j-1 ]);
          tiles[ 1 ] = &(q->tiles[ i   ][ j-1 ]);
          tiles[ 2 ] = &(q->tiles[ i+1 ][ j-1 ]);
          tiles[ 3 ] = &(q->tiles[ i-1 ][ j   ]);
          tiles[ 5 ] = &(q->tiles[ i+1 ][ j   ]);
          tiles[ 6 ] = &(q->tiles[ i-1 ][ j+1 ]);
          tiles[ 7 ] = &(q->tiles[ i   ][ j+1 ]);
          tiles[ 8 ] = &(q->tiles[ i+1 ][ j+1 ]);
        }
      }
      landscape_tile_optimize(ls, qtx + i, qtz + j, tiles);
    }
  }
}

void landscape_quadrant_get_new
  (wlandscape_t* ls, wquadrant_t* q, int qx, int qz)
{
  q->qx = qx;
  q->qz = qz;
  if (walker_db_quadrant_retrieve(ls->walker, q) == 0) {
#ifdef _DEBUG
    fprintf(stderr, "quadrant %d, %d cached in db\n", qx, qz);
#endif
    return;
  }
#ifdef _DEBUG
  fprintf(stderr, "Initializing quadrant %d, %d\n", qx, qz);
#endif
  for (int dtx = 0; dtx < WQUADRANT_DIMENSION; dtx++) {
    for (int dtz = 0; dtz < WQUADRANT_DIMENSION; dtz++) {
      int tx = (q->qx * WQUADRANT_DIMENSION) + dtx;
      int tz = (q->qz * WQUADRANT_DIMENSION) + dtz;
      landscape_tile_get_new(tx, tz, &(q->tiles[ dtx ][ dtz ]));
    }
  }
  landscape_quadrant_optimize(ls, q);
  walker_db_quadrant_store(ls->walker, q);
}

void landscape_quadrant_get
  (wlandscape_t* ls, int qx, int qz, wquadrant_t* q)
{
  if (qx >= ls->cache.mem.qp.x && qx < ls->cache.mem.qp.x + 3
      && qz >= ls->cache.mem.qp.z && qz < ls->cache.mem.qp.z + 3)
  {
    *q = ls->cache.mem.quadrant[ qx - ls->cache.mem.qp.x ]
                               [ qz - ls->cache.mem.qp.z ];
    return;
  }
  q->qx = qx;
  q->qz = qz;
  return landscape_quadrant_get_new(ls, q, qx, qz);
}

void landscape_cache_update
  (wlandscape_t* ls, int qx, int qz)
{
  int cached[ 3 ][ 3 ] = { 0 };
  wquadrant_t swap[ 3 ][ 3 ] = { 0 };

  if (qx == ls->cache.mem.qp.x && qz == ls->cache.mem.qp.z) {
    return;
  }
#ifdef _DEBUG
  fprintf(stderr, "Adjusting cache to %d, %d\n", qx, qz);
#endif
  for (int i=0; i < 3; i++) {
    for (int j=0; j < 3; j++) {
      walker_db_quadrant_store(ls->walker, &(ls->cache.mem.quadrant[ i ][ j ]));
      if (qx+i >= ls->cache.mem.qp.x && qx+i < ls->cache.mem.qp.x+3
          && qz+j >= ls->cache.mem.qp.z && qz+j < ls->cache.mem.qp.z+3)
      {
        swap[ i ][ j ] =
          ls->cache.mem.quadrant[ (qx+i) - ls->cache.mem.qp.x ]
                                [ (qz+j) - ls->cache.mem.qp.z ];
        cached[ i ][ j ] = 1;
#ifdef _DEBUG
        fprintf(stderr,
           "quadrant %d, %d moved to %d, %d\n"
           , (qx+i) - ls->cache.mem.qp.x
           , (qz+j) - ls->cache.mem.qp.z
           , i
           , j
        );
#endif
      }
    }
  }
  for (int i=0; i < 3; i++) {
    for (int j=0; j < 3; j++) {
      if (cached[ i ][ j ] == 0) {
#ifdef _DEBUG
        fprintf(stderr, "quadrant %d, %d (%d, %d) is new\n", i, j, qx+i, qz+j);
#endif
        landscape_quadrant_get_new(ls, &(swap[ i ][ j ]), qx+i, qz+j);
      }
    }
  }
  for (int i=0; i < 3; i++) {
    for (int j=0; j < 3; j++) {
      ls->cache.mem.quadrant[ i ][ j ] = swap[ i ][ j ];
    }
  }
  ls->cache.mem.qp.x = qx;
  ls->cache.mem.qp.z = qz;

#ifdef _DEBUG
  for (int i=0; i < 3; i++) {
    for (int j=0; j < 3; j++) {
      fprintf(stderr, "CACHE %d, %d => %d, %d\n"
        , i
        , j
        , ls->cache.mem.quadrant[ i ][ j ].qx
        , ls->cache.mem.quadrant[ i ][ j ].qz
      );
    }
  }
#endif
}

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

#include "walker.h"

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

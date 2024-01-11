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

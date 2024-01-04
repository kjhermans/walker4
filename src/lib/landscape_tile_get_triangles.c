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
 * Eight triangles, each of three points in space, each of three coordinates.
 */
void landscape_tile_get_triangles
  (wlandscape_t* ls, int tx, int tz, int tr[ 8 ][ 3 ][ 3 ])
{
  wtile_t t = { 0 };
  int e[ 9 ];
  int ce[ 9 ];

  landscape_tile_get(ls, tx, tz, &t);
  if (t.elevation[ 0 ]) {
    landscape_tile_get_triangles_optimized(&t, tr);
    return;
  }
  e[ 4 ] = t.elevation[ 4 ];
  landscape_tile_get(ls, tx-1, tz-1, &t);
  e[ 0 ] = t.elevation[ 4 ];
  landscape_tile_get(ls, tx, tz-1, &t);
  e[ 1 ] = t.elevation[ 4 ];
  landscape_tile_get(ls, tx+1, tz-1, &t);
  e[ 2 ] = t.elevation[ 4 ];
  landscape_tile_get(ls, tx-1, tz, &t);
  e[ 3 ] = t.elevation[ 4 ];
  landscape_tile_get(ls, tx+1, tz, &t);
  e[ 5 ] = t.elevation[ 4 ];
  landscape_tile_get(ls, tx-1, tz+1, &t);
  e[ 6 ] = t.elevation[ 4 ];
  landscape_tile_get(ls, tx, tz+1, &t);
  e[ 7 ] = t.elevation[ 4 ];
  landscape_tile_get(ls, tx+1, tz+1, &t);
  e[ 8 ] = t.elevation[ 4 ];

  ce[ 0 ] = (e[ 0 ] + e[ 1 ] + e[ 3 ] + e[ 4 ]) / 4;
  ce[ 1 ] = (e[ 1 ] + e[ 4 ]) / 2;
  ce[ 2 ] = (e[ 1 ] + e[ 2 ] + e[ 4 ] + e[ 5 ]) / 4;
  ce[ 3 ] = (e[ 3 ] + e[ 4 ]) / 2;
  ce[ 4 ] =  e[ 4 ];
  ce[ 5 ] = (e[ 5 ] + e[ 4 ]) / 2;
  ce[ 6 ] = (e[ 3 ] + e[ 4 ] + e[ 6 ] + e[ 7 ]) / 4;
  ce[ 7 ] = (e[ 4 ] + e[ 7 ]) / 2;
  ce[ 8 ] = (e[ 4 ] + e[ 5 ] + e[ 7 ] + e[ 8 ]) / 4;

  tr[ 0 ][ 0 ][ 0 ] = WTILESIZE / 2;
  tr[ 0 ][ 0 ][ 1 ] = ce[ 4 ];
  tr[ 0 ][ 0 ][ 2 ] = WTILESIZE / 2;
  tr[ 0 ][ 1 ][ 0 ] = 0;
  tr[ 0 ][ 1 ][ 1 ] = ce[ 0 ];
  tr[ 0 ][ 1 ][ 2 ] = 0;
  tr[ 0 ][ 2 ][ 0 ] = WTILESIZE / 2;
  tr[ 0 ][ 2 ][ 1 ] = ce[ 1 ];
  tr[ 0 ][ 2 ][ 2 ] = 0;

  tr[ 1 ][ 0 ][ 0 ] = WTILESIZE / 2;
  tr[ 1 ][ 0 ][ 1 ] = ce[ 4 ];
  tr[ 1 ][ 0 ][ 2 ] = WTILESIZE / 2;
  tr[ 1 ][ 1 ][ 0 ] = WTILESIZE / 2;
  tr[ 1 ][ 1 ][ 1 ] = ce[ 1 ];
  tr[ 1 ][ 1 ][ 2 ] = 0;
  tr[ 1 ][ 2 ][ 0 ] = WTILESIZE;
  tr[ 1 ][ 2 ][ 1 ] = ce[ 2 ];
  tr[ 1 ][ 2 ][ 2 ] = 0;

  tr[ 2 ][ 0 ][ 0 ] = WTILESIZE / 2;
  tr[ 2 ][ 0 ][ 1 ] = ce[ 4 ];
  tr[ 2 ][ 0 ][ 2 ] = WTILESIZE / 2;
  tr[ 2 ][ 1 ][ 0 ] = WTILESIZE;
  tr[ 2 ][ 1 ][ 1 ] = ce[ 2 ];
  tr[ 2 ][ 1 ][ 2 ] = 0;
  tr[ 2 ][ 2 ][ 0 ] = WTILESIZE;
  tr[ 2 ][ 2 ][ 1 ] = ce[ 5 ];
  tr[ 2 ][ 2 ][ 2 ] = WTILESIZE / 2;

  tr[ 3 ][ 0 ][ 0 ] = WTILESIZE / 2;
  tr[ 3 ][ 0 ][ 1 ] = ce[ 4 ];
  tr[ 3 ][ 0 ][ 2 ] = WTILESIZE / 2;
  tr[ 3 ][ 1 ][ 0 ] = WTILESIZE;
  tr[ 3 ][ 1 ][ 1 ] = ce[ 5 ];
  tr[ 3 ][ 1 ][ 2 ] = WTILESIZE / 2;
  tr[ 3 ][ 2 ][ 0 ] = WTILESIZE;
  tr[ 3 ][ 2 ][ 1 ] = ce[ 8 ];
  tr[ 3 ][ 2 ][ 2 ] = WTILESIZE;

  tr[ 4 ][ 0 ][ 0 ] = WTILESIZE / 2;
  tr[ 4 ][ 0 ][ 1 ] = ce[ 4 ];
  tr[ 4 ][ 0 ][ 2 ] = WTILESIZE / 2;
  tr[ 4 ][ 1 ][ 0 ] = WTILESIZE;
  tr[ 4 ][ 1 ][ 1 ] = ce[ 8 ];
  tr[ 4 ][ 1 ][ 2 ] = WTILESIZE;
  tr[ 4 ][ 2 ][ 0 ] = WTILESIZE / 2;
  tr[ 4 ][ 2 ][ 1 ] = ce[ 7 ];
  tr[ 4 ][ 2 ][ 2 ] = WTILESIZE;

  tr[ 5 ][ 0 ][ 0 ] = WTILESIZE / 2;
  tr[ 5 ][ 0 ][ 1 ] = ce[ 4 ];
  tr[ 5 ][ 0 ][ 2 ] = WTILESIZE / 2;
  tr[ 5 ][ 1 ][ 0 ] = WTILESIZE / 2;
  tr[ 5 ][ 1 ][ 1 ] = ce[ 7 ];
  tr[ 5 ][ 1 ][ 2 ] = WTILESIZE;
  tr[ 5 ][ 2 ][ 0 ] = 0;
  tr[ 5 ][ 2 ][ 1 ] = ce[ 6 ];
  tr[ 5 ][ 2 ][ 2 ] = WTILESIZE;

  tr[ 6 ][ 0 ][ 0 ] = WTILESIZE / 2;
  tr[ 6 ][ 0 ][ 1 ] = ce[ 4 ];
  tr[ 6 ][ 0 ][ 2 ] = WTILESIZE / 2;
  tr[ 6 ][ 1 ][ 0 ] = 0;
  tr[ 6 ][ 1 ][ 1 ] = ce[ 6 ];
  tr[ 6 ][ 1 ][ 2 ] = WTILESIZE;
  tr[ 6 ][ 2 ][ 0 ] = 0;
  tr[ 6 ][ 2 ][ 1 ] = ce[ 3 ];
  tr[ 6 ][ 2 ][ 2 ] = WTILESIZE / 2;

  tr[ 7 ][ 0 ][ 0 ] = WTILESIZE / 2;
  tr[ 7 ][ 0 ][ 1 ] = ce[ 4 ];
  tr[ 7 ][ 0 ][ 2 ] = WTILESIZE / 2;
  tr[ 7 ][ 1 ][ 0 ] = 0;
  tr[ 7 ][ 1 ][ 1 ] = ce[ 3 ];
  tr[ 7 ][ 1 ][ 2 ] = WTILESIZE / 2;
  tr[ 7 ][ 2 ][ 0 ] = 0;
  tr[ 7 ][ 2 ][ 1 ] = ce[ 0 ];
  tr[ 7 ][ 2 ][ 2 ] = 0;

}

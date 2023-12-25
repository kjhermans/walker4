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

#include <math.h>

#include "walker.h"

static
int landscape_triangle_elevation
  (
    int pt0x, int pt0y, int pt0z,
    int pt1x, int pt1y, int pt1z,
    int pt2x, int pt2y, int pt2z,
    int px, int pz
  )
{
  int a1 = pt1x - pt0x;
  int b1 = pt1y - pt0y;
  int c1 = pt1z - pt0z;
  int a2 = pt2x - pt0x;
  int b2 = pt2y - pt0y;
  int c2 = pt2z - pt0z;
  int a = b1 * c2 - b2 * c1;
  int b = a2 * c1 - a1 * c2;
  int c = a1 * b2 - b1 * a2;
  int d = (- a * pt0x - b * pt0y - c * pt0z);

  float py = -((a * px) + (c * pz) + d) / b;
  return (int)py;
}

/**
 * Returns the elevation at the position coordinate, based on the
   tile triangle that the coordinate is in. The triangles inside a tile
   are modelled as follows:

  (0,0)
    +------+------+
    | \    |    / |
    |  \ 0 | 1 /  |
    | 7 \  |  / 2 |
    |    \ | /    |
    +------+------+
    |    / | \    |
    | 6 /  |  \ 3 |
    |  / 5 | 4 \  |
    | /    |    \ |
    +------+------+

 */
void landscape_get_elevation
  (wlandscape_t* ls, int px, int pz, wground_t* ground)
{
  int tx, tz;
  wtile_t tile = { 0 };

  landscape_pos2tile(px, pz, &tx, &tz);
  landscape_tile_get(ls, tx, tz, &tile);
  if (tile.elevation[ 0 ]) { // optimized tile, with known edge elevations
    int tpx = px - (tx * WTILESIZE);
    int tpz = pz - (tz * WTILESIZE);

    if (tpx < WTILESIZE / 2) {
      if (tpz < WTILESIZE / 2) {
        if (tpx > tpz) { // triangle 0
          ground->elevation = landscape_triangle_elevation(
            0, tile.elevation[ 0 ], 0,
            WTILESIZE / 2, tile.elevation[ 1 ], 0,
            WTILESIZE / 2, tile.elevation[ 4 ], WTILESIZE / 2,
            tpx, tpz
          );
          return;
        } else { // triangle 7
          ground->elevation = landscape_triangle_elevation(
            0, tile.elevation[ 0 ], 0,
            0, tile.elevation[ 7 ], WTILESIZE / 2,
            WTILESIZE / 2, tile.elevation[ 4 ], WTILESIZE / 2,
            tpx, tpz
          );
          return;
        }
      } else {
        if (WTILESIZE - tpz > tpx) { // triangle 5
          ground->elevation = landscape_triangle_elevation(
            0, tile.elevation[ 0 ], WTILESIZE,
            WTILESIZE / 2, tile.elevation[ 7 ], WTILESIZE,
            WTILESIZE / 2, tile.elevation[ 4 ], WTILESIZE / 2,
            tpx, tpz
          );
          return;
        } else { // triangle 6
          ground->elevation = landscape_triangle_elevation(
            0, tile.elevation[ 0 ], WTILESIZE / 2,
            0, tile.elevation[ 7 ], WTILESIZE,
            WTILESIZE / 2, tile.elevation[ 4 ], WTILESIZE / 2,
            tpx, tpz
          );
          return;
        }
      }
    } else {
      if (tpz < WTILESIZE / 2) {
        if (WTILESIZE - tpx < tpz) { // triangle 1
          ground->elevation = landscape_triangle_elevation(
            WTILESIZE / 2, tile.elevation[ 0 ], 0,
            WTILESIZE, tile.elevation[ 7 ], 0,
            WTILESIZE / 2, tile.elevation[ 4 ], WTILESIZE / 2,
            tpx, tpz
          );
          return;
        } else { // triangle 2
          ground->elevation = landscape_triangle_elevation(
            WTILESIZE, tile.elevation[ 0 ], 0,
            WTILESIZE, tile.elevation[ 7 ], WTILESIZE / 2,
            WTILESIZE / 2, tile.elevation[ 4 ], WTILESIZE / 2,
            tpx, tpz
          );
          return;
        }
      } else { 
        if (tpx > tpz) { // triangle 3
          ground->elevation = landscape_triangle_elevation(
            WTILESIZE, tile.elevation[ 0 ], WTILESIZE / 2,
            WTILESIZE, tile.elevation[ 7 ], WTILESIZE,
            WTILESIZE / 2, tile.elevation[ 4 ], WTILESIZE / 2,
            tpx, tpz
          );
          return;
        } else { // triangle 4
          ground->elevation = landscape_triangle_elevation(
            WTILESIZE, tile.elevation[ 0 ], WTILESIZE,
            WTILESIZE / 2, tile.elevation[ 7 ], WTILESIZE,
            WTILESIZE / 2, tile.elevation[ 4 ], WTILESIZE / 2,
            tpx, tpz
          );
          return;
        }
      }
    }
  }
  ground->elevation = tile.elevation[ 4 ];
}

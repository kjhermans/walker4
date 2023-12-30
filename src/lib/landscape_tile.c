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

#define HILL_INCIDENCE   0x1f // one in every 32 squared tiles is a hill
#define HILL_MAXDIST     ((HILL_INCIDENCE+1)*1.5) // sqrt(2) rounded up
#define CHASM_INCIDENCE  0x1f

static
void landscape_tile_hill
  (int tx, int tz, wtile_t* tile)
{
  int hillx, hillz, r, hillness, dx, dz;
  float distance;
  uint32_t c = landscape_random_get_topo(tx, tz, WTOPIC_COLOR);

  tile->color = ((unsigned char)c) % 4;
  hillx = (tx & ~(HILL_INCIDENCE));
  hillz = (tz & ~(HILL_INCIDENCE));
  r = landscape_random_get_topo(hillx, hillz, WTOPIC_HILLS);
  hillness = (r & 0x3f);
  hillx += ((r >> 16) & 0x0f);
  hillz += ((r >> 24) & 0x0f);
  dx = (hillx - tx);
  dz = (hillz - tz);

  distance = sqrt((dx * dx) + (dz * dz));
  if (distance == 0) {
    tile->elevation[ 4 ] += hillness;
  } else {
    tile->elevation[ 4 ] += (hillness / distance);
  }
  tile->hardness =
    ((( /* random */
      ((c >> 8) & 0x03) &
      /* the closer to the hilltop, the more ones */
      ((int)(4 * (HILL_MAXDIST - distance) / HILL_MAXDIST))
    ) % 4) == 0) ? 3 : 0;

  tile->elevation[ 4 ] *= 32;
  tile->elevation[ 4 ] += 1024;
}

static
void landscape_tile_chasm
  (int tx, int tz, wtile_t* tile)
{
  int chasmx = (tx & ~(CHASM_INCIDENCE));
  int chasmz = (tz & ~(CHASM_INCIDENCE));
  uint32_t r = landscape_random_get_topo(chasmx, chasmz, WTOPIC_CHASM);
  int chasmness = (r & 0x3f);
  int chasmxvar = ((r >> 8) & 0x0f);
  int chasmzvar = ((r >> 12) & 0x0f);
  chasmx += (8 - chasmxvar);
  chasmz += (8 - chasmzvar);
  float chasmangle = (((r >> 24) & 0xff) * WPI / 1024) + 0.1;
  float a = tan(chasmangle);
  float b = chasmz - (a * chasmx);
  float pa = -(1/a);
  float pb = tz - (pa * tx);
  if ((a - pa) == 0) { goto NOCHASM; }
  float mx = (pb - b) / (a - pa);
  float mz = (a * mx) + b;
  float dchasm;
  {
    int dx = (mx - tx);
    int dz = (mz - tz);
    dchasm = sqrt((dx * dx) + (dz * dz));
  }
  if (dchasm > 16) { goto NOCHASM; }
  int de = (float)chasmness * dchasm * 1.2;
  if (de > tile->elevation[ 4 ]) {
    tile->elevation[ 4 ] = 64;
    tile->hardness = 2;
  } else {
    tile->elevation[ 4 ] -= de;
    if (de > 512 || tile->elevation[ 4 ] < 600) {
      tile->hardness = 2;
    }
  }
NOCHASM:
}

/**
 * Deterministically creates a landscape quadrant's tile.
 * This code should be called only when new tiles are created (ie when
 * the player walks into a previously unknown area).
 */
void landscape_tile_get_new
  (int tx, int tz, wtile_t* tile)
{
#ifdef _DEBUG
  fprintf(stderr, "tile get (%d, %d) new\n", tx, tz);
#endif

  tile->elevation[ 4 ] =
    landscape_random_get_topo_exp(
      tx, tz, WTOPIC_ELEVATION, WTILE_VARIATION
    );
  landscape_tile_hill(tx, tz, tile);
  landscape_tile_chasm(tx, tz, tile);
}

/**
 * Optimizes the elevation values on tiles[ 4 ].
 */
void landscape_tile_optimize
  (wlandscape_t* ls, int tx, int tz, wtile_t* tiles[ 9 ])
{
  wtile_t cache[ 9 ] = { 0 };
  (void)ls;

  for (unsigned i=0; i < 9; i++) {
    if (tiles[ i ] == NULL) {
      landscape_tile_get(ls, tx+((i%3)-1), tz+((i/3)-1), &(cache[ i ]));
      tiles[ i ] = &(cache[ i ]);
    }
  }
  tiles[ 4 ]->elevation[ 0 ] = (
    tiles[ 0 ]->elevation[ 4 ] +
    tiles[ 1 ]->elevation[ 4 ] +
    tiles[ 3 ]->elevation[ 4 ] +
    tiles[ 4 ]->elevation[ 4 ]
  ) / 4;
  tiles[ 4 ]->elevation[ 1 ] = (
    tiles[ 4 ]->elevation[ 4 ] +
    tiles[ 1 ]->elevation[ 4 ]
  ) / 2;
  tiles[ 4 ]->elevation[ 2 ] = (
    tiles[ 1 ]->elevation[ 4 ] +
    tiles[ 2 ]->elevation[ 4 ] +
    tiles[ 4 ]->elevation[ 4 ] +
    tiles[ 5 ]->elevation[ 4 ]
  ) / 4;
  tiles[ 4 ]->elevation[ 3 ] = (
    tiles[ 4 ]->elevation[ 4 ] +
    tiles[ 3 ]->elevation[ 4 ]
  ) / 2;
  tiles[ 4 ]->elevation[ 5 ] = (
    tiles[ 4 ]->elevation[ 4 ] +
    tiles[ 5 ]->elevation[ 4 ]
  ) / 2;
  tiles[ 4 ]->elevation[ 6 ] = (
    tiles[ 3 ]->elevation[ 4 ] +
    tiles[ 4 ]->elevation[ 4 ] +
    tiles[ 6 ]->elevation[ 4 ] +
    tiles[ 7 ]->elevation[ 4 ]
  ) / 4;
  tiles[ 4 ]->elevation[ 7 ] = (
    tiles[ 4 ]->elevation[ 4 ] +
    tiles[ 7 ]->elevation[ 4 ]
  ) / 2;
  tiles[ 4 ]->elevation[ 8 ] = (
    tiles[ 4 ]->elevation[ 4 ] +
    tiles[ 5 ]->elevation[ 4 ] +
    tiles[ 7 ]->elevation[ 4 ] +
    tiles[ 8 ]->elevation[ 4 ]
  ) / 4;
}

void landscape_tile_get_triangles_optimized
  (wtile_t* tile, int tr[ 8 ][ 3 ][ 3 ])
{
  tr[ 0 ][ 0 ][ 0 ] = WTILESIZE / 2;
  tr[ 0 ][ 0 ][ 1 ] = tile->elevation[ 4 ];
  tr[ 0 ][ 0 ][ 2 ] = WTILESIZE / 2;
  tr[ 0 ][ 1 ][ 0 ] = 0;
  tr[ 0 ][ 1 ][ 1 ] = tile->elevation[ 0 ];
  tr[ 0 ][ 1 ][ 2 ] = 0;
  tr[ 0 ][ 2 ][ 0 ] = WTILESIZE / 2;
  tr[ 0 ][ 2 ][ 1 ] = tile->elevation[ 1 ];
  tr[ 0 ][ 2 ][ 2 ] = 0;

  tr[ 1 ][ 0 ][ 0 ] = WTILESIZE / 2;
  tr[ 1 ][ 0 ][ 1 ] = tile->elevation[ 4 ];
  tr[ 1 ][ 0 ][ 2 ] = WTILESIZE / 2;
  tr[ 1 ][ 1 ][ 0 ] = WTILESIZE / 2;
  tr[ 1 ][ 1 ][ 1 ] = tile->elevation[ 1 ];
  tr[ 1 ][ 1 ][ 2 ] = 0;
  tr[ 1 ][ 2 ][ 0 ] = WTILESIZE;
  tr[ 1 ][ 2 ][ 1 ] = tile->elevation[ 2 ];
  tr[ 1 ][ 2 ][ 2 ] = 0;

  tr[ 2 ][ 0 ][ 0 ] = WTILESIZE / 2;
  tr[ 2 ][ 0 ][ 1 ] = tile->elevation[ 4 ];
  tr[ 2 ][ 0 ][ 2 ] = WTILESIZE / 2;
  tr[ 2 ][ 1 ][ 0 ] = WTILESIZE;
  tr[ 2 ][ 1 ][ 1 ] = tile->elevation[ 2 ];
  tr[ 2 ][ 1 ][ 2 ] = 0;
  tr[ 2 ][ 2 ][ 0 ] = WTILESIZE;
  tr[ 2 ][ 2 ][ 1 ] = tile->elevation[ 5 ];
  tr[ 2 ][ 2 ][ 2 ] = WTILESIZE / 2;

  tr[ 3 ][ 0 ][ 0 ] = WTILESIZE / 2;
  tr[ 3 ][ 0 ][ 1 ] = tile->elevation[ 4 ];
  tr[ 3 ][ 0 ][ 2 ] = WTILESIZE / 2;
  tr[ 3 ][ 1 ][ 0 ] = WTILESIZE;
  tr[ 3 ][ 1 ][ 1 ] = tile->elevation[ 5 ];
  tr[ 3 ][ 1 ][ 2 ] = WTILESIZE / 2;
  tr[ 3 ][ 2 ][ 0 ] = WTILESIZE;
  tr[ 3 ][ 2 ][ 1 ] = tile->elevation[ 8 ];
  tr[ 3 ][ 2 ][ 2 ] = WTILESIZE;

  tr[ 4 ][ 0 ][ 0 ] = WTILESIZE / 2;
  tr[ 4 ][ 0 ][ 1 ] = tile->elevation[ 4 ];
  tr[ 4 ][ 0 ][ 2 ] = WTILESIZE / 2;
  tr[ 4 ][ 1 ][ 0 ] = WTILESIZE;
  tr[ 4 ][ 1 ][ 1 ] = tile->elevation[ 8 ];
  tr[ 4 ][ 1 ][ 2 ] = WTILESIZE;
  tr[ 4 ][ 2 ][ 0 ] = WTILESIZE / 2;
  tr[ 4 ][ 2 ][ 1 ] = tile->elevation[ 7 ];
  tr[ 4 ][ 2 ][ 2 ] = WTILESIZE;

  tr[ 5 ][ 0 ][ 0 ] = WTILESIZE / 2;
  tr[ 5 ][ 0 ][ 1 ] = tile->elevation[ 4 ];
  tr[ 5 ][ 0 ][ 2 ] = WTILESIZE / 2;
  tr[ 5 ][ 1 ][ 0 ] = WTILESIZE / 2;
  tr[ 5 ][ 1 ][ 1 ] = tile->elevation[ 7 ];
  tr[ 5 ][ 1 ][ 2 ] = WTILESIZE;
  tr[ 5 ][ 2 ][ 0 ] = 0;
  tr[ 5 ][ 2 ][ 1 ] = tile->elevation[ 6 ];
  tr[ 5 ][ 2 ][ 2 ] = WTILESIZE;

  tr[ 6 ][ 0 ][ 0 ] = WTILESIZE / 2;
  tr[ 6 ][ 0 ][ 1 ] = tile->elevation[ 4 ];
  tr[ 6 ][ 0 ][ 2 ] = WTILESIZE / 2;
  tr[ 6 ][ 1 ][ 0 ] = 0;
  tr[ 6 ][ 1 ][ 1 ] = tile->elevation[ 6 ];
  tr[ 6 ][ 1 ][ 2 ] = WTILESIZE;
  tr[ 6 ][ 2 ][ 0 ] = 0;
  tr[ 6 ][ 2 ][ 1 ] = tile->elevation[ 3 ];
  tr[ 6 ][ 2 ][ 2 ] = WTILESIZE / 2;

  tr[ 7 ][ 0 ][ 0 ] = WTILESIZE / 2;
  tr[ 7 ][ 0 ][ 1 ] = tile->elevation[ 4 ];
  tr[ 7 ][ 0 ][ 2 ] = WTILESIZE / 2;
  tr[ 7 ][ 1 ][ 0 ] = 0;
  tr[ 7 ][ 1 ][ 1 ] = tile->elevation[ 3 ];
  tr[ 7 ][ 1 ][ 2 ] = WTILESIZE / 2;
  tr[ 7 ][ 2 ][ 0 ] = 0;
  tr[ 7 ][ 2 ][ 1 ] = tile->elevation[ 0 ];
  tr[ 7 ][ 2 ][ 2 ] = 0;

}

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

int landscape_tile_get_cache
  (wlandscape_t* ls, int tx, int tz, wtile_t* tile)
{
  int qx, qz;
  int qtx, qtz;

  landscape_tile2quadrant(tx, tz, &qx, &qz, &qtx, &qtz);
  if (qx >= ls->cache.mem.qp.x && qx < ls->cache.mem.qp.x + 3
      && qz >= ls->cache.mem.qp.z && qz < ls->cache.mem.qp.z + 3)
  {
    int i = qx - ls->cache.mem.qp.x;
    int j = qz - ls->cache.mem.qp.z;
    *tile = ls->cache.mem.quadrant[ i ][ j ].tiles[ qtx ][ qtz ];
    return 0;
  } else {
    return ~0;
  }
}

void landscape_tile_get
  (wlandscape_t* ls, int tx, int tz, wtile_t* tile)
{
  if (landscape_tile_get_cache(ls, tx, tz, tile) == 0) {
    return;
  }
  landscape_tile_get_new(tx, tz, tile);
}

/**
 * Converts a 2D tile coordinate (tx, tz) to a quadrant coordinate
 * and (optionally) a tile coordinate within the quadrant.
 */
void landscape_tile2quadrant
  (
    int tx, int tz,
    int* qx, int* qz,
    int* qtx, int* qtz
  )
{
  *qx = tx / WQUADRANT_DIMENSION;
  *qz = tz / WQUADRANT_DIMENSION;
  if (tx < 0 && (tx % WQUADRANT_DIMENSION)) { --(*qx); }
  if (tz < 0 && (tz % WQUADRANT_DIMENSION)) { --(*qz); }
  if (qtx) {
    *qtx = tx - ((*qx) * WQUADRANT_DIMENSION);
  }
  if (qtz) {
    *qtz = tz - ((*qz) * WQUADRANT_DIMENSION);
  }
}

void landscape_tile_debug
  (wtile_t* tile)
{
  fprintf(stderr, "Tile %d ", tile->elevation[ 4 ]);
  if (tile->elevation[ 0 ]) {
    fprintf(stderr, " - %d,%d,%d, %d, %d, %d,%d,%d\n"
      , tile->elevation[ 0 ]
      , tile->elevation[ 1 ]
      , tile->elevation[ 2 ]
      , tile->elevation[ 3 ]
      , tile->elevation[ 5 ]
      , tile->elevation[ 6 ]
      , tile->elevation[ 7 ]
      , tile->elevation[ 8 ]
    );
  } else {
    fprintf(stderr, " - not optimized.\n");
  }
}

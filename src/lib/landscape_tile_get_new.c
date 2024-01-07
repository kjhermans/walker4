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
    tile->elevation[ 4 ] += hillness + 2;
  } else {
    tile->elevation[ 4 ] += (hillness / distance);
  }
  tile->hardness =
    ((( /* random */
      ((c >> 8) & 0x07) &
      /* the closer to the hilltop, the more ones */
      ((int)(4 * (HILL_MAXDIST - distance) / HILL_MAXDIST))
    ) % 8) == 0) ? 3 : 0;

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
  float chasmangle = (((r >> 24) & 0xff) * WPI / 512) + 0.01;
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
NOCHASM: ;
}

static
void landscape_tile_lake
  (int tx, int tz, wtile_t* tile)
{
  int qx, qz;
  uint32_t r;

  landscape_tile_2quadrant(tx, tz, &qx, &qz, 0, 0);
  r = landscape_random_get_topo(qx, qz, WTOPIC_LAKE);
  if ((r % 8) == 0) {
    int qctx = (qx * WQUADRANT_DIMENSION) + (WQUADRANT_DIMENSION / 2);
    int qctz = (qz * WQUADRANT_DIMENSION) + (WQUADRANT_DIMENSION / 2);
    
    int dx = tx - qctx;
    int dz = tz - qctz;

    unsigned d = sqrt(dx*dx + dz*dz);
    int div = 1;
    if (d < (WQUADRANT_DIMENSION / 3)) {
      div = 8;
    } else if (d < (WQUADRANT_DIMENSION / 2)) {
      div = 4;
    } else if (d < (WQUADRANT_DIMENSION * 2 / 3)) {
      div = 2;
    }
    if (tile->elevation[ 4 ] > WQUADRANT_DIMENSION) {
      tile->elevation[ 4 ] /= div;
    }
  }
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
  landscape_tile_lake(tx, tz, tile);
}

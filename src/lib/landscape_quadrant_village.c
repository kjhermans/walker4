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

#include "walker.h"

#define MAXHOUSES 16

static
int housepos[ MAXHOUSES ][ 2 ] = {
  {  0,  0 },
  {  0,  1 },
  {  1,  1 },
  {  2,  0 },
  {  2, -1 },
  {  1. -2 },
  {  0, -3 },
  { -1, -3 },
  { -2, -2 },
  { -2, -1 },
  { -3,  0 },
  { -2,  1 },
  { -2,  2 },
  { -1,  3 },
  {  0,  3 },
  {  1,  3 }
};

void landscape_quadrant_village
  (wlandscape_t* ls, int qx, int qz, wquadrant_t* q)
{
  ASSERT(ls)
  ASSERT(q)
  (void)q;

  uint32_t r = landscape_random_get_topo(qx, qz, WTOPIC_VILLAGE);
  unsigned nhouses;
  unsigned havevillage;
  unsigned havehouse;

  srand(r);
  havevillage = rand();
  if ((havevillage & 0x0f) == 0) {
    int tx = WQUADRANT_DIMENSION / 4 + (rand() % (WQUADRANT_DIMENSION >> 1));
    int tz = WQUADRANT_DIMENSION / 4 + (rand() % (WQUADRANT_DIMENSION >> 1));
    nhouses = rand() & (MAXHOUSES-1);
    for (unsigned i=0; i < nhouses; i++) {
      havehouse = rand() % 2;
      if (havehouse) {
        int hx = tx + housepos[ i ][ 0 ];
        int hz = tz + housepos[ i ][ 1 ];
        wobject_t o = { 0 };
        object_init(&o, WOBJTYPE_HOUSE);
        o.flags |= WOBJFLAG_VISIBLE;
        o.position.x = ((qx * WQUADRANT_DIMENSION) + hx) * WTILESIZE;
        o.position.y = 4096;
        o.position.z = ((qz * WQUADRANT_DIMENSION) + hz) * WTILESIZE;
        o.oxz = (float)(rand() % 360) / WDEGRAD;
        o.subtype.house.type = rand() % 4;
        wobjectlist_push(&(ls->walker->world.objects), o);
      }
    }
  }
}

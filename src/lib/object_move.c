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
 * Returns non zero if there's been a move.
 */
int object_move
  (wobject_t* o, wlandscape_t* ls)
{
  ASSERT(o)
  ASSERT(ls)

  pt3d_t pt = o->position;

  int camry = (int)(o->oxz / PL_RAD256);
  int camrx = (int)(o->oyz / PL_RAD256);
  o->position.x += (o->speed_hor * PL_sin[camry & PL_TRIGMSK]) >> PL_P;
  o->position.z += (o->speed_hor * PL_cos[camry & PL_TRIGMSK]) >> PL_P;
  if (o->mode == WOBJMODE_FLYING) {
    o->position.y += -((o->speed_hor * PL_sin[camrx & PL_TRIGMSK]) >> PL_P);
  } else {
    o->position.y += o->speed_vert;
  }

  /* checking whether on solid ground */
//  if (o->speed_hor) {
    o->cache.ground.supported = 0;
    landscape_get_elevation(ls, o, &(o->cache.ground));
    if (o->position.y <= o->cache.ground.elevation + 64) {
      o->position.y = o->cache.ground.elevation + 64;
      o->cache.ground.supported = 1;
    }
//  }

  if (WPOS_XMIN && o->position.x < WPOS_XMIN) {
    o->position.x = WPOS_XMIN;
  }
  if (WPOS_XMAX && o->position.x > WPOS_XMAX) {
    o->position.x = WPOS_XMAX;
  }
  if (WPOS_ZMIN && o->position.z < WPOS_ZMIN) {
    o->position.z = WPOS_ZMIN;
  }
  if (WPOS_ZMAX && o->position.z > WPOS_ZMAX) {
    o->position.z = WPOS_ZMAX;
  }

  /* boxing */
  if (o->position.y < FLOOR) {
    o->position.y = FLOOR;
  } else if (o->position.y > CEILING) {
    o->position.y = CEILING;
  }

  landscape_pos2tile(
    o->position.x, o->position.z,
    &(o->cache.tile.x), &(o->cache.tile.z)
  );
  landscape_tile_2quadrant(
    o->cache.tile.x, o->cache.tile.z,
    &(o->cache.quadrant.x), &(o->cache.quadrant.z),
    0, 0
  );

  if (o->position.x != pt.x || o->position.y != pt.y || o->position.z != pt.z) {
    return ~0;
  } else {
    return 0;
  }
}

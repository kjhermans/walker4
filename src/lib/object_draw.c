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

#include <time.h>
#include <math.h>

#include "walker.h"

/**
 * Draws the object, if it is visible, and within vision.
 * Also, calculates if the object is close enough to be in the vision's center.
 */
void object_draw
  (wobject_t* o, walker_t* w, wplayer_t* p, vec2d_t vision)
{
  ASSERT(o)
  ASSERT(w)
  ASSERT(p)

  int dx = o->position.x - p->object.position.x;
  int dz = o->position.z - p->object.position.z;
  int dy = o->position.y - p->object.position.y;

  if (o->draw
      && o->flags & WOBJFLAG_VISIBLE
      && o->cache.tile.x >= vision.o.x
      && o->cache.tile.x <= vision.d.x
      && o->cache.tile.z >= vision.o.z
      && o->cache.tile.z <= vision.d.z)
  {
    o->draw(o, w, (pt2d_t){ .x = dx, .z = dz });

    if (dx > 1600 || dy > 1600 || dz > 1600) { // TODO: definify 1600
      return;
    }
    float d3d = sqrt(dx*dx + dy*dy + dz*dz);
    float d2d = sqrt(dx*dx + dz*dz);
    if (d3d < 1) {
      return;
    }

    float oxz = atan2(dx, dz); if (oxz < 0) { oxz += (2 * WPI); }
    float oyz = atan2(-dy, d2d); if (oyz < 0) { oyz += (2 * WPI); }
    float margin = 100/d3d;

    if (oxz >= p->object.oxz - margin
        && oxz <= p->object.oxz + margin
        && oyz >= p->object.oyz - margin
        && oyz <= p->object.oyz + margin)
    {
      if (time(0) % 2) {
        object_attention_draw(dx, o->position.y, dz);
      }
      p->objectinview = o;
    }
  }
}

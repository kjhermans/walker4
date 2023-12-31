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

#define FWIDTH 64
#define FPAUSE 8

static
int vertices[] = {
  0, 0, 0, 0,
  -FWIDTH, -FPAUSE, -FPAUSE, 0,
  -FPAUSE, -FPAUSE, -FWIDTH, 0,
   FWIDTH, -FPAUSE, -FPAUSE, 0,
   FPAUSE, -FPAUSE, -FWIDTH, 0,
  -FWIDTH, -FPAUSE,  FPAUSE, 0,
  -FPAUSE, -FPAUSE,  FWIDTH, 0,
   FWIDTH, -FPAUSE,  FPAUSE, 0,
   FPAUSE, -FPAUSE,  FWIDTH, 0,
  -FWIDTH,  FPAUSE, -FPAUSE, 0,
  -FPAUSE,  FPAUSE, -FWIDTH, 0,
   FWIDTH,  FPAUSE, -FPAUSE, 0,
   FPAUSE,  FPAUSE, -FWIDTH, 0,
  -FWIDTH,  FPAUSE,  FPAUSE, 0,
  -FPAUSE,  FPAUSE,  FWIDTH, 0,
   FWIDTH,  FPAUSE,  FPAUSE, 0,
   FPAUSE,  FPAUSE,  FWIDTH, 0,
};

struct PL_POLY polys[] = {
  {
    .n_verts = 4,
    .verts[ 0 ] = 0,
    .verts[ 1 ] = 0,
    .verts[ 2 ] = 0,
    .verts[ 3 ] = 1,
    .verts[ 4 ] = 0,
    .verts[ 5 ] = 0,
    .verts[ 6 ] = 2,
    .verts[ 7 ] = 0,
    .verts[ 8 ] = 0,
    .verts[ 9 ] = 0,
    .verts[ 10 ] = 0,
    .verts[ 11 ] = 0,
    .color = 0xffffff
  },
  {
    .n_verts = 4,
    .verts[ 0 ] = 0,
    .verts[ 1 ] = 0,
    .verts[ 2 ] = 0,
    .verts[ 3 ] = 3,
    .verts[ 4 ] = 0,
    .verts[ 5 ] = 0,
    .verts[ 6 ] = 4,
    .verts[ 7 ] = 0,
    .verts[ 8 ] = 0,
    .verts[ 9 ] = 0,
    .verts[ 10 ] = 0,
    .verts[ 11 ] = 0,
    .color = 0xffffff
  },
  {
    .n_verts = 4,
    .verts[ 0 ] = 0,
    .verts[ 1 ] = 0,
    .verts[ 2 ] = 0,
    .verts[ 3 ] = 5,
    .verts[ 4 ] = 0,
    .verts[ 5 ] = 0,
    .verts[ 6 ] = 6,
    .verts[ 7 ] = 0,
    .verts[ 8 ] = 0,
    .verts[ 9 ] = 0,
    .verts[ 10 ] = 0,
    .verts[ 11 ] = 0,
    .color = 0xffffff
  },
  {
    .n_verts = 4,
    .verts[ 0 ] = 0,
    .verts[ 1 ] = 0,
    .verts[ 2 ] = 0,
    .verts[ 3 ] = 7,
    .verts[ 4 ] = 0,
    .verts[ 5 ] = 0,
    .verts[ 6 ] = 8,
    .verts[ 7 ] = 0,
    .verts[ 8 ] = 0,
    .verts[ 9 ] = 0,
    .verts[ 10 ] = 0,
    .verts[ 11 ] = 0,
    .color = 0xffffff
  },
  {
    .n_verts = 4,
    .verts[ 0 ] = 0,
    .verts[ 1 ] = 0,
    .verts[ 2 ] = 0,
    .verts[ 3 ] = 9,
    .verts[ 4 ] = 0,
    .verts[ 5 ] = 0,
    .verts[ 6 ] = 10,
    .verts[ 7 ] = 0,
    .verts[ 8 ] = 0,
    .verts[ 9 ] = 0,
    .verts[ 10 ] = 0,
    .verts[ 11 ] = 0,
    .color = 0xff0000
  },
  {
    .n_verts = 4,
    .verts[ 0 ] = 0,
    .verts[ 1 ] = 0,
    .verts[ 2 ] = 0,
    .verts[ 3 ] = 11,
    .verts[ 4 ] = 0,
    .verts[ 5 ] = 0,
    .verts[ 6 ] = 12,
    .verts[ 7 ] = 0,
    .verts[ 8 ] = 0,
    .verts[ 9 ] = 0,
    .verts[ 10 ] = 0,
    .verts[ 11 ] = 0,
    .color = 0xff0000
  },
  {
    .n_verts = 4,
    .verts[ 0 ] = 0,
    .verts[ 1 ] = 0,
    .verts[ 2 ] = 0,
    .verts[ 3 ] = 13,
    .verts[ 4 ] = 0,
    .verts[ 5 ] = 0,
    .verts[ 6 ] = 14,
    .verts[ 7 ] = 0,
    .verts[ 8 ] = 0,
    .verts[ 9 ] = 0,
    .verts[ 10 ] = 0,
    .verts[ 11 ] = 0,
    .color = 0xff0000
  },
  {
    .n_verts = 4,
    .verts[ 0 ] = 0,
    .verts[ 1 ] = 0,
    .verts[ 2 ] = 0,
    .verts[ 3 ] = 15,
    .verts[ 4 ] = 0,
    .verts[ 5 ] = 0,
    .verts[ 6 ] = 16,
    .verts[ 7 ] = 0,
    .verts[ 8 ] = 0,
    .verts[ 9 ] = 0,
    .verts[ 10 ] = 0,
    .verts[ 11 ] = 0,
    .color = 0xff0000
  }
};

static
struct PL_OBJ obj = {
  .verts = vertices,
  .n_verts = 17,
  .polys = polys,
  .n_polys = 8
};

void flyer_update
  (wobject_t* o, walker_t* w)
{
  (void)w;

  if (o->subtype.flyer.rotxz > 0) {
    o->oxz += .2;
    o->subtype.flyer.rotxz -= .2;
  } else if ((rand() % 16) == 0) {
    o->subtype.flyer.rotxz = ((float)(rand() % 16)) / WPI;
  }
  if (o->subtype.flyer.rotyz > 0) {
    o->oyz += .2;
    o->subtype.flyer.rotyz -= .2;
  } else if ((rand() % 16) == 0) {
    o->subtype.flyer.rotyz = ((float)(rand() % 16)) / WPI;
  }
  object_gravity(o);
  object_friction(o);
  int moved = object_move(o, &(w->world.landscape)); (void)moved;
}

void flyer_draw
  (wobject_t* o, walker_t* w, pt2d_t p)
{
  (void)w;

  PL_mst_push();
  PL_mst_translate(p.x, o->position.y, p.z);
  PL_mst_rotatex((int)(o->oyz / PL_RAD256));
  PL_mst_rotatey((int)(o->oxz / PL_RAD256));
  PL_render_object(&obj);
  PL_mst_pop();
}

void flyer_init
  (wobject_t* o)
{
  memset(o, 0, sizeof(*o));
  o->type = WOBJTYPE_AFLYER;
  o->id = object_get_id();
  o->update = flyer_update;
  o->draw = flyer_draw;
  o->flags |= WOBJFLAG_FLYING|WOBJFLAG_VISIBLE;
}

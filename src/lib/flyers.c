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
#undef ARRAY_EQUALS
#define ARRAY_EQUALS(a,b) (0 == memcmp(&a,&b,sizeof(a)))
MAKE_ARRAY_CODE(wflyer_t, wflyerlist_)

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

void flyers_init
  (wworld_t* w)
{
  for (int i=0; i < 10; i++) {
    wflyer_t f = {
      .object.flags = WOBJFLAG_VISIBLE|WOBJFLAG_FLYING,
      .object.position.x = ((i-5) * 200),
      .object.position.y = 1000,
      .object.position.z = 0,
      .object.oxz = 0,
      .object.oyz = 0,
      .object.speed_vert = 0,
      .object.speed_hor = 1
    };
    wflyerlist_push(&(w->flyers), f);
  }
}

void flyer_update
  (wworld_t* w, wflyer_t* f)
{
  (void)w;

  if (f->rotxz > 0) {
    f->object.oxz += .2;
    f->rotxz -= .2;
  } else if ((rand() % 16) == 0) {
    f->rotxz = ((float)(rand() % 16)) / WPI;
  }
  if (f->rotyz > 0) {
    f->object.oyz += .2;
    f->rotyz -= .2;
  } else if ((rand() % 16) == 0) {
    f->rotyz = ((float)(rand() % 16)) / WPI;
  }
  object_gravity(&(f->object));
  object_friction(&(f->object));
  int moved = object_move(&(f->object), &(w->landscape)); (void)moved;
}

void flyers_update
  (wworld_t* w)
{
  for (unsigned i=0; i < w->flyers.count; i++) {
    wflyer_t* f = wflyerlist_getptr(&(w->flyers), i);
    flyer_update(w, f);
  }
}

void flyer_draw
  (wflyer_t* f, int x, int z)
{
  if (f->object.flags & WOBJFLAG_VISIBLE) {
    PL_mst_push();
    PL_mst_translate(x, f->object.position.y, z);
    PL_mst_rotatex((int)(f->object.oyz / PL_RAD256));
    PL_mst_rotatey((int)(f->object.oxz / PL_RAD256));
    PL_render_object(&obj);
    PL_mst_pop();
  }
}

void flyers_draw
  (wworld_t* w, pt2d_t relative, vec2d_t* vision)
{
  for (unsigned i=0; i < w->flyers.count; i++) {
    wflyer_t* f = wflyerlist_getptr(&(w->flyers), i);
    if (f->object.cache.tile.x >= vision->o.x
        && f->object.cache.tile.x <= vision->d.x
        && f->object.cache.tile.z >= vision->o.z
        && f->object.cache.tile.z <= vision->d.z)
    {
      int dx = f->object.position.x - relative.x;
      int dz = f->object.position.z - relative.z;
      flyer_draw(f, dx, dz);
    }
  }
}

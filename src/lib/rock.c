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
#include "rock.h"

static struct PL_TEX rocktex = { 0 };
static int rockimg[ PL_REQ_TEX_DIM * PL_REQ_TEX_DIM ] = IMG;
static struct PL_OBJ* rockcube;

void rock_create
  ()
{
  PL_texture(&rocktex);
  rocktex.data = (int*)rockimg;
  rockcube = PL_gen_box(128, 128, 16, PL_ALL, 255, 255, 255);
  PL_texture(NULL);
}

void rock_draw
  (wobject_t* o, walker_t* w, pt2d_t p, float rx)
{
  (void)w;

  PL_raster_mode = PL_TEXTURED;
  PL_mst_push();
  PL_mst_translate(p.x, o->position.y, p.z);
  PL_mst_rotatex((int)(rx / PL_RAD256));
  PL_render_object(rockcube);
  PL_mst_pop();
  PL_raster_mode = PL_FLAT;
}

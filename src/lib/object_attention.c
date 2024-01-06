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

static
int verts[ 8 ][ 16 ] = {
  {
    -80, -80, -80, 0,
    -64, -80, -80, 0,
    -80, -64, -80, 0,
    -80, -80, -64, 0,
  },
  {
     80, -80, -80, 0,
     64, -80, -80, 0,
     80, -64, -80, 0,
     80, -80, -64, 0,
  },
  {
    -80,  80, -80, 0,
    -64,  80, -80, 0,
    -80,  64, -80, 0,
    -80,  80, -64, 0,
  },
  {
    -80, -80,  80, 0,
    -64, -80,  80, 0,
    -80, -64,  80, 0,
    -80, -80,  64, 0,
  },
  {
     80,  80, -80, 0,
     64,  80, -80, 0,
     80,  64, -80, 0,
     80,  80, -64, 0,
  },
  {
    -80,  80,  80, 0,
    -64,  80,  80, 0,
    -80,  64,  80, 0,
    -80,  80,  64, 0,
  },
  {
     80, -80,  80, 0,
     64, -80,  80, 0,
     80, -64,  80, 0,
     80, -80,  64, 0,
  },
  {
     80,  80,  80, 0,
     64,  80,  80, 0,
     80,  64,  80, 0,
     80,  80,  64, 0,
  }
};

#define POLYVERT0 \
  {  \
    0, 0, 0, \
    1, 0, 0, \
    2, 0, 0, \
    0, 0, 0  \
  }

#define POLYVERT1 \
  {  \
    0, 0, 0, \
    2, 0, 0, \
    3, 0, 0, \
    0, 0, 0  \
  }

#define POLYVERT2 \
  {  \
    0, 0, 0, \
    1, 0, 0, \
    3, 0, 0, \
    0, 0, 0  \
  }

#define POLYVERT3 \
  {  \
    1, 0, 0, \
    3, 0, 0, \
    2, 0, 0, \
    1, 0, 0  \
  }

static
struct PL_POLY polys[ 4 ] = {
  {
    .n_verts = 4,
    .verts = POLYVERT0,
    .color = 0x111111
  },
  {
    .n_verts = 4,
    .verts = POLYVERT1,
    .color = 0x111111
  },
  {
    .n_verts = 4,
    .verts = POLYVERT2,
    .color = 0x111111
  },
  {
    .n_verts = 4,
    .verts = POLYVERT3,
    .color = 0x111111
  }
};

static
struct PL_OBJ obj[ 8 ] = {
  {
    .verts = verts[ 0 ],
    .n_verts = 4,
    .polys = polys,
    .n_polys = 4
  },
  {
    .verts = verts[ 1 ],
    .n_verts = 4,
    .polys = polys,
    .n_polys = 4
  },
  {
    .verts = verts[ 2 ],
    .n_verts = 4,
    .polys = polys,
    .n_polys = 4
  },
  {
    .verts = verts[ 3 ],
    .n_verts = 4,
    .polys = polys,
    .n_polys = 4
  },
  {
    .verts = verts[ 4 ],
    .n_verts = 4,
    .polys = polys,
    .n_polys = 4
  },
  {
    .verts = verts[ 5 ],
    .n_verts = 4,
    .polys = polys,
    .n_polys = 4
  },
  {
    .verts = verts[ 6 ],
    .n_verts = 4,
    .polys = polys,
    .n_polys = 4
  },
  {
    .verts = verts[ 7 ],
    .n_verts = 4,
    .polys = polys,
    .n_polys = 4
  }
};

void object_attention_draw
  (int px, int py, int pz)
{
  PL_mst_push();
  PL_mst_translate(px, py, pz);
  for (unsigned i=0; i < 8; i++) {
    PL_render_object(&(obj[ i ]));
  }
  PL_mst_pop();
}

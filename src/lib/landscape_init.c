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

void landscape_init
  (wlandscape_t* ls, unsigned seed, walker_t* w)
{
  srand(seed);
  landscape_random_extract();

  ls->walker = w;
  ls->cache.mem.qp.x = -1;
  ls->cache.mem.qp.z = -1;
  for (int i=0; i < 3; i++) {
    for (int j=0; j < 3; j++) {
      landscape_quadrant_get_new(
        ls,
        &(ls->cache.mem.quadrant[ i ][ j ]),
        ls->cache.mem.qp.x + i,
        ls->cache.mem.qp.z + j
      );
    }
  }
  for (int i=0; i < 3; i++) {
    for (int j=0; j < 3; j++) {
      landscape_quadrant_optimize(ls, &(ls->cache.mem.quadrant[ i ][ j ]));
      walker_db_quadrant_store(w, &(ls->cache.mem.quadrant[ i ][ j ]));
    }
  }
}

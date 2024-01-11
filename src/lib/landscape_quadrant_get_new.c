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

void landscape_quadrant_get_new
  (wlandscape_t* ls, wquadrant_t* q, int qx, int qz)
{
  q->qx = qx;
  q->qz = qz;
  if (walker_db_quadrant_retrieve(ls->walker, q) == 0) {
#ifdef _DEBUG
    fprintf(stderr, "quadrant %d, %d cached in db\n", qx, qz);
#endif
    return;
  }
#ifdef _DEBUG
  fprintf(stderr, "Initializing quadrant %d, %d\n", qx, qz);
#endif
  for (int dtx = 0; dtx < WQUADRANT_DIMENSION; dtx++) {
    for (int dtz = 0; dtz < WQUADRANT_DIMENSION; dtz++) {
      int tx = (q->qx * WQUADRANT_DIMENSION) + dtx;
      int tz = (q->qz * WQUADRANT_DIMENSION) + dtz;
      landscape_tile_get_new(tx, tz, &(q->tiles[ dtx ][ dtz ]));
    }
  }
  landscape_quadrant_optimize(ls, q);
  landscape_quadrant_village(ls, qx, qz, q);
  walker_db_quadrant_store(ls->walker, q);
}

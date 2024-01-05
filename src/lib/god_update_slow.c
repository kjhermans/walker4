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

/**
 * Brings and directs all of the evil into the world.
 */
void god_update_slow
  (walker_t* w)
{
  unsigned nflyers = 0;

  for (unsigned i=0; i < w->world.objects.count; i++) {
    wobject_t* f = wobjectlist_getptr(&(w->world.objects), i);
    if (f->type == WOBJTYPE_AFLYER) {
      int dx = w->world.player.object.position.x - f->position.x;
      int dz = w->world.player.object.position.z - f->position.z;
      ++nflyers;
      if (w->world.player.object.position.y > 3000) {
        if ((dx > -1000 && dx < 1000) || (dz > -1000 && dz < 1000)) {
          continue;
        } else {
          double a = (double)dz/(double)dx;
          f->oxz = atan(1/a);
          if (dx > 0 && dz < 0) {
            f->oxz += (3/2 * WPI);
          } else if (dx < 0 && dz > 0) {
            f->oxz += (1/2 * WPI);
          } else if (dx > 0 && dz > 0) {
//            f->oxz += WPI;
          } else  {
            f->oxz += WPI;
          }
          while (f->oxz < 0) {
            f->oxz += (2 * WPI);
          }
        }
        if (f->position.y < w->world.player.object.position.y) {
          f->oyz = .01;
        } else if (f->position.y > w->world.player.object.position.y) {
          f->oyz = (2 * WPI) - .01;
        }
      }
    }
  }
  if (nflyers < GOD_MIN_FLYERS) {
    int d = GOD_MIN_FLYERS - nflyers;
    for (int i=0; i < d; i++) {
#ifdef _DEBUG
      fprintf(stderr, "Creating adversarial flyer.\n");
#endif
      wobject_t o = { 0 };
      object_init(&o, WOBJTYPE_AFLYER);
      o.flags |= WOBJFLAG_FLYING;
      o.position.y = 3000;
      o.oxz = ((float)rand() / RAND_MAX) * WPI * 2;
      o.speed_hor = 30;
      o.subtype.flyer.circle = 1 / ((float)(rand() % 1000) + 1);
      wobjectlist_push(&(w->world.objects), o);
    }
  }
}

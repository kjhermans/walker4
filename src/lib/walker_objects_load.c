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

struct ol
{
  unsigned objcount[ WOBJTYPE_MAX ];
  unsigned maxid;
};

static
void walker_load_object
  (walker_t* w, wobject_t* o, void* arg)
{
  struct ol* ol = arg;

  fprintf(stderr, "Walker::init Loaded ");
  object_debug(o);
  o->draw = NULL;
  o->update = NULL;
  o->engage = NULL;
  if (o->id > ol->maxid) {
    ol->maxid = o->id;
  }
  if (o->type >= WOBJTYPE_MAX) {
    return; // not a valid object
  }
  (ol->objcount[ o->type ])++;
  switch (o->type) {
  case WOBJTYPE_PLAYER:
    w->world.player.object = *o;
    if (o->flags & WOBJFLAG_FLYING) {
      overlay_set_flying(1);
    }
    break;
  case WOBJTYPE_PFLYER:
    object_init(o, o->type);
    w->world.player.flyer = *o;
    if (o->flags & WOBJFLAG_FLYING) {
      w->world.player.flyer.flags &= ~(WOBJFLAG_VISIBLE);
    }
    break;
  case WOBJTYPE_AFLYER:
    object_init(o, o->type);
    wobjectlist_push(&(w->world.objects), *o);
    break;
  case WOBJTYPE_TNT:
    object_init(o, o->type);
    wobjectlist_push(&(w->world.objects), *o);
    break;
  }
}

/**
 * Loads stored objects from the database from the database on init.
 */
void walker_objects_load
  (walker_t* w)
{
  struct ol ol = { 0 };
  walker_db_object_iterate(w, walker_load_object, &ol);
  if (0 == ol.objcount[ WOBJTYPE_PLAYER]) {
    fprintf(stderr, "Walker::init Newly create player.\n");
    player_init(&(w->world.player));
  } else {
    int tx, tz;
    int qx, qz;
    landscape_pos2tile(w->world.player.object.position.x,
                       w->world.player.object.position.z,
                       &tx, &tz);
    landscape_tile_2quadrant(tx, tz, &qx, &qz, 0, 0);
    landscape_cache_update(&(w->world.landscape), qx-1, qz-1);
  }
  if (0 == ol.objcount[ WOBJTYPE_PFLYER ]) {
    fprintf(stderr, "Walker::init Newly create player flyer.\n");
    flyer_init(&(w->world.player.flyer), WOBJTYPE_PFLYER);
  }
}

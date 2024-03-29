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

static char _dialogkeys[ 32 ] = { 0 };
static char* dialogkeys = 0;
static void(*dialogfnc)(wplayer_t*,walker_t*,char) = 0;

/**
 *
 */
void player_handle_keys
  (wplayer_t* p, walker_t* w, int key)
{
  (void)key;

  if (pkb_key_pressed(FW_KEY_ESCAPE)) {
    fprintf(stderr, "Walker::Shutdown\n");
    walker_on_exit();
    sys_shutdown();
  } else if (pkb_key_held(FW_KEY_ARROW_RIGHT)) {
    object_turnright(&(p->object), PLAYER_TURNRATE);
  } else if (pkb_key_held(FW_KEY_ARROW_LEFT)) {
    object_turnleft(&(p->object), PLAYER_TURNRATE);
  } else if (pkb_key_held(FW_KEY_ARROW_UP)) {
    if (p->object.mode == WOBJMODE_FLYING) {
      object_turndown(&(p->object), PLAYER_TURNRATE);
    } else {
      object_turnup(&(p->object), PLAYER_TURNRATE);
    }
  } else if (pkb_key_held(FW_KEY_ARROW_DOWN)) {
    if (p->object.mode == WOBJMODE_FLYING) {
      object_turnup(&(p->object), PLAYER_TURNRATE);
    } else {
      object_turndown(&(p->object), PLAYER_TURNRATE);
    }
  } else if (pkb_key_held(w->config.keybindings.forward)) {
    object_forward(&(p->object));
  } else if (pkb_key_held(w->config.keybindings.backward)) {
    object_backward(&(p->object));
#ifdef _DEBUG
  } else if (pkb_key_pressed('e')) {
    landscape_explosion(
      &(w->world.landscape),
      p->object.position.x,
      p->object.position.z,
      -1 // replace with a 3defined GROUNDLEVEL
    );
  } else if (pkb_key_pressed('b')) {
    p->object.speed_hor = 0;
  } else if (pkb_key_pressed('l')) {
    p->object.oyz = 0;
  } else if (pkb_key_pressed('a')) {
    if (p->object.mode == WOBJMODE_FLYING) {
       p->object.mode = WOBJMODE_WALKING;
    } else {
       p->object.mode = WOBJMODE_FLYING;
    }
#endif
  } else if (pkb_key_pressed('x')) {
    if (p->objectinview) {
      object_engage(p->objectinview, w);
    }
  } else if (pkb_key_pressed('d')) {
    w->display.show_stats = !(w->display.show_stats);
    text_object_set_visibility(w->display.overlay.stats, w->display.show_stats);
  } else if (pkb_key_pressed('i')) {
    w->display.show_inventory = !(w->display.show_inventory);
    overlay_set_inventory(w->display.show_inventory);
  } else if (pkb_key_pressed('r')) {
    player_release(p, w);
  } else if (pkb_key_pressed('h')) {
    w->display.show_help = !(w->display.show_help);
    fprintf(stderr, "To toggle help, use 'h' again.\n");
    text_object_set_visibility(w->display.overlay.help, w->display.show_help);
  } else if (pkb_key_pressed('f')) {
    player_flying(p, w);
  }
  if (pkb_key_pressed(w->config.keybindings.jump)) {
    if (p->object.cache.ground.supported) {
      p->object.speed_vert = PLAYER_JUMPSPEED;
    }
  }
  if (dialogkeys) {
    char* dk = dialogkeys;
    while (*dk) {
      if (pkb_key_pressed(*dk) && dialogfnc) {
        dialogfnc(p, w, *dk);
      }
      ++dk;
    }
  }
}

void player_flying
  (wplayer_t* p, walker_t* w)
{
  if (p->object.mode == WOBJMODE_FLYING) {
    if (p->object.cache.ground.supported) {
      p->object.mode = WOBJMODE_WALKING;
      p->object.oyz = 0;
      p->flyer.position = p->object.position;
      p->flyer.flags |= WOBJFLAG_VISIBLE;
      overlay_set_flying(0);
      walker_warn(w, "You have dismounted your flyer.");
    } else {
      walker_warn(w, "You have to be grounded to dismount your flyer.");
    }
  } else {
    int dx = p->object.position.x - p->flyer.position.x;
    int dz = p->object.position.z - p->flyer.position.z;
    int d = sqrt(dx*dx + dz*dz);
    if (d < 2 * WTILESIZE) {
      p->flyer.flags &= ~(WOBJFLAG_VISIBLE);
      p->object.mode = WOBJMODE_FLYING;
      p->object.oyz = 0;
      overlay_set_flying(1);
      walker_warn(w, "You have mounted your flyer.");
    } else {
      walker_warn(w, "You are not close enough to your flyer.");
    }
  }
}

/**
 *
 */
void player_update
  (wplayer_t* p, wlandscape_t* ls)
{
  pt2d_t q = p->object.cache.quadrant;
  int m;

  m = object_move(&(p->object), ls);
  object_gravity(&(p->object));
  object_friction(&(p->object));

  if (m) {
    if (q.x != p->object.cache.quadrant.x || q.z != p->object.cache.quadrant.z)
    {
      landscape_cache_update(
        ls,
        p->object.cache.quadrant.x-1,
        p->object.cache.quadrant.z-1
      );
    }
  }
  if (p->object.mode == WOBJMODE_FLYING) {
    hud.angle_compass = (p->object.oxz / WDEGRAD);
    hud.angle_ascend = (p->object.oyz / WDEGRAD);
    hud.my_height = p->object.position.y;
  }
}

void player_dialog
  (
    wplayer_t* p,
    walker_t* w,
    char* text,
    char* opts,
    void(*fnc)(wplayer_t*,walker_t*,char)
  )
{
  char str[ 512 ];
  (void)p;

  snprintf(str, sizeof(str), "%s [%s]", text, opts);
  strncpy(_dialogkeys, opts, strlen(opts));
  dialogkeys = _dialogkeys;
  dialogfnc = fnc;
  walker_warn(w, str);
}

void player_release_charge
  (wplayer_t* p, walker_t* w)
{
  wobject_t o = { 0 };

  object_init(&o, WOBJTYPE_TNT);
  o.flags |= WOBJFLAG_VISIBLE|WOBJFLAG_INITIALIZED;
  o.position.x = p->object.position.x + 64;
  o.position.z = p->object.position.z;
  o.position.y = p->object.position.y;
  wobjectlist_push(&(w->world.objects), o);
}

static
void player_release2
  (wplayer_t* p, walker_t* w, char key)
{
  unsigned ii = key - '0';

  if (ii < 10 && p->inventory[ ii ].amount) { // INVENTORYSIZE definify
    switch (p->inventory[ ii ].item) {
    case 'c': // definify
      --(p->inventory[ ii ].amount);
      player_release_charge(p, w);
      break;
    }
  }
}

void player_release
  (wplayer_t* p, walker_t* w)
{
  char opts[ 32 ];
  char* opt = opts;

  for (unsigned i=0; i < 10; i++) {
    if (p->inventory[ i ].amount) {
      *opt++ = '0' + i;
    }
  }
  *opt++ = 0;
  player_dialog(p, w, "Press to release", opts, player_release2);
}

void player_init
  (wplayer_t* p)
{
  p->object.position.x = WQUADRANT_DIMENSION * WTILESIZE / 2;
  p->object.position.y = 4096;
  p->object.position.z = WQUADRANT_DIMENSION * WTILESIZE / 2;
  p->object.mode = WOBJMODE_WALKING;
  p->flyer.flags |= WOBJFLAG_VISIBLE;
  p->flyer.position.x = 1024 + WQUADRANT_DIMENSION * WTILESIZE / 2;
  p->flyer.position.y = 4096;
  p->flyer.position.z = 1024 + WQUADRANT_DIMENSION * WTILESIZE / 2;
}

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

#include <stdio.h>
#include <time.h>

#include "walker.h"

walker_t* wglobal;

static char*           helptext =
  "Help:\n\n"
  "h         Toggle this message\n"
  "arrows    Change direction\n"
  "w / s     Accellerate / decellerate\n"
  "space     Jump (when not flying)\n"
  "f         Access / leave flyer\n"
  "d         Toggle statistics display\n"
  "i         Toggle inventory display\n"
  "Esc       Quit\n"
;

static char            statsbuffer[ 1024 ];
static unsigned        fpsclock = 0;
static int             fps = 0;
static time_t          t0, t, tlast;
static const unsigned  t_cycle = 60;
static const unsigned  t_bgswitch = 24;
static const unsigned  t_background[][ 4 ] = {
  { 150, 200, 230, 1 },
  { 180, 220, 255, 1 },
  { 180, 220, 255, 1 },
  { 180, 220, 255, 1 },
  { 180, 220, 255, 1 },
  { 180, 220, 255, 1 },
  { 180, 220, 255, 1 },
  { 180, 220, 255, 1 },
  { 180, 220, 255, 1 },
  { 150, 200, 230, 1 },
  { 100, 150, 200, 1 },
  { 50, 100, 150, 1 },
  { 0, 50, 90, 0 },
  { 0, 0, 50, 0 },
  { 0, 0, 20, 0 },
  { 0, 0, 0, 0 },
  { 0, 0, 0, 0 },
  { 0, 0, 0, 0 },
  { 0, 0, 0, 0 },
  { 0, 0, 20, 0 },
  { 0, 0, 50, 0 },
  { 0, 50, 90, 0 },
  { 50, 100, 150, 0 },
  { 100, 150, 200, 0 }
};

/**
 *
 */
void walker_configure
  (walker_t* w, char* path)
{
  char* buf = 0;
  unsigned len = 0;
  char dfltcnf[ 1024 ];

  fprintf(stderr, "Walker::configure\n");
  if (path == NULL) {
    snprintf(dfltcnf, sizeof(dfltcnf), "%s" WDEFAULTCONFIG, getenv("HOME"));
    path = dfltcnf;
  }
  if (absorb_file(path, (unsigned char**)(&buf), &len) == 0) {
    if ((w->json = json_parse(buf)) == NULL) {
      fprintf(stderr, "WARNING: Config file '%s' JSON parse error.\n", path);
    }
    free(buf);
  }
}

void walker_update_slow
  (walker_t* w)
{
  god_update_slow(w);
}

void walker_update
  (walker_t* w)
{
  int tx, tz;
  int qx, qz;

  landscape_pos2tile(
    w->world.player.object.position.x,
    w->world.player.object.position.z,
    &tx, &tz
  );
  landscape_tile_2quadrant(tx, tz, &qx, &qz, 0, 0);

  player_handle_keys(&(w->world.player), w, 0);
  player_update(&(w->world.player), &(w->world.landscape));
  flyer_update(&(w->world.player.flyer), w);
  god_update(w);

  for (unsigned i=0; i < w->world.objects.count; i++) {
    wobject_t* o = wobjectlist_getptr(&(w->world.objects), i);
    if (o->update) {
      o->update(o, w);
    }
  }

  snprintf(statsbuffer, sizeof(statsbuffer)
    , "px:%d, pz:%d, py:%d, tx:%d, tz:%d, qx:%d, qz:%d, "
      "oxz:%d, oyz:%d, spd:%d, fll: %d"
    , w->world.player.object.position.x
    , w->world.player.object.position.z
    , w->world.player.object.position.y
    , tx, tz, qx, qz
    , (int)(w->world.player.object.oxz / WDEGRAD)
    , (int)(w->world.player.object.oyz / WDEGRAD)
    , w->world.player.object.speed_hor
    , w->world.player.object.speed_vert
  );
  text_object_set_text(w->display.overlay.stats, statsbuffer);
  if (w->display.overlay.warn_timeout) {
    --(w->display.overlay.warn_timeout);
  } else {
    text_object_set_visibility(w->display.overlay.warn, 0);
  }
}

void walker_set_windowsize
  (int w, int h)
{
  (void)w; (void)h;
  fprintf(stderr, "TODO\n");
}

static unsigned colors[] = WCOLORS;
static unsigned colors_hard[] = WCOLORS_HARD;
static unsigned colors_chasm[] = WCOLORS_CHASM;

void PL_OBJ_debug
  (struct PL_OBJ* obj)
{
  fprintf(stderr, "obj nverts=%d npolys=%d\n", obj->n_verts, obj->n_polys);
  for (int i=0; i < obj->n_verts; i++) {
    fprintf(stderr, "vert: %d %d %d %d\n"
      , obj->verts[ i*4 + 0 ]
      , obj->verts[ i*4 + 1 ]
      , obj->verts[ i*4 + 2 ]
      , obj->verts[ i*4 + 3 ]);
  }
  for (int i=0; i < obj->n_polys; i++) {
    fprintf(stderr, "poly:\n");
    for (int j=0; j < obj->polys[i].n_verts; j++) {
      fprintf(stderr, "  vert: %d %d %d\n"
        , obj->polys[i].verts[j*3+0]
        , obj->polys[i].verts[j*3+1]
        , obj->polys[i].verts[j*3+2]);
    }
  }
}

static
void update
  ()
{
  t = time(0);
  if (t > tlast) {
    tlast = t;
    walker_update_slow(wglobal);
  }
  walker_update(wglobal);
}

int walker_is_day
  ()
{
  const int tod = ((t - t0) / t_cycle) % t_bgswitch;
  const unsigned* rgb_back = t_background[ tod ];
  return rgb_back[ 3 ];
}

static
void display
  ()
{
  const int tod = ((t - t0) / t_cycle) % t_bgswitch;
  const unsigned* rgb_back = t_background[ tod ];

  if (wglobal->world.player.object.position.y < 512) {
    PL_clear_vp(0, 0, 32);
    set_fade_to_white(0);
  } else {
    PL_clear_vp(rgb_back[ 0 ], rgb_back[ 1 ], rgb_back[ 2 ]);
    if (!rgb_back[ 3 ]) {
      set_fade_to_white(0);
    } else {
      set_fade_to_white(1);
    }
  }

  if (wglobal->world.player.object.position.y > 3000) {
    overlay_set_redlamp(1);
  } else {
    overlay_set_redlamp(0);
  }

  PL_polygon_count = 0;
    
  /* define camera orientation */
  PL_set_camera(
    0,
    wglobal->world.player.object.position.y,
    0,
    (int)(wglobal->world.player.object.oyz / PL_RAD256),
    (int)(wglobal->world.player.object.oxz / PL_RAD256)
  );

  /* retrieve field of vision */
  vec2d_t vision;
  object_get_vision( &(wglobal->world.player.object), &vision);

  for (int tx = vision.o.x; tx < vision.d.x; tx++) {
    for (int tz = vision.o.z; tz < vision.d.z; tz++) {
      wtile_t tile;
      landscape_tile_get(&(wglobal->world.landscape), tx, tz, &tile);
      int tr[ 8 ][ 3 ][ 3 ];
      landscape_tile_get_triangles(&(wglobal->world.landscape), tx, tz, tr);
      for (int i=0; i < 8; i++) {
        unsigned color;
        switch (tile.hardness) {
        case 2:
          color = colors_chasm[ tile.color ];
          break;
        case 3:
          color = colors_hard[ tile.color ];
          break;
        default:
          color = colors[ tile.color ];
          break;
        }
#ifdef _DEBUG
        if (tx % WQUADRANT_DIMENSION == 0 || tz % WQUADRANT_DIMENSION == 0) {
          color = 0xff0000;
        }
#endif
        struct PL_POLY poly = {
          .n_verts = 4,
          .verts[ 0 ] = 1,
          .verts[ 1 ] = 0,
          .verts[ 2 ] = 0,
          .verts[ 3 ] = 0,
          .verts[ 4 ] = 0,
          .verts[ 5 ] = 0,
          .verts[ 6 ] = 2,
          .verts[ 7 ] = 0,
          .verts[ 8 ] = 0,
          .verts[ 9 ] = 1,
          .verts[ 10 ] = 0,
          .verts[ 11 ] = 0,
          .color = color
        };
        int _tr[ 12 ] = {
          tr[i][0][0], tr[i][0][1], tr[i][0][2], 0,
          tr[i][1][0], tr[i][1][1], tr[i][1][2], 0,
          tr[i][2][0], tr[i][2][1], tr[i][2][2], 0
        };
        struct PL_OBJ obj = {
          .verts = _tr,
          .n_verts = 3,
          .polys = &poly,
          .n_polys = 1
        };
        {
          int x = (tx * WTILESIZE) - wglobal->world.player.object.position.x;
          int y = 0;
          int z = (tz * WTILESIZE) - wglobal->world.player.object.position.z;
          PL_mst_push();
          PL_mst_translate(x, y, z);
          PL_render_object(&obj);
          PL_mst_pop();
        }
      }

      if (tile.elevation[ 4 ] < 600) {
        struct PL_POLY waterpoly = {
          .n_verts = 5,
          .verts[ 0 ] = 0,
          .verts[ 1 ] = 0,
          .verts[ 2 ] = 0,
          .verts[ 3 ] = 3,
          .verts[ 4 ] = 0,
          .verts[ 5 ] = 0,
          .verts[ 6 ] = 2,
          .verts[ 7 ] = 0,
          .verts[ 8 ] = 0,
          .verts[ 9 ] = 1,
          .verts[ 10 ] = 0,
          .verts[ 11 ] = 0,
          .verts[ 12 ] = 0,
          .verts[ 13 ] = 0,
          .verts[ 14 ] = 0,
          .color = 0xff0000ff
        };
        int waterverts[] = {
          0, 512, 0, 0,
          WTILESIZE, 512, 0, 0,
          WTILESIZE, 512, WTILESIZE, 0,
          0, 512, WTILESIZE, 0
        };
        struct PL_OBJ waterobj = {
          .verts = waterverts,
          .n_verts = 4,
          .polys = &waterpoly,
          .n_polys = 1
        };
        {
          int x = (tx * WTILESIZE) - wglobal->world.player.object.position.x;
          int y = 0;
          int z = (tz * WTILESIZE) - wglobal->world.player.object.position.z;
          PL_mst_push();
          PL_mst_translate(x, y, z);
          PL_render_object(&waterobj);
          PL_mst_pop();
        }
      }

    }
  }

  wglobal->world.player.objectinview = 0;
  for (unsigned i=0; i < wglobal->world.objects.count; i++) {
    wobject_t* o = wobjectlist_getptr(&(wglobal->world.objects), i);
    object_draw(o, wglobal, &(wglobal->world.player), vision);
  }
  object_draw(
    &(wglobal->world.player.flyer),
    wglobal,
    &(wglobal->world.player),
    vision
  );

  if (clk_sample() > fpsclock) {
    fps = sys_getfps();
    fpsclock = clk_sample() + 1000;
    printf("FPS: %d, ToD: %d, %s\n", fps, tod, statsbuffer);
  }

 /* update window and sync */
  vid_blit();
  vid_sync();
}

/**
 *
 */
void walker_show
  (walker_t* w, int argc, char* argv[])
{
  fprintf(stderr, "Walker::show\n");

  (void)argc;
  (void)argv;
  sys_init();
  sys_updatefunc(update);
  sys_displayfunc(display);

  clk_mode(FW_CLK_MODE_HIRES);
  pkb_reset();
  sys_sethz(70);
  sys_capfps(0);

  if (vid_open(
        "PL", w->display.width, w->display.height, 1, FW_VFLAG_VIDFAST)
        != FW_VERR_OK)
  {
    fprintf(stderr, "unable to create window\n");
    return;
  }

  /* give the video memory to PL */
  PL_init(vid_getinfo()->video, w->display.width, w->display.height);
}

/**
 *
 */
void walker_run
  (walker_t* w)
{
  fprintf(stderr, "Walker::run\n");
  wglobal = w;
  sys_start();
}

void walker_inventory
  (void* arg, unsigned i, int* isfilled, char* c, unsigned* amount)
{
  walker_t* w = arg;
  if (w->world.player.inventory[ i ].amount == 0) {
    *isfilled = 0;
  } else {
    *isfilled = 1;
    *c = (char)(w->world.player.inventory[ i ].item);
    *amount = w->world.player.inventory[ i ].amount;
  }
}

/**
 *
 */
void walker_init
  (walker_t* w, unsigned* optseed, int reset)
{
  unsigned seed;

  fprintf(stderr, "Walker::init (PL); press 'h' for help\n");
  memset(w, 0, sizeof(*w));

  if (optseed) {
    seed = *optseed;
  } else {
    seed = WDEFAULTSEED;
  }
  walker_db_init(w, seed, reset);

  world_init(&(w->world), seed, w);
  w->display.width = WDISPLAY_DEFAULT_WIDTH;
  w->display.height = WDISPLAY_DEFAULT_HEIGHT;
  //w->world.player.object.supported = 1;
  //w->world.player.object.position.y = 0;
  w->config.keybindings.forward = 'w';
  w->config.keybindings.backward = 's';
  w->config.keybindings.jump = ' ';
  w->display.overlay.stats = overlay_add_textobject(20, 20, "Waiting..");
  w->display.overlay.warn = overlay_add_textobject(20, 40, "--");
  w->display.overlay.help = overlay_add_textobject(20, 60, helptext);
  text_object_set_visibility(w->display.overlay.stats, 0); // todo
  text_object_set_visibility(w->display.overlay.help, 0);
  overlay_set_callback(walker_inventory, w);
  w->world.player.inventory[ 0 ].item = 'c';
  w->world.player.inventory[ 0 ].amount = 32;
  walker_objects_load(w);
  t = t0 = tlast = time(0);
}

static
char* options = 0;

static
void(*optfnc)(walker_t*,char) = 0;

void walker_warn
  (walker_t* w, char* str)
{
  text_object_set_text(w->display.overlay.warn, str);
  text_object_set_visibility(w->display.overlay.warn, 1);
  w->display.overlay.warn_timeout = 64;
  fprintf(stderr, "Walker::warn %s\n", str);
}

void walker_dialog
  (walker_t* w, char* str, char* opt, void(*fnc)(walker_t*,char))
{
  char text[ 128 ];

  snprintf(text, sizeof(text), "%s [%s]", str, opt);
  walker_warn(w, text);
  options = opt;
  optfnc = fnc;
}

void walker_on_exit
  ()
{
  for (unsigned i=0; i < wglobal->world.objects.count; i++) {
    wobject_t* o = wobjectlist_getptr(&(wglobal->world.objects), i);
    walker_db_object_store(wglobal, o);
  }
  walker_db_object_store(wglobal, &(wglobal->world.player.object));
  walker_db_object_store(wglobal, &(wglobal->world.player.flyer));
  for (int i=0; i < 3; i++) {
    for (int j=0; j < 3; j++) {
      walker_db_quadrant_store(
        wglobal,
        &(wglobal->world.landscape.cache.mem.quadrant[ i ][ j ])
      );
    }
  }
  fprintf(stderr, "Walker::OnExit. Saved all objects.\n");
}

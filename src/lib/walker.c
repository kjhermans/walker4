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
  "d         Show stats\n"
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
  fprintf(stderr, "Walker::configure\n");
  if (path) {
    char* buf = 0;
    unsigned len = 0;
    if (absorb_file(path, (unsigned char**)(&buf), &len) == 0) {
      if ((w->json = json_parse(buf)) == NULL) {
        fprintf(stderr, "WARNING: Config file '%s' JSON parse error.\n", path);
      }
      free(buf);
    }
  }
}

void walker_update
  (walker_t* w)
{
  int tx, tz;
  int qx, qz;

  t = time(0);
  landscape_pos2tile(
    wglobal->world.player.object.position.x,
    wglobal->world.player.object.position.z,
    &tx, &tz
  );
  landscape_tile2quadrant(tx, tz, &qx, &qz, 0, 0);

  player_handle_keys(&(w->world.player), wglobal, 0);
  player_update(&(w->world.player), &(w->world.landscape));
  flyers_update(&(wglobal->world));
  flyer_update(&(wglobal->world), &(wglobal->world.player.flyer));
  snprintf(statsbuffer, sizeof(statsbuffer)
    , "px:%d, pz:%d, py:%d, tx:%d, tz:%d, qx:%d, qz:%d, "
      "oxz:%d, oyz:%d, spd:%d, fll: %d"
    , wglobal->world.player.object.position.x
    , wglobal->world.player.object.position.z
    , wglobal->world.player.object.position.y
    , tx, tz, qx, qz
    , (int)(wglobal->world.player.object.oxz / WDEGRAD)
    , (int)(wglobal->world.player.object.oyz / WDEGRAD)
    , wglobal->world.player.object.speed_hor
    , wglobal->world.player.object.speed_vert
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

#ifdef _USE_PL
/*
static
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
*/

static
void update
  ()
{
  walker_update(wglobal);
}

static
void display
  ()
{
  const int tod = ((t - t0) / t_cycle) % t_bgswitch;
  const unsigned* rgb_back = t_background[ tod ];

  /* clear viewport */
  PL_clear_vp(rgb_back[ 0 ], rgb_back[ 1 ], rgb_back[ 2 ]);

  if (rgb_back[ 3 ]) {
    set_fade_to_white(1);
  } else {
    set_fade_to_white(0);
  }

  PL_polygon_count = 0;
    
  /* define camera orientation */
  PL_set_camera(
#ifdef _PL_ABSOLUTE
    wglobal->world.player.object.position.x,
    wglobal->world.player.object.position.y,
    wglobal->world.player.object.position.z,
#else
    0,
    wglobal->world.player.object.position.y,
    0,
#endif
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
        int x = tx * WTILESIZE;
        int y = 0; //tile.elevation * WTILESIZE;
        int z = tz * WTILESIZE;
#ifndef _PL_ABSOLUTE
        x -= wglobal->world.player.object.position.x;
        z -= wglobal->world.player.object.position.z;
#endif
        {
          PL_mst_push();
          PL_mst_translate(x, y, z);
          PL_render_object(&obj);
          PL_mst_pop();
        }
      }

      if (tile.elevation[ 4 ] < 512) {
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
        int x = tx * WTILESIZE;
        int y = 0; //tile.elevation * WTILESIZE;
        int z = tz * WTILESIZE;
#ifndef _PL_ABSOLUTE
        x -= wglobal->world.player.object.position.x;
        z -= wglobal->world.player.object.position.z;
#endif
        {
          PL_mst_push();
          PL_mst_translate(x, y, z);
          PL_render_object(&waterobj);
          PL_mst_pop();
        }
      }

    }
  }

  flyers_draw(
    &(wglobal->world),
    (pt2d_t){
      .x = wglobal->world.player.object.position.x,
      .z = wglobal->world.player.object.position.z
    },
    &vision
  );
  flyer_draw(
    &(wglobal->world.player.flyer),
    wglobal->world.player.flyer.object.position.x - 
      wglobal->world.player.object.position.x,
    wglobal->world.player.flyer.object.position.z - 
      wglobal->world.player.object.position.z
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
#endif // _USE_PL

#ifdef _USE_GL

static unsigned char keypress[ 256 ] = { 0 };
static unsigned char specialkeypress[ 4 ] = { 0 };
static int keypressed = 0;
static int specialkeypressed = 0;

static
void keyboard
  (unsigned char key, int x, int y)
{
  //player_handle_keys(&(wglobal->world.player), wglobal, (int)key);
  keypress[ key ] = 1;
  keypressed = 1;
}

static
void keyboard_s
  (int key, int x, int y)
{
  //player_handle_keys(&(wglobal->world.player), wglobal, key);
  switch (key) {
  case GLUT_KEY_LEFT:  specialkeypress[ 0 ] = 1; break;
  case GLUT_KEY_RIGHT: specialkeypress[ 1 ] = 1; break;
  case GLUT_KEY_UP:    specialkeypress[ 2 ] = 1; break;
  case GLUT_KEY_DOWN:  specialkeypress[ 3 ] = 1; break;
  }
  specialkeypressed = 1;
}

static
void keyboard_up
  (unsigned char key, int x, int y)
{
  //player_handle_keys(&(wglobal->world.player), wglobal, (int)key);
  keypress[ key ] = 0;
  keypressed = 0;
  for (unsigned i=0; i < 256; i++) {
    if (keypress[ i ]) {
      keypressed = 1;
    }
  }
}

static
void keyboard_s_up
  (int key, int x, int y)
{
  //player_handle_keys(&(wglobal->world.player), wglobal, key);
  switch (key) {
  case GLUT_KEY_LEFT:  specialkeypress[ 0 ] = 0; break;
  case GLUT_KEY_RIGHT: specialkeypress[ 1 ] = 0; break;
  case GLUT_KEY_UP:    specialkeypress[ 2 ] = 0; break;
  case GLUT_KEY_DOWN:  specialkeypress[ 3 ] = 0; break;
  }
  if (!specialkeypress[ 0 ] 
      && !specialkeypress[ 1 ] 
      && !specialkeypress[ 2 ] 
      && !specialkeypress[ 3 ])
  {
    specialkeypressed = 0;
  }
}

static
void update_key
  ()
{
  if (keypressed) {
    for (unsigned i=0; i < 256; i++) {
      if (keypress[ i ]) {
        player_handle_keys(&(wglobal->world.player), wglobal, i);
      }
    }
  }
  if (specialkeypressed) {
    if (specialkeypress[ 0 ]) {
      player_handle_keys(&(wglobal->world.player), wglobal, GLUT_KEY_LEFT);
    }
    if (specialkeypress[ 1 ]) {
      player_handle_keys(&(wglobal->world.player), wglobal, GLUT_KEY_RIGHT);
    }
    if (specialkeypress[ 2 ]) {
      player_handle_keys(&(wglobal->world.player), wglobal, GLUT_KEY_UP);
    }
    if (specialkeypress[ 3 ]) {
      player_handle_keys(&(wglobal->world.player), wglobal, GLUT_KEY_DOWN);
    }
  }
}

#define SCALE 16

static
void display_tile
  (int tx, int tz)
{
  wtile_t tile;
  uint32_t c;
  float r, g, b;
  int tr[ 8 ][ 3 ][ 3 ];

  landscape_tile_get(&(wglobal->world.landscape), tx, tz, &tile);
  c = colors[ tile.color ];
  if (tile.hardness == 3) {
    c = colors_hard[ tile.color ];
  }
  r = ( (float)((c>>16)&0xff)/256);
  g = ( (float)((c>>8)&0xff)/256);
  b = ( (float)((c)&0xff)/256);
  landscape_tile_get_triangles(&(wglobal->world.landscape), tx, tz, tr);
  for (int i=0; i < 8; i++) {
    float v[ 9 ] = {
      (tx*WTILESIZE) + tr[i][2][0], tr[i][2][1], (tz*WTILESIZE) + tr[i][2][2],
      (tx*WTILESIZE) + tr[i][1][0], tr[i][1][1], (tz*WTILESIZE) + tr[i][1][2],
      (tx*WTILESIZE) + tr[i][0][0], tr[i][0][1], (tz*WTILESIZE) + tr[i][0][2]
    };
    glBegin(GL_TRIANGLES);
    glColor3f(r, g, b);
    glVertex3f(v[ 0 ] / SCALE, v[ 1 ] / SCALE, v[ 2 ] / SCALE);
    glVertex3f(v[ 3 ] / SCALE, v[ 4 ] / SCALE, v[ 5 ] / SCALE);
    glVertex3f(v[ 6 ] / SCALE, v[ 7 ] / SCALE, v[ 8 ] / SCALE);
    glEnd();
  }
}

static
void display
  ()
{
  glLoadIdentity();

  glRotatef(wglobal->world.player.object.oyz / WDEGRAD, 1, 0, 0);
  glRotatef(wglobal->world.player.object.oxz / WDEGRAD, 0, 1, 0);

  glTranslatef(
    ((float)wglobal->world.player.object.position.x) / SCALE,
    -((((float)wglobal->world.player.object.position.y) / SCALE) + 16),
    ((float)wglobal->world.player.object.position.z) / SCALE
  );

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* retrieve field of vision */
  int txmin, txmax, tzmin, tzmax;
  object_get_vision(
    &(wglobal->world.player.object),
    &txmin,
    &txmax,
    &tzmin,
    &tzmax
  );

  for (int tx = txmin; tx < txmax; tx++) {
    for (int tz = tzmin; tz < tzmax; tz++) {
      display_tile(tx, tz);
    }
  }

  glutSwapBuffers();
}

/*
static
void mouse
  (int button, int state, int x, int y)
{
}

static
void motion
  (int x, int y)
{
}
*/

static
void* update
  (void* arg)
{
  (void)arg;

  while (1) {
    usleep(25000);
    t = time(0);
    if (wglobal == 0) {
      continue;
    }
    if (tlast < t) {
      int tx, tz, qx, qz;
    
      landscape_pos2tile(
        wglobal->world.player.object.position.x,
        wglobal->world.player.object.position.z,
        &tx, &tz
      );
      landscape_tile2quadrant(tx, tz, &qx, &qz, 0, 0);
      snprintf(statsbuffer, sizeof(statsbuffer)
        , "px:%d, pz:%d, py:%d, tx:%d, tz:%d, qx:%d, qz:%d, "
          "oxz:%d, oyz:%d, spd:%d, fll:%d"
        , wglobal->world.player.object.position.x
        , wglobal->world.player.object.position.z
        , wglobal->world.player.object.position.y
        , tx, tz, qx, qz
        , (int)(wglobal->world.player.object.oxz / WDEGRAD)
        , (int)(wglobal->world.player.object.oyz / WDEGRAD)
        , wglobal->world.player.object.speed_hor
        , wglobal->world.player.object.speed_vert
      );
      fprintf(stderr, "Walker::Display %s\n", statsbuffer);
      tlast = t;
    }

    update_key();
    player_update(&(wglobal->world.player), &(wglobal->world.landscape));
  }
  return NULL;
}
#endif

/**
 *
 */
void walker_show
  (walker_t* w, int argc, char* argv[])
{
  fprintf(stderr, "Walker::show\n");

#ifdef _USE_PL
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

#elif defined _USE_GL
  glutInit(&argc, argv);
//  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(80, 80);
  glutInitWindowSize(WDISPLAY_DEFAULT_WIDTH, WDISPLAY_DEFAULT_HEIGHT);
  glutCreateWindow("Walker");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutKeyboardUpFunc(keyboard_up);
  glutSpecialFunc(keyboard_s);
  glutSpecialUpFunc(keyboard_s_up);
  glutIgnoreKeyRepeat(1);
//  glutMouseFunc(mouse);
//  glutMotionFunc(motion);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  // Set the current clear color to sky blue and the current drawing color to
  // white.
  glClearColor(0.1, 0.39, 0.88, 1.0);
  glColor3f(1.0, 1.0, 1.0);

  // Tell the rendering engine not to draw backfaces.  Without this code,
  // all four faces of the tetrahedron would be drawn and it is possible
  // that faces farther away could be drawn after nearer to the viewer.
  // Since there is only one closed polyhedron in the whole scene,
  // eliminating the drawing of backfaces gives us the realism we need.
  // THIS DOES NOT WORK IN GENERAL.
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // Set the camera lens so that we have a perspective viewing volume whose
  // horizontal bounds at the near clipping plane are -2..2 and vertical
  // bounds are -1.5..1.5.  The near clipping plane is 1 unit from the camera
  // and the far clipping plane is 40 units away.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-2, 2, -1.5, 1.5, 1, 320);
//  glFrustum(-2, 2, -1.5, 1.5, 1, 40);
  //glFrustum(-200, 200, -150, 150, 1, 4000);

  // Set up transforms so that the tetrahedron which is defined right at
  // the origin will be rotated and moved into the view volume.  First we
  // rotate 70 degrees around y so we can see a lot of the left side.
  // Then we rotate 50 degrees around x to "drop" the top of the pyramid
  // down a bit.  Then we move the object back 3 units "into the screen".
  glMatrixMode(GL_MODELVIEW);
//  glLoadIdentity();

#endif
}

/**
 *
 */
void walker_run
  (walker_t* w)
{
  fprintf(stderr, "Walker::run\n");
  wglobal = w;

#ifdef _USE_PL
  sys_start();
#elif defined _USE_GL
  glutMainLoop();
#endif
}

/**
 *
 */
void walker_init
  (walker_t* w, unsigned* optseed)
{
#ifdef _USE_PL
  fprintf(stderr, "Walker::init (PL); press 'h' for help\n");
#elif defined _USE_GL
  pthread_t t;
fprintf(stderr, "Creating thread.\n");
  pthread_create(&t, 0, update, 0);
  fprintf(stderr, "Walker::init (OpenGL); press 'h' for help\n");
#endif
  fprintf(stderr, "Walker::init\n");
  memset(w, 0, sizeof(*w));
  landscape_init(&(w->world.landscape), optseed);
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
  flyers_init(&(w->world));
  player_init(&(w->world.player));
  t = t0 = tlast = time(0);
}

void walker_warn
  (walker_t* w, char* str)
{
  text_object_set_text(w->display.overlay.warn, str);
  text_object_set_visibility(w->display.overlay.warn, 1);
  w->display.overlay.warn_timeout = 64;
}


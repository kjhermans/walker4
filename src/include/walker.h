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

#ifndef _WALKER_H_
#define _WALKER_H_

#include <pl.h>
#include <fw/fw.h>
#include <sdbm_tree/td.h>
#include <inttypes.h>

#ifdef _DEBUG
#define ASSERT(cnd) \
{ if (!(cnd)) { \
  fprintf(stderr, "Assertion '%s' failed at %s:%d\n", \
    #cnd, __FILE__, __LINE__); \
}}
#else
#define ASSERT(exp)
#endif

typedef struct walker walker_t;

#define WPI 3.141592
#define WDEGRAD (WPI / 180)

/**
 * You can choose to have PL or openGL for 3D graphics
 */

#define PL_RAD256 (WPI / 128)

#define WDEFAULTSEED        0xf04d2965
#define WDEFAULTCONFIG      "/.walker.conf"
#define GRAVITY             2
#define TERMINALVELOCITY    -64
#define MAXSPEED            32
#define MINSPEED            -8
#define FLOOR               0
#define CEILING             4096
/* For an unlimited world, set them all to zero */
#define WPOS_XMIN           0 //-((1<<16) - 4096)
#define WPOS_XMAX           0 //((1<<16) - 4096)
#define WPOS_ZMIN           0 //-((1<<16) - 4096)
#define WPOS_ZMAX           0 //((1<<16) - 4096)

typedef struct
{
  int                       x;
  int                       z;
}
pt2d_t;

typedef struct
{
  pt2d_t                    o;
  pt2d_t                    d;
}
vec2d_t;

typedef struct
{
  int                       x;
  int                       y;
  int                       z;
}
pt3d_t;

typedef struct
{
  int                       supported;
  int                       elevation;
  float                     oxy;
  float                     oyz;
}
wground_t;

typedef struct wobject wobject_t;

struct wobject
{
  unsigned                  id;

#define WOBJTYPE_PLAYER     0
#define WOBJTYPE_PFLYER     1 // player flyer
#define WOBJTYPE_AFLYER     2 // adversarial flyer
#define WOBJTYPE_TNT        3
  unsigned                  type;

#define WOBJFLAG_INITIALIZED (1<<0)
#define WOBJFLAG_VISIBLE     (1<<1)
#define WOBJFLAG_FLYING      (1<<2)
  unsigned                  flags;

  pt3d_t                    position;   /* high res position ('pixels') */
  float                     oxz, oyz;   /* orientation angle (radians) */
  int                       speed_vert; /* falling speed (negative is down) */
  int                       speed_hor;  /* ground speed (positive is forward)*/

  void(*                    update)(wobject_t*, walker_t*);
  void(*                    draw)(wobject_t*, walker_t*, pt2d_t);

  struct {
    pt2d_t                    tile;
    pt2d_t                    quadrant;
    wground_t                 ground;
  }                         cache;

  union {
    struct {
      int                       adversarial;
      float                     rotxz;
      float                     rotyz;
      float                     oxz;
      float                     oyz;
      float                     circle;
    }                         flyer;
    struct {
      int                       set;
      unsigned                  countdown;
    }                         tnt;
  }                         subtype;
};

#define PLAYER_START_PX     0
#define PLAYER_START_PZ     0

#define PLAYER_TURNRATE     (2 * WDEGRAD) /* 2 degrees per turn move */
#define PLAYER_JUMPSPEED    32

typedef struct
{
  wobject_t                 object;
  wobject_t                 flyer;
  wobject_t*                objectinview;
  struct {
    unsigned                  item;
    unsigned                  amount;
  }                         inventory[ 10 ];
}
wplayer_t;

#define WTILESIZE           256
#define VISION              80

#define WCOLORS {                   \
  ( 32 << 16) | (127 << 8) | ( 32), \
  ( 40 << 16) | (132 << 8) | ( 40), \
  ( 32 << 16) | (140 << 8) | ( 40), \
  ( 40 << 16) | (127 << 8) | ( 32)  \
}

#define WCOLORS_HARD {                \
  ( 127 << 16) | (120 << 8) | ( 127), \
  ( 108 << 16) | (132 << 8) | ( 108), \
  ( 127 << 16) | (140 << 8) | ( 108), \
  ( 108 << 16) | (100 << 8) | ( 108)  \
}

#define WCOLORS_CHASM {               \
  (  64 << 16) | ( 80 << 8) | (  40), \
  (  80 << 16) | ( 64 << 8) | (  72), \
  (  64 << 16) | ( 72 << 8) | (  80), \
  (  72 << 16) | ( 40 << 8) | (  72)  \
}

typedef struct
{
  unsigned char             color;
  unsigned char             hardness;
  uint16_t                  elevation[ 9 ];
}
wtile_t;

#define WQUADRANT_DIMENSION 64

#define WTILE_VARIATION     8
#define WHILL_VARIATION     16

typedef struct
{
  int                       qx, qz; /* quadrant position */
  wtile_t                   tiles[ WQUADRANT_DIMENSION ][ WQUADRANT_DIMENSION ];
}
wquadrant_t;

#define WTOPIC_COLOR        1
#define WTOPIC_ELEVATION    2
#define WTOPIC_QELEVATION   3
#define WTOPIC_HILLS        4
#define WTOPIC_CHASM        5

typedef struct
{
  struct {
    struct {
      pt2d_t                    qp;
      wquadrant_t               quadrant[ 3 ][ 3 ];
    }                         mem;
  }                         cache;
  walker_t*                 walker;
}
wlandscape_t;

#include <array.h>
MAKE_ARRAY_HEADER(wobject_t, wobjectlist_)
MAKE_ARRAY_HEADER(wobject_t*, wobjptrlist_)

#define GOD_MIN_FLYERS      16

typedef struct
{
  wobject_t                 viewport;
  wplayer_t                 player;
  wobjectlist_t             objects;
  wlandscape_t              landscape;
  struct {
    wobjptrlist_t             flyers;
  }                         god;
}
wworld_t;

#include "json.h"

#define WDISPLAY_DEFAULT_WIDTH  1024
#define WDISPLAY_DEFAULT_HEIGHT 768

#include "text.h"
#include "overlay.h"

struct walker
{
  wworld_t                  world;
  td_t                      db;
  struct {
    unsigned                  width, height;
    struct {
      text_object_t*            stats;
      text_object_t*            warn;
      unsigned                  warn_timeout;
      text_object_t*            help;
    }                         overlay;
    int                       show_help;
    int                       show_stats;
    int                       show_inventory;
  }                         display;
  json_t*                   json;
  struct {
    struct {
      char                      forward;
      char                      backward;
      char                      jump;
    }                         keybindings;
  }                         config;
};

#include <inttypes.h>
#include "walker_functions.h"

#endif

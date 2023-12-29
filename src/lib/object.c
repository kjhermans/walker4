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

/**
 * Applies gravity to the object, that is: increase downward velocity,
 * when the object is not flying and is not on solid ground.
 */
void object_gravity
  (wobject_t* o)
{
  if (o->flying || o->ground.supported) {
    return;
  }
  if ((o->speed_vert -= GRAVITY) < TERMINALVELOCITY) {
    o->speed_vert = TERMINALVELOCITY;
  }
}

void object_friction
  (wobject_t* o)
{
  if (o->ground.supported) {
    if (o->speed_hor > 0) {
      --(o->speed_hor);
    } else if (o->speed_hor < 0) {
      ++(o->speed_hor);
    }
  }
}

/**
 *
 */
void object_turnright
  (wobject_t* o, float radians)
{
  if ((o->oxz += radians) > (2 * WPI)) {
    o->oxz -= (2 * WPI);
  }
}

/**
 *
 */
void object_turnleft
  (wobject_t* o, float radians)
{
  if ((o->oxz -= radians) < 0) {
    o->oxz += (2 * WPI);
  }
}

/**
 *
 */
void object_turnup
  (wobject_t* o, float radians)
{
  if ((o->oyz -= radians) < 0) {
    o->oyz += (2 * WPI);
  }
}

/**
 *
 */
void object_turndown
  (wobject_t* o, float radians)
{
  if ((o->oyz += radians) > (2 * WPI)) {
    o->oyz -= (2 * WPI);
  }
}

/**
 *
 */
void object_forward
  (wobject_t* o)
{
  if ((o->speed_hor += 2) > MAXSPEED) {
    o->speed_hor = MAXSPEED;
  }
}

/**
 *
 */
void object_backward
  (wobject_t* o)
{
  if ((o->speed_hor -= 2) < MINSPEED) {
    o->speed_hor = MINSPEED;
  }
}

/**
 * Returns non zero if there's been a move.
 */
int object_move
  (wobject_t* o, wlandscape_t* ls)
{
  pt3d_t pt = o->position;

  int camry = (int)(o->oxz / PL_RAD256);
  int camrx = (int)(o->oyz / PL_RAD256);
  o->position.x += (o->speed_hor * PL_sin[camry & PL_TRIGMSK]) >> PL_P;
  o->position.z += (o->speed_hor * PL_cos[camry & PL_TRIGMSK]) >> PL_P;
  if (o->flying) {
    o->position.y += -((o->speed_hor * PL_sin[camrx & PL_TRIGMSK]) >> PL_P);
  } else {
    o->position.y += o->speed_vert;
  }

  /* checking whether on solid ground */
//  if (o->speed_hor) {
    o->ground.supported = 0;
    landscape_get_elevation(ls, o, &(o->ground));
    if (o->position.y <= o->ground.elevation + 64) {
      o->position.y = o->ground.elevation + 64;
      o->ground.supported = 1;
    }
//  }

  if (WPOS_XMIN && o->position.x < WPOS_XMIN) {
    o->position.x = WPOS_XMIN;
  }
  if (WPOS_XMAX && o->position.x > WPOS_XMAX) {
    o->position.x = WPOS_XMAX;
  }
  if (WPOS_ZMIN && o->position.z < WPOS_ZMIN) {
    o->position.z = WPOS_ZMIN;
  }
  if (WPOS_ZMAX && o->position.z > WPOS_ZMAX) {
    o->position.z = WPOS_ZMAX;
  }

  /* boxing */
  if (o->position.y < FLOOR) {
    o->position.y = FLOOR;
  } else if (o->position.y > CEILING) {
    o->position.y = CEILING;
  }

  landscape_pos2tile(
    o->position.x, o->position.z,
    &(o->tile.x), &(o->tile.z)
  );
  landscape_tile2quadrant(
    o->tile.x, o->tile.z,
    &(o->quadrant.x), &(o->quadrant.z),
    0, 0
  );

  if (o->position.x != pt.x || o->position.y != pt.y || o->position.z != pt.z) {
    return ~0;
  } else {
    return 0;
  }
}

void object_get_vision
  (wobject_t* o, vec2d_t* vision)
{
  int tx, tz;

  landscape_pos2tile(o->position.x, o->position.z, &tx, &tz);
  vision->o.x = tx - (VISION / 2);
  vision->d.x = tx + (VISION / 2);
  vision->o.z = tz - (VISION / 2);
  vision->d.z = tz + (VISION / 2);
}

void object_debug
  (wobject_t* o)
{
  fprintf(stderr, "Object: pos:%d,%d,%d, or:%f,%f, spd:%d,%d fl:%d, sup:%d\n",
    o->position.x, o->position.y, o->position.z, 
    o->oxz, o->oyz,
    o->speed_hor, o->speed_vert,
    o->flying,
    o->ground.supported
  );
}

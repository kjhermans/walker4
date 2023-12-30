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

#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <fcntl.h>

#include "walker.h"

void walker_db_init
  (walker_t* w, unsigned seed, int reset)
{
  char path[ 256 ];
  int openflags = O_RDWR|O_CREAT;

  snprintf(path, sizeof(path), "%s/.walker4.%u.db", getenv("HOME"), seed);
  if (reset) {
    fprintf(stderr, "Walker::init Resetting database for seed %u\n", seed);
    openflags |= O_TRUNC;
  }
  fprintf(stderr, "Walker::init Opening database at '%s'\n", path);
  td_open(&(w->db), path, 0, openflags, 0644);
}

struct wqkey {
  uint32_t  discriminant;
  int       qx;
  int       qz;
};

static const uint32_t wqkeydiscriminant =
  ('q' << 24) |
  ('u' << 16) |
  ('a' << 8) |
  'd'
;

void walker_db_quadrant_store
  (walker_t* w, wquadrant_t* q)
{
  struct wqkey keydata = {
    .discriminant = wqkeydiscriminant,
    .qx = q->qx,
    .qz = q->qz
  };
  tdt_t key = { &keydata, sizeof(keydata) };
  tdt_t val = { q, sizeof(wquadrant_t) };

  td_put(&(w->db), &key, &val, 0);
}

int walker_db_quadrant_retrieve
  (walker_t* w, wquadrant_t* q)
{
  struct wqkey keydata = {
    .discriminant = wqkeydiscriminant,
    .qx = q->qx,
    .qz = q->qz
  };
  tdt_t key = { &keydata, sizeof(keydata) };
  tdt_t val = { q, sizeof(wquadrant_t) };

  return td_get(&(w->db), &key, &val, 0);
}

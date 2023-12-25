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

void landscape_td_init
  (wlandscape_t* ls, unsigned seed)
{
  char path[ 256 ];

  snprintf(path, sizeof(path), "%s/.walker4.%u.db", getenv("HOME"), seed);
  td_open(&(ls->cache.disk), path, 0, O_RDWR|O_CREAT, 0644);
}

void landscape_td_put
  (wlandscape_t* ls, wquadrant_t* q)
{
  int keydata[ 2 ] = { q->qx, q->qz };
  tdt_t key = { keydata, sizeof(keydata) };
  tdt_t val = { q, sizeof(wquadrant_t) };

  td_put(&(ls->cache.disk), &key, &val, 0);
}

int landscape_td_get
  (wlandscape_t* ls, wquadrant_t* q)
{
  int keydata[ 2 ] = { q->qx, q->qz };
  tdt_t key = { keydata, sizeof(keydata) };
  tdt_t val = { q, sizeof(wquadrant_t) };

  return td_get(&(ls->cache.disk), &key, &val, 0);
}

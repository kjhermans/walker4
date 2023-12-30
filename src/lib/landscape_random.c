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

#define LOTSOFRANDOM (32 * 1024 * 1024)

static
unsigned char lotsofrandom[ LOTSOFRANDOM + sizeof(int64_t) + sizeof(int) ];

/**
 * Extracts a lot of random on initialization, and puts it into a buffer.
 */
void landscape_random_extract
  ()
{
  memset(lotsofrandom, 0, sizeof(lotsofrandom));
  for (unsigned i=0; i < sizeof(lotsofrandom) - sizeof(int); i++) {
    int r = rand();
    unsigned char* _r = (unsigned char*)(&r);
    for (unsigned j=0; j < sizeof(int); j++) {
      lotsofrandom[ i + j ] ^= _r[ j ];
    }
  }
#ifdef _DEBUG
  fprintf(stderr, "Sample random: ");
  for (unsigned i=0; i < 16; i++) {
    fprintf(stderr, "%.2x ", lotsofrandom[ i ]);
  }
  fprintf(stderr, "\n");
#endif
}

/**
 * Returns topologically, topically deterministic random.
 * Hashes the parameters (tile-x, tile-z, and a topic of your choosing)
 * to give a big random buffer bit offset, then returns that as a
 * 32-bit unsinged integer.
 */
uint32_t landscape_random_get_topo
  (int tx, int tz, int topic)
{
  uint64_t state = 0;
  unsigned bitoffset, byteoffset;
  uint64_t buf = 0;

  fnv(&state, &tx, sizeof(tx));
  fnv(&state, &tz, sizeof(tz));
  fnv(&state, &topic, sizeof(topic));

  byteoffset = state % LOTSOFRANDOM;
  bitoffset = state % 8;

  buf =
    ((uint64_t)(lotsofrandom[ byteoffset ]) << 32) |
    (lotsofrandom[ byteoffset+1 ] << 24) |
    (lotsofrandom[ byteoffset+2 ] << 16) |
    (lotsofrandom[ byteoffset+3 ] << 8) |
     lotsofrandom[ byteoffset+4 ];
  buf >>= bitoffset;

  return (uint32_t)buf;
}

/**
 * Returns topologically, topically deterministic random
 * with an exponentially decreasing chance of getting the max value.
 */
uint32_t landscape_random_get_topo_exp
  (int tx, int tz, int topic, uint32_t max)
{
  uint32_t bits = landscape_random_get_topo(tx, tz, topic);

  for (uint32_t i=0; i < max && i < 32; i++) {
    if (((bits >> i) & 0x1) == 1) {
      return i;
    }
  }
  return max;
}

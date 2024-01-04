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
 * Optimizes the elevation values on tiles[ 4 ].
 */
void landscape_tile_optimize
  (wlandscape_t* ls, int tx, int tz, wtile_t* tiles[ 9 ])
{
  wtile_t cache[ 9 ] = { 0 };
  (void)ls;

  for (unsigned i=0; i < 9; i++) {
    if (tiles[ i ] == NULL) {
      wtile_t* t = landscape_tile_get_cache_ptr(ls, tx+((i%3)-1), tz+((i/3)-1));
      if (t) {
        tiles[ i ] = t;
      } else {
        landscape_tile_get(ls, tx+((i%3)-1), tz+((i/3)-1), &(cache[ i ]));
        tiles[ i ] = &(cache[ i ]);
      }
    }
  }
  tiles[ 0 ]->elevation[ 8 ] =
  tiles[ 1 ]->elevation[ 6 ] =
  tiles[ 3 ]->elevation[ 2 ] =
  tiles[ 4 ]->elevation[ 0 ] = (
    tiles[ 0 ]->elevation[ 4 ] +
    tiles[ 1 ]->elevation[ 4 ] +
    tiles[ 3 ]->elevation[ 4 ] +
    tiles[ 4 ]->elevation[ 4 ]
  ) / 4;
  tiles[ 1 ]->elevation[ 7 ] =
  tiles[ 4 ]->elevation[ 1 ] = (
    tiles[ 4 ]->elevation[ 4 ] +
    tiles[ 1 ]->elevation[ 4 ]
  ) / 2;
  tiles[ 2 ]->elevation[ 6 ] =
  tiles[ 1 ]->elevation[ 8 ] =
  tiles[ 5 ]->elevation[ 0 ] =
  tiles[ 4 ]->elevation[ 2 ] = (
    tiles[ 1 ]->elevation[ 4 ] +
    tiles[ 2 ]->elevation[ 4 ] +
    tiles[ 4 ]->elevation[ 4 ] +
    tiles[ 5 ]->elevation[ 4 ]
  ) / 4;
  tiles[ 3 ]->elevation[ 5 ] =
  tiles[ 4 ]->elevation[ 3 ] = (
    tiles[ 4 ]->elevation[ 4 ] +
    tiles[ 3 ]->elevation[ 4 ]
  ) / 2;
  tiles[ 5 ]->elevation[ 3 ] =
  tiles[ 4 ]->elevation[ 5 ] = (
    tiles[ 4 ]->elevation[ 4 ] +
    tiles[ 5 ]->elevation[ 4 ]
  ) / 2;
  tiles[ 6 ]->elevation[ 2 ] =
  tiles[ 3 ]->elevation[ 8 ] =
  tiles[ 7 ]->elevation[ 0 ] =
  tiles[ 4 ]->elevation[ 6 ] = (
    tiles[ 3 ]->elevation[ 4 ] +
    tiles[ 4 ]->elevation[ 4 ] +
    tiles[ 6 ]->elevation[ 4 ] +
    tiles[ 7 ]->elevation[ 4 ]
  ) / 4;
  tiles[ 7 ]->elevation[ 1 ] =
  tiles[ 4 ]->elevation[ 7 ] = (
    tiles[ 4 ]->elevation[ 4 ] +
    tiles[ 7 ]->elevation[ 4 ]
  ) / 2;
  tiles[ 8 ]->elevation[ 0 ] =
  tiles[ 7 ]->elevation[ 2 ] =
  tiles[ 5 ]->elevation[ 6 ] =
  tiles[ 4 ]->elevation[ 8 ] = (
    tiles[ 4 ]->elevation[ 4 ] +
    tiles[ 5 ]->elevation[ 4 ] +
    tiles[ 7 ]->elevation[ 4 ] +
    tiles[ 8 ]->elevation[ 4 ]
  ) / 4;
}

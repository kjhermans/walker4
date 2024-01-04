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
 *
 */
void landscape_tile_get_triangles_optimized
  (wtile_t* tile, int tr[ 8 ][ 3 ][ 3 ])
{
  tr[ 0 ][ 0 ][ 0 ] = WTILESIZE / 2;
  tr[ 0 ][ 0 ][ 1 ] = tile->elevation[ 4 ];
  tr[ 0 ][ 0 ][ 2 ] = WTILESIZE / 2;
  tr[ 0 ][ 1 ][ 0 ] = 0;
  tr[ 0 ][ 1 ][ 1 ] = tile->elevation[ 0 ];
  tr[ 0 ][ 1 ][ 2 ] = 0;
  tr[ 0 ][ 2 ][ 0 ] = WTILESIZE / 2;
  tr[ 0 ][ 2 ][ 1 ] = tile->elevation[ 1 ];
  tr[ 0 ][ 2 ][ 2 ] = 0;

  tr[ 1 ][ 0 ][ 0 ] = WTILESIZE / 2;
  tr[ 1 ][ 0 ][ 1 ] = tile->elevation[ 4 ];
  tr[ 1 ][ 0 ][ 2 ] = WTILESIZE / 2;
  tr[ 1 ][ 1 ][ 0 ] = WTILESIZE / 2;
  tr[ 1 ][ 1 ][ 1 ] = tile->elevation[ 1 ];
  tr[ 1 ][ 1 ][ 2 ] = 0;
  tr[ 1 ][ 2 ][ 0 ] = WTILESIZE;
  tr[ 1 ][ 2 ][ 1 ] = tile->elevation[ 2 ];
  tr[ 1 ][ 2 ][ 2 ] = 0;

  tr[ 2 ][ 0 ][ 0 ] = WTILESIZE / 2;
  tr[ 2 ][ 0 ][ 1 ] = tile->elevation[ 4 ];
  tr[ 2 ][ 0 ][ 2 ] = WTILESIZE / 2;
  tr[ 2 ][ 1 ][ 0 ] = WTILESIZE;
  tr[ 2 ][ 1 ][ 1 ] = tile->elevation[ 2 ];
  tr[ 2 ][ 1 ][ 2 ] = 0;
  tr[ 2 ][ 2 ][ 0 ] = WTILESIZE;
  tr[ 2 ][ 2 ][ 1 ] = tile->elevation[ 5 ];
  tr[ 2 ][ 2 ][ 2 ] = WTILESIZE / 2;

  tr[ 3 ][ 0 ][ 0 ] = WTILESIZE / 2;
  tr[ 3 ][ 0 ][ 1 ] = tile->elevation[ 4 ];
  tr[ 3 ][ 0 ][ 2 ] = WTILESIZE / 2;
  tr[ 3 ][ 1 ][ 0 ] = WTILESIZE;
  tr[ 3 ][ 1 ][ 1 ] = tile->elevation[ 5 ];
  tr[ 3 ][ 1 ][ 2 ] = WTILESIZE / 2;
  tr[ 3 ][ 2 ][ 0 ] = WTILESIZE;
  tr[ 3 ][ 2 ][ 1 ] = tile->elevation[ 8 ];
  tr[ 3 ][ 2 ][ 2 ] = WTILESIZE;

  tr[ 4 ][ 0 ][ 0 ] = WTILESIZE / 2;
  tr[ 4 ][ 0 ][ 1 ] = tile->elevation[ 4 ];
  tr[ 4 ][ 0 ][ 2 ] = WTILESIZE / 2;
  tr[ 4 ][ 1 ][ 0 ] = WTILESIZE;
  tr[ 4 ][ 1 ][ 1 ] = tile->elevation[ 8 ];
  tr[ 4 ][ 1 ][ 2 ] = WTILESIZE;
  tr[ 4 ][ 2 ][ 0 ] = WTILESIZE / 2;
  tr[ 4 ][ 2 ][ 1 ] = tile->elevation[ 7 ];
  tr[ 4 ][ 2 ][ 2 ] = WTILESIZE;

  tr[ 5 ][ 0 ][ 0 ] = WTILESIZE / 2;
  tr[ 5 ][ 0 ][ 1 ] = tile->elevation[ 4 ];
  tr[ 5 ][ 0 ][ 2 ] = WTILESIZE / 2;
  tr[ 5 ][ 1 ][ 0 ] = WTILESIZE / 2;
  tr[ 5 ][ 1 ][ 1 ] = tile->elevation[ 7 ];
  tr[ 5 ][ 1 ][ 2 ] = WTILESIZE;
  tr[ 5 ][ 2 ][ 0 ] = 0;
  tr[ 5 ][ 2 ][ 1 ] = tile->elevation[ 6 ];
  tr[ 5 ][ 2 ][ 2 ] = WTILESIZE;

  tr[ 6 ][ 0 ][ 0 ] = WTILESIZE / 2;
  tr[ 6 ][ 0 ][ 1 ] = tile->elevation[ 4 ];
  tr[ 6 ][ 0 ][ 2 ] = WTILESIZE / 2;
  tr[ 6 ][ 1 ][ 0 ] = 0;
  tr[ 6 ][ 1 ][ 1 ] = tile->elevation[ 6 ];
  tr[ 6 ][ 1 ][ 2 ] = WTILESIZE;
  tr[ 6 ][ 2 ][ 0 ] = 0;
  tr[ 6 ][ 2 ][ 1 ] = tile->elevation[ 3 ];
  tr[ 6 ][ 2 ][ 2 ] = WTILESIZE / 2;

  tr[ 7 ][ 0 ][ 0 ] = WTILESIZE / 2;
  tr[ 7 ][ 0 ][ 1 ] = tile->elevation[ 4 ];
  tr[ 7 ][ 0 ][ 2 ] = WTILESIZE / 2;
  tr[ 7 ][ 1 ][ 0 ] = 0;
  tr[ 7 ][ 1 ][ 1 ] = tile->elevation[ 3 ];
  tr[ 7 ][ 1 ][ 2 ] = WTILESIZE / 2;
  tr[ 7 ][ 2 ][ 0 ] = 0;
  tr[ 7 ][ 2 ][ 1 ] = tile->elevation[ 0 ];
  tr[ 7 ][ 2 ][ 2 ] = 0;

}

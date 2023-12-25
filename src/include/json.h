/**
 * This file is part of Sarthaka, a structured data binary codec.

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

#ifndef _JSON_H_
#define _JSON_H_

#include <inttypes.h>

#include <naigama/naigama.h>

typedef struct json json_t;

#include "map.h"
#include "array.h"

typedef struct
{
  unsigned                      wide;
  unsigned                      length;
  union {
    uint8_t*                      bytes;
    uint32_t*                     unicode;
  }                             value;
}
json_string_t;

MAKE_ARRAY_HEADER(json_t*, json_array_)
MAKE_MAP_HEADER(json_string_t, json_t*, json_hashtable_)

struct json
{
  unsigned                      type;
#define JSON_TYPE_NULL          0
#define JSON_TYPE_BOOLEAN       1
#define JSON_TYPE_INTEGER       2
#define JSON_TYPE_FLOAT         3
#define JSON_TYPE_STRING        4
#define JSON_TYPE_ARRAY         5
#define JSON_TYPE_HASHTABLE     6

  union {
    int64_t                       boolint;
    double                        fraction;
    json_string_t                 string;
    json_array_t                  array;
    json_hashtable_t              hashtable;
  }                             value;
};

extern
int json_string_compare
  (json_string_t* s1, json_string_t* s2);

extern
json_t* json_parse
  (char* string);

extern
void json_debug
  (json_t* json);

extern
void json_free
  (json_t* json);

#endif

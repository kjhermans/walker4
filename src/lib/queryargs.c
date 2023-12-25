/**
 * This file is part of Oroszlan, a parsing and scripting environment

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

#include <string.h>

/**
 * Queries the argument list.
 * If option and longoption are both zero (ie longoption == NULL)
 * then you're querying the nth occurence of a non-optioned argument.
 *
 * \param argc        As given to main()
 * \param argv        As given to main()
 * \param option      The '-' switch
 * \param longoption  The longform alternative to the '-' switch
 * \param index       The nth (off by zero) occurence of the option.
 * \param expectvalue Boolean. Whether or not the option is to have a value.
 * \param value       When found, will contain a pointer to the sought value.
 *
 * \returns           Zero when found, non-zero when not found.
 */
int queryargs
  (
    unsigned argc,
    char* argv[],
    char option,
    char* longoption,
    unsigned index,
    int expectvalue,
    char** remainder,
    char** value
  )
{
  for (unsigned i=1; i < argc; i++) {
    if (argv[ i ][ 0 ] == '-') {
      if (argv[ i ][ 1 ] == '-') {
        if (longoption) {
          if (0 == strcmp(&(argv[ i ][ 2 ]), longoption)) {
            if (i + (expectvalue ? 1 : 0) < argc) {
              if (value) {
                *value = argv[ i + 1 ];
              }
              return 0;
            } else {
              return ~0;
            }
          }
        }
      } else {
        for (unsigned j=1; j < strlen(argv[ i ]); j++) {
          if (argv[ i ][ j ] == option) {
            if (index == 0) {
              if (i + j - (expectvalue ? 0 : 1) < argc) {
                if (remainder) {
	          *remainder = &(argv[ i ][ j ]);
                }
                if (value) {
                  *value = argv[ i + 1 ];
                }
                return 0;
              } else {
                return ~0;
              }
            } else {
              --index;
            }
          }
        }
      }
    } else {
      if (!option && !longoption) {
        if (index == 0) {
          if (value) {
            *value = argv[ i ];
          }
          return 0;
        } else {
          --index;
        }
      }
    }
  }
  return ~0;
}

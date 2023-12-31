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

#include <signal.h>

#include <walker.h>

void exitsighandler
  (int signum)
{
  fprintf(stderr, "Caught signal %d\n", signum);
  walker_on_exit();
}

int main
  (int argc, char* argv[])
{
  walker_t w = { 0 };
  char* configfile = 0;
  char* seedstring = 0;
  unsigned seed;
  int r;
  int reset = 0;

  r = queryargs(argc, argv, 'c', "config", 0, 1, 0, &configfile); (void)r;
  r = queryargs(argc, argv, 's', "seed", 0, 1, 0, &seedstring); (void)r;
  if (seedstring) {
    seed = atoi(seedstring);
  }
  if (queryargs(argc, argv, 'r', "resetdb", 0, 0, 0, 0) == 0) {
    reset = 1;
  }
  if (queryargs(argc, argv, 'h', "help", 0, 0, 0, 0) == 0) {
    fprintf(stderr,
      "Usage: %s [options]\n\n"
      "Options:\n"
      "-c <path>   Specify location of config file (default ~%s).\n"
      "-s <int>    Specify game seed (default %u).\n"
      "-r          Reset seed database before starting.\n"
      "-h          Print this message and exit.\n\n"
      "Long options (with --), respectively:\n"
      "config, seed, resetdb, help\n\n"
      "Important default keys inside the game:\n"
      "h           Shows key help\n"
      , argv[ 0 ]
      , WDEFAULTCONFIG
      , WDEFAULTSEED
    );
    exit(0);
  }
  signal(SIGINT, exitsighandler);
  signal(SIGTERM, exitsighandler);
  walker_init(&w, (seedstring ? &seed : 0), reset);
  walker_configure(&w, configfile);
  walker_show(&w, argc, argv);
  walker_run(&w);

  return 0;
}

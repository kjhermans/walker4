/**
* \file
* \brief This file is an autogenerated function prototypes header file.
*
* Copyright 2018 K.J. Hermans
* WARNING: This file is automatically generated. Any changes will be lost.
*/

#ifndef __FUNCTIONS_H_
#define __FUNCTIONS_H_

/* declared in ./src/lib//absorb_file.c */
extern
int absorb_file
  (char* path, unsigned char** buf, unsigned* buflen)
  __attribute__ ((warn_unused_result));

/* declared in ./src/lib//flyers.c */
extern
void flyer_update
  (wobject_t* o, walker_t* w);

/* declared in ./src/lib//flyers.c */
extern
void flyer_draw
  (wobject_t* o, walker_t* w, pt2d_t p);

/* declared in ./src/lib//flyers.c */
extern
void flyer_init
  (wobject_t* o);

/* declared in ./src/lib//fnv.c */
extern
void fnv
  (uint64_t* state, void* mem, unsigned size);

/* declared in ./src/lib//json.c */
extern
int json_string_compare
  (json_string_t* s1, json_string_t* s2)
  __attribute__ ((warn_unused_result));

/* declared in ./src/lib//json.c */
extern
void json_debug
  (json_t* json);

/* declared in ./src/lib//json.c */
extern
void json_free
  (json_t* json);

/* declared in ./src/lib//landscape.c */
extern
void landscape_init
  (wlandscape_t* ls, unsigned seed, walker_t* w);

/* declared in ./src/lib//landscape_get_elevation.c */
extern
void landscape_get_elevation
  (wlandscape_t* ls, wobject_t* o, wground_t* ground);

/* declared in ./src/lib//landscape_pos2tile.c */
extern
void landscape_pos2tile
  (int px, int pz, int* tx, int* tz);

/* declared in ./src/lib//landscape_quadrant.c */
extern
void landscape_quadrant_optimize
  (wlandscape_t* ls, wquadrant_t* q);

/* declared in ./src/lib//landscape_quadrant.c */
extern
void landscape_quadrant_get_new
  (wlandscape_t* ls, wquadrant_t* q, int qx, int qz);

/* declared in ./src/lib//landscape_quadrant.c */
extern
void landscape_quadrant_get
  (wlandscape_t* ls, int qx, int qz, wquadrant_t* q);

/* declared in ./src/lib//landscape_quadrant.c */
extern
void landscape_cache_update
  (wlandscape_t* ls, int qx, int qz);

/* declared in ./src/lib//landscape_random.c */
extern
void landscape_random_extract
  ();

/* declared in ./src/lib//landscape_random.c */
extern
uint32_t landscape_random_get_topo
  (int tx, int tz, int topic)
  __attribute__ ((warn_unused_result));

/* declared in ./src/lib//landscape_random.c */
extern
uint32_t landscape_random_get_topo_exp
  (int tx, int tz, int topic, uint32_t max)
  __attribute__ ((warn_unused_result));

/* declared in ./src/lib//landscape_tile.c */
extern
void landscape_tile_get_new
  (int tx, int tz, wtile_t* tile);

/* declared in ./src/lib//landscape_tile.c */
extern
void landscape_tile_optimize
  (wlandscape_t* ls, int tx, int tz, wtile_t* tiles[ 9 ]);

/* declared in ./src/lib//landscape_tile.c */
extern
void landscape_tile_get_triangles_optimized
  (wtile_t* tile, int tr[ 8 ][ 3 ][ 3 ]);

/* declared in ./src/lib//landscape_tile.c */
extern
void landscape_tile_get_triangles
  (wlandscape_t* ls, int tx, int tz, int tr[ 8 ][ 3 ][ 3 ]);

/* declared in ./src/lib//landscape_tile.c */
extern
int landscape_tile_get_cache
  (wlandscape_t* ls, int tx, int tz, wtile_t* tile)
  __attribute__ ((warn_unused_result));

/* declared in ./src/lib//landscape_tile.c */
extern
void landscape_tile_get
  (wlandscape_t* ls, int tx, int tz, wtile_t* tile);

/* declared in ./src/lib//landscape_tile.c */
extern
void landscape_tile2quadrant
  (
    int tx, int tz,
    int* qx, int* qz,
    int* qtx, int* qtz
  );

/* declared in ./src/lib//landscape_tile.c */
extern
void landscape_tile_debug
  (wtile_t* tile);

/* declared in ./src/lib//object.c */
extern
unsigned object_get_id
  ()
  __attribute__ ((warn_unused_result));

/* declared in ./src/lib//object.c */
extern
void object_init
  (wobject_t* o, unsigned type);

/* declared in ./src/lib//object.c */
extern
void object_draw
  (wobject_t* o, walker_t* w, wplayer_t* p, vec2d_t vision);

/* declared in ./src/lib//object.c */
extern
void object_gravity
  (wobject_t* o);

/* declared in ./src/lib//object.c */
extern
void object_friction
  (wobject_t* o);

/* declared in ./src/lib//object.c */
extern
void object_turnright
  (wobject_t* o, float radians);

/* declared in ./src/lib//object.c */
extern
void object_turnleft
  (wobject_t* o, float radians);

/* declared in ./src/lib//object.c */
extern
void object_turnup
  (wobject_t* o, float radians);

/* declared in ./src/lib//object.c */
extern
void object_turndown
  (wobject_t* o, float radians);

/* declared in ./src/lib//object.c */
extern
void object_forward
  (wobject_t* o);

/* declared in ./src/lib//object.c */
extern
void object_backward
  (wobject_t* o);

/* declared in ./src/lib//object.c */
extern
int object_move
  (wobject_t* o, wlandscape_t* ls)
  __attribute__ ((warn_unused_result));

/* declared in ./src/lib//object.c */
extern
void object_get_vision
  (wobject_t* o, vec2d_t* vision);

/* declared in ./src/lib//object.c */
extern
void object_debug
  (wobject_t* o);

/* declared in ./src/lib//overlay.c */
extern
void overlay_setpixel
  (unsigned* buf, unsigned width, unsigned height, unsigned x, unsigned y);

/* declared in ./src/lib//overlay.c */
extern
void overlay_inverse
  (unsigned* buf, unsigned width, unsigned height, unsigned x, unsigned y);

/* declared in ./src/lib//overlay.c */
extern
void overlay_hud_compass
  (
    unsigned* buf,
    unsigned width,
    unsigned height
  );

/* declared in ./src/lib//overlay.c */
extern
void overlay_hud_ascend
  (
    unsigned* buf,
    unsigned width,
    unsigned height
  );

/* declared in ./src/lib//overlay.c */
extern
void overlay_hud_sight
  (
    unsigned* buf,
    unsigned width,
    unsigned height
  );

/* declared in ./src/lib//overlay.c */
extern
void overlay_hud_height
  (
    unsigned* buf,
    unsigned width,
    unsigned height
  );

/* declared in ./src/lib//overlay.c */
extern
void overlay_hud
  (
    unsigned* buf,
    unsigned width,
    unsigned height
  );

/* declared in ./src/lib//overlay.c */
extern
void overlay_cross
  (unsigned* buf, unsigned width, unsigned height);

/* declared in ./src/lib//overlay.c */
extern
void overlay_string_inverse
  (unsigned* buf, unsigned w, unsigned h, unsigned x, unsigned y, char* s);

/* declared in ./src/lib//overlay.c */
extern
void overlay_string
  (unsigned* buf, unsigned w, unsigned h, unsigned x, unsigned y, char* s);

/* declared in ./src/lib//overlay.c */
extern
void overlay_set_inventory
  (int i);

/* declared in ./src/lib//overlay.c */
extern
void overlay_set_callback
  (void(*fnc)(void*,unsigned,int*isfilled,char*c,unsigned*amount), void* arg);

/* declared in ./src/lib//overlay.c */
extern
void overlay_set_flying
  (int f);

/* declared in ./src/lib//overlay.c */
extern
void overlay
  (unsigned* buf, unsigned width, unsigned height);

/* declared in ./src/lib//player.c */
extern
void player_handle_keys
  (wplayer_t* p, walker_t* w, int key);

/* declared in ./src/lib//player.c */
extern
void player_flying
  (wplayer_t* p, walker_t* w);

/* declared in ./src/lib//player.c */
extern
void player_update
  (wplayer_t* p, wlandscape_t* ls);

/* declared in ./src/lib//player.c */
extern
void player_init
  (wplayer_t* p);

/* declared in ./src/lib//queryargs.c */
extern
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
  __attribute__ ((warn_unused_result));

/* declared in ./src/lib//text.c */
extern
void text_object_init
  (text_object_t* o, unsigned x, unsigned y, char* t);

/* declared in ./src/lib//text.c */
extern
void text_object_set_text
  (text_object_t* o, char* t);

/* declared in ./src/lib//text.c */
extern
void text_object_set_position
  (text_object_t* o, unsigned x, unsigned y);

/* declared in ./src/lib//text.c */
extern
void text_object_set_visibility
  (text_object_t* o, int visible);

/* declared in ./src/lib//walker.c */
extern
void walker_configure
  (walker_t* w, char* path);

/* declared in ./src/lib//walker.c */
extern
void walker_update
  (walker_t* w);

/* declared in ./src/lib//walker.c */
extern
void walker_set_windowsize
  (int w, int h);

/* declared in ./src/lib//walker.c */
extern
void PL_OBJ_debug
  (struct PL_OBJ* obj);

/* declared in ./src/lib//walker.c */
extern
int walker_is_day
  ()
  __attribute__ ((warn_unused_result));

/* declared in ./src/lib//walker.c */
extern
void walker_show
  (walker_t* w, int argc, char* argv[]);

/* declared in ./src/lib//walker.c */
extern
void walker_run
  (walker_t* w);

/* declared in ./src/lib//walker.c */
extern
void walker_inventory
  (void* arg, unsigned i, int* isfilled, char* c, unsigned* amount);

/* declared in ./src/lib//walker.c */
extern
void walker_init
  (walker_t* w, unsigned* optseed, int reset);

/* declared in ./src/lib//walker.c */
extern
void walker_warn
  (walker_t* w, char* str);

/* declared in ./src/lib//walker.c */
extern
void walker_on_exit
  ();

/* declared in ./src/lib//walker_db.c */
extern
void walker_db_init
  (walker_t* w, unsigned seed, int reset);

/* declared in ./src/lib//walker_db.c */
extern
void walker_db_quadrant_store
  (walker_t* w, wquadrant_t* q);

/* declared in ./src/lib//walker_db.c */
extern
int walker_db_quadrant_retrieve
  (walker_t* w, wquadrant_t* q)
  __attribute__ ((warn_unused_result));

/* declared in ./src/lib//walker_db.c */
extern
void walker_db_object_store
  (walker_t* w, wobject_t* o);

/* declared in ./src/lib//walker_db.c */
extern
int walker_db_object_retrieve
  (walker_t* w, wobject_t* o)
  __attribute__ ((warn_unused_result));

/* declared in ./src/lib//walker_db.c */
extern
void walker_db_object_iterate
  (
    walker_t* w,
    void(*fnc)(walker_t*w,wobject_t*,void*),
    void* arg
  );



#endif
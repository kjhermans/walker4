#ifndef _TYPED_MAP_H_
#define _TYPED_MAP_H_

#include <stdlib.h>
#include <string.h>

/**
 * Define MAP_EQUALS if you have, for example, strings.
 */

#ifndef MAP_EQUALS
#define MAP_EQUALS(a,b) (a == b)
#endif

// if you define MAP_COPY_KEY(a,b) or MAP_COPY_VALUE(a,b)

#define COMBINE(a, b) a##b

#define MAKE_MAP_HEADER(Tk, Tv, prefix)                                     \
  typedef struct {                                                          \
    Tk* keys;                                                               \
    Tv* values;                                                             \
    unsigned count;                                                         \
    unsigned allocated;                                                     \
  } COMBINE(prefix, t);                                                     \
                                                                            \
  extern                                                                    \
  void COMBINE(prefix, init)(COMBINE(prefix, t)* map);                      \
                                                                            \
  extern                                                                    \
  void COMBINE(prefix, free)(COMBINE(prefix, t)* map);                      \
                                                                            \
  extern                                                                    \
  unsigned COMBINE(prefix, size)(COMBINE(prefix, t)* map);                  \
                                                                            \
  extern                                                                    \
  Tv* COMBINE(prefix, put)(COMBINE(prefix, t)* map, Tk key, Tv val);        \
                                                                            \
  extern                                                                    \
  int COMBINE(prefix, get)(COMBINE(prefix, t)* map, Tk key, Tv* val);       \
                                                                            \
  extern                                                                    \
  int COMBINE(prefix, has)(COMBINE(prefix, t)* map, Tk key);                \
                                                                            \
  extern                                                                    \
  int COMBINE(prefix, del)(COMBINE(prefix, t)* map, Tk key, Tv* val);       \
                                                                            \
  extern                                                                    \
  int COMBINE(prefix, getat)(COMBINE(prefix, t)* map,                       \
                        unsigned i, Tk* key, Tv* val);                      \
                                                                            \
  extern                                                                    \
  int COMBINE(prefix, iterate)(COMBINE(prefix, t)* map,                     \
    int(*fnc)(COMBINE(prefix, t)*,unsigned,Tk,Tv,void*), void*);            \
                                                                            \
  extern                                                                    \
  int COMBINE(prefix, iterate_rw)(COMBINE(prefix, t)* map,                  \
    int(*fnc)(COMBINE(prefix, t)*,unsigned,Tk*,Tv*,void*), void*);          \




#define MAKE_MAP_CODE(Tk, Tv, prefix)                                       \
  void COMBINE(prefix, init)(COMBINE(prefix, t)* map) {                     \
    memset(map, 0, sizeof(*map));                                           \
  }                                                                         \
                                                                            \
  void COMBINE(prefix, free)(COMBINE(prefix, t)* map) {                     \
    if (map->keys) { free(map->keys); }                                     \
    if (map->values) { free(map->values); }                                 \
    memset(map, 0, sizeof(*map));                                           \
  }                                                                         \
                                                                            \
  unsigned COMBINE(prefix, size)(COMBINE(prefix, t)* map) {                 \
    return map->count;                                                      \
  }                                                                         \
                                                                            \
  Tv* COMBINE(prefix, put)(COMBINE(prefix, t)* map, Tk key, Tv val) {       \
    for (unsigned i=0; i < map->count; i++) {                               \
      if (MAP_EQUALS(map->keys[ i ], key)) {                                \
        Tv* result = &(map->values[ i ]);                                   \
        map->values[ i ] = val;                                             \
        return result;                                                      \
      }                                                                     \
    }                                                                       \
    if (map->count >= map->allocated) {                                     \
      map->allocated = map->count + 16;                                     \
      map->keys = realloc(                                                  \
        map->keys,                                                          \
        sizeof(Tk) * map->allocated                                         \
      );                                                                    \
      if (map->keys == NULL) { abort(); }                                   \
      map->values = realloc(                                                \
        map->values,                                                        \
        sizeof(Tv) * map->allocated                                         \
      );                                                                    \
      if (map->values == NULL) { abort(); }                                 \
    }                                                                       \
    map->keys[ map->count ] = key;                                          \
    map->values[ map->count ] = val;                                        \
    ++(map->count);                                                         \
    return NULL;                                                            \
  }                                                                         \
                                                                            \
  int COMBINE(prefix, get)(COMBINE(prefix, t)* map, Tk key, Tv* val) {      \
    for (unsigned i=0; i < map->count; i++) {                               \
      if (MAP_EQUALS(map->keys[ i ], key)) {                                \
        if (val) { *val = map->values[ i ]; }                               \
        return 0;                                                           \
      }                                                                     \
    }                                                                       \
    return ~0;                                                              \
  }                                                                         \
                                                                            \
  int COMBINE(prefix, has)(COMBINE(prefix, t)* map, Tk key) {               \
    for (unsigned i=0; i < map->count; i++) {                               \
      if (MAP_EQUALS(map->keys[ i ], key)) {                                \
        return 1;                                                           \
      }                                                                     \
    }                                                                       \
    return 0;                                                               \
  }                                                                         \
                                                                            \
  int COMBINE(prefix, getat)                                                \
    (COMBINE(prefix, t)* map, unsigned i, Tk* key, Tv* val)                 \
  {                                                                         \
    if (i < map->count) {                                                   \
      if (key) { *key = map->keys[ i ]; }                                   \
      if (val) { *val = map->values[ i ]; }                                 \
      return 0;                                                             \
    } else {                                                                \
      return ~0;                                                            \
    }                                                                       \
  }                                                                         \
                                                                            \
  int COMBINE(prefix, del)(COMBINE(prefix, t)* map, Tk key, Tv* val) {      \
    for (unsigned i=0; i < map->count; i++) {                               \
      if (MAP_EQUALS(map->keys[ i ], key)) {                                \
        if (val) {                                                          \
          *val = map->values[ i ];                                          \
        }                                                                   \
        memmove(                                                            \
          &(map->keys[ i ]),                                                \
          &(map->keys[ i + 1]),                                             \
          sizeof(Tk) * (map->count - (i+1))                                 \
        );                                                                  \
        memmove(                                                            \
          &(map->values[ i ]),                                              \
          &(map->values[ i + 1]),                                           \
          sizeof(Tv) * (map->count - (i+1))                                 \
        );                                                                  \
        --(map->count);                                                     \
        if (map->count == 0) {                                              \
          free(map->keys);                                                  \
          free(map->values);                                                \
          map->keys = 0;                                                    \
          map->values = 0;                                                  \
          map->allocated = 0;                                               \
        } else if (map->count < map->allocated - 32) {                      \
          map->allocated = map->count + 16;                                 \
          map->keys = realloc(                                              \
            map->keys,                                                      \
            sizeof(Tk) * map->allocated                                     \
          );                                                                \
          if (map->keys == NULL) { abort(); }                               \
          map->values = realloc(                                            \
            map->values,                                                    \
            sizeof(Tv) * map->allocated                                     \
          );                                                                \
          if (map->values == NULL) { abort(); }                             \
        }                                                                   \
        return 0;                                                           \
      }                                                                     \
    }                                                                       \
    return ~0;                                                              \
  }                                                                         \
                                                                            \
  int COMBINE(prefix, iterate)(COMBINE(prefix, t)* map,                     \
    int(*fnc)(COMBINE(prefix, t)*,unsigned,Tk,Tv,void*), void* arg)         \
  {                                                                         \
    unsigned i;                                                             \
    int r;                                                                  \
    for (i=0; i < map->count; i++) {                                        \
      if ((r = fnc(map, i, map->keys[i], map->values[i], arg)) != 0) {      \
        return r;                                                           \
      }                                                                     \
    }                                                                       \
    return 0;                                                               \
  }                                                                         \
                                                                            \
  int COMBINE(prefix, iterate_rw)(COMBINE(prefix, t)* map,                  \
    int(*fnc)(COMBINE(prefix, t)*,unsigned,Tk*,Tv*,void*), void* arg)       \
  {                                                                         \
    unsigned i;                                                             \
    int r;                                                                  \
    for (i=0; i < map->count; i++) {                                        \
      if ((r = fnc(map, i, &(map->keys[i]), &(map->values[i]), arg)) != 0) { \
        return r;                                                           \
      }                                                                     \
    }                                                                       \
    return 0;                                                               \
  }                                                                         \


#endif

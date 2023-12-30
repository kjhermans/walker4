#ifndef _WALKER2_OVERLAY_H_
#define _WALKER2_OVERLAY_H_

#include "text.h"
#include "array.h"

typedef struct
{
  unsigned angle_compass;
  unsigned angle_ascend;
  unsigned angle_bank;
  unsigned my_height;
}
overlay_hud_t;

extern overlay_hud_t hud;

void overlay_setpixel
  (unsigned* buf, unsigned width, unsigned height, unsigned x, unsigned y);

void overlay
  (unsigned* buf, unsigned width, unsigned height);

void overlay_string_inverse
  (unsigned* buf, unsigned w, unsigned h, unsigned x, unsigned y, char* s);

void overlay_string
  (unsigned* buf, unsigned w, unsigned h, unsigned x, unsigned y, char* s);

void overlay_set_callback
  (void(*fnc)(void*,unsigned,int*isfilled,char*c,unsigned*amount), void* arg);

MAKE_ARRAY_HEADER(text_object_t*, textobjlist_)

text_object_t* overlay_add_textobject
  (unsigned x, unsigned y, char* string);

void overlay_set_inventory
  (int i);

#endif

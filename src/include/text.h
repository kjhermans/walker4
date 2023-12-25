#ifndef _WALKER2_TEXT_H_
#define _WALKER2_TEXT_H_

typedef struct
{
  unsigned x, y;
  char text[ 1024 ];
  int visible;
}
text_object_t;

text_object_t* text_object_new
  (unsigned x, unsigned y, char* t);

void text_object_init
  (text_object_t* o, unsigned x, unsigned y, char* t);

void text_object_set_text
  (text_object_t* o, char* t);

void text_object_set_position
  (text_object_t* o, unsigned x, unsigned y);

void text_object_set_visibility
  (text_object_t* o, int visible);

void text_object_render
  (text_object_t* o, unsigned* buf, unsigned w, unsigned h);

#endif

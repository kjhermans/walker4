#include <stdio.h>
#include <string.h>
#include <time.h>

#include "overlay.h"

static
int inventory = 0;

static
int flying = 0;

static
int redlamp = 0;

static
unsigned char font[95][13] = {
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x36, 0x36, 0x36},
{0x00, 0x00, 0x00, 0x66, 0x66, 0xff, 0x66, 0x66, 0xff, 0x66, 0x66, 0x00, 0x00},
{0x00, 0x00, 0x18, 0x7e, 0xff, 0x1b, 0x1f, 0x7e, 0xf8, 0xd8, 0xff, 0x7e, 0x18},
{0x00, 0x00, 0x0e, 0x1b, 0xdb, 0x6e, 0x30, 0x18, 0x0c, 0x76, 0xdb, 0xd8, 0x70},
{0x00, 0x00, 0x7f, 0xc6, 0xcf, 0xd8, 0x70, 0x70, 0xd8, 0xcc, 0xcc, 0x6c, 0x38},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x1c, 0x0c, 0x0e},
{0x00, 0x00, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c},
{0x00, 0x00, 0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x18, 0x30},
{0x00, 0x00, 0x00, 0x00, 0x99, 0x5a, 0x3c, 0xff, 0x3c, 0x5a, 0x99, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0xff, 0xff, 0x18, 0x18, 0x18, 0x00, 0x00},
{0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x60, 0x60, 0x30, 0x30, 0x18, 0x18, 0x0c, 0x0c, 0x06, 0x06, 0x03, 0x03},
{0x00, 0x00, 0x3c, 0x66, 0xc3, 0xe3, 0xf3, 0xdb, 0xcf, 0xc7, 0xc3, 0x66, 0x3c},
{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x38, 0x18},
{0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0xe7, 0x7e},
{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0x07, 0x03, 0x03, 0xe7, 0x7e},
{0x00, 0x00, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0xff, 0xcc, 0x6c, 0x3c, 0x1c, 0x0c},
{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xff},
{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
{0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x03, 0x03, 0xff},
{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e},
{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x03, 0x7f, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e},
{0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00, 0x1c, 0x1c, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06},
{0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60},
{0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x18, 0x0c, 0x06, 0x03, 0xc3, 0xc3, 0x7e},
{0x00, 0x00, 0x3f, 0x60, 0xcf, 0xdb, 0xd3, 0xdd, 0xc3, 0x7e, 0x00, 0x00, 0x00},
{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18},
{0x00, 0x00, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
{0x00, 0x00, 0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
{0x00, 0x00, 0xfc, 0xce, 0xc7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc7, 0xce, 0xfc},
{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff},
{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xff},
{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xcf, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e},
{0x00, 0x00, 0x7c, 0xee, 0xc6, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06},
{0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xe0, 0xf0, 0xd8, 0xcc, 0xc6, 0xc3},
{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0},
{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xff, 0xff, 0xe7, 0xc3},
{0x00, 0x00, 0xc7, 0xc7, 0xcf, 0xcf, 0xdf, 0xdb, 0xfb, 0xf3, 0xf3, 0xe3, 0xe3},
{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe7, 0x7e},
{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
{0x00, 0x00, 0x3f, 0x6e, 0xdf, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c},
{0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0xe0, 0xc0, 0xc0, 0xe7, 0x7e},
{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff},
{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
{0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
{0x00, 0x00, 0xc3, 0xe7, 0xff, 0xff, 0xdb, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
{0x00, 0x00, 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},
{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},
{0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x7e, 0x0c, 0x06, 0x03, 0x03, 0xff},
{0x00, 0x00, 0x3c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3c},
{0x00, 0x03, 0x03, 0x06, 0x06, 0x0c, 0x0c, 0x18, 0x18, 0x30, 0x30, 0x60, 0x60},
{0x00, 0x00, 0x3c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3c},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18},
{0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x38, 0x30, 0x70},
{0x00, 0x00, 0x7f, 0xc3, 0xc3, 0x7f, 0x03, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0},
{0x00, 0x00, 0x7e, 0xc3, 0xc0, 0xc0, 0xc0, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x03, 0x03, 0x03, 0x03, 0x03},
{0x00, 0x00, 0x7f, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x33, 0x1e},
{0x7e, 0xc3, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0},
{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x00},
{0x38, 0x6c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x00, 0x00, 0x0c, 0x00},
{0x00, 0x00, 0xc6, 0xcc, 0xf8, 0xf0, 0xd8, 0xcc, 0xc6, 0xc0, 0xc0, 0xc0, 0xc0},
{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78},
{0x00, 0x00, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xfe, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xfc, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00},
{0xc0, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0x00, 0x00, 0x00, 0x00},
{0x03, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe0, 0xfe, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0xfe, 0x03, 0x03, 0x7e, 0xc0, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x1c, 0x36, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x00},
{0x00, 0x00, 0x7e, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0xc3, 0xe7, 0xff, 0xdb, 0xc3, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00},
{0xc0, 0x60, 0x60, 0x30, 0x18, 0x3c, 0x66, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0xff, 0x60, 0x30, 0x18, 0x0c, 0x06, 0xff, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x0f, 0x18, 0x18, 0x18, 0x38, 0xf0, 0x38, 0x18, 0x18, 0x18, 0x0f},
{0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
{0x00, 0x00, 0xf0, 0x18, 0x18, 0x18, 0x1c, 0x0f, 0x1c, 0x18, 0x18, 0x18, 0xf0},
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x8f, 0xf1, 0x60, 0x00, 0x00, 0x00}
};

void overlay_setpixel_color
  (
    unsigned* buf,
    unsigned width,
    unsigned height,
    unsigned x,
    unsigned y,
    unsigned color
  )
{
  if (y >= height || x >= width) {
    return;
  }
  buf[ (width*y) + x ] = color;
}

void overlay_setpixel
  (unsigned* buf, unsigned width, unsigned height, unsigned x, unsigned y)
{
  if (y >= height || x >= width) {
    return;
  }
  buf[ (width*y) + x ] = 0xffffffff;
}

void overlay_inverse
  (unsigned* buf, unsigned width, unsigned height, unsigned x, unsigned y)
{
  if (y >= height || x >= width) {
    return;
  }
  buf[ (width*y) + x ] = ~(buf[ (width*y) + x ]);
}

overlay_hud_t hud = { 0 };

void overlay_hud_compass
  (
    unsigned* buf,
    unsigned width,
    unsigned height
  )
{
  const unsigned off = width / 4;
  const unsigned len = off * 3;
  const unsigned h = (height * 5) / 8;
  const unsigned m = off * 2;
#define VISIONWIDTH 180 // degrees
#define STRIPE      30  // degrees per stripe
  const int nstripes = VISIONWIDTH / STRIPE;
  const float pixperdegree = m / VISIONWIDTH;
  const int a = hud.angle_compass;

  for (unsigned i=off; i < len; i++) {
    overlay_inverse(buf, width, height, i, h);
  }
  const int n = a - (a % STRIPE);
  for (int i=0; i < nstripes + 1; i++) {
    int v = n + ((i - nstripes/2) * STRIPE);
    unsigned sx = m - ((a - v) * pixperdegree);
    for (int j=-8; j < 8; j++) {
      overlay_inverse(buf, width, height, sx, h-j);
    }
    char str[ 8 ];
    if (v < 0) {
      v += 360;
    }
    if (v >= 360) {
      v -= 360;
    }
    snprintf(str, 8, "%d", v);
    overlay_string_inverse(buf, width, height, sx, h+16, str);
  }
}

void overlay_hud_ascend
  (
    unsigned* buf,
    unsigned width,
    unsigned height
  )
{
  const unsigned mx = width / 2;
  const unsigned chk = height / 8;
  const unsigned my = chk * 4;
  const unsigned off = chk * 2;
  const unsigned len = chk * 5;

  for (unsigned i=off; i < len; i++) {
    overlay_inverse(buf, width, height, mx, i);
  }

#define VISIONHEIGHT 90 // degrees
#define VERTSTRIPE   15 // degrees per stripe
  const int nstripes = VISIONHEIGHT / VERTSTRIPE;
  const float pixperdegree = my / VISIONHEIGHT;
  const int a = hud.angle_ascend;

  const int n = a - (a % VERTSTRIPE);
  for (int i=-nstripes+3; i < 2; i++) {
    int v = n + (i * VERTSTRIPE);
    const unsigned sy = my - ((a - v) * pixperdegree);
    const int stretch = ((v % 360) == 0) ? 16 : 8;
    for (int j=-stretch; j < stretch; j++) {
      overlay_inverse(buf, width, height, mx-j, sy);
    }
    char str[ 8 ];
    if (v < 0) {
      v += 360;
    }
    if (v >= 360) {
      v -= 360;
    }
    snprintf(str, 8, "%d", v);
    overlay_string_inverse(buf, width, height, mx+10, sy, str);
  }

  for (int i=-20; i < 20; i++) {
    overlay_inverse(buf, width, height, mx+i, my);
  }
}

void overlay_hud_sight
  (
    unsigned* buf,
    unsigned width,
    unsigned height
  )
{
  const unsigned mx = width / 2;
  const unsigned my = height / 2;

  for (int i=0; i < 16; i++) {
    overlay_inverse(buf, width, height, mx-48+i, my-32);
    overlay_inverse(buf, width, height, mx-48, my-32+i);
    overlay_inverse(buf, width, height, mx+48-i, my+32);
    overlay_inverse(buf, width, height, mx+48, my+32-i);
    overlay_inverse(buf, width, height, mx-48+i, my+32);
    overlay_inverse(buf, width, height, mx-48, my+32-i);
    overlay_inverse(buf, width, height, mx+48-i, my-32);
    overlay_inverse(buf, width, height, mx+48, my-32+i);
  }
}

void overlay_hud_height
  (
    unsigned* buf,
    unsigned width,
    unsigned height
  )
{
  const unsigned mx = width / 2;
  const unsigned my = height * 5 / 8;
  char str[ 32 ];

  snprintf(str, 8, "%d", hud.my_height);
  overlay_string_inverse(buf, width, height, mx-64, my-16, str);
}

/**
 * Displays a pilot's hud as a shimmery overlay.
 */
void overlay_hud
  (
    unsigned* buf,
    unsigned width,
    unsigned height
  )
{
  overlay_hud_compass(buf, width, height);
  overlay_hud_ascend(buf, width, height);
  overlay_hud_sight(buf, width, height);
  overlay_hud_height(buf, width, height);
}

void overlay_cross
  (unsigned* buf, unsigned width, unsigned height)
{
  for (int i=-8; i < 8; i++) {
    overlay_inverse(buf, width, height, width/2, height/2 + i);
  }
  for (int i=-8; i < 8; i++) {
    overlay_inverse(buf, width, height, width/2 + i, height/2);
  }
}

void overlay_string_inverse
  (unsigned* buf, unsigned w, unsigned h, unsigned x, unsigned y, char* s)
{
  unsigned xx = x;
  unsigned yy = y;

  for (unsigned i=0; i < strlen(s); i++) {
    if (s[ i ] == '\n') {
      yy += 16;
      xx = x;
      continue;
    }
    unsigned char* letter = font[ s[ i ] - 32 ];
    for (unsigned j=0; j < 13; j++) {
      if (letter[ j ] & 0xf0) {
        overlay_inverse(buf, w, h, xx+0, yy-j);
      }
      if (letter[ j ] & 0x70) {
        overlay_inverse(buf, w, h, xx+1, yy-j);
      }
      if (letter[ j ] & 0x30) {
        overlay_inverse(buf, w, h, xx+2, yy-j);
      }
      if (letter[ j ] & 0x10) {
        overlay_inverse(buf, w, h, xx+3, yy-j);
      }
      if (letter[ j ] & 0x0f) {
        overlay_inverse(buf, w, h, xx+4, yy-j);
      }
      if (letter[ j ] & 0x07) {
        overlay_inverse(buf, w, h, xx+5, yy-j);
      }
      if (letter[ j ] & 0x03) {
        overlay_inverse(buf, w, h, xx+6, yy-j);
      }
      if (letter[ j ] & 0x01) {
        overlay_inverse(buf, w, h, xx+7, yy-j);
      }
    }
    xx += 10;
  }
}

void overlay_string
  (unsigned* buf, unsigned w, unsigned h, unsigned x, unsigned y, char* s)
{
  unsigned xx = x;
  unsigned yy = y;

  for (unsigned i=0; i < strlen(s); i++) {
    if (s[ i ] == '\n') {
      yy += 16;
      xx = x;
      continue;
    }
    unsigned char* letter = font[ s[ i ] - 32 ];
    for (unsigned j=0; j < 13; j++) {
      if (letter[ j ] & 0xf0) {
        overlay_setpixel(buf, w, h, xx+0, yy-j);
      }
      if (letter[ j ] & 0x70) {
        overlay_setpixel(buf, w, h, xx+1, yy-j);
      }
      if (letter[ j ] & 0x30) {
        overlay_setpixel(buf, w, h, xx+2, yy-j);
      }
      if (letter[ j ] & 0x10) {
        overlay_setpixel(buf, w, h, xx+3, yy-j);
      }
      if (letter[ j ] & 0x0f) {
        overlay_setpixel(buf, w, h, xx+4, yy-j);
      }
      if (letter[ j ] & 0x07) {
        overlay_setpixel(buf, w, h, xx+5, yy-j);
      }
      if (letter[ j ] & 0x03) {
        overlay_setpixel(buf, w, h, xx+6, yy-j);
      }
      if (letter[ j ] & 0x01) {
        overlay_setpixel(buf, w, h, xx+7, yy-j);
      }
    }
    xx += 10;
  }
}

textobjlist_t textobjects = { 0 };

MAKE_ARRAY_CODE(text_object_t*, textobjlist_)

text_object_t* overlay_add_textobject
  (unsigned x, unsigned y, char* string)
{
  text_object_t* result = text_object_new(x, y, string);
  textobjlist_push(&textobjects, result);
  return result;
}

void overlay_set_inventory
  (int i)
{
  inventory = i;
  if (inventory) {
    fprintf(stderr, "Display inventory");
  }
}

static
void(*overlay_inventory_callback)
    (void*,unsigned,int*isfilled,char*c,unsigned*amount);

static
void* invarg;

void overlay_set_callback
  (void(*fnc)(void*,unsigned,int*isfilled,char*c,unsigned*amount), void* arg)
{
  overlay_inventory_callback = fnc;
  invarg = arg;
}

static
void overlay_draw_inventory
  (unsigned* buf, unsigned width, unsigned height)
{
  unsigned x = (width - (10 * 32)) / 2;
  unsigned y = height - 40;
  unsigned i;
  int isfilled;
  char c;
  unsigned amount;

  for (i=0; i < 10; i++) {
    overlay_inventory_callback(invarg, i, &isfilled, &c, &amount);
    if (isfilled) {
      char amstr[ 32 ];
      snprintf(amstr, sizeof(amstr), "%c\n%u", c, amount);
      overlay_string(buf, width, height, x + (i*32) + 10, y + 12, amstr);
    }
    for (unsigned j=0; j < 32; j++) {
      overlay_setpixel(buf, width, height, x + (i*32) + j, y);
    }
    for (unsigned j=0; j < 32; j++) {
      overlay_setpixel(buf, width, height, x + (i*32) + j, y + 32);
    }
    for (unsigned j=0; j < 32; j++) {
      overlay_setpixel(buf, width, height, x + (i*32), y + j);
    }
  }
  for (unsigned j=0; j < 32; j++) {
    overlay_setpixel(buf, width, height, x + (i*32), y + j);
  }
}

void overlay_set_flying
  (int f)
{
  flying = f;
}

void overlay_set_redlamp
  (int r)
{
  redlamp = r;
}

void overlay_draw_redlamp
  (unsigned* buf, unsigned width, unsigned height)
{
  for (unsigned i=0; i < 16; i++) {
    for (unsigned j=0; j < 16; j++) {
      overlay_setpixel_color(
        buf, width, height,
        width + i - 32,
        j + 16,
        0xff0000
      );
    }
  }
}

void overlay
  (unsigned* buf, unsigned width, unsigned height)
{
  if (flying) {
    overlay_hud(buf, width, height);
  } else {
    overlay_cross(buf, width, height);
  }
  for (unsigned i=0; i < textobjects.count; i++) {
    if (textobjects.list[ i ]->visible) {
      overlay_string(
        buf,
        width,
        height,
        textobjects.list[ i ]->x,
        textobjects.list[ i ]->y,
        textobjects.list[ i ]->text
      );
    }
  }
  if (inventory) {
    overlay_draw_inventory(buf, width, height);
  }
  if (redlamp) {
    if (time(0) % 2) {
      overlay_draw_redlamp(buf, width, height);
    }
  }
}


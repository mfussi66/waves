#ifndef GRAPHICS_H_INCLUDED_
#define GRAPHICS_H_INCLUDED_

#include "globals.h"
#include <allegro.h>

void start_allegro(int mode);

void build_gui(BITMAP* bmp, int col);

static inline void fastline_bottom_left(BITMAP *buf, int *p1, int *p2) {
  fastline(buf, p1[0], SCREEN_H - p1[1], p2[0], SCREEN_H - p2[1], 
          makecol(255, 255, 255));
}

void compute_point(double amplitude, int index, int line, int *p);

int graphics_thread(void *arg);

#endif

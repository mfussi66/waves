#ifndef GRAPHICS_H_INCLUDED_
#define GRAPHICS_H_INCLUDED_

#include "globals.h"
#include <allegro.h>

void start_allegro(int mode);

void close_allegro(void);

void build_gui(BITMAP* bmp, int col);

void fastline_bottom_left(BITMAP* buf, int x1, int y1, int x2, int y2, int col);

static inline int compute_target_x(uint32_t i, int offset) {
  return offset / 2 + (i) * (SCREEN_W - offset) / N_LINE_POINTS;
}

int graphics_thread(void *arg);

#endif

/* -------------------------- 
	 REAL TIME SYSTEMS
	 OBSTACLE AVOIDANCE
	   Mattia Fussi
	
	    GRAPHICS HEADER
 -------------------------- */

/* --- Include guard --- */

#ifndef GRAPHICS_H_INCLUDED_
#define GRAPHICS_H_INCLUDED_

#include <allegro.h>
#include <stdio.h>
#include "customdata.h"

/* Graphics */

void start_allegro(int mode);

void close_allegro(void);

void build_gui(BITMAP* bmp, int col);

void draw_exit_screen(BITMAP* bmp, int col);

void fastline_bottom_left(BITMAP* buf, int x1, int y1, int x2, int y2, int col);

#endif

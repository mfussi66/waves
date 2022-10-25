/* -------------------------- 
	 REAL TIME SYSTEMS
	 OBSTACLE AVOIDANCE
	   Mattia Fussi
	
	    GRAPHICS HEADER
 -------------------------- */

/* --- Include guard --- */

#ifndef GRAPHICS_H_INCLUDED_
#define GRAPHICS_H_INCLUDED_

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "customdata.h"

/* Graphics */

void start_allegro(int mode);

void close_allegro(void);

void build_gui(int col);

void draw_exit_screen(int col);

void fastline_bottom_left(int x1, int y1, int x2, int y2, int col);

#endif

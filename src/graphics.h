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

void update_plot(BITMAP* bmp, double* data, int coord_x, int coord_y, double scale);

void draw_msg(BITMAP* bmp, int mode, int x, int y);

#endif

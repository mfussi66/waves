/* --------------------------
         REAL TIME SYSTEMS
         OBSTACLE AVOIDANCE
           Mattia Fussi

          GRAPHICS SOURCE
 -------------------------- */

#include "graphics.h"

/* Graphics */

/*
 * Function:Init allegro engine and screen
 * ---------------------------
 */
void start_allegro(int mode) {
  allegro_init();

  set_color_depth(8);

  set_gfx_mode(mode, WIDTH_SCREEN, HEIGHT_SCREEN, 0, 0);
  install_keyboard();
  install_mouse();

  // enable_hardware_cursor();

  clear_to_color(screen, 0);

  printf("Allegro correctly initialized\n");

  return;
}

/*
 * Function: Closes allegro
 * ---------------------------
 */
void close_allegro(void) {
  allegro_exit();

  printf("Closing Allegro\n");
}

/*
 * Function: Draws gui
 * ---------------------------
 * Draws all the static gui objects
 */
void build_gui(BITMAP* bmp, int col) {
  // Environment frame
  rect(bmp, 10, 10, WIDTH_SCREEN - 10, HEIGHT_SCREEN - 10, col);
}

/*
 * Function: Show fancy exit text
 * ---------------------------
 */
void draw_exit_screen(BITMAP* bmp, int col) {
  textout_centre_ex(bmp, font, "_________________________", WIDTH_SCREEN / 2,
                    HEIGHT_SCREEN / 2 - 13, col, -1);
  textout_centre_ex(bmp, font, "   Simulation stopped    ", WIDTH_SCREEN / 2,
                    HEIGHT_SCREEN / 2, col, -1);
  textout_centre_ex(bmp, font, "  Thank you for playing  ", WIDTH_SCREEN / 2,
                    HEIGHT_SCREEN / 2 + 10, col, -1);
  textout_centre_ex(bmp, font, "_________________________", WIDTH_SCREEN / 2,
                    HEIGHT_SCREEN / 2 + 15, col, -1);
}

void fastline_bottom_left(BITMAP* buf, int x1, int y1, int x2, int y2, int col) {
    fastline(buf, x1, SCREEN_H - y1, x2, SCREEN_H - y2, col);
}

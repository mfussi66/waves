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

  printf("Closing Allegro.");
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

/*
 * Function: Plots update
 * ---------------------------
 * Updates the plot of the variable of interest
 */
void update_plot(BITMAP* bmp, double* data, int coord_x, int coord_y,
                 double scale) {
  int x = coord_x - (PLT_STEP * PLT_DATA_SIZE);
  int y = coord_y - (data[0] * scale) - PLT_FRAME_SIZE / 2;

  int x_prev = x;
  int y_prev = y;

  rectfill(bmp, coord_x - 100, coord_y - 100, coord_x - 1, coord_y - 1,
           makecol(0, 0, 0));

  for (int i = 0; i < PLT_DATA_SIZE; i++) {
    x = x + PLT_STEP;

    y = coord_y - (data[i] * scale) - PLT_FRAME_SIZE / 2;

    if (y <= (coord_y - PLT_FRAME_SIZE)) y = coord_y - PLT_FRAME_SIZE + 1;

    if (y >= coord_y) y = coord_y - 1;

    if (y_prev <= (coord_y - PLT_FRAME_SIZE))
      y_prev = coord_y - PLT_FRAME_SIZE + 1;

    if (y_prev >= coord_y) y_prev = coord_y - 1;

    if (getpixel(bmp, x_prev, y_prev) < 0) continue;
    if (getpixel(bmp, x, y) < 0) continue;

    fastline(bmp, x_prev, y_prev, x, y, COL_GREEN);

    x_prev = x;
    y_prev = y;
  }
}

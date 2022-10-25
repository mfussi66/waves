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
  al_init();



      ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_DISPLAY* disp = al_create_display(800, 800);
    ALLEGRO_FONT* font = al_create_builtin_font();

    al_install_keyboard();
    al_install_mouse();

  // enable_hardware_cursor();

  al_clear_to_color(al_map_rgb(0,0,0));

  printf("Allegro correctly initialized\n");

  return;
}

/*
 * Function: Closes allegro
 * ---------------------------
 */
void close_allegro(void) {

    // al_destroy_font(font);
    // al_destroy_display(disp);
    // al_destroy_timer(timer);
    // al_destroy_event_queue(queue);

  printf("Closing Allegro\n");
}

/*
 * Function: Draws gui
 * ---------------------------
 * Draws all the static gui objects
 */
void build_gui(int col) {
  // Environment frame

  al_draw_rectangle(10, 10, WIDTH_SCREEN - 10, HEIGHT_SCREEN - 10, al_map_rgb(255, 255, 255), 1.0);
}

/*
 * Function: Show fancy exit text
 * ---------------------------
//  */
// void draw_exit_screen(int col) {
//   textout_centre_ex(bmp, font, "_________________________", WIDTH_SCREEN / 2,
//                     HEIGHT_SCREEN / 2 - 13, col, -1);
//   textout_centre_ex(bmp, font, "   Simulation stopped    ", WIDTH_SCREEN / 2,
//                     HEIGHT_SCREEN / 2, col, -1);
//   textout_centre_ex(bmp, font, "  Thank you for playing  ", WIDTH_SCREEN / 2,
//                     HEIGHT_SCREEN / 2 + 10, col, -1);
//   textout_centre_ex(bmp, font, "_________________________", WIDTH_SCREEN / 2,
//                     HEIGHT_SCREEN / 2 + 15, col, -1);
// }

void fastline_bottom_left(int x1, int y1, int x2, int y2, int col) {
    al_draw_line(x1, 800 - y1, x2, 800 - y2, al_map_rgb(255, 255, 255), 1.0);
}

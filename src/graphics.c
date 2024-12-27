
#include "graphics.h"
#include "wmath.h"

#include <stdio.h>
#include <time.h>
#include <unistd.h> 
#include <threads.h>

void start_allegro(int mode) {
  allegro_init();
  set_color_depth(8);

  set_gfx_mode(mode, WIDTH_SCREEN, HEIGHT_SCREEN, 0, 0);
  install_keyboard();
  install_mouse();

  clear_to_color(screen, 0);
  printf("Allegro correctly initialized\n");
  return;
}

void close_allegro(void) {
  allegro_exit();
  printf("Closing Allegro\n");
}

void build_gui(BITMAP* bmp, int col) {
  rect(bmp, 10, 10, WIDTH_SCREEN - 10, HEIGHT_SCREEN - 10, col);
}

void fastline_bottom_left(BITMAP* buf, int x1, int y1, int x2, int y2, int col) {
    fastline(buf, x1, SCREEN_H - y1, x2, SCREEN_H - y2, col);
}

int graphics_task()
{

  BITMAP* buffer_gfx;

  buffer_gfx = create_bitmap(SCREEN_W, SCREEN_H);
  clear_to_color(buffer_gfx, 0);
  int WHITE = makecol(255, 255, 255);

  blit(buffer_gfx, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

  uint32_t read_index = 0;
  uint32_t read_offset = 0;

  double gaussian_kernel[N_LINE_POINTS];

  init_gaussian(N_LINE_POINTS, gaussian_kernel);

  while (key[KEY_ESC] == 0) {
    if (keypressed())
    {
       uint8_t scan = readkey() >> 8;
    }

    clear_to_color(buffer_gfx, 0);

  for (uint32_t i = 0; i < N_LINE_POINTS; ++i) {
      read_index = (i + read_offset) % (N_LINE_POINTS - 1);

      for (uint32_t l = 2; l < N_LINE_POINTS; ++l) {
        double p1[2] = {compute_target_x(i, 100),
                        10 * l + gaussian_kernel[i] * out_norm[read_index]};
        double p2[2] = {compute_target_x(i + 1, 100),
                        10 * l + gaussian_kernel[i] * out_norm[read_index + 1]};

        fastline_bottom_left(buffer_gfx, p1[0], p1[1], p2[0], p2[1], makecol(255, 255, 255));
      }
    }

    if (++read_offset > N_LINE_POINTS) read_offset = 0;

    blit(buffer_gfx, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

    thrd_sleep(&(struct timespec){.tv_nsec=2e6}, NULL); // sleep for 2msec
  }

  destroy_bitmap(buffer_gfx);
  close_allegro();

  return EXIT_SUCCESS;
}

#include "graphics.h"
#include "globals.h"
#include "wmath.h"

#include <stdio.h>
#include <string.h>
#include <threads.h>
#include <time.h>
#include <unistd.h>

void start_allegro(int mode) {
  allegro_init();
  set_color_depth(32);

  set_gfx_mode(mode, WIDTH_SCREEN, HEIGHT_SCREEN, 0, 0);
  install_keyboard();
  install_mouse();

  clear_to_color(screen, 0);
  printf("Allegro correctly initialized\n");
  return;
}

void compute_point(double amplitude, int index, int line, int *p) {
  p[0] = PAD_SIDE / 2 + (index) * (SCREEN_W - PAD_SIDE) / (N_SAMPLES_OUT);

  int amp = amplitude > PEAK_AMPLITUDE ? PEAK_AMPLITUDE : amplitude;
  p[1] = HEIGHT_SCREEN / N_VERT_LINES * (line + 1) + (int)amp;
}

int graphics_thread(void *arg) {

  start_allegro(GFX_AUTODETECT_WINDOWED);

  BITMAP *buffer_gfx;
  buffer_gfx = create_bitmap(SCREEN_W, SCREEN_H);
  blit(buffer_gfx, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

  uint32_t read_index = 0;
  uint32_t read_offset = 0;

  double gaussian_kernel[N_FREQ_BINS];
  init_gaussian(N_FREQ_BINS, gaussian_kernel, PEAK_AMPLITUDE, 25.0);

  double buf[N_FREQ_BINS];
  double vert[N_VERT_LINES];
  double matrix[N_VERT_LINES][N_SAMPLES_OUT];

  while (!key[KEY_ESC]) {
    if (keypressed()) {
      uint8_t scan = readkey() >> 8;
    }

    mtx_lock(&buffer_mutex);

    if (buffer_emptied) {
      mtx_unlock(&buffer_mutex);
      continue;
    }

    while (!buffer_ready && !key[KEY_ESC]) {
      cnd_wait(&buffer_cond, &buffer_mutex);
    }
    buffer_ready = 0;
    memcpy(buf, mono_buffer, (N_FREQ_BINS) * sizeof(double));
    mtx_unlock(&buffer_mutex);

    if (!squeeze_array(buf, vert, N_FREQ_BINS, N_VERT_LINES)) {
      printf("Error when squeezing array!\n");
    }

    shift_column_left(matrix);

    for (int i = 0; i < N_VERT_LINES; i++)
    {
      matrix[i][N_SAMPLES_OUT - 1] = vert[i];

      printf("vert(%d): %f\n", i, vert[i]);
    }
    
   for (int i = 0; i < (N_VERT_LINES); i++) { // For each column
    for (int j = 0; j < (N_SAMPLES_OUT); j++)   // For each row element
    {
      printf("(%d %d): %f\n", i, j, matrix[i][j]);
    }
  }

    clear_to_color(buffer_gfx, 0);

  for (int i = 0; i < N_SAMPLES_OUT - 1; i++) {
    for (int l = 0; l < N_VERT_LINES; l++) {

        int p1[2] = {0, 0};
        int p2[2] = {0, 0};

        compute_point(gaussian_kernel[i] * matrix[l][i ], i, l, p1);
        compute_point(gaussian_kernel[i] * matrix[l][i + 1], i + 1, l, p2);
        fastline_bottom_left(buffer_gfx, p1, p2);
        //printf("printing line %d sample %d\n", l, i);
      }
    }

    blit(buffer_gfx, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

    thrd_sleep(&(struct timespec){.tv_nsec = 0.033 * NANOSEC},
               NULL); // sleep for 15msec
  }

  destroy_bitmap(buffer_gfx);
  allegro_exit();

  return EXIT_SUCCESS;
}

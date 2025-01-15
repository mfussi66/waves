
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

void compute_point(float amplitude, int index, int line, int *p) {
  p[0] = PAD_SIDE / 2 + (index) * (SCREEN_W - PAD_SIDE) / (N_SAMPLES_OUT - 1);

  int max_amp = HEIGHT_SCREEN / N_VERT_LINES;
  int amp = amplitude > max_amp ? max_amp : amplitude;
  
  p[1] = max_amp * (line + 1) + (int)amp;
}

int graphics_thread(void *arg) {

  start_allegro(GFX_AUTODETECT_WINDOWED);

  srand(0);  

  BITMAP *buffer_gfx;
  buffer_gfx = create_bitmap(SCREEN_W, SCREEN_H);
  blit(buffer_gfx, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

  uint32_t read_index = 0;
  uint32_t read_offset = 0;

  float gaussian_kernel[N_SAMPLES_OUT];
  init_gaussian(N_SAMPLES_OUT, gaussian_kernel, PEAK_AMPLITUDE, VARIANCE);

  float buf[N_FREQ_BINS];
  float vert[N_VERT_LINES];
  float matrix[N_VERT_LINES][N_SAMPLES_OUT];

  memset(matrix, 0, sizeof(float) * N_VERT_LINES * N_SAMPLES_OUT);

  while (!key[KEY_ESC]) {
    if (keypressed()) {
      uint8_t scan = readkey() >> 8;
    }

    mtx_lock(&buffer_mutex);

    if (buffer_emptied) {
      mtx_unlock(&buffer_mutex);
    }

    while (!buffer_ready && !key[KEY_ESC]) {
      cnd_wait(&buffer_cond, &buffer_mutex);
    }
    buffer_ready = 0;
    memcpy(buf, mono_buffer, (N_FREQ_BINS) * sizeof(float));
    mtx_unlock(&buffer_mutex);

    if (!squeeze_array(buf, vert, N_FREQ_BINS, N_VERT_LINES)) {
      printf("Error when squeezing array!\n");
    }

    shift_column_left(matrix);

    for (int i = 0; i < N_VERT_LINES; i++)
    {
      matrix[i][N_SAMPLES_OUT - 1] = vert[i];
      
     // printf("vert(%d): %f\n", i, vert[i]);
    }
    
  //  for (int i = 0; i < (N_VERT_LINES); i++) { // For each column
  //   for (int j = 0; j < (N_SAMPLES_OUT); j++)   // For each row element
  //   {
  //     printf("(%d %d): %f\n", i, j, matrix[i][j]);
  //   }
  // }

    clear_to_color(buffer_gfx, 0);

  for (int i = 0; i < N_SAMPLES_OUT - 1; i++) {
    for (int l = 0; l < N_VERT_LINES - 1; l++) {

        int p1[2] = {0, 0};
        int p2[2] = {0, 0};

        int r = (rand() % (2 + 2 + 1)) - 2;
        r = 0;
        float amp1 = gaussian_kernel[i] * (matrix[l][i] + r);
        float amp2 = gaussian_kernel[i + 1] * (matrix[l][i + 1] + r);
        
        compute_point(amp1, i, l, p1);
        compute_point(amp2, i + 1, l, p2);
        fastline_bottom_left(buffer_gfx, p1, p2);
        //printf("printing line %d sample %d p2 (%f %f)\n", l, i+1,amp1, amp2);
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

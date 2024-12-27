#include <kissfft/kiss_fft.h>
#include <math.h>
#include <sndfile.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h> 

#include "graphics.h"

#define N_SAMPLES 128
#define PEAK_AMPLITUDE 50.0
#define N_LINE_POINTS 64

int linspace(double start, double end, double phase_angle, double* array) {
  double step = (end - start) / (double)N_SAMPLES;
  array[0] = start;

  for (size_t i = 0; i < N_SAMPLES; ++i) {
    array[i] = sin(start + (double)i * step + phase_angle);
  }

  return 0;
}

void sum_v(double* a, double* b, uint32_t N) {
  for (uint32_t i = 0; i < N; ++i) b[i] += a[i];
}

void mult_scalar(double a, double* b, uint32_t N) {
  for (uint32_t i = 0; i < N; ++i) b[i] *= a;
}

inline double r2d(double r) { return r * 180 / M_PI; }

inline double d2r(double d) { return d * M_PI / 180; }

void norm2_v(kiss_fft_cpx* cin, double* aout, uint32_t N) {
  for (uint32_t i = 0; i < N; ++i)
    aout[i] = sqrtf(cin[i].r * cin[i].r + cin[i].i * cin[i].i);
}

double norm2(kiss_fft_cpx* c) {
  return sqrt(c->r * c->r + c->i * c->i);
}

int compute_target_x(uint32_t i, int offset) {
  return offset / 2 + (i) * (SCREEN_W - offset) / N_LINE_POINTS;
}

void init_gaussian(int n_samples, double* array) {
  for (int32_t i = 0; i < n_samples; ++i) {
    array[i] = 0.4 * exp(-0.5 * (((double)i - (double)n_samples / 2.0) / 30.0) *
                         (((double)i - (double)n_samples / 2.0) / 30.0));
    // printf("%u: %f\n", i, array[i]);
  }
}

void movavg(double* in, double* out, int window) {
  int k = 1;
  for (uint32_t j = 0; j < N_SAMPLES; j += window) {
    for (int i = j; i < window * k; ++i) {
      out[j] += in[i];
      out[j] /= window;
      // printf("[j] %d [i] %d [r] %f\n", j, i, out[i]);
    }
    k++;
  }
}

void apply_gaussian(int n_samples, double* array, double* kernel) {
  for (uint32_t i = 0; i < n_samples; ++i) {
    array[i] = array[i] * kernel[i];
  }
}

int main(int argc, char* argv[]) {

  if(argc < 2) {

    printf("Error: no audio file was passed\n");
    return 1;
  }

  SF_INFO file_info;
  SNDFILE* sndfile = sf_open(argv[1], SFM_READ, &file_info);

  if (sndfile == NULL || file_info.frames == 0) {
    printf("Error: file not found\n");
    return 1;
  }

  BITMAP* buffer_gfx;
  uint8_t scan = 0;
  struct timespec remaining, request = {0, 2 * 1e7};

  kiss_fft_cfg cfg;
  kiss_fft_cpx in[N_SAMPLES], out[N_SAMPLES];
  double out_avg[N_LINE_POINTS];
  cfg = kiss_fft_alloc(N_SAMPLES, 0 /*is_inverse_fft*/, NULL, NULL);

  double out_matrix[N_LINE_POINTS][N_LINE_POINTS];
  int WHITE = makecol(255, 255, 255);

  double fourth[N_SAMPLES];

  double gaussian_kernel[N_LINE_POINTS];

  init_gaussian(N_LINE_POINTS, gaussian_kernel);

  start_allegro(GFX_AUTODETECT_WINDOWED);

  buffer_gfx = create_bitmap(SCREEN_W, SCREEN_H);
  clear_to_color(buffer_gfx, 0);

  blit(buffer_gfx, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
  uint32_t read_index = 0;
  uint32_t read_offset = 0;

  printf(
      "File info:\n\tRate: %d\n\tFormat: %d\n\tChannels: %d\n\tFrames: %lu\n",
      file_info.samplerate, file_info.format, file_info.channels,
      file_info.frames);

  float* samples = malloc(file_info.frames * file_info.channels * sizeof(float));

  sf_count_t count = -1;
  if (samples != NULL) {
    count = sf_read_float(sndfile, samples, file_info.frames * file_info.channels);
    printf("count: %lu frames: %lu \n", count, file_info.frames);
  }

  sf_count_t samples_counter = 0;

  while (key[KEY_ESC] == 0) {
    if (keypressed()) scan = readkey() >> 8;

    clear_to_color(buffer_gfx, 0);

    if ((samples_counter + N_SAMPLES) >= count) {
      break;
    }

    memcpy(fourth, &samples[samples_counter], sizeof(float) * N_SAMPLES);

    samples_counter += N_SAMPLES;

    for (uint32_t i = 0; i < N_SAMPLES; i++) {
      in[i].r = fourth[i];
      in[i].i = 0;
    }

    kiss_fft(cfg, in, out);

    double out_norm[N_SAMPLES];
    norm2_v(out, out_norm, N_SAMPLES);

    movavg(out_norm, out_avg, 32);

    for (uint32_t i = 0; i < N_LINE_POINTS; ++i) {
      read_index = (i + read_offset) % (N_LINE_POINTS - 1);

      for (uint32_t l = 2; l < N_LINE_POINTS; ++l) {
        double p1[2] = {compute_target_x(i, 100),
                        10 * l + gaussian_kernel[i] * out_norm[read_index]};
        double p2[2] = {compute_target_x(i + 1, 100),
                        10 * l + gaussian_kernel[i] * out_norm[read_index + 1]};

        fastline_bottom_left(buffer_gfx, p1[0], p1[1], p2[0], p2[1], WHITE);
      }
    }

    if (++read_offset > N_LINE_POINTS) read_offset = 0;

    blit(buffer_gfx, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

    usleep(1999);
  }

  free(cfg);

  sf_close(sndfile);

  destroy_bitmap(buffer_gfx);
  close_allegro();

  free(samples);

  return 0;
}

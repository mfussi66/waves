#include "wmath.h"
#include "globals.h"

#include <kissfft/kiss_fft.h>

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

void linspace(float start, float end, float phase_angle, float *array) {
  float step = (end - start) / (float)N_SAMPLES_IN;
  array[0] = start;

  for (size_t i = 0; i < N_SAMPLES_IN; ++i) {
    array[i] = sin(start + (float)i * step + phase_angle);
  }

  return;
}

int squeeze_array(float *in, float *out, size_t N_in, size_t N_out) {

  if (N_in % N_out != 0) {
    printf("Invalid size for squeezing!\n");
    return 0;
  }

  size_t ratio = N_in / N_out;

  for (size_t i = 0; i < N_out; i++) {
    float m = -INFINITY;
    for (size_t j = ratio * i; j < ratio * (i + 1); j++) {

      if (in[j] > m) {
        m = in[j];
      }
    }

    out[i] = m;
  }

  return 1;
}

void norm2_v(kiss_fft_cpx *c_in, float *a_out, uint32_t N) {

  float max = 1e-9;

  for (uint32_t i = 0; i < N; ++i) {
    a_out[i] = sqrt(c_in[i].r * c_in[i].r + 
    c_in[i].i * c_in[i].i);

    if (a_out[i] < 1e-9) {
      a_out[i] = 1e-9;
    }
  }
}

void init_gaussian(int n_samples, float *array, float amplitude,
                   float variance) {
  for (int32_t i = 0; i < n_samples; ++i) {
    array[i] = amplitude *
               exp(-0.5 * (((float)i - (float)n_samples / 2.0) / variance) *
                   (((float)i - (float)n_samples / 2.0) / variance));
  }
}

void apply_gaussian(int n_samples, float *array, float *kernel) {
  for (uint32_t i = 0; i < n_samples; ++i) {
    array[i] = array[i] * kernel[i];
  }
}

void shift_column_left(float m[N_VERT_LINES][N_SAMPLES_OUT]) {
    // For each row
    for (int i = 0; i < N_VERT_LINES; i++) {
        // For each column (except last)
        for (int j = 0; j < N_SAMPLES_OUT - 1; j++) {
            m[i][j] = m[i][j + 1];
        }
        // Clear the last column
        m[i][N_SAMPLES_OUT - 1] = 0;
    }
}
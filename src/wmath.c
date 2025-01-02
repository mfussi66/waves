#include "wmath.h"
#include "globals.h"

#include <kissfft/kiss_fft.h>

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

void linspace(double start, double end, double phase_angle, double *array) {
  double step = (end - start) / (double)N_SAMPLES_IN;
  array[0] = start;

  for (size_t i = 0; i < N_SAMPLES_IN; ++i) {
    array[i] = sin(start + (double)i * step + phase_angle);
  }

  return;
}

int squeeze_array(double *in, double *out, size_t N_in, size_t N_out) {

  if (N_in % N_out != 0) {
    printf("Invalid size for squeezing!\n");
    return 0;
  }

  size_t ratio = N_in / N_out;

  for (size_t i = 0; i < N_out; i++) {
    double m = -INFINITY;
    for (size_t j = ratio * i; j < ratio * (i + 1); j++) {

      if (in[j] > m) {
        m = in[j];
      }
    }

    out[i] = m;
  }

  return 1;
}

void norm2_v(kiss_fft_cpx *c_in, double *a_out, uint32_t N) {

  double max = 1e-9;

  for (uint32_t i = 0; i < N; ++i) {

    double mag = fabs(c_in[i].r);

    if (mag < 1e-9) {
      mag = 1e-9;
    }

    a_out[i] = mag;

    if (mag > max)
      max = mag;
  }

  for (uint32_t i = 0; i < N; ++i) {
    a_out[i] = a_out[i] / max;
  }
}

void init_gaussian(int n_samples, double *array, double amplitude,
                   double variance) {
  for (int32_t i = 0; i < n_samples; ++i) {
    array[i] = amplitude *
               exp(-0.5 * (((double)i - (double)n_samples / 2.0) / variance) *
                   (((double)i - (double)n_samples / 2.0) / variance));
  }
}

void apply_gaussian(int n_samples, double *array, double *kernel) {
  for (uint32_t i = 0; i < n_samples; ++i) {
    array[i] = array[i] * kernel[i];
  }
}

void shift_column_left(double m[N_VERT_LINES][N_SAMPLES_OUT]) {

  for (int i = 1; i < (N_SAMPLES_OUT); i++) { // For each column
    for (int j = 1; j < (N_VERT_LINES); j++)   // For each row element
    {
      m[i - 1][j - 1] = m[i][j];
    }
  }

  //memset(m[N_SAMPLES_OUT - 1], 0, N_VERT_LINES); // set last column to zero
}
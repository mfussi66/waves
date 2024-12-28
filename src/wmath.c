#include "wmath.h"
#include "globals.h"

#include <kissfft/kiss_fft.h>

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

void linspace(double start, double end, double phase_angle, double* array) {
  double step = (end - start) / (double)N_SAMPLES;
  array[0] = start;

  for (size_t i = 0; i < N_SAMPLES; ++i) {
    array[i] = sin(start + (double)i * step + phase_angle);
  }

  return;
}

void sum_v(double* a, double* b, uint32_t N) {
  for (uint32_t i = 0; i < N; ++i) b[i] += a[i];
}

void mult_scalar(double a, double* b, uint32_t N) {
  for (uint32_t i = 0; i < N; ++i) b[i] *= a;
}

void norm2_v(kiss_fft_cpx* cin, double* aout, uint32_t N) {
  for (uint32_t i = 0; i < N; ++i)
    aout[i] = sqrtf(cin[i].r * cin[i].r + cin[i].i * cin[i].i);
}

void init_gaussian(int n_samples, double* array) {
  for (int32_t i = 0; i < n_samples; ++i) {
    array[i] = 0.4 * exp(-0.5 * (((double)i - (double)n_samples / 2.0) / 30.0) *
                         (((double)i - (double)n_samples / 2.0) / 30.0));
  }
}

void movavg(double* in, double* out, int window) {
  int k = 1;
  for (uint32_t j = 0; j < N_SAMPLES; j += window) {
    for (int i = j; i < window * k; ++i) {
      out[j] += in[i];
      out[j] /= window;
    }
    k++;
  }
}

void apply_gaussian(int n_samples, double* array, double* kernel) {
  for (uint32_t i = 0; i < n_samples; ++i) {
    array[i] = array[i] * kernel[i];
  }
}
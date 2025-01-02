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

void sum_v(double *a, double *b, uint32_t N) {
  for (uint32_t i = 0; i < N; ++i)
    b[i] += a[i];
}

void mult_scalar(double a, double *b, uint32_t N) {
  for (uint32_t i = 0; i < N; ++i)
    b[i] *= a;
}

double maxf(double *a, size_t N) {
  double m = -INFINITY;

  for (size_t i = 0; i < N; i++) {

    printf("max[%lu] %f %f\n", i, a[i], m);
    if (a[i] > m) {
      m = a[i];
    }
  }

  return m;
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

void norm2_v(kiss_fft_cpx *cin, double *aout, uint32_t N) {

  double max = 1e-9;

  for (uint32_t i = 0; i < N; ++i) {

    double mag = fabs(cin[i].r);

    if (mag < 1e-9) {
      mag = 1e-9;
    }

    aout[i] = mag;

    if (mag > max)
      max = mag;
  }

  for (uint32_t i = 0; i < N; ++i) {
    aout[i] = aout[i] / max;
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

void movavg(double *in, double *out, int window) {
  int k = 1;
  for (uint32_t j = 0; j < N_SAMPLES_IN; j += window) {
    for (int i = j; i < window * k; ++i) {
      out[j] += in[i];
      out[j] /= window;
    }
    k++;
  }
}

void apply_gaussian(int n_samples, double *array, double *kernel) {
  for (uint32_t i = 0; i < n_samples; ++i) {
    array[i] = array[i] * kernel[i];
  }
}
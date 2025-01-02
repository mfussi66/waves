
#ifndef WMATH_H
#define WMATH_H

#include "globals.h"

#include <kissfft/kiss_fft.h>

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

void linspace(double start, double end, double phase_angle, double *array);

int squeeze_array(double *in, double *out, size_t N_in, size_t N_out);

void norm2_v(kiss_fft_cpx *c_in, double *a_out, uint32_t N);

void init_gaussian(int n_samples, double* array, double amplitude, double variance);

void apply_gaussian(int n_samples, double *array, double *kernel);

void shift_column_left(double m[N_VERT_LINES][N_SAMPLES_OUT]);

static inline double norm2(kiss_fft_cpx* c) {
  return sqrt(c->r * c->r + c->i * c->i);
}

#endif

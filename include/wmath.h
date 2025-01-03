
#ifndef WMATH_H
#define WMATH_H

#include "globals.h"

#include <kissfft/kiss_fft.h>

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

void linspace(float start, float end, float phase_angle, float *array);

int squeeze_array(float *in, float *out, size_t N_in, size_t N_out);

void norm2_v(kiss_fft_cpx *c_in, float *a_out, uint32_t N);

void init_gaussian(int n_samples, float* array, float amplitude, float variance);

void apply_gaussian(int n_samples, float *array, float *kernel);

void shift_column_left(float m[N_VERT_LINES][N_SAMPLES_OUT]);

static inline float norm2(kiss_fft_cpx* c) {
  return sqrt(c->r * c->r + c->i * c->i);
}

#endif

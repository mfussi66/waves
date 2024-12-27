
#ifndef WMATH_H
#define WMATH_H

#include "constants.h"

#include <kissfft/kiss_fft.h>

#include <stdlib.h>
#include <stdint.h>
#include <math.h>


void linspace(double start, double end, double phase_angle, double *array);

void sum_v(double *a, double *b, uint32_t N);

void mult_scalar(double a, double *b, uint32_t N);

void norm2_v(kiss_fft_cpx *cin, double *aout, uint32_t N);

void init_gaussian(int n_samples, double *array);

void movavg(double *in, double *out, int window);

void apply_gaussian(int n_samples, double *array, double *kernel);

static inline double norm2(kiss_fft_cpx* c) {
  return sqrt(c->r * c->r + c->i * c->i);
}

#endif
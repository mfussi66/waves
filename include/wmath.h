
#ifndef WMATH_H
#define WMATH_H

#include "globals.h"

#include <kissfft/kiss_fft.h>

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

int squeeze_array(float *in, float *out, size_t N_in, size_t N_out);

void norm2_v(kiss_fft_cpx *c_in, float *a_out, uint32_t N);

void init_gaussian(int n_samples, float *array, float amplitude,
                   float variance);

void shift_column_left(float m[N_VERT_LINES][N_SAMPLES_OUT]);

#endif

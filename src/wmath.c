#include "wmath.h"
#include "globals.h"

#include <kissfft/kiss_fft.h>

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

int squeeze_array(float *in, float *out, size_t N_in, size_t N_out) {

  if (N_in % N_out != 0) {
    printf("Invalid size for squeezing!\n");
    return 0;
  }

  size_t ratio = N_in / N_out;

  for (size_t i = 0; i < N_out; i++) {
    float m = -INFINITY;

    float acc = 0;

    for (size_t j = ratio * i; j < ratio * (i + 1); j++) {
      acc +=  in[j];
      // if (in[j] > m) {
      //   m = in[j];
      // }
    }
    //out[i] = m;
    out[i] = acc / ratio;
  }

  return 1;
}

void norm2_v(kiss_fft_cpx *c_in, float *a_out, uint32_t N) {

  for (size_t i = 0; i < N; ++i) {
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
    float arg = pow(((float)i - (float)n_samples / 2.0) / variance, 2);
    array[i] = amplitude * exp(- pow(0.5 * arg, 3));
  }
}

void shift_column_left(float m[N_VERT_LINES][N_SAMPLES_OUT]) {
    for (int i = 0; i < N_VERT_LINES; i++) {
        for (int j = 0; j < N_SAMPLES_OUT - 1; j++) {
            m[i][j] = m[i][j + 1];
        }
        m[i][N_SAMPLES_OUT - 1] = 0;
    }
}
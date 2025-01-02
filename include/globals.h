#ifndef CUSTOMDATA_H
#define CUSTOMDATA_H

#include <threads.h>

#define NANOSEC 1000000000LL

#define N_SAMPLES_IN 64             // Max number of audio input samples
#define N_FREQ_BINS (N_SAMPLES_IN/2) // Number of fft bins
#define N_SAMPLES_OUT 8      // How many fft bins to compute and bufferize
#define N_VERT_LINES 8    // Number of vertical lines


#define PEAK_AMPLITUDE 100 // Max height of waven in px

#define WIDTH_SCREEN 1024
#define HEIGHT_SCREEN 1024
#define PAD_SIDE 100

extern mtx_t buffer_mutex;
extern cnd_t buffer_cond;
extern double mono_buffer[N_FREQ_BINS];
extern int buffer_ready;
extern int buffer_emptied;

#endif

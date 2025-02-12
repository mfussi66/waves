#ifndef CUSTOMDATA_H
#define CUSTOMDATA_H

#include <threads.h>

#define NANOSEC 1000000000LL

#define N_SAMPLES_IN 1024             // Max number of audio input samples
#define N_FREQ_BINS (N_SAMPLES_IN/2) // Number of fft bins
#define N_SAMPLES_OUT 256     // How many fft bins to compute and bufferize
#define N_VERT_LINES 128    // Number of vertical lines

#define PEAK_AMPLITUDE 15 //max height of waven in px
#define VARIANCE 60.0
#define WIDTH_SCREEN 1280
#define HEIGHT_SCREEN 1280
#define PAD_SIDE 100

extern mtx_t buffer_mutex;
extern cnd_t buffer_cond;
extern float mono_buffer[N_FREQ_BINS];
extern int buffer_ready;
extern int buffer_emptied;

#endif

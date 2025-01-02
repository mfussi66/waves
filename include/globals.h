#ifndef CUSTOMDATA_H
#define CUSTOMDATA_H

#include <threads.h>

#define NANOSEC 1000000000LL

#define N_SAMPLES 512       // Max number of audio input samples
#define PEAK_AMPLITUDE 120  // Max height of waven in px
#define N_VERT_LINES 64     // Number of vertical lines

#define WIDTH_SCREEN 1024
#define HEIGHT_SCREEN 1024 
#define PAD_SIDE 100

extern mtx_t buffer_mutex;
extern cnd_t buffer_cond;
extern double mono_buffer[N_SAMPLES/2];
extern int buffer_ready;
extern int buffer_emptied;

#endif

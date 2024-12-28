#ifndef CUSTOMDATA_H
#define CUSTOMDATA_H

#include <threads.h>

#define NANOSEC 1000000000LL

#define N_SAMPLES 64
#define PEAK_AMPLITUDE 0.2
#define N_LINE_POINTS 64

#define WIDTH_SCREEN 1024
#define HEIGHT_SCREEN 1024 
#define PAD_SIDE 100

extern mtx_t buffer_mutex;
extern cnd_t buffer_cond;
extern double mono_buffer[N_SAMPLES];
extern int buffer_ready;
extern int buffer_emptied;

#endif

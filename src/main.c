
#include "graphics.h"
#include "waves.h"
#include "constants.h"
#include "wmath.h"

#include <sndfile.h>
#include <kissfft/kiss_fft.h>

#include <math.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h> 
#include <threads.h>

#define N_SAMPLES 128
#define PEAK_AMPLITUDE 50.0
#define N_LINE_POINTS 64


int main(int argc, char* argv[]) {

  if(argc < 2) {

    printf("Error: no audio file was passed\n");
    return 1;
  }

  SF_INFO file_info;
  SNDFILE* sndfile = sf_open(argv[1], SFM_READ, &file_info);

  if (sndfile == NULL || file_info.frames == 0) {
    printf("Error: file not found\n");
    return 1;
  }


  start_allegro(GFX_AUTODETECT_WINDOWED);


  return 0;
}

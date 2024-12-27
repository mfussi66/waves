#include "waves.h"

#include <time.h>
#include <unistd.h> 
#include <threads.h>

int waves_thread(void* arg){

  SF_INFO file_info;
  SNDFILE* sndfile = sf_open(argv[1], SFM_READ, &file_info);

  kiss_fft_cfg cfg;
  kiss_fft_cpx in[N_SAMPLES], out[N_SAMPLES];
  double out_avg[N_LINE_POINTS];
  cfg = kiss_fft_alloc(N_SAMPLES, 0 /*is_inverse_fft*/, NULL, NULL);

  // double out_matrix[N_LINE_POINTS][N_LINE_POINTS];

  double fourth[N_SAMPLES];

  printf(
      "File info:\n\tRate: %d\n\tFormat: %d\n\tChannels: %d\n\tFrames: %lu\n",
      file_info.samplerate, file_info.format, file_info.channels,
      file_info.frames);

  float* samples = malloc(file_info.frames * file_info.channels * sizeof(float));

  sf_count_t count = -1;
  if (samples != NULL) {
    count = sf_read_float(sndfile, samples, file_info.frames * file_info.channels);
    printf("count: %lu frames: %lu \n", count, file_info.frames);
  }

  sf_count_t samples_counter = 0;


  while(1)
  {

    if ((samples_counter + N_SAMPLES) >= count) {
      break;
    }

    memcpy(fourth, &samples[samples_counter], sizeof(float) * N_SAMPLES);

    samples_counter += N_SAMPLES;

    for (uint32_t i = 0; i < N_SAMPLES; i++) {
      in[i].r = fourth[i];
      in[i].i = 0;
    }

    kiss_fft(cfg, in, out);

    double out_norm[N_SAMPLES];
    norm2_v(out, out_norm, N_SAMPLES);

    movavg(out_norm, out_avg, 32);
    thrd_sleep(&(struct timespec){.tv_nsec=1e5}, NULL); // sleep for 10msec


  }

  free(cfg);

  sf_close(sndfile);
  free(samples);

}
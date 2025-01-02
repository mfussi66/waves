#include "waves.h"
#include "globals.h"

#include <threads.h>
#include <time.h>
#include <unistd.h>

// Get the current time in nanoseconds
long long current_time_ns() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (long long)ts.tv_sec * NANOSEC + ts.tv_nsec;
}

int waves_thread(void *arg) {

  const char *path = ((char *)arg);

  SF_INFO file_info;
  SNDFILE *sndfile = sf_open(path, SFM_READ, &file_info);

  kiss_fftr_cfg cfg;
  kiss_fft_scalar in[N_SAMPLES];
  kiss_fft_cpx out[N_SAMPLES/2];

  double out_avg[N_VERT_LINES];
  cfg = kiss_fftr_alloc(N_SAMPLES, 0 /*is_inverse_fft*/, NULL, NULL);

  printf("File info:\n\tName: %s\n\tRate: %d\n\tFormat: %d\n\tChannels: "
         "%d\n\tFrames: %lu\n",
         path, file_info.samplerate, file_info.format, file_info.channels,
         file_info.frames);

  double *samples =
      malloc(file_info.frames * file_info.channels * sizeof(double));

  sf_count_t count = -1;
  if (samples != NULL) {
    count =
        sf_read_double(sndfile, samples, file_info.frames * file_info.channels);
    printf("count: %lu frames: %lu \n", count, file_info.frames);
  }

  sf_count_t samples_counter = 0;
  long long duration =
      (long long)file_info.frames / file_info.samplerate * NANOSEC;

  while ((samples_counter + N_SAMPLES) < count) {

    long long start_time = current_time_ns();

    for (uint32_t i = 0; i < N_SAMPLES; i++) {
      in[i] = samples[samples_counter + i];
      //printf("in %f\n", in[i]);
    }

    kiss_fftr(cfg, in, out);

    mtx_lock(&buffer_mutex);
    norm2_v(out, mono_buffer, N_SAMPLES/2);
    buffer_ready = 1;
    cnd_signal(&buffer_cond);
    mtx_unlock(&buffer_mutex);

    // movavg(mono_buffer, out_avg, 32);

    samples_counter += N_SAMPLES;

    long long elapsed_time = current_time_ns() - start_time;

    if (elapsed_time < duration) {
      long long sleep_t =
          duration / file_info.frames * N_SAMPLES - elapsed_time;
      thrd_sleep(&(struct timespec){.tv_sec = sleep_t / NANOSEC,
                                    .tv_nsec = sleep_t % NANOSEC},
                 NULL);
    }
  }

  mtx_lock(&buffer_mutex);
  buffer_ready = 1;
  buffer_emptied = 1;
  cnd_signal(&buffer_cond);
  mtx_unlock(&buffer_mutex);

  free(cfg);

  sf_close(sndfile);
  free(samples);

  printf("Waves exiting \n");

  return EXIT_SUCCESS;
}


#include "graphics.h"
#include "waves.h"
#include "globals.h"

#include <stdio.h>
#include <threads.h>

mtx_t buffer_mutex;
cnd_t buffer_cond;
double mono_buffer[N_SAMPLES/2+1]; 
int buffer_ready = 0;
int buffer_emptied = 0;

int main(int argc, char* argv[]) {

  if(argc < 2) {

    printf("Error: no audio file was passed\n");
    printf("Usage: waves <path/to/wav_file.wav>\n");
    return EXIT_FAILURE;
  }

  if (access(argv[1], F_OK) != 0) {
      printf("Error: file not found\n");
      return EXIT_FAILURE;
  }

  int r = 0;

  thrd_t graphics_tid;
  thrd_t waves_tid;

  mtx_init(&buffer_mutex, mtx_plain);
  cnd_init(&buffer_cond);

  thrd_create(&graphics_tid, graphics_thread, NULL);
  thrd_create(&waves_tid, waves_thread, argv[1]);


  thrd_join(graphics_tid, &r);
  thrd_join(waves_tid, &r);

  return r;
}

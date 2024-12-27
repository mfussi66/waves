
#include "graphics.h"
#include "waves.h"
#include "constants.h"

#include <stdio.h>
#include <threads.h>

int main(int argc, char* argv[]) {

  if(argc < 2) {

    printf("Error: no audio file was passed\n");
    return 1;
  }

  if (access(argv[1], F_OK) != 0) {
      printf("Error: file not found\n");
      return 1;
  }

  thrd_t graphics_tid;
  thrd_t waves_tid;
  thrd_create(&graphics_tid, graphics_task, NULL);
  thrd_create(&waves_tid, waves_thread, NULL);

  int r = 0;
  thrd_join(graphics_tid, &r);
  thrd_join(waves_tid, &r);

  return 0;
}

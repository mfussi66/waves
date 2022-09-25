#include <stdio.h>
#include "graphics.h"
#include <math.h>
#include <time.h>

#define N_STEPS 128
#define PEAK_AMPLITUDE 50

int linspace(double start, double end, double phase_angle, double* array) {
    double step = (end - start) / N_STEPS;
    array[0] = start;

    for(uint8_t i = 0; i < N_STEPS; ++i) {
        array[i] = sin(start + (double)i * step + phase_angle);
    }

    return 0;
}


double r2d(double r) {
    return r * 180/M_PI;
}

double d2r(double d) {
    return d * M_PI / 180;
}

int main() {

    BITMAP* buffer_gfx;
    uint8_t scan = 0;
    struct timespec remaining, request = {0, 1e7};


    int WHITE = makecol(255, 255, 255);

    double first[N_STEPS];
    double second[N_STEPS];
    double third[N_STEPS];
    double fourth[N_STEPS];

    linspace(0, 4 * M_PI, d2r(0), first);
    linspace(0, 8 * M_PI, d2r(0), second);
    linspace(0, 16 * M_PI, d2r(0), third);
    linspace(0, 24 * M_PI, d2r(0), fourth);

    // Create Graphics Thread
    start_allegro(GFX_AUTODETECT_WINDOWED);

    buffer_gfx = create_bitmap(SCREEN_W, SCREEN_H);
    clear_to_color(buffer_gfx, 0);
    build_gui(buffer_gfx, makecol(255, 255, 255));

    blit(buffer_gfx, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    uint32_t ri = 0;
    uint32_t j = 0;
    // Check for pressed key
    while (key[KEY_ESC] == 0) {
        if (keypressed()) scan = readkey() >> 8;

        clear_to_color(buffer_gfx, 0);
        build_gui(buffer_gfx, makecol(255, 255, 255));


        for(uint32_t i = 0; i < N_STEPS; ++i) {
            ri = (i + j) % (N_STEPS);

            fastline(buffer_gfx, 10 + (i) * 780 / N_STEPS, 100 + PEAK_AMPLITUDE * first[ri],
                                 10 + (i+1) * 780 / N_STEPS, 100 + PEAK_AMPLITUDE * first[ri+1], makecol(255, 255, 255));
            fastline(buffer_gfx, 10 + (i) * 780 / N_STEPS, 300 + PEAK_AMPLITUDE * second[ri],
                                 10 + (i+1) * 780 / N_STEPS, 300 + PEAK_AMPLITUDE * second[ri+1], makecol(255, 255, 255));
            fastline(buffer_gfx, 10 + (i) * 780 / N_STEPS, 500 + PEAK_AMPLITUDE * third[ri],
                                 10 + (i+1) * 780 / N_STEPS, 500 + PEAK_AMPLITUDE * third[ri+1], makecol(255, 255, 255));
            fastline(buffer_gfx, 10 + (i) * 780 / N_STEPS, 700 + PEAK_AMPLITUDE * fourth[ri],
                                 10 + (i+1) * 780 / N_STEPS, 700 + PEAK_AMPLITUDE * fourth[ri+1], makecol(255, 255, 255));
        }

        if(++j > N_STEPS) j = 0;

        blit(buffer_gfx, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        nanosleep(&request, &remaining);
    }

    destroy_bitmap(buffer_gfx);
    close_allegro();

    return 0;
}

#include <stdio.h>
#include "graphics.h"
#include <math.h>
#include <time.h>
#include "extern/kissfft/kiss_fft.h"

#define N_STEPS 128
#define PEAK_AMPLITUDE 50.0

int linspace(float start, float end, float phase_angle, float* array) {
    float step = (end - start) / (float)N_STEPS;
    array[0] = start;

    for(uint8_t i = 0; i < N_STEPS; ++i) {
        array[i] = sin(start + (float)i * step + phase_angle);
    }

    return 0;
}


float r2d(float r) {
    return r * 180/M_PI;
}

float d2r(float d) {
    return d * M_PI / 180;
}

void complex2ampli(kiss_fft_cpx* cin, float* aout, uint32_t N) {
    for(uint32_t i = 0; i < N; ++i)
        aout[i] = sqrtf(cin[i].r * cin[i].r + cin[i].i * cin[i].i);
}

int main(int argc, char *argv[]) {

    BITMAP* buffer_gfx;
    uint8_t scan = 0;
    struct timespec remaining, request = {0, 33 * 1e6};

    kiss_fft_cfg cfg;
    kiss_fft_cpx in[N_STEPS], out[N_STEPS];
    cfg = kiss_fft_alloc(N_STEPS, 0/*is_inverse_fft*/, NULL, NULL);

    int WHITE = makecol(255, 255, 255);

    float first[N_STEPS];
    float second[N_STEPS];
    float third[N_STEPS];
    float fourth[N_STEPS];

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
    uint32_t read_index = 0;
    uint32_t read_offset = 0;

    // Check for pressed key
    while (key[KEY_ESC] == 0) {
        if (keypressed()) scan = readkey() >> 8;

        clear_to_color(buffer_gfx, 0);
        build_gui(buffer_gfx, makecol(255, 255, 255));


        for (uint32_t i = 0; i < N_STEPS; i++)
            in[i].r = fourth[i], in[i].i = 0;

        kiss_fft(cfg, in, out);

        for(uint32_t i = 0; i < N_STEPS; ++i) {
            read_index = (i + read_offset) % (N_STEPS);

            fastline(buffer_gfx, 10 + (i) * 780 / N_STEPS, 100 + 1e-2 * (pow(out[read_index].r,2.0) + pow(out[read_index].i,2)),
                                 10 + (i+1) * 780 / N_STEPS, 100 + 1e-2 * (pow(out[read_index+1].r,2.0) + pow(out[read_index+1].i,2)), makecol(255, 255, 255));
            fastline(buffer_gfx, 10 + (i) * 780 / N_STEPS, 300 + PEAK_AMPLITUDE * second[read_index],
                                 10 + (i+1) * 780 / N_STEPS, 300 + PEAK_AMPLITUDE * second[read_index+1], makecol(255, 255, 255));
            fastline(buffer_gfx, 10 + (i) * 780 / N_STEPS, 500 + PEAK_AMPLITUDE * third[read_index],
                                 10 + (i+1) * 780 / N_STEPS, 500 + PEAK_AMPLITUDE * third[read_index+1], makecol(255, 255, 255));
            fastline(buffer_gfx, 10 + (i) * 780 / N_STEPS, 700 + PEAK_AMPLITUDE * fourth[read_index],
                                 10 + (i+1) * 780 / N_STEPS, 700 + PEAK_AMPLITUDE * fourth[read_index+1], makecol(255, 255, 255));
        }

        if(++read_offset > N_STEPS) read_offset = 0;

        blit(buffer_gfx, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        nanosleep(&request, &remaining);
    }

    free(cfg);

    destroy_bitmap(buffer_gfx);
    close_allegro();

    return 0;
}

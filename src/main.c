#include <stdio.h>
#include "graphics.h"
#include <math.h>
#include <time.h>
#include "extern/kissfft/kiss_fft.h"
#include <sndfile.h>

#define N_SAMPLES 2048
#define PEAK_AMPLITUDE 50.0
#define N_LINE_POINTS 64
int linspace(float start, float end, float phase_angle, float* array) {
    float step = (end - start) / (float)N_SAMPLES;
    array[0] = start;

    for(size_t i = 0; i < N_SAMPLES; ++i) {
        array[i] = sin(start + (float)i * step + phase_angle);
    }

    return 0;
}

void sum_v(float* a, float* b, uint32_t N) {
    for(uint32_t i = 0; i < N; ++i) b[i] += a[i];
}

void mult_scalar(float a, float* b, uint32_t N) {
    for(uint32_t i = 0; i < N; ++i) b[i] *= a;
}

float r2d(float r) {
    return r * 180/M_PI;
}

float d2r(float d) {
    return d * M_PI / 180;
}

void norm2_v(kiss_fft_cpx* cin, float* aout, uint32_t N) {
    for(uint32_t i = 0; i < N; ++i)
        aout[i] = sqrtf(cin[i].r * cin[i].r + cin[i].i * cin[i].i);
}

float norm2(kiss_fft_cpx* c) {

        float f = sqrtf(c->r * c->r + c->i * c->i);
        //printf("[f] %f\n", f);
        //printf("[r] %.2f [i] %.2f\n", c->r, c->i);
        return f;
}

int compute_target_x(uint32_t i, int offset) {
    return  offset / 2 + (i) * (SCREEN_W - offset) / N_LINE_POINTS;
}

void init_gaussian(int n_samples, double* array) {
    for(int32_t i= 0 ;i < n_samples ; ++i) {
        array[i] = 0.4* exp(-0.5*(((double)i - (double)n_samples / 2.0)/30.0) * (((double)i - (double)n_samples / 2.0) /30.0));
         //printf("%u: %f\n", i, array[i]);
    }
}

void movavg(float *in, float *out, int window) {
    int k = 1;
    for(uint32_t j = 0; j < N_SAMPLES; j+=window)
        {
            for(int i = j ; i < window * k; ++i)
            {
                out[j] += in[i];
                out[j] /= window;
               // printf("[j] %d [i] %d [r] %f\n", j, i, out[i]);

            }
            k++;
        }
}

void apply_gaussian(int n_samples, float* array, float* kernel) {
    for(uint32_t i = 0; i < n_samples; ++i) {
        array[i] = array[i] * kernel[i];
    }
}

int main(int argc, char *argv[]) {

    BITMAP* buffer_gfx;
    uint8_t scan = 0;
    struct timespec remaining, request = {0, 2 * 1e7};

    kiss_fft_cfg cfg;
    kiss_fft_cpx in[N_SAMPLES], out[N_SAMPLES];
    float out_avg[N_LINE_POINTS];
    cfg = kiss_fft_alloc(N_SAMPLES, 0/*is_inverse_fft*/, NULL, NULL);


    float out_matrix[N_LINE_POINTS][N_LINE_POINTS];
    int WHITE = makecol(255, 255, 255);

    float fourth[N_SAMPLES];

    double gaussian_kernel[N_LINE_POINTS];

    init_gaussian(N_LINE_POINTS, gaussian_kernel);

    start_allegro(GFX_AUTODETECT_WINDOWED);

    buffer_gfx = create_bitmap(SCREEN_W, SCREEN_H);
    clear_to_color(buffer_gfx, 0);

    blit(buffer_gfx, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    uint32_t read_index = 0;
    uint32_t read_offset = 0;

    SF_INFO file_info;

     SNDFILE *sndfile = sf_open("./disorder.wav", SFM_READ, &file_info);

    printf("File info:\n\tRate: %d\n\tFormat: %d\n\tChannels: %d\n\tFrames: %lu\n",
           file_info.samplerate, file_info.format, file_info.channels, file_info.frames);


    float *samples = (float*)malloc(file_info.frames * file_info.channels * sizeof(float));

    sf_count_t count = -1;
    if (samples != NULL)
    {
        count = sf_readf_float(sndfile, samples, file_info.frames);
        printf("count: %lu frames: %lu \n",count , file_info.frames);
    }

    sf_count_t samples_counter = 0;

    while (key[KEY_ESC] == 0) {
        if (keypressed()) scan = readkey() >> 8;

        clear_to_color(buffer_gfx, 0);

        memcpy(fourth, &samples[samples_counter], sizeof(float) * N_SAMPLES);

        samples_counter+= N_SAMPLES;
        if(samples_counter >= count)
        {
            break;
        }

        for (uint32_t i = 0; i < N_SAMPLES; i++)
        {
            in[i].r = fourth[i];
            in[i].i = 0;
        }

        kiss_fft(cfg, in, out);

        float out_norm[N_SAMPLES];
        norm2_v(out, out_norm, N_SAMPLES);

        movavg(out_norm, out_avg, 32);

        for(uint32_t i = 0; i < N_LINE_POINTS; ++i) {
            read_index = (i + read_offset) % (N_LINE_POINTS-1);

            for(uint32_t l = 2; l < N_LINE_POINTS; ++l) {

                float p1[2] = {compute_target_x(i, 100),
                                10 * l + gaussian_kernel[i] * out_norm[read_index]};
                float p2[2] = {compute_target_x(i+1, 100),
                                10 * l + gaussian_kernel[i] * out_norm[read_index+1]};

                fastline_bottom_left(buffer_gfx, p1[0], p1[1], p2[0], p2[1], makecol(255, 255, 255));
            }
        }

        if(++read_offset > N_LINE_POINTS) read_offset = 0;

        blit(buffer_gfx, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        nanosleep(&request, &remaining);
    }

    free(cfg);

    sf_close(sndfile);

    destroy_bitmap(buffer_gfx);
    close_allegro();

    free(samples);

    return 0;
}

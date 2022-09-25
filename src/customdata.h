/* --------------------------
         REAL TIME SYSTEMS
         OBSTACLE AVOIDANCE
           Mattia Fussi

 COMMON CONSTANTS AND STRUCTS
 -------------------------- */

#ifndef CUSTOMDATA_H
#define CUSTOMDATA_H

/* --- Project Constants --- */

#define SIZE_X 12
#define SIZE_U 4
#define SIZE_Y 6
#define SIZE_PID 9

#define WIDTH_SCREEN 800
#define HEIGHT_SCREEN 800

#define ENV_OFFSET_X 20
#define ENV_OFFSET_Y 743
#define ENV_SCALE 50

#define MAX_WPOINTS 5
#define OBS_NUM 6

#define OFFSET_LASER 0
#define APERTURE 150
#define N_BEAMS 15
#define ANGLE_RES 10
#define BEAM_DMIN 0.4
#define BEAM_DMAX 5
#define BEAM_DSTEP 0.02

#define COL_GREEN 48

#define PLT_FRAME_SIZE 100
#define PLT_DATA_SIZE 25
#define PLT_SCALE_X PLT_DATA_SIZE
#define PLT_STEP 4
#define PLT_SCALE_Y 1

#endif // CUSTOMDATA_H

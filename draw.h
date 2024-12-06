#pragma once
#include "raylib.h"

class Draw
{
    public:
        void draw_dens(int N, float *dens, int interface);
        void draw_velocity(int N, float* v, int interface);
        void draw_forces(int N, float* u, int interface);
        void draw_sliders(float diffusion, float new_diffusion, float viscosity, float new_viscosity, int screenWidth);
};
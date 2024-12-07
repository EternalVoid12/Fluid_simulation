#pragma once
#include "raylib.h"
#include "Settings.h"

class Draw
{
    public:
        void draw_dens(int N, float *dens, int interface, Color new_color);
        void draw_velocity(int N, float* v, int interface);
        void draw_forces(int N, float* u, int interface);
        void draw_sliders_1(float* diffusion, float new_diffusion, float* viscosity, float new_viscosity, Settings settings);
        void draw_sliders_2(float* Source_strength_u,float new_Source_strength_u,
                                  float* Source_strength_v, float new_Source_strength_v, int screenWidth );
        void draw_checkboxes(bool* rnd_forces, bool* rnd_velocities, bool* rnd_dens, int screenWidth);
};
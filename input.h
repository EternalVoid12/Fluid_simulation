#pragma once
#include "raylib.h"

class Input
{
    public:
        void get_from_UI(int N, float* s, float* u_prev, float* v_prev, int interface, float source_strength_u, float Source_strength_v);
};
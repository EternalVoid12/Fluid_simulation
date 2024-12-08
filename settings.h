#pragma once
#include <string>

class Settings
{
    public:
        void getSettings(const std::string& filename);
        int TargetFPS;
        int N;
        int screenWidth;
        int screenHeight;
        int interfaceWidth;
        float diffusion;
        float viscosity;
        float Source_strength_u;
        float Source_strength_v;
        float minDiffusion;
        float maxDiffusion;
        float minViscosity;
        float maxViscosity;
        float minSource_strength_u;
        float maxSource_strength_u;
        float minSource_strength_v;
        float maxSource_strength_v;
};


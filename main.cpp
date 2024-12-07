#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <string>
#include <iostream>
#include <bitset>
#include <climits>
#include <cstring>

#include "raylib.h"
#include "input.h"
#include "draw.h"
#include "solver.h"
#include "settings.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

//THIS VALUE IS HARDCODED
const int N = 200; // Grid size (100x100)
const int size = (N+2)*(N+2); // Total grid size with boundary

static float u[size], v[size], u_prev[size], v_prev[size];
static float dens[size], dens_prev[size]; 
static float s[size];

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    Settings settings;
    Solver solver;
    Input input;
    Draw draw;

    settings.getSettings("Settings.txt");

    GuiLoadStyleDefault();
    //Font defaultFont = GetFontDefault();

    const int screenWidth = settings.screenWidth;
    const int screenHeight = settings.screenHeight;

    float diffusion = settings.diffusion;
    float viscosity = settings.viscosity;

    float new_diffusion = 0.0f;
    float new_viscosity = 0.0f;

    float Source_strength_u = 255.0f;
    float new_Source_strength_u = 0.0f;

    float Source_strength_v = 255.0f;
    float new_Source_strength_v = 0.0f;

    bool rnd_forces = false;
    bool rnd_velocities = false;
    bool rnd_dens = false;

    InitWindow(screenWidth + settings.interfaceWidth, screenHeight, "Fluid");

    int TargetFPS = settings.TargetFPS;
    SetTargetFPS(TargetFPS);               // Set our simulation to run at 60 frames-per-second

    float dt = 1.0f/((float)TargetFPS);
    int timeStamp = 0;

    std::random_device rd; 
    std::mt19937 gen(rd());
    
    std::uniform_real_distribution<float> u_dis(-255.0f, 255.0f); 
    std::uniform_real_distribution<float> v_dis(-255.0f, 255.0f); 
    std::uniform_real_distribution<float> d_dis(0.0f,100.0f);
    //std::uniform_real_distribution<float> color_dis(-255.0f, 0.0f);
    //std::uniform_int_distribution<int> radius_dis(0, 50);
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        //  Update your variables here
        //----------------------------------------------------------------------------------
        //timeStamp+=1;

        input.get_from_UI(N, dens_prev,u_prev, v_prev, settings.interfaceWidth, Source_strength_u, Source_strength_v);
        solver.vel_step(N, u, v, u_prev, v_prev, viscosity, dt);
        solver.dens_step(N, dens, dens_prev, u, v, diffusion, dt);

        if (rnd_forces) for(int i=0; i<size;i++) u_prev[i] = u_dis(gen); //set random force distribution
        if (rnd_velocities) for(int i=0; i<size;i++) v_prev[i] = v_dis(gen);
        if(rnd_dens) for(int i=0; i<size;i++) dens_prev[i] = d_dis(gen);

        if (IsKeyPressed(KEY_A)) //Delete all densities
        {
                for(int i = 0; i < size; i++) s[i]=0.0f;
                for(int i = 0; i < size; i++) dens[i]=0.0f;
                for(int i = 0; i < size; i++) dens_prev[i]=0.0f;
        }
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            if (IsKeyDown(KEY_SPACE)) 
            {
                draw.draw_velocity(N,v, settings.interfaceWidth );
            } else if(IsKeyDown(KEY_F)) 
            {
                draw.draw_forces(N,u, settings.interfaceWidth);
            } else
            {
                draw.draw_dens(N,dens, settings.interfaceWidth);
            }

            //draw.draw_sliders(diffusion,new_diffusion,viscosity,new_viscosity,screenWidth); // Maybe rewrite this

            //draw.draw_sliders_1( &diffusion, new_diffusion,  &viscosity,  new_viscosity, settings );
            draw.draw_sliders_1( &diffusion, new_diffusion,  &viscosity,  new_viscosity, settings ); //NOt WORKING strange behaivor, work now
            //TODO: add next lines in function, problems
            const float minSource_strength_u = -2000.0f;
            const float maxSource_strength_u = 2000.0f;
            new_Source_strength_u = GuiSlider((Rectangle){ screenWidth + 50, 300, 200, 30 }, NULL, NULL, 
                                                          &Source_strength_u, minSource_strength_u, maxSource_strength_u);
            DrawText(TextFormat("Source_u: %0.1f", Source_strength_u), screenWidth + 50, 350, 10, BLACK);

            const float minSource_strength_v = -2000.0f;
            const float maxSource_strength_v = 2000.0f;
            new_Source_strength_v = GuiSlider((Rectangle){ screenWidth + 50, 400, 200, 30 }, NULL, NULL, 
                                                          &Source_strength_v, minSource_strength_v, maxSource_strength_v);
            DrawText(TextFormat("Source_u: %0.1f", Source_strength_v), screenWidth + 50, 450, 10, BLACK);

            draw.draw_checkboxes(&rnd_forces, &rnd_velocities, &rnd_dens, screenWidth);

            DrawFPS(10, 10); 
            std::string str = std::to_string(timeStamp);
            DrawText(str.c_str(),10,30,20, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}
#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <string>
#include <iostream>
#include <bitset>
#include <climits>
#include <cstring>
#include "solver.h"
#include "raylib.h"


const int N = 100; // Grid size (100x100)
const int size = (N+2)*(N+2); // Total grid size with boundary

static float u[size], v[size], u_prev[size], v_prev[size];
static float dens[size], dens_prev[size]; 
static float s[size];



void get_from_UI(float* s, float* u_prev, float* v_prev)
{
    // Assume the simulation grid is N x N
    Vector2 mousePos = GetMousePosition();

    // Map mouse position to the simulation grid (scaled to N x N grid size)
    int i_grid = (int)(mousePos.x / GetScreenWidth() * N) + 1; // X grid position
    int j_grid = (int)(mousePos.y / GetScreenHeight() * N) + 1; // Y grid position

    if (IsKeyDown(KEY_R)){
        //int source_radius = 10;
        float source_strength = 250.0f; // Adjust as needed
        for (int i = i_grid; i <= (10 + i_grid); i++) {
            for (int j = j_grid; j <= (10 + j_grid); j++) {
                    s[i + j * (N + 2)] = source_strength;
            }
        }
    }
    // Check if the left mouse button is being pressed
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        // Add a source of density at the mouse position (you can adjust the strength)
        float source_strength = 250.0f; // Adjust as needed
        s[i_grid + j_grid * (N + 2)] = source_strength;
        
        //d[i_grid + j_grid * (N + 2)] += source_strength;
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        // Add a source of density at the mouse position (you can adjust the strength)
        float source_strength = 2500.0f; // Adjust as needed
        u_prev[i_grid + j_grid * (N + 2)] += source_strength;
        //d[i_grid + j_grid * (N + 2)] += source_strength;
    }
        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
    {
        // Add a source of density at the mouse position (you can adjust the strength)
        float source_strength = 2500.0f; // Adjust as needed
        v_prev[i_grid + j_grid * (N + 2)] += source_strength;
        //d[i_grid + j_grid * (N + 2)] += source_strength;
    }
    // Optionally, you could include more UI controls (like sliders) to adjust velocity or density more precisely.
    // For example, if you have sliders for viscosity, diffusion, etc., you can capture their values here.
}
void draw_dens(int N, float *dens)
{
    // Get the screen width and height
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Size of each grid cell in pixels (scaling factor)
    int cellWidth = screenWidth / N;
    int cellHeight = screenHeight / N;

    // Iterate over the grid (skip the boundary cells)
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            // Calculate the index for the current grid cell
            int index = i + j*(N+2); // +2 to account for boundary cells

            // Get the density at this grid cell
            float density = dens[index];

            // Map the density value to a color (you can adjust this to your liking)
            // For example, we map the density value (between 0 and some max value) to a color
            //if(density>255) dens[index] = f;
            Color color;
            if (density >= 0 && density <= 255) {  // Ensure velocity is within the valid color range
                color = {static_cast<unsigned char>((int)density%255),0,0,255};  // Cast to unsigned char for proper range
            } else {
                dens[index] = 0.0f;  // Set the corresponding value to 0 if velocity is out of range
            }              // Black for zero or low density

            // Draw the rectangle at the position (scaled to screen)
            DrawRectangle(i * cellWidth, j * cellHeight, cellWidth, cellHeight, color);
        }
    }
}
void draw_velocity(int N, float* v)
{
    // Get the screen width and height
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Size of each grid cell in pixels (scaling factor)
    int cellWidth = screenWidth / N;
    int cellHeight = screenHeight / N;

    // Iterate over the grid (skip the boundary cells)
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            // Calculate the index for the current grid cell
            int index = i + j * (N + 2); // +2 to account for boundary cells

            // Get the density at this grid cell
            float velocity = v[index];
            Color color;
            // Map the density value to a color (you can adjust this to your liking)
            // For example, we map the density value (between 0 and some max value) to a color
            if (velocity >= 0 && velocity <= 255) {  // Ensure velocity is within the valid color range
                color = {static_cast<unsigned char>((int)velocity*10),0,0,255};  // Cast to unsigned char for proper range
            } else if(velocity <= 0 && velocity >= -255) {
                color = {0,0,static_cast<unsigned char>((int)abs(velocity)*10),255};  // Set the corresponding value to 0 if velocity is out of range
            }
            // Draw the rectangle at the position (scaled to screen)
            DrawRectangle(i * cellWidth, j * cellHeight, cellWidth, cellHeight, color);
        }
    }
}
void draw_forces(int N, float* u)
{
    // Get the screen width and height
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Size of each grid cell in pixels (scaling factor)
    int cellWidth = screenWidth / N;
    int cellHeight = screenHeight / N;

    // Iterate over the grid (skip the boundary cells)
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            // Calculate the index for the current grid cell
            int index = i + j * (N + 2); // +2 to account for boundary cells

            // Get the density at this grid cell
            float force = u[index];
            Color color;
            // Map the density value to a color (you can adjust this to your liking)
            // For example, we map the density value (between 0 and some max value) to a color
            if (force >= 0 && force <= 255) {  // Ensure velocity is within the valid color range
                color = {static_cast<unsigned char>((int)force*10),0,0,255};  // Cast to unsigned char for proper range
            } else if(force <= 0 && force >= -255) {
                color = {0,0,static_cast<unsigned char>((int)abs(force)*10),255};  // Set the corresponding value to 0 if velocity is out of range
            }
            // Draw the rectangle at the position (scaled to screen)
            DrawRectangle(i * cellWidth, j * cellHeight, cellWidth, cellHeight, color);
        }
    }
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    for (int i = 0; i < size; ++i) {
    u[i] = 0.0f;
    v[i] = 0.0f;
    u_prev[i] = 0.0f;
    v_prev[i] = 0.0f;
    dens[i] = 0.0f;
    dens_prev[i] = 0.0f;
    s[i] = 0.0f;
    }

    Solver solver;

    const int screenWidth = 1000;
    const int screenHeight = 1000;

    const float diffusion = 0.00001f;
    const float viscosity = 5.0f;

    Vector2 ballPosition = { -100.0f, -100.0f };
    Color ballColor = DARKBLUE;

    InitWindow(screenWidth, screenHeight, "Fluid");

    SetTargetFPS(60);               // Set our simulation to run at 60 frames-per-second

    float dt = 1.0f/60.0f;
    int timeStamp = 0;

    //std::random_device rd; 
    //std::mt19937 gen(rd());
    
    //std::uniform_real_distribution<float> dens_dis(0.0f, 100.0f);
    //std::uniform_real_distribution<float> color_dis(-255.0f, 0.0f);
    //std::uniform_int_distribution<int> radius_dis(0, 50);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        timeStamp+=1;
        ballPosition = GetMousePosition(); // IT AFFECTS V_PREV SOMEHOW!!!!

        get_from_UI(dens_prev,u_prev, v_prev);
        solver.vel_step(N, u, v, u_prev, v_prev, viscosity, dt);
        solver.dens_step(N, dens, dens_prev, u, v, diffusion, dt);
        
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
                draw_velocity(N,v);
            } else if(IsKeyDown(KEY_F)) 
            {
                draw_forces(N,u);
            } else
            {
                draw_dens(N,dens);
            }

            DrawCircleV(ballPosition, 5, ballColor);
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
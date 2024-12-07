#include "draw.h"
#include <math.h>
#include "raylib.h"
#include "raygui.h"
#include "Settings.h"

void Draw::draw_sliders_1(float* diffusion,float new_diffusion, float* viscosity, float new_viscosity, Settings settings){

            new_diffusion = GuiSlider((Rectangle){ settings.screenWidth + 50, 100, 200, 30 }, NULL, NULL, diffusion, 
                                      settings.minDiffusion, settings.maxDiffusion);
            DrawText(TextFormat("diffusion: %0.5f", *diffusion), settings.screenWidth + 50, 150, 10, BLACK);
 
            new_viscosity = GuiSlider((Rectangle){ settings.screenWidth + 50, 200, 200, 30 }, NULL, NULL, viscosity,
                                      settings.minViscosity, settings.maxViscosity);
            DrawText(TextFormat("viscosity: %0.4f", *viscosity), settings.screenWidth + 50, 250, 10, BLACK);
}

void Draw::draw_sliders_2(float* Source_strength_u,float new_Source_strength_u, float* Source_strength_v, float new_Source_strength_v, int screenWidth ){
            const float minSource_strength_u = -2000.0f;
            const float maxSource_strength_u = 2000.0f;
            new_Source_strength_u = GuiSlider((Rectangle){ screenWidth + 50, 300, 200, 30 }, NULL, NULL, 
                                                          Source_strength_u, minSource_strength_u, maxSource_strength_u);
            DrawText(TextFormat("Source_u: %0.1f", Source_strength_u), screenWidth + 50, 350, 10, BLACK);

            const float minSource_strength_v = -2000.0f;
            const float maxSource_strength_v = 2000.0f;
            new_Source_strength_v = GuiSlider((Rectangle){ screenWidth + 50, 400, 200, 30 }, NULL, NULL, 
                                                          Source_strength_v, minSource_strength_v, maxSource_strength_v);
            DrawText(TextFormat("Source_u: %0.1f", Source_strength_v), screenWidth + 50, 450, 10, BLACK);
}
void Draw::draw_checkboxes(bool* rnd_forces, bool* rnd_velocities, bool* rnd_dens, int screenWidth){

            DrawText(TextFormat("Random u:"), screenWidth + 50, 500, 10, BLACK);
            GuiCheckBox((Rectangle){ screenWidth + 150, 500, 30, 30 },NULL, rnd_forces);

            DrawText(TextFormat("Random v:"), screenWidth + 50, 550, 10, BLACK);
            GuiCheckBox((Rectangle){ screenWidth + 150, 550, 30, 30 },NULL, rnd_velocities);

            DrawText(TextFormat("Random dens:"), screenWidth + 50, 600, 10, BLACK);
            GuiCheckBox((Rectangle){ screenWidth + 150, 600, 30, 30 },NULL, rnd_dens);
}

void Draw::draw_dens(int N, float *dens, int interface)
{
    // Get the screen width and height
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Size of each grid cell in pixels (scaling factor)
    int cellWidth = (screenWidth - interface) / N;
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
void Draw::draw_velocity(int N, float* v, int interface)
{
    // Get the screen width and height
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Size of each grid cell in pixels (scaling factor)
    int cellWidth = (screenWidth-interface) / N;
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
void Draw::draw_forces(int N, float* u, int interface)
{
    // Get the screen width and height
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    // Size of each grid cell in pixels (scaling factor)
    int cellWidth = (screenWidth-interface) / N;
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

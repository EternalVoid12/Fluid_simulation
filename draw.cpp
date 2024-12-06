#include "draw.h"
#include <math.h>
#include "raylib.h"
#include "raygui.h"

void Draw::draw_sliders(float diffusion,float new_diffusion, float viscosity, float new_viscosity, int screenWidth ){
            const float minDiffusion = 0.00001f;
            const float maxDiffusion = 0.01f;

            new_diffusion = GuiSlider((Rectangle){ screenWidth + 50, 100, 200, 30 }, NULL, NULL, &diffusion, minDiffusion, maxDiffusion);
            DrawText(TextFormat("diffusion: %0.5f", diffusion), screenWidth + 50, 150, 10, BLACK);

            const float minViscosity = 0.1f;
            const float maxViscosity = 10.0f;

            new_viscosity = GuiSlider((Rectangle){ screenWidth + 50, 200, 200, 30 }, NULL, NULL, &viscosity, minViscosity, maxViscosity);
            DrawText(TextFormat("viscosity: %0.4f", viscosity), screenWidth + 50, 250, 10, BLACK);
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

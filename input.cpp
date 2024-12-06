#include "input.h"

void Input::get_from_UI(int N, float* s, float* u_prev, float* v_prev, int interface, float source_strength_u, float Source_strength_v)
{
    // Assume the simulation grid is N x N
    Vector2 mousePos = GetMousePosition();
    if(mousePos.x<(GetScreenWidth()-interface)){
        // Map mouse position to the simulation grid (scaled to N x N grid size)
        int i_grid = (int)(mousePos.x / (GetScreenWidth() - interface) * N); // X grid position
        int j_grid = (int)(mousePos.y / GetScreenHeight() * N); // Y grid position

        if (IsKeyDown(KEY_R)){
            //int source_radius = 10;
            float source_strength = 250.0f; // Adjust as needed
            for (int i = i_grid; i <= (5 + i_grid); i++) {
                for (int j = j_grid; j <= (5 + j_grid); j++) {
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
            //float source_strength_u = 2500.0f; // Adjust as needed
            u_prev[i_grid + j_grid * (N + 2)] += source_strength_u;
            //d[i_grid + j_grid * (N + 2)] += source_strength;
        }
            if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
        {
            // Add a source of density at the mouse position (you can adjust the strength)
            //float source_strength = 2500.0f; // Adjust as needed
            v_prev[i_grid + j_grid * (N + 2)] += Source_strength_v;
            //d[i_grid + j_grid * (N + 2)] += source_strength;
        }
    }
    // Optionally, you could include more UI controls (like sliders) to adjust velocity or density more precisely.
    // For example, if you have sliders for viscosity, diffusion, etc., you can capture their values here.
}
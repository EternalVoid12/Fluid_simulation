#include <iostream>
#include <string>
#include "raylib.h"
#include <random>
#include <ctime>
#include <string>
#include <iostream>
#include <bitset>
#include <climits>
#include <cstring>

#define IX(i,j) ((i)+(N+2)*(j))
#define SWAP(x0,x) {float *tmp=x0;x0=x;x=tmp;}

const int N = 100; // Grid size (100x100)
const int size = (N+2)*(N+2); // Total grid size with boundary

static float u[size], v[size], u_prev[size], v_prev[size];
static float dens[size], dens_prev[size]; 
static float s[size];


void add_source ( int N, float * x, float * s, float dt )
{
    int i, size=(N+2)*(N+2);
    for ( i=0 ; i<size ; i++ ) x[i] += dt*s[i]; //GENERATES DENSITY WITH TIME
}
void add_source_v ( int N, float * x, float * s, float dt )
{
    int i, size=(N+2)*(N+2);
    for ( i=0 ; i<size ; i++ ) x[i] += dt*s[i];
}
void add_source_u ( int N, float * x, float * s, float dt )
{
    int i, size=(N+2)*(N+2);
    for ( i=0 ; i<size ; i++ ) x[i] += dt*s[i];
}
void set_bnd ( int N, int b, float * x )
{
        int i;
        for ( i=1 ; i<=N ; i++ ) {
        x[IX(0 ,i)] = b==1 ? -x[IX(1,i)] : x[IX(1,i)];
        x[IX(N+1,i)] = b==1 ? -x[IX(N,i)] : x[IX(N,i)];
        x[IX(i,0 )] = b==2 ? -x[IX(i,1)] : x[IX(i,1)];
        x[IX(i,N+1)] = b==2 ? -x[IX(i,N)] : x[IX(i,N)];
        }
        x[IX(0 ,0 )] = 0.5*(x[IX(1,0 )]+x[IX(0 ,1)]);
        x[IX(0 ,N+1)] = 0.5*(x[IX(1,N+1)]+x[IX(0 ,N )]);
        x[IX(N+1,0 )] = 0.5*(x[IX(N,0 )]+x[IX(N+1,1)]);
        x[IX(N+1,N+1)] = 0.5*(x[IX(N,N+1)]+x[IX(N+1,N )]);
}
void diffuse ( int N, int b, float * x, float * x0, float diff, float dt )
{
    int i, j, k;
    float a=dt*diff*N*N;
    for ( k=0 ; k<20 ; k++ ) {
    for ( i=1 ; i<=N ; i++ ) {
    for ( j=1 ; j<=N ; j++ ) {
        x[IX(i,j)] = (x0[IX(i,j)] + a*(x[IX(i-1,j)]+x[IX(i+1,j)]+
        x[IX(i,j-1)]+x[IX(i,j+1)]))/(1+4*a);
    }
    }
        set_bnd ( N, b, x );
    }
}
void advect ( int N, int b, float * d, float * d0, float * u, float * v, float dt )
{
    int i, j, i0, j0, i1, j1;
    float x, y, s0, t0, s1, t1, dt0;
    dt0 = dt*N;
    for ( i=1 ; i<=N ; i++ ) {
        for ( j=1 ; j<=N ; j++ ) {
        x = i-dt0*u[IX(i,j)]; y = j-dt0*v[IX(i,j)];
        if (x<0.5) x=0.5; 
        if (x>N+0.5) x=N+ 0.5; //TODO: CHEck if statements
        i0=(int)x; 
        i1=i0+1;
        if (y<0.5) y=0.5; 
        if (y>N+0.5) y=N+ 0.5; 
        j0=(int)y; 
        j1=j0+1;
        s1 = x-i0; s0 = 1-s1; t1 = y-j0; t0 = 1-t1;
        d[IX(i,j)] = s0*(t0*d0[IX(i0,j0)]+t1*d0[IX(i0,j1)])+ s1*(t0*d0[IX(i1,j0)]+t1*d0[IX(i1,j1)]);
}
}
set_bnd ( N, b, d );
}
void dens_step ( int N, float * x, float * x0, float * u, float * v, float diff,
                float dt )
{
    //add_source ( N, x, x0, dt );
    SWAP(x0, x) // Ads density as it is not as source
    SWAP ( x0, x ); diffuse ( N, 0, x, x0, diff, dt );
    SWAP ( x0, x ); advect ( N, 0, x, x0, u, v, dt );
}

void project ( int N, float * u, float * v, float * p, float * div )
{
        int i, j, k;
        float h;
        h = 1.0/N;
        for ( i=1 ; i<=N ; i++ ) {
        for ( j=1 ; j<=N ; j++ ) {
        div[IX(i,j)] = -0.5*h*(u[IX(i+1,j)]-u[IX(i-1,j)]+
        v[IX(i,j+1)]-v[IX(i,j-1)]);
        p[IX(i,j)] = 0;
        }
        }
        set_bnd ( N, 0, div ); set_bnd ( N, 0, p );

        for ( k=0 ; k<20 ; k++ ) {
        for ( i=1 ; i<=N ; i++ ) {
        for ( j=1 ; j<=N ; j++ ) {
        p[IX(i,j)] = (div[IX(i,j)]+p[IX(i-1,j)]+p[IX(i+1,j)]+
        p[IX(i,j-1)]+p[IX(i,j+1)])/4;
        }
        }
        set_bnd ( N, 0, p );
        }
        for ( i=1 ; i<=N ; i++ ) {
        for ( j=1 ; j<=N ; j++ ) {
        u[IX(i,j)] -= 0.5*(p[IX(i+1,j)]-p[IX(i-1,j)])/h;
        v[IX(i,j)] -= 0.5*(p[IX(i,j+1)]-p[IX(i,j-1)])/h;
        }
        }
        set_bnd ( N, 1, u ); set_bnd ( N, 2, v );
}

void vel_step ( int N, float * u, float * v, float * u0, float * v0,
float visc, float dt )
{
    add_source_u ( N, u, u0, dt ); add_source_v ( N, v, v0, dt );
    SWAP ( u0, u ); diffuse ( N, 1, u, u0, visc, dt );
    SWAP ( v0, v ); diffuse ( N, 2, v, v0, visc, dt );
    project ( N, u, v, u0, v0 );
    SWAP ( u0, u ); SWAP ( v0, v );
    advect ( N, 1, u, u0, u0, v0, dt ); advect ( N, 2, v, v0, u0, v0, dt );
    project ( N, u, v, u0, v0 );
}
void get_from_UI(float* s, float* u_prev, float* v_prev)
{
    // Assume the simulation grid is N x N
    Vector2 mousePos = GetMousePosition();

    // Map mouse position to the simulation grid (scaled to N x N grid size)
    int i_grid = (int)(mousePos.x / GetScreenWidth() * N) + 1; // X grid position
    int j_grid = (int)(mousePos.y / GetScreenHeight() * N) + 1; // Y grid position

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
        float source_strength = 25000.0f; // Adjust as needed
        u_prev[i_grid + j_grid * (N + 2)] += source_strength;
        //d[i_grid + j_grid * (N + 2)] += source_strength;
    }
        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
    {
        // Add a source of density at the mouse position (you can adjust the strength)
        float source_strength = 25000.0f; // Adjust as needed
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

    const int screenWidth = 1000;
    const int screenHeight = 1000;


    //const float dt = 0.1f;
    const float diffusion = 0.00001f;
    const float viscosity = 5.0f;

    Vector2 ballPosition = { -100.0f, -100.0f };
    Color ballColor = DARKBLUE;

    InitWindow(screenWidth, screenHeight, "Fluid");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    float dt = 1.0f/60.0f;
    int timeStamp = 0;
    //int stepCounter = 0;

    std::random_device rd; // Источник случайности
    std::mt19937 gen(rd()); // Генератор случайных чисел
    
    std::uniform_real_distribution<float> dens_dis(0.0f, 100.0f); // Равномерное распределение
    std::uniform_real_distribution<float> color_dis(-255.0f, 0.0f);
    std::uniform_int_distribution<int> radius_dis(0, 50);
    for (int i = 1; i <= N; i++) {
    for (int j = 1; j <= N; j++) {
    // Calculate the index for the current grid cell
    int index = i + j * (N + 2); // +2 to account for boundary cells
        //v[index]=color_dis(gen);
        //u[index] = color_dis(gen);
        //s[index] = dens_dis(gen);

    }
    }
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        timeStamp+=1;
        ballPosition = GetMousePosition(); // IT AFFECTS V_PREV SOMEHOW!!!!NOt
        //for (int i = 0; i < size; i++) v_prev[i]=200.0f; //draw newgative velocity too
        for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
        // Calculate the index for the current grid cell
            int index = i + j * (N + 2); // +2 to account for boundary cells
            //v[index]=color_dis(gen);
            //u[index] = color_dis(gen);
            //dens_prev[index] = dens_dis(gen);

        }
        }
        //for (int i = 0; i < size; i++) v[i]=1000.0f;
        get_from_UI(dens_prev,u_prev, v_prev);

        //for (int i = 0; i < size; i++) s[i]=0.0f;

        vel_step(N, u, v, u_prev, v_prev, viscosity, dt);
        dens_step(N, dens, dens_prev, u, v, diffusion, dt);
        
        //for (int i = 0; i < size; i++) s[i]=0.0f;
        if (IsKeyPressed(KEY_A)){
                //for(int i = 0; i < size; i++) std::cout<<s[i];
                for(int i = 0; i < size; i++) s[i]=0.0f;
                for(int i = 0; i < size; i++) dens[i]=0.0f;
                for(int i = 0; i < size; i++) dens_prev[i]=0.0f;
            }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();


            ClearBackground(RAYWHITE);

            if (IsKeyDown(KEY_SPACE)) {
                draw_velocity(N,v);
                //draw_forces(N,u);
            } else if(IsKeyDown(KEY_F)) 
            {
                draw_forces(N,u);
            } else{
                draw_dens(N,dens);
            }
            DrawCircleV(ballPosition, 5, ballColor);
            DrawFPS(10, 10); 
            std::string str = std::to_string(timeStamp);
            DrawText(str.c_str(),10,30,20, WHITE);
            //DrawText(str.c_str(),20,20,10, WHITE);
            //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
#include "raylib.h"

int main()
{
     SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(1280, 720, "Sketchpad 3D");
    MaximizeWindow();
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        // scene here
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
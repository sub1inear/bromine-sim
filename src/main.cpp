#include "raylib.h"

constexpr int screenWidth = 800;
constexpr int screenHeight = 800;

int main() {
    InitWindow(screenWidth, screenHeight, "CobaltSim");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
    }

    CloseWindow();
    return 0;
}

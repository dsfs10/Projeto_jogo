#include "raylib.h"

void Gameover(){

    ClearBackground(BLACK);

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    DrawText("GAME OVER", (screenWidth / 2) - 150, screenHeight / 2 - 25, 50, WHITE);
    
}
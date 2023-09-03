#include "raylib.h"
#include <stdlib.h>

int main(void) {
    const int screenWidth = 1280;
    const int screenHeight = 720;
    
    InitWindow(screenWidth, screenHeight, "Menu");
    InitAudioDevice();
    ToggleFullscreen();
    
    Texture2D menuImage = LoadTexture("C:\\Users\\Silvana\\Desktop\\Game - the game\\5.png");
    Texture2D board = LoadTexture("C:\\Users\\Silvana\\Desktop\\Game - the game\\board.png");
    
    Sound mainMenuSoundtrack = LoadSound("C:\\Users\\Silvana\\Desktop\\Game - the game\\Staff Roll - The Legend of Zelda_ Ocarina of Time(MP3_160K).mp3");
    
    SetTargetFPS(60);
    
    PlaySound(mainMenuSoundtrack);
    
    while(!WindowShouldClose()) {
        
        //if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && GetMouseX() >= 505 && GetMouseX() <= 755 && GetMouseY() >= 415 && GetMouseY() <= 460) {
                
            
        
        BeginDrawing();
            
            ClearBackground(BLACK);
            
            DrawTexture(menuImage, screenWidth/2 - menuImage.width/2, screenHeight/2 - menuImage.height/2, WHITE);
            //DrawTexture()
            DrawText("GAME - THE GAME", 315, 200, 70, WHITE);
            
            DrawText("Iniciar", 555, 350, 50, WHITE);
            DrawText("Controles", 505, 415, 50, WHITE);
            DrawText("Sair", 580, 480, 50, WHITE);
            
            if(GetMouseX() >= 555 && GetMouseX() <= 705 && GetMouseY() >= 350 && GetMouseY() <= 395) {
                DrawText("Iniciar", 555, 350, 50, MAROON);
            }
            else if(GetMouseX() >= 505 && GetMouseX() <= 755 && GetMouseY() >= 415 && GetMouseY() <= 460) {
                DrawText("Controles", 505, 415, 50, MAROON);
                
                if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    
                    
                        DrawTexture(board, screenWidth/2 - board.width/2, screenHeight/2 - board.height/2, WHITE);
                        DrawText("D - Andar para a direita", 110, 20, 40, WHITE);
                        DrawText("A - Andar para a esquerda", 110, 100, 40, WHITE);
                        DrawText("ESPAÇO - Pular", 110, 180, 40, WHITE);
                       
                    
                }    
            }
            else if(GetMouseX() >= 580 && GetMouseX() <= 680 && GetMouseY() >= 480 && GetMouseY() <= 525) {
                DrawText("Sair", 580, 480, 50, MAROON);
                
                if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    exit(1);
                }    
            }   
        
        EndDrawing();
        
    }
        
    CloseWindow();
    
    
    return 0;
}    

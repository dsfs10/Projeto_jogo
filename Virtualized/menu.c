#include "raylib.h"

void Menu(){
  DrawText("Virtualized", 350, 200, 100, DARKPURPLE);
  DrawText("Iniciar", 555, 350, 50, WHITE);
  DrawText("Controles", 505, 415, 50, WHITE);
  DrawText("Recordes", 510, 480, 50, WHITE);
  DrawText("Sair", 580, 545, 50, WHITE);
  if(GetMouseX() >= 555 && GetMouseX() <= 705 && GetMouseY() >= 350 && GetMouseY() <= 395) {
      DrawText("Iniciar", 555, 350, 50, BLUE);
      }else if(GetMouseX() >= 505 && GetMouseX() <= 755 && GetMouseY() >= 415 && GetMouseY() <= 460) {
          DrawText("Controles", 505, 415, 50, BLUE);
      }else if(GetMouseX() >= 510 && GetMouseX() <= 755 && GetMouseY() >= 480 && GetMouseY() <= 535) {
                DrawText("Recordes", 510, 480, 50, BLUE);
      }else if(GetMouseX() >= 580 && GetMouseX() <= 680 && GetMouseY() >= 545 && GetMouseY() <= 595) {
                DrawText("Sair", 580, 545, 50, BLUE);
      }
}

void placa_texto(int *frames_counter,char *msg1)
{
    if (IsKeyDown(KEY_B)) (*frames_counter) += 8;
    else (*frames_counter)++;
    
    if((*frames_counter)<10000){
        DrawText(TextSubtext(msg1, 0, (*frames_counter)/10), 50, 100, 30, MAROON);
    }
}

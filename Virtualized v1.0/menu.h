#ifndef MENU
#define MENU

typedef struct {
  Rectangle rect; // Retângulo que representa a plataforma
  Color color;    // Cor da plataforma (opcional)
} Platform;

typedef struct{
  Rectangle rect;
  char texto[700];
} Placa;


void Menu();
void placa_texto(int *frames_counter,char *msg1);

#endif
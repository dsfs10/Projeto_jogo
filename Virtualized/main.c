#include "raylib.h"
#include "raymath.h"
#include "gameover.h"
#include "player.h"
#include "menu.h"
#include "recordes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int main(void) {
  const int screenWidth = 1280;
  const int screenHeight = 720;

  const int characterSpeed = 6;
  const int enemySpeed = 2;
  const int gravity = 1;
  const int groundYPos = 3.1 * screenHeight / 4; // Posição do chão

  int wallCollision = 0;
  int introducao = 0;

  int frameCounter = 0;

  const float PlatformRefX = 469.5, PlatformRefY = 455.5;

  InitWindow(screenWidth, screenHeight, "Virtualized");
  InitAudioDevice();
  ToggleFullscreen();

  //PRINCIPAIS TEXTURAS E SONS DO JOGO
  Texture2D background0 = LoadTexture("Sprites/Background0.png");
  Texture2D background = LoadTexture("Sprites/Mapa_-_Jogo.png");

  Texture2D character = LoadTexture("Sprites/George_1.png");
  Texture2D characterJump = LoadTexture("Sprites/George_Pulo.png");
  Texture2D characterLittle = LoadTexture("Sprites/Little_George.png");
  Texture2D characterLittleJump = LoadTexture("Sprites/Little_George_Pulo.png");

  Texture2D portal = LoadTexture("Sprites/portal.png");
  Texture2D ballon = LoadTexture("Sprites/balaodialogo.png");

  Texture2D enemyText = LoadTexture("Sprites/Enemy.png");//tem q trocar pro certo quando tiver a animacao

  Sound footstepSound = LoadSound("Sprites/Single-footstep-in-grass-A.mp3");
  Sound landingSound = LoadSound("Sprites/Single-footstep-in-grass-B.mp3");
  Sound soundtrack = LoadSound("Sprites/musica_jogo.mp3");
  Sound decrease = LoadSound("Sprites/diminuir.mp3");
  Sound increase = LoadSound("Sprites/aumentar.mp3");
  Sound jump = LoadSound("Sprites/Pular.mp3");
  Sound gameover = LoadSound("Sprites/gameover.mp3");
  Sound endgame = LoadSound("Sprites/win.mp3");

  //TEXTURAS E SOM DO MENU
  Texture2D menuImage = background0;
  Texture2D board = LoadTexture("Sprites/board.png");
  Sound mainMenuSoundtrack = LoadSound("Sprites/musica_menu.mp3");
  //FIM DE TEXTURAS E SOM DO MENU

  //FIM PRINCIPAIS TEXTURAS E SONS DO JOGO

  //ANIMAÇÃO E AGREGADOS
  Animacao player_run = {0};
  Animacao player_jump = {0};
  Animacao enemy_run = {0};

  player_run.numFrames = 4; // Declarando o numero de frames da animacao 1 (correr)
  enemy_run.numFrames = 16;
  player_jump.numFrames = 1; // Declarando o numero de frames da animacao 2 (pular)
  player_run.frameWidth = character.width / player_run.numFrames; // Comprimento de um frame da animacao 1

  int frameLittleWidth = characterLittle.width / player_run.numFrames;
  player_jump.frameWidth = characterJump.width / player_jump.numFrames; // Comprimento de um frame da animacao 2
  int frame2LittleWidth = characterLittleJump.width / player_jump.numFrames;

  enemy_run.frameWidth = enemyText.width / 16;

  float hitboxWidth = player_run.frameWidth / 2.5;
  float hitboxHeight = character.height / 2.3;

  Rectangle frameRec = {0.0f, 0.0f, (float)player_run.frameWidth,
                        (float)character.height};
  Rectangle frameRec2 = {0.0f, 0.0f, (float)player_jump.frameWidth,
                         (float)characterJump.height};
  Rectangle frameEnemy = {0.0f, 0.0f, (float)enemy_run.frameWidth,
                          (float)enemyText.height};

  unsigned int frameDelay = 5;
  unsigned int frameEnemyDelay = 15;
  unsigned int frameDelayCounter = 0;
  unsigned int frameDelayCounter2 = 0;
  unsigned int frameIndex = 0;
  unsigned int frameEnemyIndex = 0;

  //FIM DE ANIMAÇÃO E AGREGADOS

  //PLAYER E RELACIONADOS
  Player player = {0};
  player.characterPos = (Vector2){
      groundYPos - 50, screenHeight / 2.0f}; // Posicao inicial do player
  player.characterVelocity = (Vector2){0.0f, 0.0f};
  player.isJumping = false;
  player.vida = 90;
  float numVida;

  Vector2 hitboxPos = player.characterPos;
  //FIM PLAYER E RELACIONADOS
  
  //INIMIGO
  int numEnemies = 10;
  Enemy enemy[numEnemies];
  // Posicao inicial de cada inimigo
  enemy[0].enemyPos = (Vector2){3274, 335};
  enemy[0].limitL = 2986;
  enemy[0].limitR = 3970;

  enemy[1].enemyPos = (Vector2){7432, 595};
  enemy[1].limitL = 6868;
  enemy[1].limitR = 7936;

  enemy[2].enemyPos = (Vector2){7642, 1173};
  enemy[2].limitL = 7324;
  enemy[2].limitR = 7972;

  enemy[3].enemyPos = (Vector2){5860, 2448};
  enemy[3].limitL = 5560;
  enemy[3].limitR = 6406;

  enemy[4].enemyPos = (Vector2){7126, 2448};
  enemy[4].limitL = 6310;
  enemy[4].limitR = 7510;

  enemy[5].enemyPos = (Vector2){886, 2519};
  enemy[5].limitL = 568;
  enemy[5].limitR = 1378;

  enemy[6].enemyPos = (Vector2){2134, 3857};
  enemy[6].limitL = 1804;
  enemy[6].limitR = 2380;

  enemy[7].enemyPos = (Vector2){2776, 3278};
  enemy[7].limitL = 2446;
  enemy[7].limitR = 2992;

  enemy[8].enemyPos = (Vector2){2422, 3278};
  enemy[8].limitL = 2068;
  enemy[8].limitR = 2602;

  enemy[9].enemyPos = (Vector2){2032, 3278};
  enemy[9].limitL = 1810;
  enemy[9].limitR = 2338;

  for (int i = 0; i < numEnemies; i++) {
    enemy[i].enemyVelocity = (Vector2){0.0f, 0.0f};
    enemy[i].enemyCollision1 = 0;
    enemy[i].enemyCollision2 = 0;
    enemy[i].collision_flag = 0;
    enemy[i].flag_turn = 0;
  }


  //FIM INIMIGO

  // PLATAFORMAS E RELACIONADOS
  
  // Defina as informações das plataformas
  Platform platforms[53]; // 3 plataformas no exemplo
  int qtd_plataformas = 53;

  // Defina as informações das plataformas
  platforms[0].rect.x = 1110.0f;
  platforms[0].rect.y = 840.0f;
  platforms[0].rect.width = 1480.0f;
  platforms[0].rect.height = 20.0f;
  platforms[0].color = GRAY;

  platforms[1].rect.x = 2070.0f;
  platforms[1].rect.y = 712.0f;
  platforms[1].rect.width = 190.0f;
  platforms[1].rect.height = 20.0f;
  platforms[1].color = GRAY;

  platforms[2].rect.x = 2325.0f;
  platforms[2].rect.y = 584.0f;
  platforms[2].rect.width = 190.0f;
  platforms[2].rect.height = 20.0f;
  platforms[2].color = GRAY;

  platforms[3].rect = (Rectangle){-95.0f, 522.0f, 1200.0f, 500.0f};
  platforms[3].color = RED; // Plataforma inicial

  platforms[4].rect = (Rectangle){2580.0f, groundYPos - 100.f, 1540.0f,
                                  198.0f}; // Segundo vermelho
  platforms[4].color = RED;

  platforms[5].rect = (Rectangle){3930.0f, groundYPos + 220, 370.0f, 20.0f};
  platforms[5].color = GRAY;
  
  platforms[6].rect = (Rectangle){PlatformRefX + (68*64), PlatformRefY + (5*64), 15*64.0f, 50.0f};
  platforms[6].color = GRAY;
  
  platforms[7].rect = (Rectangle){PlatformRefX + (56*64), PlatformRefY + (9*64), 3*64.0f, 50.0f};
  platforms[7].color = GRAY;
  
  platforms[8].rect = (Rectangle){PlatformRefX + (59*64), PlatformRefY + (12*64), 2*64.0f, 11.0f};
  platforms[8].color = GRAY;
  
  platforms[9].rect = (Rectangle){PlatformRefX + (55*64), PlatformRefY + (15*64), 8*64.0f, 50.0f};
  platforms[9].color = GRAY;
  
  platforms[10].rect = (Rectangle){PlatformRefX + (54*64), PlatformRefY + (16*64), 20*64.0f, 50.0f};
  platforms[10].color = GRAY;
  
  //============================================AREA 02======================================================
  
  platforms[11].rect = (Rectangle){PlatformRefX + (83*64), PlatformRefY + (1*64), 10*64.0f, 50.0f};
  platforms[11].color = GRAY;
  
  platforms[12].rect = (Rectangle){PlatformRefX + (93*64), PlatformRefY + (11*64), 7*64.0f, 50.0f};
  platforms[12].color = GRAY;
  
  platforms[13].rect = (Rectangle){PlatformRefX + (96*64), PlatformRefY + (10*64), 4*64.0f, 50.0f};
  platforms[13].color = GRAY;
  
  platforms[14].rect = (Rectangle){PlatformRefX + (97*64), PlatformRefY + (7*64), 3*64.0f, 50.0f};
  platforms[14].color = GRAY;
  
  platforms[15].rect = (Rectangle){PlatformRefX + (100*64), PlatformRefY + (4*64), 19*64.0f, 50.0f};
  platforms[15].color = GRAY;
  
  platforms[16].rect = (Rectangle){PlatformRefX + (121*64), PlatformRefY + (3*64), 3*64.0f, 50.0f};
  platforms[16].color = GRAY;
  
  platforms[17].rect = (Rectangle){PlatformRefX + (121*64), PlatformRefY + (8*64), 3*64.0f, 50.0f};
  platforms[17].color = GRAY;
  
  platforms[18].rect = (Rectangle){PlatformRefX + (121*64), PlatformRefY + (13*64), 3*64.0f, 50.0f};
  platforms[18].color = GRAY;
  
  platforms[19].rect = (Rectangle){PlatformRefX + (108*64), PlatformRefY + (13*64), 11*64.0f, 50.0f};
  platforms[19].color = GRAY;
  
  platforms[20].rect = (Rectangle){PlatformRefX + (90*64), PlatformRefY + (18*64), 36*64.0f, 50.0f};
  platforms[20].color = GRAY;
  
  //============================================AREA 03======================================================
  
  platforms[21].rect = (Rectangle){PlatformRefX + (82*64), PlatformRefY + (25*64), 10*64.0f, 50.0f};
  platforms[21].color = GRAY;
  
  platforms[22].rect = (Rectangle){PlatformRefX + (82*64), PlatformRefY + (24*64), 6*64.0f, 50.0f};
  platforms[22].color = GRAY;
  
  platforms[23].rect = (Rectangle){PlatformRefX + (82*64), PlatformRefY + (21*64), 4*64.0f, 50.0f};
  platforms[23].color = GRAY;
  
  platforms[24].rect = (Rectangle){PlatformRefX + (80*64), PlatformRefY + (18*64), 2*64.0f, 50.0f};
  platforms[24].color = GRAY;
  
  platforms[25].rect = (Rectangle){PlatformRefX + (78*64), PlatformRefY + (33*64), 34*64.0f, 50.0f};
  platforms[25].color = GRAY;
  
  platforms[26].rect = (Rectangle){PlatformRefX + (113*64), PlatformRefY + (45*64), 3*64.0f, 50.0f};
  platforms[26].color = GRAY;
  
  platforms[27].rect = (Rectangle){PlatformRefX + (102*64), PlatformRefY + (45*64), 3*64.0f, 50.0f};
  platforms[27].color = GRAY;
  
  platforms[28].rect = (Rectangle){PlatformRefX + (91*64), PlatformRefY + (45*64), 3*64.0f, 50.0f};
  platforms[28].color = GRAY;
  
  platforms[29].rect = (Rectangle){PlatformRefX + (80*64), PlatformRefY + (45*64), 3*64.0f, 50.0f};
  platforms[29].color = GRAY;
  
  platforms[30].rect = (Rectangle){PlatformRefX + (69*64), PlatformRefY + (45*64), 3*64.0f, 50.0f};
  platforms[30].color = GRAY;
  
  platforms[31].rect = (Rectangle){PlatformRefX + (59*64), PlatformRefY + (45*64), 5*64.0f, 50.0f};
  platforms[31].color = GRAY;
  
  platforms[32].rect = (Rectangle){PlatformRefX + (54*64), PlatformRefY + (41*64), 6*64.0f, 50.0f};
  platforms[32].color = GRAY;
  
  platforms[33].rect = (Rectangle){PlatformRefX + (57*64), PlatformRefY + (34*64), 4*64.0f, 50.0f};
  platforms[33].color = GRAY;
  
  //============================================AREA 04======================================================
  
  platforms[34].rect = (Rectangle){PlatformRefX + (37*64), PlatformRefY + (27*64), 21*64.0f, 50.0f};
  platforms[34].color = GRAY;
  
  platforms[35].rect = (Rectangle){PlatformRefX + (26*64), PlatformRefY + (28*64), 3*64.0f, 50.0f};
  platforms[35].color = GRAY;
  
  platforms[36].rect = (Rectangle){PlatformRefX + (29*64), PlatformRefY + (31*64), 3*64.0f, 50.0f};
  platforms[36].color = GRAY;
  
  platforms[37].rect = (Rectangle){PlatformRefX + (26*64), PlatformRefY + (37*64), 7*64.0f, 50.0f};
  platforms[37].color = GRAY;
  
  platforms[38].rect = (Rectangle){PlatformRefX + (26*64), PlatformRefY + (38*64), 11*64.0f, 50.0f};
  platforms[38].color = GRAY;
  
  platforms[39].rect = (Rectangle){PlatformRefX + (5*64), PlatformRefY + (27*64), 21*64.0f, 50.0f};
  platforms[39].color = GRAY;
  
  platforms[40].rect = (Rectangle){PlatformRefX + (0*64), PlatformRefY + (34*64), 16*64.0f, 50.0f};
  platforms[40].color = GRAY;
  
  platforms[41].rect = (Rectangle){PlatformRefX + (16*64), PlatformRefY + (56*64), 6*64.0f, 50.0f};
  platforms[41].color = GRAY;
  
  platforms[42].rect = (Rectangle){PlatformRefX + (16*64), PlatformRefY + (56*64), 6*64.0f, 50.0f};
  platforms[42].color = GRAY;
  
  platforms[43].rect = (Rectangle){PlatformRefX + (22*64), PlatformRefY + (55*64), 19*64.0f, 50.0f};
  platforms[43].color = GRAY;
  
  platforms[44].rect = (Rectangle){PlatformRefX + (41*64), PlatformRefY + (56*64), 9*64.0f, 50.0f};
  platforms[44].color = GRAY;
  
  platforms[45].rect = (Rectangle){PlatformRefX + (42*64), PlatformRefY + (55*64), 7*64.0f, 50.0f};
  platforms[45].color = GRAY;
  
  platforms[46].rect = (Rectangle){PlatformRefX + (43*64), PlatformRefY + (52*64), 3*64.0f, 50.0f};
  platforms[46].color = GRAY;
  
  platforms[47].rect = (Rectangle){PlatformRefX + (46*64), PlatformRefY + (50*64), 2*64.0f, 50.0f};
  platforms[47].color = GRAY;
  
  platforms[48].rect = (Rectangle){PlatformRefX + (22*64), PlatformRefY + (46*64), 19*64.0f, 50.0f};
  platforms[48].color = GRAY;
  
  platforms[49].rect = (Rectangle){PlatformRefX + (44*64), PlatformRefY + (46*64), 2*64.0f, 50.0f};
  platforms[49].color = GRAY;
  
  platforms[50].rect = (Rectangle){PlatformRefX + (44*64), PlatformRefY + (39*64), 2*64.0f, 50.0f};
  platforms[50].color = GRAY;
  
  platforms[51].rect = (Rectangle){PlatformRefX + (48*64), PlatformRefY + (42*64), 2*64.0f, 50.0f};
  platforms[51].color = GRAY;
  
  platforms[52].rect = (Rectangle){PlatformRefX + (41*64), PlatformRefY + (35*64), 9*64.0f, 50.0f};
  platforms[52].color = GRAY;

  Platform walls[8];

  walls[0].rect = (Rectangle){1632, 721 , 64 + (64*4), 64 };
  walls[1].rect = (Rectangle){5216, 657 , 64 + (64*3), 64 };
  walls[2].rect = (Rectangle){5920, 398 , 64 + (64*10), 64};
  walls[3].rect = (Rectangle){6290, 1478, 64 + (64*13), 64};
  walls[4].rect = (Rectangle){2912, 2059, 64 + (64*17), 64};
  walls[5].rect = (Rectangle){ 854, 2075, 64 + (64*17), 64};
  walls[6].rect = (Rectangle){4900, 4000, 64 +(64*50), 64 + (64*20)};
  
  //FIM PLATAFORMAS E RELACIONADOS

  int flag1 = 0; // FLAG DA HABILIDADE REALLOC
  
  //CAMERA E RELACIONADOS
  Camera2D camera = {0};
  camera.target = (Vector2){player.characterPos.x + 20.0f, player.characterPos.y + 20.0f};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;
  camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
  //FIM CAMERA E RELACIONADOS

  //PLACAS DIALOGOS
  int qtd_placa = 4;
  Placa placas[qtd_placa];
  placas[0].rect = (Rectangle) {PlatformRefX+(8*64),PlatformRefY,64,64};
  strcpy(placas[0].texto,"Bem vindo, aventureiro de outro mundo, ao cerne do processamento. \nMeu nome é FILEpa, o arquivo, e serei sua guia por esses corredores.");
  placas[1].rect = (Rectangle) {PlatformRefX+(16*64),PlatformRefY + (5 *64),64,64};
  strcpy(placas[1].texto,"Quando você chegou aqui, foi transformado em um ponteiro, \nuse realloc(R) para diminuir de tamanho e passar pela abertura.");
  placas[2].rect = (Rectangle) {PlatformRefX+(36*64),PlatformRefY - (1 *64),64,64};
  strcpy(placas[2].texto,"Não te confundas pela sua aparência, os Li-mês são criaturas impiedosas. \nNão se preocupe também, ele não pode nos ver, mas não o toque, ou \nvc será desintegrado.");
  placas[3].rect = (Rectangle) {PlatformRefX+(58*64),PlatformRefY + (4 *64),64,64};
  strcpy(placas[3].texto,"Esse espaço está em manutenção no momento, mas você ainda consegue \nchegar do outro lado, use malloc(SpaceX2) para se impulsionar para frente \ndurante a queda.");

  //FIM PLACAS DIALOGOS


  //VARIAVEIS DE CONTROLE DE CENA
  bool isGameOver=false;
  bool isInMenu=true;
  bool isInGame=false;
  bool isInRecorde=false;
  bool isInControl=false;
  bool isEndGame=false;
  bool closeGame=false;
  bool isIntroducao=false;
  //FIM DE VARIAVEIS DE CONTROLE DE CENA
  
  //RECORDE TEMPO ARQUIVO
  int qtd_rec=0;
  double *tempo=NULL;
  double tempoi;
  double tempof;
  double dtempo;
  int destaque=-1;
  //===============================ARQUIVO=====================
  FILE *recorde = fopen("recorde.txt", "r");  //NESSA PARTE DO CODIGO ESTAMOS ABRINDO OU CRIANDO O ARQUIVO E, CASO EXISTA, PASSANDO AS INFORMAÇÕES PARA O VETOR TEMPO.
  if (recorde == NULL) {            //Caso o arquivo nao exista, criaremos um novo
    recorde = fopen("recorde.txt", "w");
  } 
  else {
      while (!feof(recorde)){
        double *aux = tempo;
        tempo = (double *) realloc(tempo, sizeof(double)*(qtd_rec+1));
        if(tempo == NULL){
          printf("erro de alocacao");
          fclose(recorde);
          free(aux);

          UnloadTexture(background0);
          UnloadTexture(background);
          UnloadTexture(board);
          UnloadTexture(menuImage);

          UnloadTexture(character);
          UnloadTexture(characterJump);
          UnloadTexture(characterLittle);
          UnloadTexture(characterLittleJump);
          UnloadTexture(enemyText);
          UnloadTexture(portal);
          UnloadTexture(ballon);

          UnloadSound(footstepSound);
          UnloadSound(landingSound);
          UnloadSound(soundtrack);
          UnloadSound(decrease);
          UnloadSound(increase);
          UnloadSound(jump); 
          UnloadSound(mainMenuSoundtrack);
          UnloadSound(gameover);

          exit(1);
        }
        fscanf(recorde, "%lf\n", &tempo[qtd_rec]);
        qtd_rec++;
      }
    }
  
  fclose(recorde);
  //FIM DE RECORDE TEMPO ARQUIVO

  //END GAME
    Rectangle fim_de_jogo = (Rectangle){PlatformRefX + (45*64), PlatformRefY + (34*64),64,64};
  //END GAME

  SetTargetFPS(60);
  if(isInMenu){
    PlaySound(mainMenuSoundtrack);
  }
     
  bool jumpKeyPressed = false;

  while (!WindowShouldClose() && !closeGame ) {
    if(isInMenu){
      BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(menuImage, screenWidth/2 - menuImage.width/2, screenHeight/2 - menuImage.height/2, WHITE);
        Menu();
        if (IsKeyPressed(KEY_I)||(GetMouseX() >= 555 && GetMouseX() <= 705 && GetMouseY() >= 350 && GetMouseY() <= 395&&IsMouseButtonDown(MOUSE_BUTTON_LEFT))) {
          isInMenu=false;
          isIntroducao=true;
            
          //INICIA O JOGO
        } else if(IsKeyPressed(KEY_C)||((GetMouseX() >= 505 && GetMouseX() <= 755 && GetMouseY() >= 415 && GetMouseY() <= 460)&&IsMouseButtonDown(MOUSE_BUTTON_LEFT))){
            isInMenu=false;
            isInControl=true;
          //ABRE O MENU DE INSTRUNÇÕES
        } else if(IsKeyPressed(KEY_S)||(GetMouseX() >= 580 && GetMouseX() <= 680 && GetMouseY() >= 545 && GetMouseY() <= 595)&&IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            //Fechando o jogo
            EndDrawing();
            closeGame = true;
            
          //ainda precisamos fazer as unloads correta
        }else if(IsKeyPressed(KEY_R)||((GetMouseX() >= 510 && GetMouseX() <= 755 && GetMouseY() >= 480 && GetMouseY() <= 535)&&IsMouseButtonDown(MOUSE_BUTTON_LEFT))){ //ABRE O MENU DE RECORDES
            isInRecorde=true;
            isInMenu = false;
        }
      EndDrawing();
    }
    else if(isInRecorde){
      BeginDrawing();
        Recordes(qtd_rec,tempo,destaque);
        if(IsKeyPressed(KEY_V)||((GetMouseX() >= 990 && GetMouseX() <= 1140 && GetMouseY() >= 600 && GetMouseY() <= 640)&&IsMouseButtonDown(MOUSE_BUTTON_LEFT))){
            isInRecorde=false;
            isInMenu = true;
        }
      EndDrawing();
      }
    else if(isInControl){
      BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(board, screenWidth/2 - board.width/2, screenHeight/2 - board.height/2, WHITE);
        DrawText("D ou -> : Andar para a direita", 110, 20, 40, WHITE);
        DrawText("A ou <- : Andar para a esquerda", 110, 100, 40, WHITE);
        DrawText("ESPAÇO : Pular", 110, 180, 40, WHITE);
        DrawText(TextFormat("Voltar"), 990, 600, 40, WHITE);
        if(GetMouseX() >= 990 && GetMouseX() <= 1130 && GetMouseY() >= 600 && GetMouseY() <= 640) {
            DrawText("Voltar", 990, 600, 40, BLUE);
            }
            if(IsKeyPressed(KEY_V)||((GetMouseX() >= 990 && GetMouseX() <= 1140 && GetMouseY() >= 600 && GetMouseY() <= 640)&&IsMouseButtonDown(MOUSE_BUTTON_LEFT))){
                isInControl=false;
                isInMenu = true;
                }
      EndDrawing();
    }
    else if(isGameOver){
      StopSound(soundtrack);
      PlaySound(gameover);
      BeginDrawing();
      Gameover();
      EndDrawing();
      WaitTime(5.0);
      closeGame = true;
    }
    else if(isIntroducao) {
        BeginDrawing();
            ClearBackground(BLACK);
            placa_texto(&frameCounter, "George, um programador de jogos com um rosto impassível de ('-'),\né transportado para dentro de seu próprio computador. Ele se une a FILEpa,\num arquivo falante, para navegar por um labirinto digital. George é silencioso,\nmas corajoso. Juntos, enfrentam desafios tecnológicos para descobrir como\nescapar do mundo digital...\n\nPressione Enter");
        EndDrawing();
        
        if(frameCounter > 10000 || IsKeyPressed(KEY_ENTER)) {
            isIntroducao = false;
            isInGame = true;
            PlaySound(soundtrack);
        }
    }    
    else if(isInGame){
        StopSound(mainMenuSoundtrack);      

        Rectangle hitbox = {hitboxPos.x, hitboxPos.y, hitboxWidth, hitboxHeight};

      for(int i = 0; i < numEnemies; i++) {
        enemy[i].enemyHeadHitbox = (Rectangle){10.0f, 400.0f, 50, 10};
        enemy[i].enemyHitbox = (Rectangle){10.0f, 400.0f, 50, 60};
      }

      //============================================INIMIGO======================================================
      for (int i = 0; i < numEnemies; i++) {
        if (enemy[i].enemyCollision1) {
          enemy[i].collision_flag++;
        }

        if (enemy[i].enemyPos.x == enemy[i].limitL) {
          enemy[i].flag_turn = 0;
          frameEnemy.width = -frameEnemy.width;
        } else if (enemy[i].enemyPos.x == enemy[i].limitR) {
          enemy[i].flag_turn = 1;
          frameEnemy.width = -frameEnemy.width;
        }

        if (enemy[i].flag_turn) {
          enemy[i].enemyVelocity.x = -enemySpeed;
        } else {
          enemy[i].enemyVelocity.x = enemySpeed;
        }
      }

      // ANIMACAO DO INIMIGO
      frameDelayCounter2++;

      if (frameDelayCounter2 > frameEnemyDelay) {
        frameDelayCounter2 = 0;

        frameEnemyIndex++;
        frameIndex %=
            enemy_run.numFrames; // msm numFrames entre inimigo e versao pequena

        frameEnemy.x = (float)enemy_run.frameWidth * frameEnemyIndex;
      }

      //==========================================HABILIDADES====================================================

      // REALLOC
      if (IsKeyPressed(KEY_R) &&
          (flag1 % 2 == 0)) { // Se R for pressionado e a flag for par, entao ele
                              // vai diminuir o tamanho
        hitboxWidth = hitboxWidth / 2;
        hitboxHeight = hitboxHeight / 2;

        PlaySound(decrease);

        if (frameRec.width < 0) {
          frameRec.width = (float)-frameLittleWidth;
          frameRec2.width = (float)-frame2LittleWidth;
        } else {
          frameRec.width = (float)frameLittleWidth;
          frameRec2.width = (float)frame2LittleWidth;
        }

        frameRec.height = (float)characterLittle.height;
        frameRec2.height = (float)characterLittleJump.height;
        flag1++;
      } else if (IsKeyPressed(KEY_R) &&
                (flag1 % 2 != 0)) { // Se R for pressionado e a flag nao for par,
                                    // entao ele vai voltar ao tamanho padrao
        hitboxWidth = player_run.frameWidth / 2.5;
        hitboxHeight = character.height / 2.3;

        PlaySound(increase);

        if (frameRec.width < 0) {
          frameRec.width = (float)-player_run.frameWidth;
          frameRec2.width = (float)-player_jump.frameWidth;
        } else {
          frameRec.width = (float)player_run.frameWidth;
          frameRec2.width = (float)player_jump.frameWidth;
        }

        frameRec.height = (float)character.height;
        frameRec2.height = (float)characterJump.height;
        flag1++;
      }

      //==========================================MOVIMENTACAO====================================================
      jumpKeyPressed = IsKeyPressed(KEY_SPACE);

      // Verifique se o personagem está no chão antes de permitir o salto
      if (IsCharacterOnGround(character, player.characterPos, groundYPos,
                              platforms, qtd_plataformas)) {
        player.characterVelocity.y = 0;
        player.isJumping = false;

        player.doubleJumpFlag = 0;

        if (jumpKeyPressed) {
          player.characterVelocity.y = -4 * characterSpeed;
          player.isJumping = true;
          PlaySound(jump);
        }

        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
          player.characterVelocity.x = characterSpeed;

          if (frameRec.width < 0) {
            frameRec.width = -frameRec.width;
            frameRec2.width = -frameRec2.width;
          }
        } else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
          player.characterVelocity.x = -characterSpeed;

          if (frameRec.width > 0) {
            frameRec.width = -frameRec.width;
            frameRec2.width = -frameRec2.width;
          }
        } else {
          player.characterVelocity.x = 0;
        }
      } else {
        player.isJumping = true;
        player.characterVelocity.y += gravity;

        if (jumpKeyPressed &&
            player.doubleJumpFlag == 0) { // MALLOC (double jump)
          player.characterVelocity.y = -4 * characterSpeed;
          player.isJumping = true;
          PlaySound(jump);
          player.doubleJumpFlag++;
        }

        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
          player.characterVelocity.x = characterSpeed;

          if (frameRec.width < 0) {
            frameRec.width = -frameRec.width;
            frameRec2.width = -frameRec2.width;
          }
        } else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
          player.characterVelocity.x = -characterSpeed;

          if (frameRec.width > 0) {
            frameRec.width = -frameRec.width;
            frameRec2.width = -frameRec2.width;
          }
        } else {
          player.characterVelocity.x = 0;
        }

        if (!player.wasCharacterOnGround) {
          PlaySound(landingSound);
        }
      }

      //==========================================ANIMACAO
      //PERSONAGEM====================================================

      player.characterMoving = player.characterVelocity.x != 0.0f ||
                              player.characterVelocity.y != 0.0f;

      frameDelayCounter++;

      if (frameDelayCounter > frameDelay) {
        frameDelayCounter = 0;

        if (player.characterMoving) { // Se o personagem estiver se movendo,
                                      // atualiza o frame

          if (player.characterIsOnGround) {
            frameIndex++;
            frameIndex %= player_run.numFrames;

            if (frameIndex == 0 || frameIndex == 2) {
              PlaySound(footstepSound);
            }

            if (flag1 % 2 != 0) {
              frameRec.x = (float)frameLittleWidth * frameIndex;
            } else {
              frameRec.x = (float)player_run.frameWidth * frameIndex;
            }
          } else {
            if (flag1 % 2 != 0) {
              frameRec2.x = (float)frame2LittleWidth;
            } else {
              frameRec2.x = (float)player_jump.frameWidth;
            }
          }

        } else {
          if (player.characterIsOnGround) {
            frameRec.x = (float)player_run.frameWidth * 2;
          }
        }
      }
      //======================================================================================================

      // Atualiza a posição do personagem
      player.characterPos =
          Vector2Add(player.characterPos, player.characterVelocity);
      player.characterIsOnGround = IsCharacterOnGround(
          character, player.characterPos, groundYPos, platforms, qtd_plataformas);

      // Atualiza a posicao dos inimigos
      for (int i = 0; i < numEnemies; i++) {
        enemy[i].enemyPos = Vector2Add(enemy[i].enemyPos, enemy[i].enemyVelocity);
      }

      if (flag1 % 2 == 0) {
        hitbox.x = player.characterPos.x + 47;
        hitbox.y = player.characterPos.y + 47;
      } else {
        hitbox.x = player.characterPos.x + 70;
        hitbox.y = player.characterPos.y + 80;
      }

      player.characterLittlePos.x = player.characterPos.x + 50;
      player.characterLittlePos.y = player.characterPos.y + 60;

      for (int i = 0; i < numEnemies; i++) {
        if (enemy[i].collision_flag == 0) {
          enemy[i].enemyHeadHitbox.x = enemy[i].enemyPos.x + 55;
          enemy[i].enemyHeadHitbox.y = enemy[i].enemyPos.y + 55;
          enemy[i].enemyHitbox.x = enemy[i].enemyPos.x + 55;
          enemy[i].enemyHitbox.y = enemy[i].enemyPos.y + 65;
        } else {
          enemy[i].enemyHeadHitbox.x = 0;
          enemy[i].enemyHeadHitbox.y = 0;
          enemy[i].enemyHitbox.x = 0;
          enemy[i].enemyHitbox.y = 0;
        }
      }

      for(int i=0;i<8;i++){
        wallCollision = CheckCollisionRecs(hitbox, walls[i].rect); // Colosao com a parede teste
        if(wallCollision == true){
          i = 9;
        }
      }
      

      for (int i = 0; i < numEnemies; i++) { // Colisao com os inimigos
        enemy[i].enemyCollision1 =
            CheckCollisionRecs(enemy[i].enemyHeadHitbox,
                              (Rectangle){player.characterPos.x + 55,
                                          player.characterPos.y + 110, 40, 20});
        enemy[i].enemyCollision2 =
            CheckCollisionRecs(enemy[i].enemyHitbox, hitbox);
      }


      //==========================================CAMERA======================================================

      camera.target = (Vector2){player.characterPos.x + 20.0f, player.characterPos.y};//atualiza posição da camera
      
      //==========================================FIMDEJOGO====================================================

        if(CheckCollisionRecs(hitbox, fim_de_jogo)){
          isInGame = false;
          isEndGame = true;
        }

      //==========================================DESENHAR====================================================

      BeginDrawing();
      

      ClearBackground(SKYBLUE);
      DrawTexture(background0, screenWidth/2 - menuImage.width/2, screenHeight/2 - menuImage.height/2, WHITE);

      BeginMode2D(camera);

        DrawTexture(background, -330, -250, WHITE);

        for (int i = 0; i < numEnemies; i++) {
        if (wallCollision || enemy[i].enemyCollision2) {//verifica se o jogador está tomando dano
          player.vida--;
          if (player.vida <= 0) {
            isGameOver = true;
            isInGame = false;
            player.vida = 0;
          }
        }
      }
     
        // Desenha o personagem:
        if (!player.characterMoving &&
            player.characterIsOnGround) { // parado e no chao
          if (flag1 % 2 != 0) {
            DrawTextureRec(characterLittle, frameRec, player.characterLittlePos,
                          WHITE);
            // DrawRectangleRec(hitbox, GOLD);
          } else {
            DrawTextureRec(character, frameRec, player.characterPos, WHITE);
            // DrawRectangleRec(hitbox, GOLD);
          }
        } else if (!player.characterIsOnGround) { // pulando
          if (flag1 % 2 != 0) {
            DrawTextureRec(characterLittleJump, frameRec2,
                          player.characterLittlePos, WHITE);
            // DrawRectangleRec(hitbox, GOLD);
          } else {
            DrawTextureRec(characterJump, frameRec2, player.characterPos, WHITE);
          }
        } else { // se movendo
          if (flag1 % 2 != 0) {
            DrawTextureRec(characterLittle, frameRec, player.characterLittlePos, WHITE);
            // DrawRectangleRec(hitbox, GOLD);
          } else {
            DrawTextureRec(character, frameRec, player.characterPos, WHITE);
            // DrawRectangleRec(hitbox, GOLD);
          }
        }

        // Desenha inimigo
        for (int i = 0; i < numEnemies; i++) {
          if (enemy[i].collision_flag == 0) {
            DrawTextureRec(enemyText, frameEnemy, enemy[i].enemyPos, WHITE);
          }
        }

        DrawTexture(portal, fim_de_jogo.x, fim_de_jogo.y - 64, WHITE);

        EndMode2D();

        //placas e dialogos
        for (int i = 0; i < qtd_placa; i++) {
          if (CheckCollisionRecs(hitbox, placas[i].rect)) {
            DrawTexture(ballon, -220, -200, WHITE);
            placa_texto(&frameCounter, placas[i].texto);
          }
        }

        // Desenha o contador de vida
        char vidaText[20];
        numVida = ceilf(player.vida/30.0);
        sprintf(vidaText, "Vidas: %.0f", numVida);
        DrawText(vidaText, 10, 10, 20, WHITE);

      EndDrawing();
    }
    else if(isEndGame){//O jogador finalizou o jogo com sucesso, gravar o recorde aqui
        StopSound(soundtrack);
        PlaySound(endgame);
        BeginDrawing();

          ClearBackground(BLACK);
          DrawText("PARABENS VOCÊ ESCAPOU DO MUNDO C!!", 100, 200 , 50, WHITE);
          DrawText("OBRIGADO POR JOGAR!!", 315, 400, 50, WHITE);

        EndDrawing();

        tempof = GetTime();			//Estamos gravando a partida e o recorde novamente
				dtempo = tempof - tempoi;	//Tempo da partida
				double *aux = tempo;
        tempo = (double *) realloc(tempo, sizeof(double)*qtd_rec + 1);
        if(tempo == NULL){
          printf("Erro de alocacao");
          free(aux);
          UnloadTexture(background0);
          UnloadTexture(background);
          UnloadTexture(board);
          UnloadTexture(menuImage);

          UnloadTexture(character);
          UnloadTexture(characterJump);
          UnloadTexture(characterLittle);
          UnloadTexture(characterLittleJump);
          UnloadTexture(enemyText);
          UnloadTexture(portal);
          UnloadTexture(ballon);

          UnloadSound(footstepSound);
          UnloadSound(landingSound);
          UnloadSound(soundtrack);
          UnloadSound(decrease);
          UnloadSound(increase);
          UnloadSound(jump); 
          UnloadSound(mainMenuSoundtrack);
          UnloadSound(gameover);
          UnloadSound(endgame);

          exit(1);
        }
        tempo[qtd_rec] = dtempo;
        qtd_rec++;
				//Bubblesort para colocar o vetor tempo em ordem decrescente.
				destaque = lfbubbleSort(tempo,qtd_rec);				//o lfbubblesort vai retornar a posicao do recorde atual
				FILE *recorde = fopen("recorde.txt", "w");
				if(recorde==NULL){
          printf("Falha ao salvar o jogo");
          free(tempo);
          UnloadTexture(background0);
          UnloadTexture(background);
          UnloadTexture(board);
          UnloadTexture(menuImage);

          UnloadTexture(character);
          UnloadTexture(characterJump);
          UnloadTexture(characterLittle);
          UnloadTexture(characterLittleJump);
          UnloadTexture(enemyText);
          UnloadTexture(portal);
          UnloadTexture(ballon);

          UnloadSound(footstepSound);
          UnloadSound(landingSound);
          UnloadSound(soundtrack);
          UnloadSound(decrease);
          UnloadSound(increase);
          UnloadSound(jump); 
          UnloadSound(mainMenuSoundtrack);
          UnloadSound(gameover);

          exit(1);
        }
				for(int i=0;i<qtd_rec;i++){
					fprintf(recorde,"%.2lf\n",tempo[i]);
				}
				fclose(recorde);

        WaitTime(3.0);

        BeginDrawing();
        Recordes(qtd_rec, tempo, destaque);
        EndDrawing();

        WaitTime(5.5);

        isEndGame = false;
        closeGame = true;
    }

  }

  UnloadTexture(background0);
  UnloadTexture(background);
  UnloadTexture(board);
  UnloadTexture(menuImage);

  UnloadTexture(character);
  UnloadTexture(characterJump);
  UnloadTexture(characterLittle);
  UnloadTexture(characterLittleJump);
  UnloadTexture(enemyText);
  UnloadTexture(portal);
  UnloadTexture(ballon);

  UnloadSound(footstepSound);
  UnloadSound(landingSound);
  UnloadSound(soundtrack);
  UnloadSound(decrease);
  UnloadSound(increase);
  UnloadSound(jump); 
  UnloadSound(mainMenuSoundtrack);
  UnloadSound(gameover);

  free(tempo);

  CloseAudioDevice();
  CloseWindow();
  return 0;
}
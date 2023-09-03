#include "raylib.h"
#include "raymath.h"

typedef struct{
    Vector2 characterPos;
    Vector2 characterVelocity;
    bool characterMoving;
    bool wasCharacterOnGround;
    bool characterIsOnGround;
} Player;


bool IsCharacterOnGround(Texture2D character, Vector2 characterPos, const int groundYPos);


int main(void) {
    const int screenWidth = 1280;
    const int screenHeight = 720;
    
    const int characterSpeed = 5;
    const int gravity = 1;
    const int groundYPos = 2.9 * screenHeight / 4;
    
    const int jumpUpFrame = 3;
    const int jumpDownFrame = 4;
    
    InitWindow(screenWidth, screenHeight, "Game - The game - test window"); // Abrindo janela
    
    InitAudioDevice(); // Abrindo dispositivo de audio
    
    Image image = LoadImage("./Sprites/5.png");
    Texture2D background = LoadTextureFromImage(image);
    Texture2D character = LoadTexture("./Sprites/man_running.png");
    Texture2D characterStop = LoadTexture("./Sprites/man_jumping.png");
    UnloadImage(image);
    
    unsigned int numFrames = 8; // Declarando o numero de frames da animacao 1 (correr)
    unsigned int numFrames2 = 6; // Declarando o numero de frames da animacao 2 (parar)
    int frameWidth = character.width / numFrames; // Comprimento de um frame da animacao 1
    int frame2Width = characterStop.width / numFrames2; // Comprimento de um frame da animacao 2
    
    Rectangle frameRec = {0.0f, 0.0f, (float)frameWidth, (float)character.height};
    Rectangle frameRec2 = {0.0f, 0.0f, (float)frame2Width, (float)characterStop.height};

    Player player = {0};
    player.characterPos = (Vector2) {groundYPos, screenHeight / 2.0f};
    player.characterVelocity = (Vector2) {0.0f, 0.0f};
    
    Sound footstepSound = LoadSound("./Sprites/Single-footstep-in-grass-A.mp3");
    Sound landingSound = LoadSound("./Sprites/Single-footstep-in-grass-B.mp3");
    Sound soundtrack = LoadSound("./Sprites/08 - Minecraft.mp3");
    
    
    unsigned int frameDelay = 7;
    unsigned int frameDelayCounter = 0;
    unsigned int frameIndex = 0;
    //unsigned frameIndex2 = 0;
    
    SetTargetFPS(60);
    
    PlaySound(soundtrack);
    
    while(!WindowShouldClose()) { // Enquanto nao apertar ESC      
    
        if(IsCharacterOnGround(character, player.characterPos, groundYPos)) { // Se o personagem estiver no chao
        
            // Pulando
            if(IsKeyDown(KEY_SPACE)) {
                player.characterVelocity.y = - 4 * characterSpeed; //o numero que ta multiplicando altera a altura do pulo
            }
            
            
            // Movendo para direita e para esquerda
            if(IsKeyDown(KEY_D)) {
                player.characterVelocity.x = characterSpeed;
                
                if(frameRec.width < 0) {
                    frameRec.width = -frameRec.width;
                    frameRec2.width = -frameRec2.width;
                } 
            }
            else if(IsKeyDown(KEY_A)) {
                player.characterVelocity.x = -characterSpeed;
                
                if(frameRec.width > 0) {
                    frameRec.width = -frameRec.width;
                    frameRec2.width = -frameRec2.width;
                }    
            }
            else {
                player.characterVelocity.x = 0; 
            }
        }
        
        player.characterMoving = player.characterVelocity.x != 0.0f || player.characterVelocity.y != 0.0f;
        
        
        player.wasCharacterOnGround = IsCharacterOnGround(character, player.characterPos, groundYPos);
        player.characterPos = Vector2Add(player.characterPos, player.characterVelocity);
        player.characterIsOnGround = IsCharacterOnGround(character, player.characterPos, groundYPos);
        
        if(player.characterIsOnGround) {
            player.characterVelocity.y = 0;
            player.characterPos.y = groundYPos - character.height;
            
            if(!player.wasCharacterOnGround) {
                PlaySound(landingSound);
            }    
        }
        else {
            player.characterVelocity.y += gravity;
        } 
        
    
        frameDelayCounter++;
        
        if(frameDelayCounter > frameDelay) {
            frameDelayCounter = 0;
            
            if(player.characterMoving) { // Se o personagem estiver se movendo, atualiza o frame
                
                if(player.characterIsOnGround) {
                    frameIndex++;
                    frameIndex %= numFrames;
                    
                    if(frameIndex == 0 || frameIndex == 4) {
                        PlaySound(footstepSound);
                    }    
                }
                else {
                    if(player.characterVelocity.y < 0) {
                        frameIndex = jumpUpFrame;
                    }    
                    else {
                        frameIndex = jumpDownFrame;
                    }    
                }
                frameRec.x = (float) frameWidth * frameIndex;
            }
            else {
                if(player.characterIsOnGround) {
                    frameRec2.x = (float) frame2Width * 3;
                }    
            }    
        }    
        
        BeginDrawing();
            
            ClearBackground(RAYWHITE);
            
            DrawTexture(background, screenWidth/2 - background.width/2, screenHeight/2 - background.height/2, WHITE);
            
            if(!player.characterMoving && player.characterIsOnGround) {
                DrawTextureRec(characterStop, frameRec2, player.characterPos, WHITE);
            }
            else {
                DrawTextureRec(character , frameRec, player.characterPos, WHITE);
            } 
            
        EndDrawing();
    }

    CloseAudioDevice(); //Fechando o dispositivo de audio
    CloseWindow(); //Fechando janela
    
    return 0;
}

bool IsCharacterOnGround(Texture2D character, Vector2 characterPos, const int groundYPos) {//characterPos nem character precisam ser um ponteiro pois nn mexe no valor original 
    if(characterPos.y + character.height >= groundYPos) {
        return true;
    }
    else {
        return false;
    }    
}    

#include "raylib.h"
#include "raymath.h"

bool IsCharacterOnGround(Texture2D *character, Vector2 *characterPos, const int groundYPos);



int main(void) {
    const int screenWidth = 1280;
    const int screenHeight = 720;
    
    const int characterSpeed = 5;
    const int gravity = 1;
    const int groundYPos = 3 * screenHeight / 4;
    
    const int jumpUpFrame = 3;
    const int jumpDownFrame = 4;
    
    InitWindow(screenWidth, screenHeight, "Game - The game - test window"); // Abrindo janela
    
    InitAudioDevice(); // Abrindo dispositivo de audio
    
    Texture2D character = LoadTexture("C:\\Users\\Silvana\\Desktop\\Sprites\\man_running.png");
    unsigned numFrames = 8; // Declarando o numero de frames
    int frameWidth = character.width / numFrames; // Comprimento de um frame
    Rectangle frameRec = {0.0f, 0.0f, (float)frameWidth, (float)character.height};
    Vector2 characterPos = {groundYPos, screenHeight / 2.0f};
    Vector2 characterVelocity = {0.0f, 0.0f};
    
    Sound footstepSound = LoadSound("C:\\Users\\Silvana\\Desktop\\Sprites\\Single-footstep-in-grass-A-www.fesliyanstudios.com.mp3");
    Sound landingSound = LoadSound("C:\\Users\\Silvana\\Desktop\\Sprites\\Single-footstep-in-grass-B-www.fesliyanstudios.com.mp3");
    
    
    unsigned frameDelay = 7;
    unsigned frameDelayCounter = 0;
    unsigned frameIndex = 0;
    
    SetTargetFPS(60);
    
    while(!WindowShouldClose()) { // Enquanto nao apertar ESC
    
        if(IsCharacterOnGround(&character, &characterPos, groundYPos)) { // Se o personagem estiver no chao
        
            // Pulando
            if(IsKeyDown(KEY_SPACE)) {
                characterVelocity.y = - 4 * characterSpeed; //o numero que ta multiplicando altera a altura do pulo
            }
            
            
            // Movendo para direita e para esquerda
            if(IsKeyDown(KEY_D)) {
                characterVelocity.x = characterSpeed;
                
                if(frameRec.width < 0) {
                    frameRec.width = -frameRec.width;
                } 
            }
            else if(IsKeyDown(KEY_A)) {
                characterVelocity.x = -characterSpeed;
                
                if(frameRec.width > 0) {
                    frameRec.width = -frameRec.width;
                }    
            }
            else {
                characterVelocity.x = 0; 
            }
        }
        
        bool characterMoving = characterVelocity.x != 0.0f || characterVelocity.y != 0.0f;
        
        
        bool wasCharacterOnGround = IsCharacterOnGround(&character, &characterPos, groundYPos);
        characterPos = Vector2Add(characterPos, characterVelocity);
        bool characterIsOnGround = IsCharacterOnGround(&character, &characterPos, groundYPos);
        
        if(characterIsOnGround) {
            characterVelocity.y = 0;
            characterPos.y = groundYPos - character.height;
            
            if(!wasCharacterOnGround) {
                PlaySound(landingSound);
            }    
        }
        else {
            characterVelocity.y += gravity;
        } 
        
    
        frameDelayCounter++;
        
        if(frameDelayCounter > frameDelay) {
            frameDelayCounter = 0;
            
            if(characterMoving) { // Se o personagem estiver se movendo, atualiza o frame
                
                if(characterIsOnGround) {
                    frameIndex++;
                    frameIndex %= numFrames;
                    
                    if(frameIndex == 0 || frameIndex == 4) {
                        PlaySound(footstepSound);
                    }    
                }
                else {
                    if(characterVelocity.y < 0) {
                        frameIndex = jumpUpFrame;
                    }    
                    else {
                        frameIndex = jumpDownFrame;
                    }    
                }
                frameRec.x = (float) frameWidth * frameIndex;
            }
        }    
        
        BeginDrawing();
            
            ClearBackground(RAYWHITE);
            
            DrawTextureRec(character , frameRec, characterPos, WHITE);
            
        EndDrawing();
    }

    CloseAudioDevice(); //Fechando o dispositivo de audio
    CloseWindow(); //Fechando janela
    
    return 0;
}



bool IsCharacterOnGround(Texture2D *character, Vector2 *characterPos, const int groundYPos) {
    if(characterPos->y + character->height >= groundYPos) {
        return true;
    }
    else {
        return false;
    }    
}    

#include "raylib.h"
#include "raymath.h"

bool IsCharacterOnGround(Texture2D *character, Vector2 *characterPos, const int groundYPos);



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
    
    Image image = LoadImage("C:\\Users\\Silvana\\Desktop\\Game - the game\\5.png");
    Texture2D background = LoadTextureFromImage(image);
    Texture2D character = LoadTexture("C:\\Users\\Silvana\\Desktop\\Sprites\\man_running.png");
    Texture2D characterStop = LoadTexture("C:\\Users\\Silvana\\Desktop\\Sprites\\man_jumping.png");
    UnloadImage(image);
    
    unsigned numFrames = 8; // Declarando o numero de frames da animacao 1 (correr)
    unsigned numFrames2 = 6; // Declarando o numero de frames da animacao 2 (parar)
    int frameWidth = character.width / numFrames; // Comprimento de um frame da animacao 1
    int frame2Width = characterStop.width / numFrames2; // Comprimento de um frame da animacao 2
    
    Rectangle frameRec = {0.0f, 0.0f, (float)frameWidth, (float)character.height};
    Rectangle frameRec2 = {0.0f, 0.0f, (float)frame2Width, (float)characterStop.height};
    Vector2 characterPos = {groundYPos, screenHeight / 2.0f};
    Vector2 characterVelocity = {0.0f, 0.0f};
    
    Sound footstepSound = LoadSound("C:\\Users\\Silvana\\Desktop\\Sprites\\Single-footstep-in-grass-A-www.fesliyanstudios.com.mp3");
    Sound landingSound = LoadSound("C:\\Users\\Silvana\\Desktop\\Sprites\\Single-footstep-in-grass-B-www.fesliyanstudios.com.mp3");
    Sound soundtrack = LoadSound("C:\\Users\\Silvana\\Desktop\\Game - the game\\08 - Minecraft.mp3");
    
    
    unsigned frameDelay = 7;
    unsigned frameDelayCounter = 0;
    unsigned frameIndex = 0;
    //unsigned frameIndex2 = 0;
    
    SetTargetFPS(60);
    
    PlaySound(soundtrack);
    
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
                    frameRec2.width = -frameRec2.width;
                } 
            }
            else if(IsKeyDown(KEY_A)) {
                characterVelocity.x = -characterSpeed;
                
                if(frameRec.width > 0) {
                    frameRec.width = -frameRec.width;
                    frameRec2.width = -frameRec2.width;
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
            else {
                if(characterIsOnGround) {
                    frameRec2.x = (float) frame2Width * 3;
                }    
            }    
        }    
        
        BeginDrawing();
            
            ClearBackground(RAYWHITE);
            
            DrawTexture(background, screenWidth/2 - background.width/2, screenHeight/2 - background.height/2, WHITE);
            
            if(!characterMoving && characterIsOnGround) {
                DrawTextureRec(characterStop, frameRec2, characterPos, WHITE);
            }
            else {
                DrawTextureRec(character , frameRec, characterPos, WHITE);
            } 
            
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

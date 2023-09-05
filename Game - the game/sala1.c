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
    const int groundYPos = 3.1 * screenHeight / 4;
    
    const int jumpUpFrame = 3;
    const int jumpDownFrame = 3;
    
    InitWindow(screenWidth, screenHeight, "Game - The game - test window"); // Abrindo janela
    
    InitAudioDevice(); // Abrindo dispositivo de audio
    ToggleFullscreen();
    
    Image image = LoadImage("D:\\Users\\dsfs\\Desktop\\Game\\5.png");
    Texture2D background = LoadTextureFromImage(image);
    Texture2D character = LoadTexture("D:\\Users\\dsfs\\Desktop\\Game\\George (1).png");
    //Texture2D characterStop = LoadTexture("C:\\Users\\Silvana\\Desktop\\Game - the game\\Sprites\\man_jumping.png");
    UnloadImage(image);
    
    unsigned int numFrames = 4; // Declarando o numero de frames da animacao 1 (correr)
    //unsigned int numFrames2 = 6; // Declarando o numero de frames da animacao 2 (parar)
    int frameWidth = character.width / numFrames; // Comprimento de um frame da animacao 1
    //int frame2Width = characterStop.width / numFrames2; // Comprimento de um frame da animacao 2
    
    Rectangle frameRec = {0.0f, 0.0f, (float)frameWidth, (float)character.height};
    //Rectangle frameRec2 = {0.0f, 0.0f, (float)frame2Width, (float)characterStop.height};

    Player player = {0};
    player.characterPos = (Vector2) {groundYPos, screenHeight / 2.0f};
    player.characterVelocity = (Vector2) {0.0f, 0.0f};
    
    Sound footstepSound = LoadSound("D:\\Users\\dsfs\\Desktop\\Game\\Single-footstep-in-grass-A.mp3");
    Sound landingSound = LoadSound("D:\\Users\\dsfs\\Desktop\\Game\\Single-footstep-in-grass-B.mp3");
    Sound soundtrack = LoadSound("D:\\Users\\dsfs\\Desktop\\Game\\[8bit] Genshin Impact _ Faruzan Theme (Master of Ingenious Devices) [Chiptune Cover](MP3_160K).mp3");
    
    
    unsigned int frameDelay = 3;
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
            if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
                player.characterVelocity.x = characterSpeed;
                
                if(frameRec.width < 0) {
                    frameRec.width = -frameRec.width;
                    //frameRec2.width = -frameRec2.width;
                } 
            }
            else if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
                player.characterVelocity.x = -characterSpeed;
                
                if(frameRec.width > 0) {
                    frameRec.width = -frameRec.width;
                    //frameRec2.width = -frameRec2.width;
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
                    
                    if(frameIndex == 0 || frameIndex == 2) {
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
                    frameRec.x = (float) frameWidth * 2;
                }    
            }    
        }    
        
        BeginDrawing();
            
            ClearBackground(BLACK);
            
            DrawTexture(background, screenWidth/2 - background.width/2, screenHeight/2 - background.height/2, WHITE);
            
            if(!player.characterMoving && player.characterIsOnGround) {
                DrawTextureRec(character, frameRec, player.characterPos, WHITE);
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

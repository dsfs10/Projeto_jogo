#ifndef JOGADOR
#define JOGADOR

#include "raylib.h"
#include "raymath.h"
#include "menu.h"

typedef struct {
  Vector2 characterPos;
  Vector2 characterLittlePos;
  Vector2 characterVelocity;
  bool characterMoving;
  bool wasCharacterOnGround;
  bool characterIsOnGround;
  bool isJumping;
  int vida;
  int doubleJumpFlag;
} Player;

typedef struct {
  Vector2 enemyPos;
  Vector2 enemyVelocity;
  Rectangle enemyHeadHitbox;
  Rectangle enemyHitbox;
  int collision_flag;
  int enemyCollision1;
  int enemyCollision2;
  int flag_turn;
  float limitL;
  float limitR;
} Enemy;

typedef struct {
  unsigned int numFrames;
  int frameWidth;
} Animacao;

bool IsCharacterOnGround(Texture2D character, Vector2 characterPos, const int groundYPos, Platform platforms[], int numPlatforms);

#endif
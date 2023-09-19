#include "raylib.h"
#include "menu.h"

bool IsCharacterOnGround(Texture2D character, Vector2 characterPos,
                         const int groundYPos, Platform platforms[],
                         int numPlatforms) {
  for (int i = 0; i < numPlatforms; i++) {
    if (CheckCollisionRecs((Rectangle){characterPos.x + 55, characterPos.y + 110, 40, 20}, platforms[i].rect)) {
      return true;
    }
  }

  return false;
}

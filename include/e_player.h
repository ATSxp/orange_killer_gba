#ifndef __E_PLAYER__
#define __E_PLAYER__

#include "engine/gba.h"

typedef struct Player {
  POINT32 pos;
  FIXED dx, dy;
  u32 w, h;
  TSprite *spr;
  BOOL dead;
} Player;

extern Player player;

void initPlayer(Player *p);
void updatePlayer(Player *p);

#endif // !__E_PLAYER__

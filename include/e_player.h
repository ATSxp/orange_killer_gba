#ifndef __E_PLAYER__
#define __E_PLAYER__

#include "engine/gba.h"

#define PLAYER_DIR_LEFT 0x00
#define PLAYER_DIR_RIGHT 0x01

typedef struct Player {
  POINT32 pos;
  FIXED dx, dy;
  u32 w, h;
  TSprite *spr;
  u16 dir;
  BOOL dead;
} Player;

extern Player player;

void initPlayer(Player *p);
void updatePlayer(Player *p);

#endif // !__E_PLAYER__

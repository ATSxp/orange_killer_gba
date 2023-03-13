#ifndef __E_PLAYER__
#define __E_PLAYER__

#include "engine/gba.h"

#define PLAYER_DIR_LEFT 0x00
#define PLAYER_DIR_RIGHT 0x01

#define PLAYER_STATE_IDLE 0x00
#define PLAYER_STATE_WALK 0x01
#define PLAYER_STATE_JUMP 0x02
#define PLAYER_STATE_ATK  0x03
#define PLAYER_STATE_DASH 0x04

typedef struct Player {
  POINT32 pos;
  FIXED dx, dy, dash_speed;
  u32 w, h;
  TSprite *spr;
  u16 dir;
  u16 state;
  BOOL dead;
} Player;

extern Player player;

void initPlayer(Player *p);
void updatePlayer(Player *p);

#endif // !__E_PLAYER__

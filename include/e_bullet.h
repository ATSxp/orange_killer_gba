#ifndef __E_BULLET__
#define __E_BULLET__

#include "engine/gba.h"

typedef struct {
  POINT32 pos;
  FIXED dx, dy;
  TSprite *spr;
  BOOL destroyed;
} Bullet;

void initBullet(Bullet *b, int x, int y, u32 tid, u16 pal, FIXED dx, FIXED dy);
void updateBullet(Bullet *b);

#endif // !__E_BULLET__

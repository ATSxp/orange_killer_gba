#include "e_bullet.h"

void destroyBullet(Bullet *b);

void initBullet(Bullet *b, int x, int y, u32 tid, u16 pal, FIXED dx, FIXED dy) {
  b->destroyed = FALSE;

  b->pos.x = x << 8;
  b->pos.y = y << 8;

  b->dx = dx;
  b->dy = dy;

  b->spr = T_addObj(x >> 8, y >> 8, OBJ_8X8, tid, pal, 0, NULL);
}

void updateBullet(Bullet *b) {
  POINT pt = {b->pos.x >> 8, b->pos.y >> 8};

  if (b->destroyed) return;

  if (pt.x < 0 || pt.x > SCREEN_WIDTH || pt.y < 0 || pt.y > SCREEN_HEIGHT ) 
    destroyBullet(b);

  b->pos.x += b->dx;
  b->pos.y += b->dy;

  b->spr->x = b->pos.x >> 8;
  b->spr->y = b->pos.y >> 8;
}

void destroyBullet(Bullet *b) {
  if (!b->destroyed) {
    b->destroyed = TRUE;
    T_removeObj(b->spr);
  }

}

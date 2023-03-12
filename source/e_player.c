#include "e_player.h"
#include "utils.h"

#include "gfx_player.h"

#define PLAYER_SPEED 0x0100 // tmp
#define PLAYER_JUMP 0x0290

void updateColl(Player *p);
INLINE BOOL sol(int x, int y); // Tmp

Player player;

void initPlayer(Player *p) {
  p->dead = FALSE;
  p->pos.x = p->pos.y = 32 << 8;
  p->w = p->h = 16;
  p->spr = T_addObj(p->pos.x >> 8, p->pos.y >> 8, OBJ_16X16, 1, 0, 1, SET_GFX_OBJ(FALSE, gfx_player));
}

void updatePlayer(Player *p) {
  p->dx = 0;

  if (key_is_down(KEY_LEFT)) p->dx = -PLAYER_SPEED;
  else if (key_is_down(KEY_RIGHT)) p->dx = PLAYER_SPEED;

  updateColl(p);

  // Jump
  if (p->dy == 0 && key_hit(KEY_A)) p->dy = -PLAYER_JUMP;

  p->pos.x += p->dx;
  p->pos.y += p->dy;

  p->spr->x = p->pos.x >> 8;
  p->spr->y = p->pos.y >> 8;
}

void updateColl(Player *p) {
  POINT32 pt = {(p->pos.x + p->dx) >> 8, (p->pos.y + p->dy) >> 8};
  u32 w = p->w - 1, h = p->h - 1;

  // Check left and right
  if (sol(pt.x, pt.y) || sol(pt.x + w, pt.y) ||
  sol(pt.x, pt.y + h) || sol(pt.x + w, pt.y + h)) {
    p->dx = 0;
  }

  // Check up and down
  if (sol((p->pos.x >> 8), pt.y + (h + 1)) || sol((p->pos.x >> 8) + w, pt.y + (h + 1))) {
    p->dy = 0;
  } else {
    p->dy += 0x033; // .8 = 0.2
  }

  // Check Up and ... left/right maybe?
  if (p->dy < 0 && (sol(pt.x, pt.y) || sol(pt.x + w, pt.y)))
    p->dy = 0;
}

INLINE BOOL sol(int x, int y) {
  return (getTile(1, x, y) & 1);
}

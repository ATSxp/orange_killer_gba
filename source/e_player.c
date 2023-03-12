#include "e_player.h"

#include "gfx_player.h"

Player player;

void initPlayer(Player *p) {
  p->dead = FALSE;
  p->spr = T_addObj(p->pos.x, p->pos.y, OBJ_16X16, 1, 0, 1, SET_GFX_OBJ(FALSE, gfx_player));
}

void updatePlayer(Player *p) {
  p->dx = key_tri_horz() << 8;
  p->dy = key_tri_vert() << 8;

  p->pos.x += p->dx;
  p->pos.y += p->dy;

  p->spr->x = p->pos.x >> 8;
  p->spr->y = p->pos.y >> 8;
}

void updateColl(Player *p) {}

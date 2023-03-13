#include "e_player.h"
#include "utils.h"

#include "e_bullet.h"

#include "gfx_player.h"
#include "gfx_orange.h"

#define PLAYER_SPEED 0x0100 // tmp
#define PLAYER_JUMP 0x0380
#define PLAYER_DASH 0x0600
#define ORANGE_SPEED 0x0280
#define MAX_ORANGES 8
#define MAX_SHOOT_COOLDOWN 0x0400

#define sol(x, y) isSolid(1, x, y, 1 | 2)

Player player;
Bullet p_oranges[MAX_ORANGES];

FIXED p_shoot_cooldown;
u8 p_orange_count;

void updateColl(Player *p);
void playerShoot(Player *p);
void playerDash(Player *p);

void initPlayer(Player *p) {
  p->dead = FALSE;
  p->pos.x = p->pos.y = 32 << 8;
  p->w = p->h = 16;
  p->spr = T_addObj(p->pos.x >> 8, p->pos.y >> 8, OBJ_16X16, 1, 0, 1, SET_GFX_OBJ(FALSE, gfx_player));

  GRIT_CPY(pal_obj_bank[1], gfx_orangePal);
  GRIT_CPY(&tile_mem[4][5], gfx_orangeTiles);
  
  p_shoot_cooldown = MAX_SHOOT_COOLDOWN;
  p_orange_count = 0;
}

void updatePlayer(Player *p) {
  if (p->dx != 0) {
    T_flipObj(p->spr, p->dx < 0, FALSE);
    p->dir = p->dx > 0 ? PLAYER_DIR_RIGHT : PLAYER_DIR_LEFT;
  }

  if (p->state == PLAYER_STATE_IDLE || p->state == PLAYER_STATE_WALK) {
    p->dash_speed = PLAYER_DASH;

    if (key_is_down(KEY_LEFT)) p->dx = -PLAYER_SPEED;
    else if (key_is_down(KEY_RIGHT)) p->dx = PLAYER_SPEED;
    else p->dx = 0;

    // Jump
    if (p->dy == 0 && key_hit(KEY_A)) p->dy = -PLAYER_JUMP;

    // Dash
    if (key_hit(KEY_L)) {
      p->dir = PLAYER_DIR_LEFT;
      p->state = PLAYER_STATE_DASH;
    } else if (key_hit(KEY_R)) {
      p->dir = PLAYER_DIR_RIGHT;
      p->state = PLAYER_STATE_DASH;
    }

  } else if (p->state == PLAYER_STATE_DASH) {
    playerDash(p);

  }

  updateColl(p);
  playerShoot(p);

  p->pos.x += p->dx;
  p->pos.y += p->dy;

  p->spr->x = p->pos.x >> 8;
  p->spr->y = p->pos.y >> 8;
}

void updateColl(Player *p) {
  POINT32 pt = {(p->pos.x + p->dx) >> 8, (p->pos.y + p->dy) >> 8};
  u32 w = p->w - 1, h = p->h - 1;

  // Check left and right
  if ((sol(pt.x, pt.y) || sol(pt.x + w, pt.y) ||
  sol(pt.x, pt.y + h) || sol(pt.x + w, pt.y + h)) && p->dx != 0) {
    p->dx = 0;
  }

  // Check up and down
  if (sol((p->pos.x >> 8), pt.y + (h + 1)) || sol((p->pos.x >> 8) + w, pt.y + (h + 1))) {
    p->dy = 0;
  } else {
    if (p->state != PLAYER_STATE_DASH)
      p->dy += 0x033; // .8 = 0.2
    else 
      p->dy = 0;

  }

  // Check Up and ... left/right maybe?
  if (p->dy < 0 && (sol(pt.x, pt.y) || sol(pt.x + w, pt.y)))
    p->dy = 0;

}

void playerShoot(Player *p) {
  u32 ii;
  Bullet *oran = p_oranges;

  p_shoot_cooldown -= 0x020;

  if (key_hit(KEY_B)) {
    if (p_shoot_cooldown <= 0x00) {
      Bullet *o = &p_oranges[p_orange_count];

      initBullet(o, p->pos.x >> 8, p->pos.y >> 8, 5, 1, 0, 0);
      o->dx = p->dir > 0 ? ORANGE_SPEED : -ORANGE_SPEED;

      p_orange_count = (p_orange_count + 1) & 7;
      p_shoot_cooldown = MAX_SHOOT_COOLDOWN;
    }

  }

  for (ii = 0; ii < MAX_ORANGES; ii++) {
    if (oran == NULL) continue;
    updateBullet(oran);
    oran++;
  }

}

void playerDash(Player *p) {
  p->dash_speed -= 0x080;

  p->dx = p->dir > 0 ? p->dash_speed : -p->dash_speed ;

  if ((p->dash_speed <= 0x00)) p->state = PLAYER_STATE_IDLE;

}

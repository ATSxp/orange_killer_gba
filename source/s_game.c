#include "engine/gba.h"
#include "utils.h"
#include "e_player.h"

void initGame() {
  T_setMode(0);
  T_enableBg(0);
  T_enableObjs();

  T_initObjs(1);

  tte_init_se_default(0, BG_CBB(1) | BG_SBB(31));

  initPlayer(&player);
}

void updateGame() {
  updatePlayer(&player);
  T_updateObjs();
}

void endGame() {}

Scene s_game = {initGame, updateGame, endGame};

#include "engine/gba.h"
#include "utils.h"
#include "e_player.h"

#include "tests_bg1.h"

void initGame() {
  T_setMode(0);
  T_enableBg(0);
  T_enableBg(1);
  T_enableObjs();

  T_initObjs(1);

  tte_init_se_default(0, BG_CBB(1) | BG_SBB(31));

  // Temp
  GRIT_CPY(pal_bg_mem, tests_bg1Pal);
  GRIT_CPY(&tile_mem[0], tests_bg1Tiles);

  REG_BG1CNT = BG_CBB(0) | BG_SBB(28);

  memcpy16(se_mem[28], tests_bg1Map, tests_bg1MapLen);

  initPlayer(&player);
}

void updateGame() {
  updatePlayer(&player);

  T_updateObjs();
}

void endGame() {}

Scene s_game = {initGame, updateGame, endGame};

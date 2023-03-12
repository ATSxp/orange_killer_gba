#include "engine/gba.h"
#include "utils.h"
#include "all_scenes.h"

int main() {
  T_init();

  setScene(s_game);

  while (1) {
    T_update();

    updateScene();
  }

  return 0;
}

#ifndef __UTILS_H__
#define __UTILS_H__

#include "engine/gba.h"

typedef struct {
  fnptr init, update, end;
} Scene;

INLINE u8 getTile(u32 bgnr, int x, int y);

void setScene(Scene next_scene);
void updateScene();

INLINE u8 getTile(u32 bgnr, int x, int y) {
  SCR_ENTRY *dst = (SCR_ENTRY *)se_mem[BFN_GET(REG_BGCNT[bgnr], BG_SBB)];
  return dst[(y >> 3) * 32 + (x >> 3)];
}

#endif // !__UTILS_H__

#include "engine/sprites.h"

u32 spr_max = 0;
u32 spr_count = 0;

TSprite spr_buffer[128];

u32 unussed_ids_count = 0;
u8 unussed_ids[128];

int sort_keys[128];
u8 sort_ids[128];

IWRAM_CODE void idSortShell(int keys[], u8 ids[], int count) {
  u32 ii, inc;

  for (inc = 1; inc <= count; inc++)
    inc *= 3;

  do {
    inc /= 3;

    for (ii = inc; ii < count; ii++) {
      u32 j, id0 = ids[ii];
      int key0 = keys[id0];

      for (j = ii; j >= inc && keys[ids[j - inc]] > key0; j -= inc)
        ids[j] = ids[j - inc];

      ids[j] = id0;

    }

  } while(inc > 1);

}

INLINE u16 _getShapeFromSize(u16 size);
void _setGfxObj(TSprite *spr, TGfx *gfx, u16 tid);

void T_initObjs(u16 total) {
  u32 ii;

  for (ii = 0; ii < 128; ii++)
    spr_buffer[ii].id = sort_ids[ii] = ii;

  oam_init(oam_mem, 128);

  spr_max = total;
}

TSprite *T_addObj(int x, int y, u16 size, u16 tid, u16 pb, u16 prio, TGfx *gfx) {
  TSprite *spr;
  OBJ_ATTR *obj;
  u8 un_id = unussed_ids[unussed_ids_count - 1];
  u8 id = unussed_ids_count > 0 ? un_id : spr_count;

  if (spr_count >= spr_max)
    return NULL;

  spr = &spr_buffer[id];
  obj = &spr_buffer[id].obj;

  if (unussed_ids_count > 0) unussed_ids_count--;

  spr_count++;

  spr->active = TRUE;

  _setGfxObj(spr, gfx, tid);

  obj->attr0 = _getShapeFromSize(size);
  obj->attr1 = (size << ATTR1_SIZE_SHIFT);
  obj->attr2 = ATTR2_BUILD(tid, pb, prio);

  BFN_SET2(obj->attr1, spr->x = x, ATTR1_X);
  BFN_SET2(obj->attr0, spr->y = y, ATTR0_Y);

  return spr;
}

void T_removeObj(TSprite *spr) {
  if (!spr->active) return;

  unussed_ids[unussed_ids_count++] = spr->id;

  spr_buffer[spr->id].obj = (OBJ_ATTR){0, 0, 0};
  obj_hide(&spr_buffer[spr->id].obj);
  spr->active = FALSE;

  spr_count--;
}

void T_updateObjs() {
  u32 ii;
  int *keys = sort_keys;
  TSprite *spr = spr_buffer;
  OBJ_ATTR *obj;

  for (ii = 0; ii < spr_max; ii++) {
    obj = &spr_buffer[spr->id].obj;
    T_setPosObj(spr, spr->x, spr->y);

    *keys++ = spr->y + ((obj->attr0 & ATTR0_MODE_MASK) & ATTR0_HIDE);

    spr++;
  }

  idSortShell(sort_keys, sort_ids, spr_max);

  for (ii = 0; ii < spr_max; ii++)
    oam_copy(&oam_mem[ii], &spr_buffer[sort_ids[ii]].obj, 1);
}

INLINE u16 _getShapeFromSize(u16 size) {
  if (size > 3) return ATTR0_TALL;
  else if(size > 7) return ATTR0_WIDE;

  return ATTR0_SQUARE;
}

void _setGfxObj(TSprite *spr, TGfx *gfx, u16 tid) {
  TILE *tile_dst;
  COLOR *pal_dst;

  if (gfx) {
    spr->gfx.bpp = gfx->bpp;
    spr->gfx.tiles = gfx->tiles;
    spr->gfx.pal = gfx->pal;

    spr->gfx.tiles_len = gfx->tiles_len;
    spr->gfx.pal_len = gfx->pal_len;

    pal_dst = pal_obj_bank[BFN_GET(spr->obj.attr2, ATTR2_PALBANK)];

    if (!gfx->bpp) tile_dst = &tile_mem[4][tid];
    else tile_dst = &tile_mem[4][tid * 2];

    memcpy32(tile_dst, gfx->tiles, gfx->tiles_len / 4);
    memcpy32(pal_dst, gfx->pal, gfx->pal_len / 4);
  }

}

#include <grrlib.h>
#include <gccore.h>
#include <stdint.h>
#include <stdlib.h>
#include "graphics.h"

// Embedded assets
static const uint8_t palette_file[] = {
  #embed "../../assets/palette.pal"
};
static const uint8_t font_file[] = {
  #embed "../../assets/font.4bpp"
};

u32 wii_colors[16];
GRRLIB_texImg *font_tileset = NULL;
GRRLIB_texImg *spr_tileset  = NULL;

#define TILE_W 8
#define TILE_H 8
#define GRID_COLS 16                 // <- fixed 16 tiles wide
#define ATLAS_W (GRID_COLS * TILE_W) // 128 px
#define MAX_TEX_DIM 1024             // GX practical limit

static inline u32 make_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  return ((u32)r << 24) | ((u32)g << 16) | ((u32)b << 8) | (u32)a;
}

void load_palette(void) {
  for (uint32_t i = 0; i < 16; i++) {
    uint8_t r = palette_file[i*3+0];
    uint8_t g = palette_file[i*3+1];
    uint8_t b = palette_file[i*3+2];
    wii_colors[i] = make_rgba(r, g, b, 0xFF);
  }
  // Make index 0 transparent
  wii_colors[0] &= 0xFFFFFF00u;
}

static void blit_tile_rgba4bpp(GRRLIB_texImg *tex,
                               const uint8_t *tile32, // 32 bytes (8*8/2)
                               int base_x, int base_y,
                               const u32 *pal) {
  for (int row = 0; row < TILE_H; row++) {
    const uint8_t *row_bytes = tile32 + row * 4; // 8 px → 4 bytes
    int y = base_y + row;
    for (int col = 0; col < TILE_W; col++) {
      uint8_t b   = row_bytes[col >> 1];
      uint8_t idx = (col & 1) ? (b >> 4) : (b & 0x0F);
      GRRLIB_SetPixelTotexImg(base_x + col, y, tex, pal[idx]);
    }
  }
}

static GRRLIB_texImg* create_4bpp_tileset(const uint8_t *data, int tile_count) {
  int rows = (tile_count + GRID_COLS - 1) / GRID_COLS;
  int atlas_h = rows * TILE_H;

  // Defensive: stay within GX limit (caller can adjust assets if exceeded)
  if (atlas_h > MAX_TEX_DIM) {
    // Clamp to avoid bad alloc; in practice you should split the sheet if hit.
    rows = MAX_TEX_DIM / TILE_H;
    atlas_h = rows * TILE_H;
  }

  GRRLIB_texImg *tex = GRRLIB_CreateEmptyTextureFmt(ATLAS_W, atlas_h, GX_TF_RGBA8);
  if (!tex) return NULL;

  for (int t = 0; t < tile_count; t++) {
    int col = t % GRID_COLS;
    int row = t / GRID_COLS;
    if (row >= rows) break; // safety if clamped above
    const uint8_t *tile_ptr = data + (t * 32);
    blit_tile_rgba4bpp(tex, tile_ptr, col * TILE_W, row * TILE_H, wii_colors);
  }

  GRRLIB_FlushTex(tex);
  GRRLIB_InitTileSet(tex, TILE_W, TILE_H, 0);
  return tex;
}


void load_font_tiles(void) {
  if (font_tileset) return;

  // Derive font tile count from asset size (expand freely later)
  const uint32_t bytes = (uint32_t)sizeof(font_file);
  const int tile_count = (int)((bytes * 2u) / (TILE_W * TILE_H)); // 2 nibbles/byte

  font_tileset = create_4bpp_tileset(font_file, tile_count);
}

void load_sprite_tiles(uint8_t tile_data[SPR_TILES_SIZE]) {
  if (spr_tileset) {
    GRRLIB_FreeTexture(spr_tileset);
    spr_tileset = NULL;
  }
  const int tile_count = (int)((SPR_TILES_SIZE * 2u) / (TILE_W * TILE_H));
  spr_tileset = create_4bpp_tileset(tile_data, tile_count);
}

void unload_graphics(void) {
  if (spr_tileset)  { GRRLIB_FreeTexture(spr_tileset);  spr_tileset = NULL; }
  if (font_tileset) { GRRLIB_FreeTexture(font_tileset); font_tileset = NULL; }
}


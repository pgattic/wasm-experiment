#include <grrlib.h>
#include <gccore.h>
#include <stdint.h>
#include <stdlib.h>
#include "graphics.h"

static const uint8_t palette_file[] = {
  #embed "../../assets/palette.pal"
};
static const uint8_t font_file[] = {
  #embed "../../assets/font.4bpp"
};

u32 wii_colors[16];
GRRLIB_texImg *font_tileset = NULL;
GRRLIB_texImg *spr_tileset  = NULL;

static inline u32 make_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  // GRRLIB ships RGBA() macro, but we keep it explicit
  return ((u32)r << 24) | ((u32)g << 16) | ((u32)b << 8) | (u32)a;
}

void load_palette(void) {
  for (uint32_t i = 0; i < 16; i++) {
    uint8_t r = palette_file[i*3+0];
    uint8_t g = palette_file[i*3+1];
    uint8_t b = palette_file[i*3+2];
    wii_colors[i] = make_rgba(r, g, b, 0xFF);
  }
  // First color transparent (matches your SDL behavior)
  wii_colors[0] ^= 0xFF; // flip A from 0xFF -> 0x00
}

#define FONT_TILE_COUNT 128
#define FONT_TILES_SIZE 0x1000 // 8*8*128 / 2 = 4096 bytes

static GRRLIB_texImg* build_linear_rgba_texture(uint32_t w, uint32_t h) {
  // RGBA8, CPU-writable; GRRLIB handles tiling internally for per-pixel writes
  GRRLIB_texImg *t = GRRLIB_CreateEmptyTextureFmt(w, h, GX_TF_RGBA8);
  return t;
}

static void write_tile_strip_rgba(GRRLIB_texImg *tex, const uint8_t *src4bpp, uint32_t tiles) {
  // src4bpp is 8x8*tiles in **4bpp**, packed two pixels/byte, stacked vertically.
  // We replicate your SDL swizzle: texture_x = (i%4)*2 ; texture_y = i/4
  const uint32_t total_bytes = (8*8*tiles) / 2;
  for (uint32_t i = 0; i < total_bytes; i++) {
    uint8_t p1 = src4bpp[i] & 0xF;
    uint8_t p2 = src4bpp[i] >> 4;

    int tx = (int)((i % 4) * 2);
    int ty = (int)(i / 4);

    GRRLIB_SetPixelTotexImg(tx + 0, ty, tex, wii_colors[p1]);
    GRRLIB_SetPixelTotexImg(tx + 1, ty, tex, wii_colors[p2]);
  }
  // Flush data cache to memory for GX
  GRRLIB_FlushTex(tex);
}

void load_font_tiles(void) {
  if (font_tileset) return;
  font_tileset = build_linear_rgba_texture(8, 8 * FONT_TILE_COUNT);
  write_tile_strip_rgba(font_tileset, font_file, FONT_TILE_COUNT);
  // Teach GRRLIB where tile boundaries are so we can use DrawTile()
  GRRLIB_InitTileSet(font_tileset, 8, 8, 0);
}

void load_sprite_tiles(uint8_t tile_data[SPR_TILES_SIZE]) {
  if (spr_tileset) GRRLIB_FreeTexture(spr_tileset);
  spr_tileset = build_linear_rgba_texture(8, 8 * 256);
  write_tile_strip_rgba(spr_tileset, tile_data, 256);
  GRRLIB_InitTileSet(spr_tileset, 8, 8, 0);
}

void unload_graphics(void) {
  if (spr_tileset) { GRRLIB_FreeTexture(spr_tileset); spr_tileset = NULL; }
  if (font_tileset){ GRRLIB_FreeTexture(font_tileset); font_tileset = NULL; }
}


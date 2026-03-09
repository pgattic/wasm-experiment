#include <stdint.h>
#include "graphics.h"

static const uint8_t palette_file[] = {
  #embed "../../assets/palette.pal"
};

static const uint8_t font_file[] = {
  #embed "../../assets/font.4bpp"
};

uint32_t wii_colors[16];
uint8_t font_tiles[128][64];
uint8_t spr_tiles[256][64];

static inline uint32_t make_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  return ((uint32_t)r << 24) | ((uint32_t)g << 16) | ((uint32_t)b << 8) | (uint32_t)a;
}

static void decode_4bpp_tiles(const uint8_t *src, size_t bytes, uint8_t *dst, size_t tile_count) {
  size_t max_tiles = bytes / 32u;
  if (tile_count > max_tiles) {
    tile_count = max_tiles;
  }

  memset(dst, 0, tile_count * 64u);
  for (size_t t = 0; t < tile_count; t++) {
    const uint8_t *tile = src + (t * 32u);
    uint8_t *out = dst + (t * 64u);
    for (size_t row = 0; row < 8u; row++) {
      const uint8_t *row_bytes = tile + (row * 4u);
      uint8_t *row_out = out + (row * 8u);
      for (size_t col = 0; col < 8u; col++) {
        uint8_t b = row_bytes[col >> 1];
        row_out[col] = (col & 1u) ? (b >> 4) : (b & 0x0Fu);
      }
    }
  }
}

void load_palette(void) {
  for (uint32_t i = 0; i < 16; i++) {
    uint8_t r = palette_file[i * 3u + 0u];
    uint8_t g = palette_file[i * 3u + 1u];
    uint8_t b = palette_file[i * 3u + 2u];
    wii_colors[i] = make_rgba(r, g, b, 0xFF);
  }
  wii_colors[0] &= 0xFFFFFF00u;
}

void load_font_tiles(void) {
  decode_4bpp_tiles(font_file, sizeof(font_file), &font_tiles[0][0], 128u);
}

void load_sprite_tiles(uint8_t tile_data[SPR_TILES_SIZE]) {
  decode_4bpp_tiles(tile_data, SPR_TILES_SIZE, &spr_tiles[0][0], 256u);
}

void unload_graphics(void) {
}


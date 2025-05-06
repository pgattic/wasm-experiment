#include <stdint.h>
#include "graphics.h"
#include "../cartridge.h"

// Color rl_palette[16]; // Keep this synched with the palette from memory

void load_palette(uint8_t palette[PALETTE_SIZE]) {
  // for (uint32_t i = 0; i < 16; i++) {
  //   uint32_t color = palette[i*3] << 24 |
  //                palette[i*3+1] << 16 |
  //                palette[i*3+2] << 8;
  //   rl_palette[i] = color;
  // }
}

// Color rl_spr_tiles[256*8*8];

void load_sprite_tiles(uint8_t tile_data[SPR_TILES_SIZE]) {
  // for (uint32_t i = 0; i < SPR_TILES_SIZE; i++) {
  //   rl_spr_tiles[i*2] = rl_palette[tile_data[i]&0xF];
  //   rl_spr_tiles[i*2+1] = rl_palette[tile_data[i]>>4];
  // }
}

void unload_graphics() {
}


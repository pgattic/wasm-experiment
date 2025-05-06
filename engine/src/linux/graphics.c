#include <SDL3/SDL_pixels.h>
#include <stdint.h>
#include <SDL3/SDL.h>
#include "graphics.h"
#include "../cartridge.h"

// Color rl_palette[16]; // Keep this synched with the palette from memory

SDL_Color sdl_colors[16];

SDL_Palette sdl_palette = {
  .ncolors = 16,
  .colors = sdl_colors,
};

void load_palette(uint8_t palette[PALETTE_SIZE]) {
  for (uint32_t i = 0; i < 16; i++) {
    sdl_colors[i].r = palette[i*3];
    sdl_colors[i].g = palette[i*3+1];
    sdl_colors[i].b = palette[i*3+2];
  }
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


#include <SDL3/SDL_render.h>
#include <stdint.h>
#include <stdlib.h>
#include "graphics.h"
#include "../cartridge.h"

SDL_Color sdl_colors[16]; // SDL colors
uint32_t rgb_colors[16];

SDL_Palette sdl_palette = {
  .ncolors = 16,
  .colors = sdl_colors,
};

SDL_Window *os_window; // Game window (Full size of OS window)
SDL_Renderer *renderer;
SDL_Texture *game_window; // Where graphics are drawn
SDL_Texture *spr_tileset;

void load_palette(uint8_t palette[PALETTE_SIZE]) {
  for (uint32_t i = 0; i < 16; i++) {
    sdl_colors[i].r = palette[i*3];
    sdl_colors[i].g = palette[i*3+1];
    sdl_colors[i].b = palette[i*3+2];
    rgb_colors[i] = palette[i*3] << 24 | palette[i*3+1] << 16 | palette[i*3+2] << 8 | 0xff;
  }
  rgb_colors[0] ^= 0xff; // First color is transparent
}

void load_sprite_tiles(uint8_t tile_data[SPR_TILES_SIZE]) {
  uint32_t *pixels = malloc(8 * 8 * 256 * sizeof(uint32_t));

  for (uint32_t i = 0; i < SPR_TILES_SIZE; i++) {
    // 4bpp = 2 pixels/byte
    uint8_t p1 = tile_data[i]&0xF;
    uint8_t p2 = tile_data[i]>>4;
    int texture_x = (i % 4) * 2;
    int texture_y = i/4;
    pixels[texture_y * 8 + texture_x] = rgb_colors[p1];
    pixels[texture_y * 8 + texture_x + 1] = rgb_colors[p2];
  }

  SDL_Rect texture_rect = {
    .x = 0,
    .y = 0,
    .w = 8,
    .h = 8*256
  };

  spr_tileset = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_RGBA8888,
    SDL_TEXTUREACCESS_STATIC,
    8, 8 * 256
  );
  SDL_UpdateTexture(spr_tileset, &texture_rect, pixels, 8 * sizeof(uint32_t));
  free(pixels);
}


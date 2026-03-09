#pragma once
#include <stdint.h>
#include "../game.h"

// 16-color RGBA palette
extern uint32_t wii_colors[16];

// Decoded tile data, one palette index per pixel (8x8 tiles).
extern uint8_t font_tiles[128][64];
extern uint8_t spr_tiles[256][64];

// Loads embedded palette -> wii_colors (index 0 gets alpha=0)
void load_palette(void);

// Decode embedded 4bpp font tiles into font_tiles
void load_font_tiles(void);

// Decode 4bpp sprite tiles into spr_tiles
void load_sprite_tiles(uint8_t tile_data[SPR_TILES_SIZE]);

// Free graphics resources (no-op for current implementation)
void unload_graphics(void);


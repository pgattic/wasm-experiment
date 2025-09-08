#pragma once
#include <stdint.h>
#include <grrlib.h>
#include "../game.h"

// 16-color RGBA (GRRLIB expects RGBA u32)
extern u32 wii_colors[16];

extern GRRLIB_texImg *font_tileset;  // 128 tiles, 8×1024
extern GRRLIB_texImg *spr_tileset;   // 256 tiles, 8×2048

// Loads embedded palette -> wii_colors (index 0 gets alpha=0)
void load_palette(void);

// Build the 8×(8*128) font atlas from embedded 4bpp
void load_font_tiles(void);

// Build the 8×(8*256) sprite atlas from 4bpp tiles
void load_sprite_tiles(uint8_t tile_data[SPR_TILES_SIZE]);

// Free textures
void unload_graphics(void);


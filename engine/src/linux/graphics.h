#pragma once
#include <stdint.h>
#include <gl2d.h>
#include "../cartridge.h"

// Code for managing graphical information such as tilemaps and sprites

extern Color rl_palette[16]; // Keep this synched with the palette from memory
extern Color rl_spr_tiles[256*8*8];

/// Synchronizes the palette from VRAM with the NDS-sized palette.
/// Should be called whenever the palette's data in VRAM is modified.
void load_palette(uint8_t palette[PALETTE_SIZE]);

/// Loads the sprite tiles in using the NDS GL2D library.
/// The `load_palette` function must be called at least once before this one.
void load_sprite_tiles(uint8_t tile_data[SPR_TILES_SIZE]);

void unload_graphics();


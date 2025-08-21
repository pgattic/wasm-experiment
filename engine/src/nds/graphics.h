#pragma once
#include <stdint.h>
#include <gl2d.h>
#include "../game.h"

// Code for managing graphical information such as tilemaps and sprites

extern uint16_t nds_palette[16];
extern glImage spr_tiles[256];
extern int spr_texture_id;

extern glImage font_tiles[128];
extern int font_texture_id;

void load_font_tiles();

/// Loads the included palette into NDS VRAM
void load_palette();

/// Loads the sprite tiles in using the NDS GL2D library.
/// The `load_palette` function must be called at least once before this one.
void load_sprite_tiles(uint8_t tile_data[SPR_TILES_SIZE]);

void unload_graphics();


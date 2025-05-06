#pragma once
#include <SDL3/SDL_render.h>
#include <stdint.h>
#include "../cartridge.h"

// Code for managing graphical information such as tilemaps and sprites

extern SDL_Color sdl_colors[16];
extern SDL_Palette sdl_palette;

extern SDL_Window *os_window;
extern SDL_Renderer *renderer;
extern SDL_Texture *game_window; // Window where textures are drawn
extern SDL_Texture *spr_tileset;

/// Synchronizes the palette from VRAM with the NDS-sized palette.
/// Should be called whenever the palette's data in VRAM is modified.
void load_palette(uint8_t palette[PALETTE_SIZE]);

/// Loads the sprite tiles in using the NDS GL2D library.
/// The `load_palette` function must be called at least once before this one.
void load_sprite_tiles(uint8_t tile_data[SPR_TILES_SIZE]);


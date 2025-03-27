#include "memory.h"
#include <stdint.h>
#include <nds.h>
#include "cartridge.h"

Memory gameMemory;
uint16_t ndsPalette[16]; // Keep this synched with the palette from memory (see SyncPalette)

/// Synchronizes the palette from VRAM with the NDS-sized palette.
/// Should be called whenever the palette's data in VRAM is modified.
/// see `ndsPalette` from memory.h.
void syncPalette(uint8_t palette[PALETTE_SIZE]) {
  for (uint8_t i = 0; i < 16; i++) {
    ndsPalette[i] = RGB15(((palette)[i*3]) >> 3,
                          ((palette)[i*3+1]) >> 3,
                          ((palette)[i*3+2]) >> 3);
  }
}

/// Copies the relevant information from the cartridge data into the memory state of the machine.
void loadMemory(Cart *cart) {
  memcpy(gameMemory.palette,  cart->palette, PALETTE_SIZE);
  memcpy(gameMemory.sprTiles, cart->spr_tiles, SPR_TILES_SIZE);
  memcpy(gameMemory.bgTiles,  cart->bg_tiles, BG_TILES_SIZE);
  memcpy(gameMemory.tilemap,  cart->tilemap, TILEMAP_SIZE);
  syncPalette(gameMemory.palette);
}


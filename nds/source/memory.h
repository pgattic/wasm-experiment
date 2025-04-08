#pragma once

#include <stdint.h>
#include "cartridge.h"

typedef struct {
  uint8_t palette[PALETTE_SIZE];
  uint8_t sprTiles[SPR_TILES_SIZE];
  uint8_t bgTiles[BG_TILES_SIZE];
  uint8_t tilemap[TILEMAP_SIZE];
} Memory;

/// Singleton value representing the VRAM state of the cartridge
extern Memory gameMemory;
/// Singleton value representing the `palette` attribute of `gameMemory`,
/// but in the NDS's RGB15 style.
/// Keep this synched with the palette from memory (see `SyncPalette`)
extern uint16_t ndsPalette[16];

void syncPalette(uint8_t palette[PALETTE_SIZE]);
void loadMemory(Cart *cart);


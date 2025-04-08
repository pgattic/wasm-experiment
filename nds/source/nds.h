#pragma once

#include <stdint.h>
#include "cartridge.h"
#include "m3_env.h"

void LinkNDSFunctions(IM3Module module);
void loadPalette(Cart * cart);
int loadSpriteTiles(Cart * cart);
void collectKeys();

extern uint16_t nds_palette[16];
extern glImage sprTiles[256];

#define NDS_SC_W 256
#define NDS_SC_H 192

// GBA size
#define GAME_SC_W 240
#define GAME_SC_H 160

#define LEFT_MARGIN ((NDS_SC_W - GAME_SC_W) / 2) // 8
#define TOP_MARGIN ((NDS_SC_H - GAME_SC_H) / 2) // 16


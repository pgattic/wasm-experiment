#ifndef NDS_H
#define NDS_H

#include <stdint.h>
#include "cartridge.h"
#include "m3_env.h"

void LinkNDSFunctions(IM3Module module);
void loadPalette(Cart * cart);
int loadSpriteTiles(Cart * cart);

extern uint16_t nds_palette[16];
extern glImage sprTiles[256];

#endif


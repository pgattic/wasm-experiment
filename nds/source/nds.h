#ifndef NDS_H
#define NDS_H

#include <stdint.h>
#include "cartridge.h"
#include "m3_env.h"

void LinkNDSFunctions(IM3Module module);
void loadPalette(Cart * cart);

#endif


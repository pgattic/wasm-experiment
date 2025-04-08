#pragma once

#include <stdint.h>
#include <stddef.h>

#define FILE_SIZE         0x100000
#define HEADER_SIZE       0x10
#define PALETTE_SIZE      0x30
#define SPR_TILES_SIZE    0x2000
#define BG_TILES_SIZE     0x2000
#define TILEMAP_SIZE      0x10000
#define METAPROG_SIZE     (HEADER_SIZE + PALETTE_SIZE + SPR_TILES_SIZE + BG_TILES_SIZE + TILEMAP_SIZE)
#define PRG_CODE_SIZE     (FILE_SIZE - METAPROG_SIZE)

typedef struct __attribute__((__packed__)) {
  uint8_t header[HEADER_SIZE];
  uint8_t palette[PALETTE_SIZE];
  uint8_t spr_tiles[SPR_TILES_SIZE];
  uint8_t bg_tiles[BG_TILES_SIZE];
  uint8_t tilemap[TILEMAP_SIZE];
  uint8_t prg_code[PRG_CODE_SIZE];
} Cart;

// Declaration for file-loading function
Cart *loadCartridge(const char *filepath, size_t *fileSize);


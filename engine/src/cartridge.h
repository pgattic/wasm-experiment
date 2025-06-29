#pragma once

#include <stdint.h>
#include <stddef.h>

#define FILE_SIZE         0x100000 // Maximum file size is 1 MiB
#define HEADER_SIZE       0x40
#define SPR_TILES_SIZE    0x2000
#define BG_TILES_SIZE     0x2000
#define TILEMAP_SIZE      0x10000
#define METAPROG_SIZE     (HEADER_SIZE + SPR_TILES_SIZE + BG_TILES_SIZE + TILEMAP_SIZE)
#define PRG_CODE_SIZE     (FILE_SIZE - METAPROG_SIZE)

typedef struct __attribute__((__packed__)) {
  uint8_t header[HEADER_SIZE];
  uint8_t spr_tiles[SPR_TILES_SIZE];
  uint8_t bg_tiles[BG_TILES_SIZE];
  uint8_t tilemap[TILEMAP_SIZE];
  uint8_t prg_code[PRG_CODE_SIZE];
} Cart;

Cart *load_cartridge(const char *filepath, size_t *fileSize);


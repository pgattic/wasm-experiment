#include <nds.h>
#include <ulib/ulib.h>
#include <fat.h>
#include <stdio.h>
#include <wasm3.h>
#include <m3_env.h>
#include "nds.h"

// File layout constants.
#define FILE_SIZE         0x100000 // 1048576
#define HEADER_SIZE       0x10     // 16 (Don't know what this'll be about yet)
#define PALETTE_SIZE      0x30     // 8bits per channel * three channels (RGB) * 16 colors = 48 bytes
#define SPR_TILES_SIZE    0x2000   // 4bit/px * 8px * 8px * 256tiles = 32768bits (8192 bytes)
#define BG_TILES_SIZE     0x2000   // 4bit/px * 8px * 8px * 256tiles = 32768bits (8192 bytes)
#define METAPROG_SIZE     HEADER_SIZE + PALETTE_SIZE + SPR_TILES_SIZE + BG_TILES_SIZE // 16448
#define PRG_CODE_SIZE     FILE_SIZE - METAPROG_SIZE // 1032128

// Packed struct that exactly matches the file layout.
typedef struct __attribute__((__packed__)) {
  unsigned char header[HEADER_SIZE];           // 0x00000-0x0000F
  unsigned char palette[PALETTE_SIZE];         // 0x00010-0x0003F
  unsigned char spr_tiles[SPR_TILES_SIZE];     // 0x00040-0x0203F
  unsigned char bg_tiles[BG_TILES_SIZE];       // 0x02040-0x04040
  unsigned char prg_code[PRG_CODE_SIZE];       // 0x04041-0xFFFFF
} CustomFile;

// Loads the entire file into memory and casts the buffer to a CustomFile pointer.
CustomFile * loadCustomFile(const char *filepath) {
  FILE *fp = fopen(filepath, "rb");
  if (!fp) {
    printf("Error opening file: %s\n", filepath);
    return NULL;
  }

  unsigned char *buffer = (unsigned char*) malloc(FILE_SIZE);
  if (!buffer) {
    printf("Memory allocation failed.\n");
    fclose(fp);
    return NULL;
  }

  size_t bytesRead = fread(buffer, 1, FILE_SIZE, fp);
  if (bytesRead != FILE_SIZE) {
    printf("Error reading file. Expected %d bytes but got %zu.\n", FILE_SIZE, bytesRead);
    free(buffer);
    fclose(fp);
    return NULL;
  }
  fclose(fp);

  // Cast the loaded buffer to a CustomFile pointer
  return (CustomFile *) buffer;
}

int main(void) {
  consoleDemoInit();

  printf("\n");

  if (!fatInitDefault()) {
    printf("FAT initialization failed!\n");
    while (1) {}
  }

  CustomFile *customFile = loadCustomFile("fat:/myfile.bin");
  if (!customFile) {
    printf("Failed to load custom file.\n");
    while (1) {}
  }

  IM3Environment env = m3_NewEnvironment();
  IM3Runtime runtime = m3_NewRuntime(env, 1024, NULL);
  IM3Module module;
  M3Result result;

  result = m3_ParseModule(env, &module, customFile->prg_code, 2633);
  if (result) {
    printf("Error parsing module: %s\n", result);
    free(customFile);
    while (1) {}
  }

  result = m3_LoadModule(runtime, module);
  if (result) {
    printf("Error loading module: %s\n", result);
    free(customFile);
    while (1) {}
  }

  LinkNDSFunctions(module);

  IM3Function f;
  result = m3_FindFunction(&f, runtime, "start");
  if (result) {
    printf("Function 'start' not found: %s\n", result);
    free(customFile);
    while (1) {}
  }

  ulInit(UL_INIT_ALL);
  ulInitGfx();
  ulInitText();
  result = m3_CallV(f, 10);
  if (result) {
    printf("Error calling function: %s\n", result);
    while (1) {}
  }

  free(customFile);
  return 0;
}


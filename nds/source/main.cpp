#include <nds.h>
#include <ulib/ulib.h>
#include <fat.h>
#include <stdio.h>
#include <wasm3.h>
#include <m3_env.h>
#include "nds.h"
#include <sys/stat.h>

// File layout constants.
#define FILE_SIZE         0x100000 // 1048576
#define HEADER_SIZE       0x10     // 16 (Don't know what this'll be about yet)
#define PALETTE_SIZE      0x30     // 8bits per channel * three channels (RGB) * 16 colors = 48 bytes
#define SPR_TILES_SIZE    0x2000   // 4bit/px * 8px * 8px * 256tiles = 32768bits (8192 bytes)
#define BG_TILES_SIZE     0x2000   // 4bit/px * 8px * 8px * 256tiles = 32768bits (8192 bytes)
#define METAPROG_SIZE     (HEADER_SIZE + PALETTE_SIZE + SPR_TILES_SIZE + BG_TILES_SIZE) // 16448
#define PRG_CODE_SIZE     (FILE_SIZE - METAPROG_SIZE) // 1032128

// Packed struct that exactly matches the file layout.
typedef struct __attribute__((__packed__)) {
  uint8_t header[HEADER_SIZE];           // 0x00000-0x0000F
  uint8_t palette[PALETTE_SIZE];         // 0x00010-0x0003F
  uint8_t spr_tiles[SPR_TILES_SIZE];     // 0x00040-0x0203F
  uint8_t bg_tiles[BG_TILES_SIZE];       // 0x02040-0x04040
  uint8_t prg_code[PRG_CODE_SIZE];       // 0x04041-0xFFFFF
} CustomFile;

// Loads the entire file into memory and casts the buffer to a CustomFile pointer.
CustomFile * loadCustomFile(const char *filepath, size_t *fileSize) {
  FILE *fp = fopen(filepath, "rb");
  if (!fp) {
    printf("Error opening file: %s\n", filepath);
    return NULL;
  }

  struct stat file_stats;
  if (stat(filepath, &file_stats) == -1) {
    printf("Error Reading File Stats\n");
  }
  *fileSize = file_stats.st_size;

  uint8_t *buffer = (uint8_t*) malloc(*fileSize);
  if (!buffer) {
    printf("Memory allocation failed.\n");
    fclose(fp);
    return NULL;
  }

  size_t bytesRead = fread(buffer, 1, FILE_SIZE, fp);
  if (bytesRead != *fileSize) {
    printf("Error reading file. Expected %d bytes but got %zu.\n", *fileSize, bytesRead);
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
    printf("FAT initialization failed.\n");
    while (1) {}
  }

  size_t fileSize;
  CustomFile *customFile = loadCustomFile("fat:/myfile.bin", &fileSize);

  if (!customFile) {
    printf("Failed to load file.\n");
    while (1) {}
  }

  size_t wasmSize = fileSize - METAPROG_SIZE;

  printf(" \"fat:/myfile.bin\"\n");
  printf(" File size: %d bytes\n", fileSize);
  printf(" WASM size: %d bytes\n", wasmSize);

  IM3Environment env = m3_NewEnvironment();
  IM3Runtime runtime = m3_NewRuntime(env, 1024, NULL);
  IM3Module module;
  M3Result result;

  printf("\n");
  printf(" Initialized WASM runtime\n");

  result = m3_ParseModule(env, &module, customFile->prg_code, wasmSize);
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

  printf(" Loaded WASM module\n");

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

  printf(" Started WASM program\n");
  result = m3_CallV(f, 10);
  if (result) {
    printf("Error calling function: %s\n", result);
    while (1) {}
  }

  free(customFile);
  return 0;
}


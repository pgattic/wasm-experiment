#include <nds.h>
#include <gl2d.h>
#include <dirent.h>
#include <fat.h>
#include <stdio.h>
#include <wasm3.h>
#include <m3_env.h>
#include "nds.h"
#include <sys/stat.h>
#include "cartridge.h"

IM3Function wasmInit(Cart * cart, size_t fileSize) {
  size_t wasmSize = fileSize - METAPROG_SIZE;
  printf("File size: %d bytes\n", fileSize);
  printf("WASM size: %d bytes\n", wasmSize);

  IM3Environment env = m3_NewEnvironment();
  IM3Runtime runtime = m3_NewRuntime(env, 1024, NULL);
  IM3Module module;
  M3Result result;

  printf("\n");
  printf("Initialized WASM runtime\n");

  result = m3_ParseModule(env, &module, cart->prg_code, wasmSize);
  if (result) {
    printf("Error parsing module: %s\n", result);
    free(cart);
    while (1) {}
  }

  result = m3_LoadModule(runtime, module);
  if (result) {
    printf("Error loading module: %s\n", result);
    free(cart);
    while (1) {}
  }

  LinkNDSFunctions(module);

  printf("Loaded WASM module\n");

  IM3Function f;
  result = m3_FindFunction(&f, runtime, "start");
  if (result) {
    printf("Function 'start' not found: %s\n", result);
    free(cart);
    while (1) {}
  }
  return f;
}

int browseFiles(char *targetFile) {
  // Get dir entries
  char cwd[100] = "fat:/wasmcarts/";
  DIR *dirp = opendir(cwd);
  if (dirp == NULL) {
    perror("Error");
    return 1;
  }

  char entries[10][20] = {0};
  bool is_dir[10] = {0};
  int num_dirs = 0;

  for (int i=0; i < 10; i++) {
    struct dirent *cur = readdir(dirp);
    if (cur == NULL)
      break;

    if (strlen(cur->d_name) == 0)
      break;

    strcpy(entries[i], cur->d_name);
    if (cur->d_type == DT_DIR) is_dir[i] = 1;
    num_dirs++;
  }
  closedir(dirp);

  // Select file
  consoleClear();
  int curr = 0;
  for (int i = 0; i < 10; i++) {
    printf("%s %s%s\n", (i==curr ? ">":" "), entries[i], (is_dir[i] ? "/" : " "));
  }

  while (1) {
    swiWaitForVBlank();

    scanKeys();
    bool changed = false;

    uint32_t keys_down = keysDown();
    if (keys_down & KEY_UP && curr > 0) {
      curr--;
      changed = true;
    }
    if (keys_down & KEY_DOWN && curr < num_dirs-1) {
      curr++;
      changed = true;
    }
    if (keys_down & KEY_A) {
      strcpy(targetFile, cwd);
      strcat(targetFile, entries[curr]);
      return 0;
    }

    if (changed) {
      consoleClear();
      for (int i = 0; i < 10; i++) {
        printf("%s %s%s\n", (i==curr ? ">":" "), entries[i], (is_dir[i] ? "/" : " "));
      }
    }
  }
}

int main(void) {
  consoleDemoInit();
  videoSetMode(MODE_0_3D);
  glScreen2D();

  printf("\n");

  if (!fatInitDefault()) {
    printf("FAT initialization failed.\n");
    while (1) {}
  }

  char targetFile[100] = {0};
  int browseRes = browseFiles(targetFile);
  if (browseRes) {
    while (1) {}
  }

  size_t fileSize;
  Cart *cart = loadCartridge(targetFile, &fileSize);

  if (!cart) {
    printf("Failed to load file.\n");
    while (1) {}
  }

  loadPalette(cart); // Read the cartridge's palette data

  IM3Function startF = wasmInit(cart, fileSize);

  vramSetBankA(VRAM_A_TEXTURE);
  vramSetBankE(VRAM_E_TEX_PALETTE);

  int tiles_id = loadSpriteTiles(cart);

  printf("Started WASM program\n");
  consoleClear();
  M3Result result = m3_CallV(startF, 10);
  if (result) {
    printf("Error calling function: %s\n", result);
    while (1) {}
  }

  free(cart);
  glDeleteTextures(1, &tiles_id);

  return 0;
}


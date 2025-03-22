#include <nds.h>
#include <gl2d.h>
#include <fat.h>
#include <stdio.h>
#include <wasm3.h>
#include <m3_env.h>
#include "nds.h"
#include <sys/stat.h>
#include "cartridge.h"

int main(void) {
  consoleDemoInit();

  printf("\n");

  if (!fatInitDefault()) {
    printf("FAT initialization failed.\n");
    while (1) {}
  }

  size_t fileSize;
  Cart *cart = loadCartridge("fat:/myfile.bin", &fileSize);

  if (!cart) {
    printf("Failed to load file.\n");
    while (1) {}
  }

  size_t wasmSize = fileSize - METAPROG_SIZE;
  printf("\"fat:/myfile.bin\"\n");
  printf("File size: %d bytes\n", fileSize);
  printf("WASM size: %d bytes\n", wasmSize);

  loadPalette(cart); // Read the cartridge's palette data

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

  videoSetMode(MODE_0_3D);
  glScreen2D();

  printf("Started WASM program\n");
  consoleClear();
  result = m3_CallV(f, 10);
  if (result) {
    printf("Error calling function: %s\n", result);
    while (1) {}
  }

  free(cart);
  return 0;
}


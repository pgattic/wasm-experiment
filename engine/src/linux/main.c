#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <wasm3.h>
#include <m3_env.h>
#include "../api.h"
#include "../cartridge.h"
#include "../platform.h"

void wasm_init(Cart * cart, size_t fileSize, IM3Function * setup, IM3Function * update) {
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

  link_api_functions(module);

  printf("Loaded WASM module\n");

  result = m3_FindFunction(setup, runtime, "setup");
  if (result) {
    printf("Function 'setup' not found: %s\n", result);
    free(cart);
    while (1) {}
  }

  result = m3_FindFunction(update, runtime, "update");
  if (result) {
    printf("Function 'update' not found: %s\n", result);
    free(cart);
    while (1) {}
  }
}

int main(int argc, char** argv) {
  if (argc < 2) {				// end program if no file specified
    printf("Please specify a file.\n");
    return 1;
  }

  size_t fileSize;
  Cart *cart = load_cartridge(argv[1], &fileSize);
  srand(time(NULL));

  platform_init();

  IM3Function setup;
  IM3Function update;
  wasm_init(cart, fileSize, &setup, &update);

  platform_prepare_cartridge(cart);

  printf("Started WASM program\n");
  M3Result result = m3_CallV(setup, 10);
  if (result) {
    printf("Error calling function: %s\n", result);
    while (1) {}
  }

  while (!platform_begin_frame()) {

    result = m3_CallV(update, 10);
    if (result) {
      printf("Error calling function: %s\n", result);
      while (1) {}
    }

    platform_end_frame();
  }

  free(cart);

  platform_deinit();

  return 0;
}


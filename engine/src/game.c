#include <stdint.h>
#include <wasm3.h>
#include <m3_env.h>
#include "api.h"
#include "game.h"

// Gameplay Data
Cart loaded_cartridge;
size_t cart_size;
IM3Environment cart_env;
IM3Runtime cart_runtime;
IM3Module cart_module;
IM3Function cart_setup;
IM3Function cart_update;

char* wasm_init() {
  cart_env = m3_NewEnvironment();
  cart_runtime = m3_NewRuntime(cart_env, 1024, NULL);
  cart_module;
  M3Result result;

  size_t wasm_size = cart_size - METAPROG_SIZE;
  result = m3_ParseModule(cart_env, &cart_module, loaded_cartridge.prg_code, wasm_size);
  if (result) {
    printf("Error parsing module: %s\n", result);
    return "Error parsing WASM3 module";
  }

  result = m3_LoadModule(cart_runtime, cart_module);
  if (result) {
    printf("Error loading module: %s\n", result);
    return "Error loading WASM3 module";
  }

  link_api_functions(cart_module);

  result = m3_FindFunction(&cart_setup, cart_runtime, "setup");
  if (result) {
    printf("Function 'setup' not found: %s\n", result);
    return "'setup' function not found";
  }

  result = m3_FindFunction(&cart_update, cart_runtime, "update");
  if (result) {
    printf("Function 'update' not found: %s\n", result);
    return "'update' function not found";
  }

  result = m3_CallV(cart_setup);
  if (result) {
    printf("Error calling function: %s\n", result);
    return "Error calling `setup` function";
  }

  printf("Initialized WASM runtime\n");
  return 0;
}

void wasm_deinit() {
  m3_FreeRuntime(cart_runtime);
  m3_FreeEnvironment(cart_env);
  printf("Deinitialized WASM runtime\n");
}

#include <sys/stat.h>

char* load_cartridge(char *filepath) {
  FILE* fp = fopen(filepath, "rb");
  if (!fp) {
    printf("Error opening file: %s\n", filepath);
    return "Failed to open file";
  }

  struct stat file_stats;
  if (stat(filepath, &file_stats) == -1) {
    printf("Error reading file stats\n");
    return "Failed to read file info";
  }
  if (file_stats.st_size > FILE_SIZE) {
    fclose(fp);
    return "File too large";
  }

  size_t bytes_read = fread(&loaded_cartridge, 1, file_stats.st_size, fp);
  if (bytes_read != file_stats.st_size) {
    printf("Error reading file. Expected %zu bytes but got %zu.\n", file_stats.st_size, bytes_read);
    fclose(fp);
    return "Unexpected end of file";
  }

  cart_size = file_stats.st_size;

  fclose(fp);
  return 0;
}

char* update_game() {
  M3Result result = m3_CallV(cart_update);
  if (result) {
    printf("Error calling function: %s\n", result);
    return "Error calling `update` function";
  }
}


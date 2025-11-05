#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "game.h"
#include "platform.h"
#include "string.h"
#include "f_sel.h"
#include "game_menu.h"

// Global Data
enum Screen current_screen = FILE_SELECT;
char* err_msg = 0; // Set to NULL if no error to display
bool running = true; // Set to false to quit the engine

char* startup() {
  char* result;
  result = platform_set_start_dir(fsel_path, MAX_FILE_PATH);
  if (result) return result;
  result = platform_init();
  if (result) return result;
  result = platform_init_fsel_data(fsel_path, &fsel_list);
  if (result) return result;
  return NULL;
}

int main(int argc, char* argv[]) {
  err_msg = startup();
  if (err_msg) {
    printf("Fatal error. Failed to initalize system: %s\n", err_msg);
    return 1;
  }
  if (argc > 1) {
    printf("Loading file: %s\n", argv[1]);
    err_msg = load_cartridge(argv[1]);
    if (err_msg == 0) {
      platform_prepare_cartridge();
      err_msg = wasm_init();
      current_screen = PLAYING;
    }
  }
  char* result;
  while (running) {
    platform_begin_frame();
    switch (current_screen) {
      case FILE_SELECT:
        result = update_file_select();
        if (result) {
          err_msg = result;
        }
        break;
      case PLAYING:
        result = update_game();
        if (result) { // Quit the game
          err_msg = result;
          wasm_deinit();
          current_screen = FILE_SELECT;
        }
        if (platform_menu_pressed()) {
          current_screen = GAME_MENU;
        }
        break;
      case GAME_MENU:
        update_game_menu();
        break;
    }
    platform_end_frame();
  }
  platform_deinit();
  return 0;
}


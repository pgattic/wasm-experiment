#include "main.h"
#include "game.h"
#include "platform.h"
#include "string.h"
#include "f_sel.h"
#include "game_menu.h"
#include <stdio.h>

// Global Data
enum Screen current_screen = FILE_SELECT;
char* err_msg = 0; // Set to NULL if no error to display
bool running = true; // Set to false to quit the engine

// Game Menu Data
int gm_cursor_location = 0;

int main(int argc, char* argv[]) {
  strcpy(fsel_path, FALLBACK_FILE_DIR);
  if (argc > 0) {
    // if (argc > 1) {
    //   set_fsel_path(argv[1]); // Take file select path from cart path
    // } else {
      set_fsel_path(argv[0]); // Else take it from binary location
    // }
  }
  err_msg = platform_init();
  if (err_msg) {
    printf("Fatal error. Failed to initalize system: %s\n", err_msg);
    return 1;
  }
  platform_init_fsel_data();
  if (argc > 1) {
    printf("Loading file: %s\n", argv[1]);
    err_msg = load_cartridge(argv[1]);
    if (err_msg == 0) {
      platform_prepare_cartridge();
      err_msg = wasm_init();
      current_screen = PLAYING;
    }
  }
  while (running) {
    platform_begin_frame();
    switch (current_screen) {
      case FILE_SELECT:
        char* result = update_file_select();
        if (result) {
          err_msg = result;
        }
        break;
      case PLAYING:
        err_msg = update_game();
        if (err_msg) { // Quit the game
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


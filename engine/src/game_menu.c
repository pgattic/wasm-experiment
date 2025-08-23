#include <stdint.h>
#include "main.h"
#include "platform.h"
#include "game.h"

size_t menu_cursor_location = 0;

// Menu options:
// - Resume
// - Restart
// - Quit

void update_game_menu() {
  char* result = 0;

  platform_clear_screen(1);
  // Render file select
  char progress[10];
  platform_print(8, 8, "Game Menu");
  platform_print(24, 24, "Resume Game");
  platform_print(24, 40, "Restart Game");
  platform_print(24, 56, "Quit Game");
  platform_render_char(8, 24 + menu_cursor_location * 16, '>');

  // Handle inputs
  if (platform_button_pressed(4)) { // "A" button
    switch (menu_cursor_location) {
      case 0: // Resume
        current_screen = PLAYING;
        break;
      case 1: // Restart
        wasm_deinit();
        wasm_init();
        current_screen = PLAYING;
        break;
      case 2: // Quit
        wasm_deinit();
        current_screen = FILE_SELECT;
        break;
    }
    menu_cursor_location = 0;
  }
  if (platform_button_pressed(5) || platform_menu_pressed()) { // "B" button or Menu button
    current_screen = PLAYING;
    menu_cursor_location = 0;
  }
  if (platform_button_pressed(2)) { // Up button
    if (menu_cursor_location == 0) {
      menu_cursor_location = 2;
    } else {
      menu_cursor_location--;
    }
  }
  if (platform_button_pressed(3)) { // Down button
    menu_cursor_location = (menu_cursor_location + 1) % 3;
  }
}


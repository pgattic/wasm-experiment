#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "main.h"
#include "platform.h"
#include "game.h"
#include "f_sel.h"

// I might want to figure out how to make this more dynamic
char fsel_path[MAX_FILE_PATH];
size_t fsel_cursor_location = 0;
char fsel_curr_files[MAX_DIR_ENTRIES][MAX_FILE_LENGTH];
size_t fsel_curr_files_c = 0;
bool is_dir[MAX_DIR_ENTRIES];

char* set_fsel_path(const char* path) {
  if (!path) return "No path given";

  const char *last_slash = strrchr(path, '/');
  if (!last_slash) return "Invalid path given";

  size_t dir_len = last_slash - path + 1;
  if (dir_len >= MAX_FILE_PATH) dir_len = MAX_FILE_PATH - 1;

  strncpy(fsel_path, path, dir_len);
  fsel_path[dir_len] = '\0';
  return 0;
}

#define FSEL_ROWS 17

char* update_file_select() {
  char* result = 0;

  platform_clear_screen(1);
  // Render file select
  char progress[10];
  sprintf(progress, "[%zu/%zu]", fsel_cursor_location+1, fsel_curr_files_c);
  int fsel_p_len = strlen(fsel_path);
  if (fsel_p_len > (WC_SCREEN_WIDTH / 8)) {
    platform_print(0, 0, "...");
    platform_print(24, 0, fsel_path + (fsel_p_len - (WC_SCREEN_WIDTH / 8)) + 3);
  } else {
    platform_print(0, 0, fsel_path);
  }
  platform_print(0, WC_SCREEN_HEIGHT - 16, "Press SELECT to quit");
  platform_print(WC_SCREEN_WIDTH - (strlen(progress)*8), WC_SCREEN_HEIGHT - 16, progress);
  if (err_msg) {
    platform_print(0, WC_SCREEN_HEIGHT - 8, err_msg);
  }
  platform_render_char(0, 72, '>');

  for (int i = 0; i < FSEL_ROWS; i++) {
    int f_index = fsel_cursor_location - 8 + i;
    if (f_index < 0 || f_index >= fsel_curr_files_c) {
      continue;
    }
    platform_print(16, i * 8 + 8, fsel_curr_files[f_index]);
  }

  // Handle inputs
  if (platform_button_pressed(4)) { // "A" button
    err_msg = 0;
    if (is_dir[fsel_cursor_location]) {
      if (strcmp(fsel_curr_files[fsel_cursor_location], "../") == 0) { // ".." selected, actually going up
        fsel_path[strlen(fsel_path) - 1] = '\0'; // delete '/' at end
        *(strrchr(fsel_path, '/') + 1) = '\0';
      } else {
        strcat(fsel_path, fsel_curr_files[fsel_cursor_location]);
        fsel_cursor_location = 0;
      }
      result = platform_init_fsel_data();
      if (result) return result;
    } else {
      char cart_name[256];
      strcpy(cart_name, fsel_path);
      strcat(cart_name, fsel_curr_files[fsel_cursor_location]);
      printf("Loading file: %s\n", cart_name);
      result = load_cartridge(cart_name);
      if (result) return result;
      platform_prepare_cartridge();
      result = wasm_init();
      if (result) return result;
      current_screen = PLAYING;
    }
  }
  if (platform_button_pressed(5)) { // "B" button
    if (strcmp(fsel_curr_files[0], "../") == 0) { // ".." is an option (not at root)
      fsel_path[strlen(fsel_path) - 1] = '\0'; // delete '/' at end
      *(strrchr(fsel_path, '/') + 1) = '\0';
    }
    result = platform_init_fsel_data();
    if (result) return result;
  }
  if (platform_button_pressed(0)) { // Left button
    if (fsel_cursor_location < 10) {
      fsel_cursor_location = 0;
    } else {
      fsel_cursor_location -= 10;
    }
  }
  if (platform_button_pressed(1)) { // Right button
    if (fsel_cursor_location + 10 >= fsel_curr_files_c) {
      fsel_cursor_location = fsel_curr_files_c - 1;
    } else {
      fsel_cursor_location += 10;
    }
  }
  if (platform_button_pressed(2)) { // Up button
    if (fsel_cursor_location == 0) {
      fsel_cursor_location = fsel_curr_files_c - 1;
    } else {
      fsel_cursor_location--;
    }
  }
  if (platform_button_pressed(3)) { // Down button
    fsel_cursor_location = (fsel_cursor_location + 1) % fsel_curr_files_c;
  }

  if (platform_button_pressed(11)) { // "Select" button
    running = false;
  }
  return result;
}


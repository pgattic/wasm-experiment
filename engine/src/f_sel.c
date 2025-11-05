#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "main.h"
#include "platform.h"
#include "game.h"
#include "f_sel.h"

char fsel_path[MAX_FILE_PATH];
size_t fsel_cursor_location = 0;
file_list fsel_list = {0, NULL, NULL};

// Note: this function is currently used only once, consider refactoring or removing
// char* set_fsel_path(const char* path) {
//   if (!path) return "No path given";
//
//   const char *last_slash = strrchr(path, '/');
//   if (!last_slash) return "Invalid path given";
//
//   size_t dir_len = last_slash - path + 1;
//   if (dir_len >= MAX_FILE_PATH) dir_len = MAX_FILE_PATH - 1;
//
//   strncpy(fsel_path, path, dir_len);
//   fsel_path[dir_len] = '\0';
//   return 0;
// }

size_t str_char_count(char* str, char ch) {
  size_t count = 0;
  for (int i = 0; str[i] != 0; i++) {
    if (str[i] == ch) count++;
  }
  return count;
}

bool at_root(char* path) {
  return (str_char_count(path, '/') <= 1);
}

#define FSEL_ROWS 17

char* update_file_select() {
  char* result = 0;

  platform_clear_screen(1);
  // Render file select
  char progress[16];
  sprintf(progress, "[%zu/%zu]", fsel_cursor_location+1, fsel_list.length);
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

  file_list_node* sel_line = file_list_seek(&fsel_list, fsel_cursor_location);
  file_list_node* curr_line = sel_line;
  const size_t middle_row = (FSEL_ROWS - 1) / 2; // 8. the middle value between 0 and 16 inclusive.
  if (sel_line != NULL) {
    platform_print(16, middle_row*8 + 8, curr_line->name);
    curr_line = sel_line->prev;
    for (int row = middle_row - 1; row >= 0; row--) {
      if (curr_line == NULL) {
        break;
      }
      platform_print(16, row*8 + 8, curr_line->name);
      curr_line = curr_line->prev;
    }
    curr_line = sel_line->next;
    for (size_t row = middle_row + 1; row < FSEL_ROWS; row++) {
      if (curr_line == NULL) {
        break;
      }
      platform_print(16, row*8 + 8, curr_line->name);
      curr_line = curr_line->next;
    }
  }

  // Handle inputs
  if (platform_button_pressed(4) && sel_line != NULL) { // "A" button
    err_msg = 0;
    if (sel_line->is_dir) {
      if (strcmp(sel_line->name, "../") == 0) { // ".." selected, actually going up
        fsel_path[strlen(fsel_path) - 1] = '\0'; // delete '/' at end
        *(strrchr(fsel_path, '/') + 1) = '\0';
      } else {
        strcat(fsel_path, sel_line->name);
        fsel_cursor_location = 0;
      }
      clear_file_list(&fsel_list);
      result = platform_init_fsel_data(fsel_path, &fsel_list);
      if (result) return result;
    } else {
      char cart_name[MAX_FILE_PATH];
      strcpy(cart_name, fsel_path);
      strcat(cart_name, sel_line->name);
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
    if (!at_root(fsel_path)) {
      fsel_path[strlen(fsel_path) - 1] = '\0'; // delete '/' at end
      *(strrchr(fsel_path, '/') + 1) = '\0';
      clear_file_list(&fsel_list);
      result = platform_init_fsel_data(fsel_path, &fsel_list);
      if (result) return result;
    }
  }
  if (platform_button_pressed(0)) { // Left button
    if (fsel_cursor_location < 10) {
      fsel_cursor_location = 0;
    } else {
      fsel_cursor_location -= 10;
    }
  }
  if (platform_button_pressed(1)) { // Right button
    if (fsel_cursor_location + 10 >= fsel_list.length) {
      fsel_cursor_location = fsel_list.length - 1;
    } else {
      fsel_cursor_location += 10;
    }
  }
  if (platform_button_pressed(2)) { // Up button
    if (fsel_cursor_location == 0) {
      fsel_cursor_location = fsel_list.length - 1;
    } else {
      fsel_cursor_location--;
    }
  }
  if (platform_button_pressed(3)) { // Down button
    fsel_cursor_location = (fsel_cursor_location + 1) % fsel_list.length;
  }

  if (platform_button_pressed(11)) { // "Select" button
    running = false;
  }
  return result;
}


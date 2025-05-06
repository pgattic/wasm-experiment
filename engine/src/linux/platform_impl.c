#include <stdint.h>
#include <stdio.h>
#include "graphics.h"
#include "../platform.h"

// const uint8_t control_mapping[12] = {
//   KEY_A,
//   KEY_D,
//   KEY_W,
//   KEY_S,
//   KEY_J,
//   KEY_K,
//   KEY_U,
//   KEY_I,
//   KEY_Y,
//   KEY_O,
//   KEY_ENTER,
//   KEY_L,
// };

void platform_init() {
}

void platform_begin_frame() {
}

void platform_end_frame() {
}

void platform_deinit() {
}

void platform_prepare_cartridge(Cart *c) {
  load_palette(c->palette);
}

uint32_t platform_rand() {
  printf("Rand called!\n");
  return 4; // Favorite random number
}

void platform_clear_screen(uint8_t color) {
}

void platform_set_pixel(uint8_t x, uint8_t y, uint8_t color) {
}

void platform_rect_outline(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color) {
}

void platform_rect_fill(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color) {
}

void platform_sprite(uint8_t x, uint8_t y, uint8_t sprite) {
}

void platform_tile_map(int16_t draw_x, int16_t draw_y, uint8_t map_x, uint8_t map_y, uint8_t map_w, uint8_t map_h) {
}

bool platform_button(uint8_t button) {
  printf("button called with %d\n", button);
  return false;
  // return button < 12 ? IsKeyDown(control_mapping[button]) : false;
}

bool platform_button_pressed(uint8_t button) {
  printf("buttonP called with %d\n", button);
  return false;
  // return button < 12 ? IsKeyPressed(control_mapping[button]) : false;
}

void platform_print_line(const char *text) {
  printf("%s\n", text);
}

int platform_select_file(char *targetFile) {
}


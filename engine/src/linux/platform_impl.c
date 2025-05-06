#include <SDL3/SDL_scancode.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
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

SDL_Window *window;
SDL_Renderer *renderer;

int platform_init() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
    return 1;
  }

  // Create a window
  window = SDL_CreateWindow("SDL3 Port", WC_SCREEN_WIDTH, WC_SCREEN_HEIGHT, 0);

  if (window == nullptr) {
    fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  renderer = SDL_CreateRenderer(window, NULL);
  if (!renderer) {
    SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }
  SDL_SetRenderVSync(renderer, 1);  // 1 = enable vsync
  return 0;
}


#define NUM_KEYS 12

bool sdl_held_keys[NUM_KEYS] = {0};
bool sdl_pressed_keys[NUM_KEYS] = {0};

  // nds_held_keys[0] = (held & KEY_LEFT) != 0;
  // nds_held_keys[1] = (held & KEY_RIGHT) != 0;
  // nds_held_keys[2] = (held & KEY_UP) != 0;
  // nds_held_keys[3] = (held & KEY_DOWN) != 0;
  // nds_held_keys[4] = (held & KEY_A) != 0;
  // nds_held_keys[5] = (held & KEY_B) != 0;
  // nds_held_keys[6] = (held & KEY_X) != 0;
  // nds_held_keys[7] = (held & KEY_Y) != 0;
  // nds_held_keys[8] = (held & KEY_L) != 0;
  // nds_held_keys[9] = (held & KEY_R) != 0;
  // nds_held_keys[10] = (held & KEY_START) != 0;
  // nds_held_keys[11] = (held & KEY_SELECT) != 0;
  //
  // nds_pressed_keys[0] = (pressed & KEY_LEFT) != 0;
  // nds_pressed_keys[1] = (pressed & KEY_RIGHT) != 0;
  // nds_pressed_keys[2] = (pressed & KEY_UP) != 0;
  // nds_pressed_keys[3] = (pressed & KEY_DOWN) != 0;
  // nds_pressed_keys[4] = (pressed & KEY_A) != 0;
  // nds_pressed_keys[5] = (pressed & KEY_B) != 0;
  // nds_pressed_keys[6] = (pressed & KEY_X) != 0;
  // nds_pressed_keys[7] = (pressed & KEY_Y) != 0;
  // nds_pressed_keys[8] = (pressed & KEY_L) != 0;
  // nds_pressed_keys[9] = (pressed & KEY_R) != 0;
  // nds_pressed_keys[10] = (pressed & KEY_START) != 0;
  // nds_pressed_keys[11] = (pressed & KEY_SELECT) != 0;

int platform_begin_frame() {
  // Clear just-pressed keys
  for (int i = 0; i < NUM_KEYS; i++) {
    sdl_pressed_keys[i] = false;
  }

  // Go through events
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) {
      return 1;
    }
    if (event.type == SDL_EVENT_KEY_DOWN) {
      SDL_Scancode sc = event.key.scancode;
      if (sc == SDL_SCANCODE_LEFT) { sdl_held_keys[0] = true; sdl_pressed_keys[0] = true; }
      else if (sc == SDL_SCANCODE_RIGHT) {  sdl_held_keys[1] = true; sdl_pressed_keys[1] = true; }
      else if (sc == SDL_SCANCODE_UP) {     sdl_held_keys[2] = true; sdl_pressed_keys[2] = true; }
      else if (sc == SDL_SCANCODE_DOWN) {   sdl_held_keys[3] = true; sdl_pressed_keys[3] = true; }
      else if (sc == SDL_SCANCODE_S) {      sdl_held_keys[4] = true; sdl_pressed_keys[4] = true; }
      else if (sc == SDL_SCANCODE_A) {      sdl_held_keys[5] = true; sdl_pressed_keys[5] = true; }
      else if (sc == SDL_SCANCODE_X) {      sdl_held_keys[6] = true; sdl_pressed_keys[6] = true; }
      else if (sc == SDL_SCANCODE_Z) {      sdl_held_keys[7] = true; sdl_pressed_keys[7] = true; }
      else if (sc == SDL_SCANCODE_Q) {      sdl_held_keys[8] = true; sdl_pressed_keys[8] = true; }
      else if (sc == SDL_SCANCODE_W) {      sdl_held_keys[9] = true; sdl_pressed_keys[9] = true; }
      else if (sc == SDL_SCANCODE_KP_ENTER){sdl_held_keys[10] = true; sdl_pressed_keys[10] = true; }
    }
    if (event.type == SDL_EVENT_KEY_UP) {
      SDL_Scancode sc = event.key.scancode;
      if (sc == SDL_SCANCODE_LEFT) { sdl_held_keys[0] = false; }
      else if (sc == SDL_SCANCODE_RIGHT) {  sdl_held_keys[1] = false; }
      else if (sc == SDL_SCANCODE_UP) {     sdl_held_keys[2] = false; }
      else if (sc == SDL_SCANCODE_DOWN) {   sdl_held_keys[3] = false; }
      else if (sc == SDL_SCANCODE_S) {      sdl_held_keys[4] = false; }
      else if (sc == SDL_SCANCODE_A) {      sdl_held_keys[5] = false; }
      else if (sc == SDL_SCANCODE_X) {      sdl_held_keys[6] = false; }
      else if (sc == SDL_SCANCODE_Z) {      sdl_held_keys[7] = false; }
      else if (sc == SDL_SCANCODE_Q) {      sdl_held_keys[8] = false; }
      else if (sc == SDL_SCANCODE_W) {      sdl_held_keys[9] = false; }
      else if (sc == SDL_SCANCODE_KP_ENTER){sdl_held_keys[10] = false; }
    }
  }
}

void platform_end_frame() {
  SDL_RenderPresent(renderer);
}

void platform_deinit() {
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void platform_prepare_cartridge(Cart *c) {
  load_palette(c->palette);
}

uint32_t platform_rand() {
  return rand();
}

void platform_clear_screen(uint8_t color) {
  Uint8 r = sdl_colors[color].r;
  Uint8 g = sdl_colors[color].g;
  Uint8 b = sdl_colors[color].b;
  SDL_SetRenderDrawColor(renderer, r, g, b, 255);
  SDL_RenderClear(renderer);
}

void platform_set_pixel(uint8_t x, uint8_t y, uint8_t color) {
}

void platform_rect_outline(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color) {
  SDL_FRect rect = {
    .x = (float)x,
    .y = (float)y,
    .w = (float)width,
    .h = (float)height
  };
  Uint8 r = sdl_colors[color].r;
  Uint8 g = sdl_colors[color].g;
  Uint8 b = sdl_colors[color].b;
  SDL_SetRenderDrawColor(renderer, r, g, b, 255);
  SDL_RenderRect(renderer, &rect);
}

void platform_rect_fill(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color) {
  SDL_FRect rect = {
    .x = (float)x,
    .y = (float)y,
    .w = (float)width,
    .h = (float)height
  };
  Uint8 r = sdl_colors[color].r;
  Uint8 g = sdl_colors[color].g;
  Uint8 b = sdl_colors[color].b;
  SDL_SetRenderDrawColor(renderer, r, g, b, 255);
  SDL_RenderFillRect(renderer, &rect);
}

void platform_sprite(uint8_t x, uint8_t y, uint8_t sprite) {
  SDL_FRect rect = {
    .x = (float)x,
    .y = (float)y,
    .w = (float)8,
    .h = (float)8
  };
  Uint8 r = 255;
  Uint8 g = 255;
  Uint8 b = 255;
  SDL_SetRenderDrawColor(renderer, r, g, b, 255);
  SDL_RenderFillRect(renderer, &rect);
}

void platform_tile_map(int16_t draw_x, int16_t draw_y, uint8_t map_x, uint8_t map_y, uint8_t map_w, uint8_t map_h) {
}

bool platform_button(uint8_t button) {
  return button < 12 ? sdl_held_keys[button] : false;
}

bool platform_button_pressed(uint8_t button) {
  return button < 12 ? sdl_pressed_keys[button] : false;
}

void platform_print_line(const char *text) {
  printf("%s\n", text);
}

int platform_select_file(char *targetFile) {
}


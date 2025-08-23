#include "m3_env.h"
#include <stdint.h>
#include "platform.h"

// NOTE: WebAssembly passes all integers as little-endian 32-bit values.
// On big-endian hosts (Wii/PowerPC), reading into uint8_t/bool directly will
// yield wrong values. Always read as 32-bit and cast locally.

m3ApiRawFunction(api_rand) {
  m3ApiReturnType(uint32_t);
  m3ApiReturn(platform_rand());
};

m3ApiRawFunction(api_clearScreen) {
  m3ApiGetArg(uint32_t, color32);
  platform_clear_screen((uint8_t)color32);
  m3ApiSuccess();
}

m3ApiRawFunction(api_pSet) {
  m3ApiGetArg(int32_t,  x);
  m3ApiGetArg(int32_t,  y);
  m3ApiGetArg(uint32_t, color32);
  platform_set_pixel(x, y, (uint8_t)color32);
  m3ApiSuccess();
}

m3ApiRawFunction(api_rect) {
  m3ApiGetArg(int32_t,  x);
  m3ApiGetArg(int32_t,  y);
  m3ApiGetArg(uint32_t, width);
  m3ApiGetArg(uint32_t, height);
  m3ApiGetArg(uint32_t, color32);
  platform_rect_outline(x, y, width, height, (uint8_t)color32);
  m3ApiSuccess();
}

m3ApiRawFunction(api_rectFill) {
  m3ApiGetArg(int32_t,  x);
  m3ApiGetArg(int32_t,  y);
  m3ApiGetArg(uint32_t, width);
  m3ApiGetArg(uint32_t, height);
  m3ApiGetArg(uint32_t, color32);
  platform_rect_fill(x, y, width, height, (uint8_t)color32);
  m3ApiSuccess();
}

m3ApiRawFunction(api_sprite) {
  m3ApiGetArg(int32_t,  x);
  m3ApiGetArg(int32_t,  y);
  m3ApiGetArg(uint32_t, sprite32);
  platform_sprite(x, y, (uint8_t)sprite32);
  m3ApiSuccess();
}

m3ApiRawFunction(api_showChar) {
  m3ApiGetArg(int32_t,  x);
  m3ApiGetArg(int32_t,  y);
  m3ApiGetArg(uint32_t, ch32);
  platform_render_char(x, y, (unsigned char)ch32);
  m3ApiSuccess();
}

m3ApiRawFunction(api_print) {
  m3ApiGetArg(int32_t, x);
  m3ApiGetArg(int32_t, y);
  m3ApiGetArgMem(char *, text);
  platform_print(x, y, text);
  m3ApiSuccess();
}

m3ApiRawFunction(api_tileMap) {
  m3ApiGetArg(int32_t,  draw_x);
  m3ApiGetArg(int32_t,  draw_y);
  m3ApiGetArg(uint32_t, map_x32);
  m3ApiGetArg(uint32_t, map_y32);
  m3ApiGetArg(uint32_t, map_w32);
  m3ApiGetArg(uint32_t, map_h32);

  platform_tile_map(draw_x, draw_y,
                    (uint8_t)map_x32, (uint8_t)map_y32,
                    (uint8_t)map_w32, (uint8_t)map_h32);
  m3ApiSuccess();
}

m3ApiRawFunction(api_btn) {
  m3ApiReturnType(uint32_t);
  m3ApiGetArg(uint32_t, button32);
  m3ApiReturn(platform_button((uint8_t)button32) ? 1u : 0u);
}

m3ApiRawFunction(api_btnP) {
  m3ApiReturnType(uint32_t);
  m3ApiGetArg(uint32_t, button32);
  m3ApiReturn(platform_button_pressed((uint8_t)button32) ? 1u : 0u);
}

m3ApiRawFunction(api_printDbg) {
  m3ApiGetArgMem(const char *, text);
  platform_print_line(text);
  m3ApiSuccess();
}

// Hook all the engine-relevant functions declared here into the WASM module
void link_api_functions(IM3Module module) {
  m3_LinkRawFunction(module, "env", "_rand",       "i()",      &api_rand);
  m3_LinkRawFunction(module, "env", "_clearScreen","v(i)",     &api_clearScreen);
  m3_LinkRawFunction(module, "env", "_pSet",       "v(iii)",   &api_pSet);
  m3_LinkRawFunction(module, "env", "_rect",       "v(iiiii)", &api_rect);
  m3_LinkRawFunction(module, "env", "_rectFill",   "v(iiiii)", &api_rectFill);
  m3_LinkRawFunction(module, "env", "_sprite",     "v(iii)",   &api_sprite);
  m3_LinkRawFunction(module, "env", "_showChar",   "v(iii)",   &api_showChar);
  m3_LinkRawFunction(module, "env", "_print",      "v(iii)",   &api_print);
  m3_LinkRawFunction(module, "env", "_tileMap",    "v(iiiiii)",&api_tileMap);
  m3_LinkRawFunction(module, "env", "_btn",        "i(i)",     &api_btn);
  m3_LinkRawFunction(module, "env", "_btnP",       "i(i)",     &api_btnP);
  m3_LinkRawFunction(module, "env", "_printLnDbg", "v(i)",     &api_printDbg);
}


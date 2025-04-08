#include "m3_env.h"
#include <stdint.h>
#include "platform.h"

m3ApiRawFunction(nds_rand) {
  m3ApiReturnType(uint32_t)
  m3ApiReturn(platform_rand());
};

m3ApiRawFunction(gl_clearScreen) {
  m3ApiGetArg(uint8_t, color);

  platform_clear_screen(color);
  m3ApiSuccess();
}

m3ApiRawFunction(gl_pSet) {
  m3ApiGetArg(uint8_t, x);
  m3ApiGetArg(uint8_t, y);
  m3ApiGetArg(uint8_t, color);

  platform_set_pixel(x, y, color);
  m3ApiSuccess();
}

m3ApiRawFunction(gl_rect) {
  m3ApiGetArg(uint8_t, x);
  m3ApiGetArg(uint8_t, y);
  m3ApiGetArg(uint8_t, width);
  m3ApiGetArg(uint8_t, height);
  m3ApiGetArg(uint8_t, color);

  platform_rect_outline(x, y, width, height, color);
  m3ApiSuccess();
}

m3ApiRawFunction(gl_rectFill) {
  m3ApiGetArg(uint8_t, x);
  m3ApiGetArg(uint8_t, y);
  m3ApiGetArg(uint8_t, width);
  m3ApiGetArg(uint8_t, height);
  m3ApiGetArg(uint8_t, color);

  platform_rect_fill(x, y, width, height, color);
  m3ApiSuccess();
}

m3ApiRawFunction(gl_sprite) {
  m3ApiGetArg(uint8_t, x);
  m3ApiGetArg(uint8_t, y);
  m3ApiGetArg(uint8_t, sprite);

  platform_sprite(x, y, sprite);
  m3ApiSuccess();
}

m3ApiRawFunction(draw_sample_bg) {
  // // TEST rendering a map
  // for (int y = 0; y < (192/8); y++) {
  //   for (int x = 0; x < (256/8); x++) {
  //     uint8_t tileId = gameMemory.tilemap[y*32 + x];
  //     glSprite(x*8, y*8, GL_FLIP_NONE, &sprTiles[tileId]);
  //   }
  // }

  m3ApiSuccess();
}

m3ApiRawFunction(nds_btn) {
  m3ApiReturnType(bool);
  m3ApiGetArg(uint8_t, button);
  m3ApiReturn(platform_button(button));
}

m3ApiRawFunction(nds_btnP) {
  m3ApiReturnType(bool);
  m3ApiGetArg(uint8_t, button);
  m3ApiReturn(platform_button_pressed(button));
}

m3ApiRawFunction(nds_printDbg) {
    m3ApiGetArgMem(const char *, text);
    platform_print_line(text);
    m3ApiSuccess();
}

// Hook all the engine-relevant functions declared here into the WASM module
void link_api_functions(IM3Module module) {
  m3_LinkRawFunction(module, "env", "_rand", "i()", &nds_rand);
  m3_LinkRawFunction(module, "env", "_clearScreen", "v(i)", &gl_clearScreen);
  m3_LinkRawFunction(module, "env", "_pSet", "v(iii)", &gl_pSet);
  m3_LinkRawFunction(module, "env", "_rect", "v(iiiii)", &gl_rect);
  m3_LinkRawFunction(module, "env", "_rectFill", "v(iiiii)", &gl_rectFill);
  m3_LinkRawFunction(module, "env", "_sprite", "v(iii)", &gl_sprite);
  m3_LinkRawFunction(module, "env", "_sampleBg", "v()", &draw_sample_bg);
  m3_LinkRawFunction(module, "env", "_btn", "i(i)", &nds_btn);
  m3_LinkRawFunction(module, "env", "_btnP", "i(i)", &nds_btnP);
  m3_LinkRawFunction(module, "env", "_printLnDbg", "v(i)", &nds_printDbg);
}


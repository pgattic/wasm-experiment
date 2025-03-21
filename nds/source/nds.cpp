#include "m3_env.h"
#include <nds.h>
#include <gl2d.h>

m3ApiRawFunction(m3_rand) {
  m3ApiReturnType (uint32_t)
  m3ApiReturn(rand());
};

bool ndsHeldKeys[8] = {0};
bool ndsPressedKeys[8] = {0};

uint32_t palette[16] = { // PICO-8 palette
  RGB15(  0 >> 3,   0 >> 3,   0 >> 3),
  RGB15( 29 >> 3,  43 >> 3,  83 >> 3),
  RGB15(126 >> 3,  37 >> 3,  83 >> 3),
  RGB15(  0 >> 3, 135 >> 3,  81 >> 3),
  RGB15(171 >> 3,  82 >> 3,  54 >> 3),
  RGB15( 95 >> 3,  87 >> 3,  79 >> 3),
  RGB15(194 >> 3, 195 >> 3, 199 >> 3),
  RGB15(255 >> 3, 241 >> 3, 232 >> 3),
  RGB15(255 >> 3,   0 >> 3,  77 >> 3),
  RGB15(255 >> 3, 163 >> 3,   0 >> 3),
  RGB15(255 >> 3, 236 >> 3,  39 >> 3),
  RGB15(  0 >> 3, 228 >> 3,  54 >> 3),
  RGB15( 41 >> 3, 173 >> 3, 255 >> 3),
  RGB15(131 >> 3, 118 >> 3, 156 >> 3),
  RGB15(255 >> 3, 119 >> 3, 168 >> 3),
  RGB15(255 >> 3, 204 >> 3, 170 >> 3),
};

void collectKeys() {
  scanKeys();

  uint16_t held = keysHeld();
  uint16_t pressed = keysDown();

  ndsHeldKeys[0] = (held & KEY_LEFT) != 0;
  ndsHeldKeys[1] = (held & KEY_RIGHT) != 0;
  ndsHeldKeys[2] = (held & KEY_UP) != 0;
  ndsHeldKeys[3] = (held & KEY_DOWN) != 0;
  ndsHeldKeys[4] = (held & KEY_A) != 0;
  ndsHeldKeys[5] = (held & KEY_B) != 0;
  ndsHeldKeys[6] = (held & KEY_X) != 0;
  ndsHeldKeys[7] = (held & KEY_Y) != 0;

  ndsPressedKeys[0] = (pressed & KEY_LEFT) != 0;
  ndsPressedKeys[1] = (pressed & KEY_RIGHT) != 0;
  ndsPressedKeys[2] = (pressed & KEY_UP) != 0;
  ndsPressedKeys[3] = (pressed & KEY_DOWN) != 0;
  ndsPressedKeys[4] = (pressed & KEY_A) != 0;
  ndsPressedKeys[5] = (pressed & KEY_B) != 0;
  ndsPressedKeys[6] = (pressed & KEY_X) != 0;
  ndsPressedKeys[7] = (pressed & KEY_Y) != 0;
}

m3ApiRawFunction(gl_pSet) {
  m3ApiGetArg(uint8_t, x);
  m3ApiGetArg(uint8_t, y);
  m3ApiGetArg(uint8_t, c);

  glPutPixel(x, y, palette[c]);
  m3ApiSuccess();
}

m3ApiRawFunction(gl_rect) {
  m3ApiGetArg(uint8_t, x);
  m3ApiGetArg(uint8_t, y);
  m3ApiGetArg(uint8_t, w);
  m3ApiGetArg(uint8_t, h);
  m3ApiGetArg(uint8_t, c);

  glBox(x, y, x + w - 2, y + h - 2, palette[c]);
  m3ApiSuccess();
}

m3ApiRawFunction(gl_rectFill) {
  m3ApiGetArg(uint8_t, x);
  m3ApiGetArg(uint8_t, y);
  m3ApiGetArg(uint8_t, w);
  m3ApiGetArg(uint8_t, h);
  m3ApiGetArg(uint8_t, c);

  glBoxFilled(x, y, x + w - 1, y + h - 1, palette[c]);
  m3ApiSuccess();
}

m3ApiRawFunction(gl_syncFrame) {
  glEnd2D();
  glFlush(0);
  swiWaitForVBlank();
  /*consoleClear();*/
  collectKeys();
  glBegin2D();
  m3ApiSuccess();
}

m3ApiRawFunction(gl_btn) {
  m3ApiReturnType(bool);
  m3ApiGetArg(uint8_t, btn);
  m3ApiReturn(btn > 7 ? 0 : ndsHeldKeys[btn]);
}

m3ApiRawFunction(gl_btnP) {
  m3ApiReturnType(bool);
  m3ApiGetArg(uint8_t, btn);
  m3ApiReturn(btn > 7 ? 0 : ndsPressedKeys[btn]);
}

// Hook all the engine-relevant functions declared here into the WASM module
void LinkNDSFunctions(IM3Module module) {
  m3_LinkRawFunction (module, "env", "_rand", "i()", &m3_rand);
  m3_LinkRawFunction (module, "env", "_pSet", "v(iii)", &gl_pSet);
  m3_LinkRawFunction (module, "env", "_rect", "v(iiiii)", &gl_rect);
  m3_LinkRawFunction (module, "env", "_rectFill", "v(iiiii)", &gl_rectFill);
  m3_LinkRawFunction (module, "env", "_syncFrame", "v()", &gl_syncFrame);
  m3_LinkRawFunction (module, "env", "_btn", "i(i)", &gl_btn);
  m3_LinkRawFunction (module, "env", "_btnP", "i(i)", &gl_btnP);
}


#include "m3_env.h"
#include <nds.h>
#include <gl2d.h>
#include <stdint.h>
#include "cartridge.h"

uint32_t palette[16] = {0};

/// Loads a Palette from the game's cartridge data
void loadPalette(Cart * cart) {
  for (uint8_t i = 0; i < 16; i++) {
    palette[i] = RGB15(cart->palette[i*3] >> 3,
                       cart->palette[i*3+1] >> 3,
                       cart->palette[i*3+2] >> 3);
  }
}

bool ndsHeldKeys[8] = {0};
bool ndsPressedKeys[8] = {0};

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

m3ApiRawFunction(nds_rand) {
  m3ApiReturnType (uint32_t)
  m3ApiReturn(rand());
};

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
  // "End of frame" stuff
  glEnd2D();
  glFlush(0);

  // "Beginning of frame" stuff
  collectKeys();
  glBegin2D();
  m3ApiSuccess();
}

m3ApiRawFunction(nds_btn) {
  m3ApiReturnType(bool);
  m3ApiGetArg(uint8_t, btn);
  m3ApiReturn(btn > 7 ? 0 : ndsHeldKeys[btn]);
}

m3ApiRawFunction(nds_btnP) {
  m3ApiReturnType(bool);
  m3ApiGetArg(uint8_t, btn);
  m3ApiReturn(btn > 7 ? 0 : ndsPressedKeys[btn]);
}

m3ApiRawFunction(nds_printDbg) {
    m3ApiGetArgMem (const char *, str)
    printf("%s\n", str);
    m3ApiSuccess();
}

// Hook all the engine-relevant functions declared here into the WASM module
void LinkNDSFunctions(IM3Module module) {
  m3_LinkRawFunction (module, "env", "_rand", "i()", &nds_rand);
  m3_LinkRawFunction (module, "env", "_pSet", "v(iii)", &gl_pSet);
  m3_LinkRawFunction (module, "env", "_rect", "v(iiiii)", &gl_rect);
  m3_LinkRawFunction (module, "env", "_rectFill", "v(iiiii)", &gl_rectFill);
  m3_LinkRawFunction (module, "env", "_syncFrame", "v()", &gl_syncFrame);
  m3_LinkRawFunction (module, "env", "_btn", "i(i)", &nds_btn);
  m3_LinkRawFunction (module, "env", "_btnP", "i(i)", &nds_btnP);
  m3_LinkRawFunction (module, "env", "_printLnDbg", "v(i)", &nds_printDbg);
}


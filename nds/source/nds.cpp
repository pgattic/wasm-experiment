#include "m3_env.h"
#include <nds.h>
#include <ulib/ulib.h>

m3ApiRawFunction(m3_rand) {
  m3ApiReturnType (uint32_t)
  m3ApiReturn(rand());
};

/// An abstract machine that automatically invokes the relevant start- and end-draw functions when
/// the drawing is started/done

bool isDrawing = 0;

void setDraw() {
  if (!isDrawing) {
    isDrawing = 1;
    ulStartDrawing2D();
  }
}

void clearDraw() {
  if (isDrawing) {
    isDrawing = 0;
    ulEndDrawing();
  }
}

bool ndsHeldKeys[8] = {0};
bool ndsPressedKeys[8] = {0};

UL_COLOR palette[16] = { // PICO-8 palette
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
  ulReadKeys(0);

  ndsHeldKeys[0] = ul_keys.held.left;
  ndsHeldKeys[1] = ul_keys.held.right;
  ndsHeldKeys[2] = ul_keys.held.up;
  ndsHeldKeys[3] = ul_keys.held.down;
  ndsHeldKeys[4] = ul_keys.held.A;
  ndsHeldKeys[5] = ul_keys.held.B;
  ndsHeldKeys[6] = ul_keys.held.X;
  ndsHeldKeys[7] = ul_keys.held.Y;

  ndsPressedKeys[0] = ul_keys.pressed.left;
  ndsPressedKeys[1] = ul_keys.pressed.right;
  ndsPressedKeys[2] = ul_keys.pressed.up;
  ndsPressedKeys[3] = ul_keys.pressed.down;
  ndsPressedKeys[4] = ul_keys.pressed.A;
  ndsPressedKeys[5] = ul_keys.pressed.B;
  ndsPressedKeys[6] = ul_keys.pressed.X;
  ndsPressedKeys[7] = ul_keys.pressed.Y;
}

// ulib abstractions

m3ApiRawFunction(ulib_rect) {
  m3ApiGetArg(uint8_t, x);
  m3ApiGetArg(uint8_t, y);
  m3ApiGetArg(uint8_t, w);
  m3ApiGetArg(uint8_t, h);
  m3ApiGetArg(uint8_t, c);

  setDraw();
  ulDrawRect(x, y, x + w, y + h, palette[c]);
  m3ApiSuccess();
}

m3ApiRawFunction(ulib_fillRect) {
  m3ApiGetArg(uint8_t, x);
  m3ApiGetArg(uint8_t, y);
  m3ApiGetArg(uint8_t, w);
  m3ApiGetArg(uint8_t, h);
  m3ApiGetArg(uint8_t, c);

  setDraw();
  ulDrawFillRect(x, y, x + w, y + h, palette[c]);
  m3ApiSuccess();
}

m3ApiRawFunction(ulib_syncFrame) {
  clearDraw();
  ulSyncFrame();
  collectKeys();
  m3ApiSuccess();
}

m3ApiRawFunction(ulib_btn) {
  m3ApiReturnType(bool);
  m3ApiGetArg(uint8_t, btn);
  m3ApiReturn(btn > 7 ? 0 : ndsHeldKeys[btn]);
}

m3ApiRawFunction(ulib_btnp) {
  m3ApiReturnType(bool);
  m3ApiGetArg(uint8_t, btnp);
  m3ApiReturn(btnp > 7 ? 0 : ndsPressedKeys[btnp]);
}

// Hook all the engine-relevant functions declared here into the WASM module
void LinkNDSFunctions(IM3Module module) {
  m3_LinkRawFunction (module, "env", "_rand", "i()", &m3_rand);
  m3_LinkRawFunction (module, "env", "_rect", "v(iiiii)", &ulib_rect);
  m3_LinkRawFunction (module, "env", "_fillRect", "v(iiiii)", &ulib_fillRect);
  m3_LinkRawFunction (module, "env", "_syncFrame", "v()", &ulib_syncFrame);
  m3_LinkRawFunction (module, "env", "_btn", "i(i)", &ulib_btn);
  m3_LinkRawFunction (module, "env", "_btnp", "i(i)", &ulib_btnp);
}


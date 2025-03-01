#include "m3_env.h"
#include <nds.h>
#include <ulib/ulib.h>

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

bool ndsHeldKeys[4] = {0};
bool ndsPressedKeys[4] = {0};

void collectKeys() {
  ulReadKeys(0);

  ndsHeldKeys[0] = ul_keys.held.left;
  ndsHeldKeys[1] = ul_keys.held.right;
  ndsHeldKeys[2] = ul_keys.held.up;
  ndsHeldKeys[3] = ul_keys.held.down;

  ndsPressedKeys[0] = ul_keys.pressed.left;
  ndsPressedKeys[1] = ul_keys.pressed.right;
  ndsPressedKeys[2] = ul_keys.pressed.up;
  ndsPressedKeys[3] = ul_keys.pressed.down;
}

m3ApiRawFunction(m3_swiWaitForVBlank) {
  swiWaitForVBlank();
  m3ApiSuccess();
}

m3ApiRawFunction(_print) {
  m3ApiGetArgMem (const char *, str)
  printf(str);
  m3ApiSuccess();
}

m3ApiRawFunction(m3_consoleDemoInit) {
  consoleDemoInit();
  m3ApiSuccess();
};

m3ApiRawFunction(m3_consoleClear) {
  consoleClear();
  m3ApiSuccess();
};

m3ApiRawFunction(m3_oamUpdate) {
  oamUpdate(&oamMain);
  m3ApiSuccess();
}

m3ApiRawFunction(m3_keysHeld) {
  m3ApiReturnType (uint32_t)
  m3ApiReturn(keysHeld());
};

m3ApiRawFunction(m3_rand) {
  m3ApiReturnType (uint32_t)
  m3ApiReturn(rand());
};

m3ApiRawFunction(m3_scanKeys) {
  scanKeys();
  m3ApiSuccess();
};

m3ApiRawFunction(ulib_fillRedRect) {
  m3ApiGetArg(uint8_t, x);
  m3ApiGetArg(uint8_t, y);
  m3ApiGetArg(uint8_t, w);
  m3ApiGetArg(uint8_t, h);

  setDraw();
  ulDrawFillRect(x, y, x + w, y + h, RGB15(31, 0, 0));
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
  m3ApiReturn(ndsHeldKeys[btn]);
}

m3ApiRawFunction(ulib_btnp) {
  m3ApiReturnType(bool);
  m3ApiGetArg(uint8_t, btnp);
  m3ApiReturn(ndsPressedKeys[btnp]);
}

void LinkNDSFunctions(IM3Module module) {
  m3_LinkRawFunction (module, "nds", "swiWaitForVBlank", "v()", &m3_swiWaitForVBlank);
  m3_LinkRawFunction (module, "nds", "_print", "v(i)", &_print);
  m3_LinkRawFunction (module, "nds", "consoleDemoInit", "v()", &m3_consoleDemoInit);
  m3_LinkRawFunction (module, "nds", "consoleClear", "v()", &m3_consoleClear);
  m3_LinkRawFunction (module, "nds", "oamUpdate", "v()", &m3_oamUpdate);
  m3_LinkRawFunction (module, "nds", "keysHeld", "i()", &m3_keysHeld);
  m3_LinkRawFunction (module, "nds", "scanKeys", "v()", &m3_scanKeys);
  m3_LinkRawFunction (module, "nds", "rand", "i()", &m3_rand);

  /*m3_LinkRawFunction (module, "nds", "ulibInit", "v()", &ulib_gfx_init);*/
  /*m3_LinkRawFunction (module, "nds", "ulibStartDraw", "v()", &ulib_startDraw);*/
  /*m3_LinkRawFunction (module, "nds", "ulibEndDraw", "v()", &ulib_endDraw);*/
  m3_LinkRawFunction (module, "nds", "_fillRedRect", "v(iiii)", &ulib_fillRedRect);
  m3_LinkRawFunction (module, "nds", "syncFrame", "v()", &ulib_syncFrame);
  m3_LinkRawFunction (module, "nds", "_btn", "i(i)", &ulib_btn);
  m3_LinkRawFunction (module, "nds", "_btnp", "i(i)", &ulib_btnp);
}


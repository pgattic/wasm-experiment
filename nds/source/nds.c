#include "m3_env.h"
#include <nds.h>
#include <gl2d.h>
#include <stdint.h>
#include "cartridge.h"
#include "memory.h"

#define NDS_SC_W 256
#define NDS_SC_H 192

/*#define GAME_SC_W 160*/
/*#define GAME_SC_H 144*/

/*#define SCREEN_SCALE 2*/
/*#define LEFT_MARGIN ((NDS_SC_W - (GAME_SC_W * SCREEN_SCALE)) / 2) // 8*/
/*#define TOP_MARGIN ((NDS_SC_H - (GAME_SC_H * SCREEN_SCALE)) / 2) // 16*/

#define MAP_WIDTH 32
#define MAP_HEIGHT 32

glImage sprTiles[256];

int loadSpriteTiles(Cart * cart) {
  /*int textureID = glLoadTileSet(*/
  /*  sprTiles,    // Pointer to glImage array*/
  /*  8, 8,        // Sprite Dimensions*/
  /*  8, 8*128,       // Bitmap Dimensions (the part that contains useful images)*/
  /*  GL_RGB16,    // Texture type for glTexImage2D()*/
  /*  8, 8*128,        // Full texture (image) dimensions*/
  /*  TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT, // Parameters for glTexImage2D()*/
  /*  16,          // Length of the palette to use (# of colors)*/
  /*  &nds_palette, // Pointer to texture palette data*/
  /*  cart->spr_tiles // Pointer to texture data*/
  /*);*/

  int textureID;
  int numTiles = 128; // Should be 256 but currently only works with 128 (TODO figure out 256 tiles)
  glGenTextures( 1, &textureID );
  glBindTexture( 0, textureID );
  glTexImage2D( 0, 0, GL_RGB16, 8, 8*numTiles, 0, TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT, cart->spr_tiles );
  glColorTableEXT( 0, 0, 16, 0, 0, &ndsPalette );

  // init sprites texture coords and texture ID
  for (int i = 0; i < numTiles; i++) {
    sprTiles[i].width        = 8;
    sprTiles[i].height       = 8;
    sprTiles[i].u_off        = 0;
    sprTiles[i].v_off        = i*8; 
    sprTiles[i].textureID    = textureID;
  }

  if (textureID < 0)
    printf("Failed to load texture: %d\n", textureID);
  return textureID;
}

bool ndsHeldKeys[12] = {0};
bool ndsPressedKeys[12] = {0};

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
  ndsHeldKeys[8] = (held & KEY_L) != 0;
  ndsHeldKeys[9] = (held & KEY_R) != 0;
  ndsHeldKeys[10] = (held & KEY_START) != 0;
  ndsHeldKeys[11] = (held & KEY_SELECT) != 0;

  ndsPressedKeys[0] = (pressed & KEY_LEFT) != 0;
  ndsPressedKeys[1] = (pressed & KEY_RIGHT) != 0;
  ndsPressedKeys[2] = (pressed & KEY_UP) != 0;
  ndsPressedKeys[3] = (pressed & KEY_DOWN) != 0;
  ndsPressedKeys[4] = (pressed & KEY_A) != 0;
  ndsPressedKeys[5] = (pressed & KEY_B) != 0;
  ndsPressedKeys[6] = (pressed & KEY_X) != 0;
  ndsPressedKeys[7] = (pressed & KEY_Y) != 0;
  ndsPressedKeys[8] = (pressed & KEY_L) != 0;
  ndsPressedKeys[9] = (pressed & KEY_R) != 0;
  ndsPressedKeys[10] = (pressed & KEY_START) != 0;
  ndsPressedKeys[11] = (pressed & KEY_SELECT) != 0;
}

m3ApiRawFunction(nds_rand) {
  m3ApiReturnType (uint32_t)
  m3ApiReturn(rand());
};

m3ApiRawFunction(gl_clearScreen) {
  m3ApiGetArg(uint8_t, c);

  glBoxFilled(0, 0, 255, 191, ndsPalette[c]);
  m3ApiSuccess();
}

m3ApiRawFunction(gl_pSet) {
  m3ApiGetArg(uint8_t, x);
  m3ApiGetArg(uint8_t, y);
  m3ApiGetArg(uint8_t, c);

  glPutPixel(x, y, ndsPalette[c]);
  m3ApiSuccess();
}

m3ApiRawFunction(gl_rect) {
  m3ApiGetArg(uint8_t, x);
  m3ApiGetArg(uint8_t, y);
  m3ApiGetArg(uint8_t, w);
  m3ApiGetArg(uint8_t, h);
  m3ApiGetArg(uint8_t, c);

  glBox(x, y, x + w - 2, y + h - 2, ndsPalette[c]);
  m3ApiSuccess();
}

m3ApiRawFunction(gl_rectFill) {
  m3ApiGetArg(uint8_t, x);
  m3ApiGetArg(uint8_t, y);
  m3ApiGetArg(uint8_t, w);
  m3ApiGetArg(uint8_t, h);
  m3ApiGetArg(uint8_t, c);

  glBoxFilled(x, y, x + w - 1, y + h - 1, ndsPalette[c]);
  m3ApiSuccess();
}

m3ApiRawFunction(gl_sprite) {
  m3ApiGetArg(uint8_t, x);
  m3ApiGetArg(uint8_t, y);
  m3ApiGetArg(uint8_t, sprite);

  glSprite(x, y, GL_FLIP_NONE, &sprTiles[sprite]);
  m3ApiSuccess();
}

m3ApiRawFunction(gl_syncFrame) {
  // "End of frame" stuff
  /*glBoxFilled(GAME_SC_W, 0, NDS_SC_W - 1, NDS_SC_H - 1, nds_palette[0]);*/
  /*glBoxFilled(0, GAME_SC_H, NDS_SC_W - 1, NDS_SC_H - 1, nds_palette[0]);*/
  glEnd2D();
  glFlush(0);

  // "Beginning of frame" stuff
  oamUpdate(&oamMain);
  collectKeys();
  glBegin2D();
  m3ApiSuccess();
}

m3ApiRawFunction(draw_sample_bg) {
  // TEST rendering a map
  for (int y = 0; y < (192/8); y++) {
    for (int x = 0; x < (256/8); x++) {
      uint8_t tileId = gameMemory.tilemap[y*32 + x];
      glSprite(x*8, y*8, GL_FLIP_NONE, &sprTiles[tileId]);
    }
  }

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
  m3_LinkRawFunction (module, "env", "_clearScreen", "v(i)", &gl_clearScreen);
  m3_LinkRawFunction (module, "env", "_pSet", "v(iii)", &gl_pSet);
  m3_LinkRawFunction (module, "env", "_rect", "v(iiiii)", &gl_rect);
  m3_LinkRawFunction (module, "env", "_rectFill", "v(iiiii)", &gl_rectFill);
  m3_LinkRawFunction (module, "env", "_sprite", "v(iii)", &gl_sprite);
  m3_LinkRawFunction (module, "env", "_syncFrame", "v()", &gl_syncFrame);
  m3_LinkRawFunction (module, "env", "_sampleBg", "v()", &draw_sample_bg);
  m3_LinkRawFunction (module, "env", "_btn", "i(i)", &nds_btn);
  m3_LinkRawFunction (module, "env", "_btnP", "i(i)", &nds_btnP);
  m3_LinkRawFunction (module, "env", "_printLnDbg", "v(i)", &nds_printDbg);
}


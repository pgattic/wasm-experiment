#include <stdint.h>
#include <nds.h>
#include "graphics.h"
#include "../cartridge.h"

// Embedded files
static const uint8_t palette_file[] = {
  #embed "../../assets/palette.pal"
};
static const uint8_t font_file[] = {
  #embed "../../assets/font.4bpp"
};

uint16_t nds_palette[16]; // Keep this synched with the palette from memory
void load_palette() {
  for (uint8_t i = 0; i < 16; i++) {
    nds_palette[i] = RGB15(((palette_file)[i*3]) >> 3,
                           ((palette_file)[i*3+1]) >> 3,
                           ((palette_file)[i*3+2]) >> 3);
  }
}

glImage font_tiles[128];
int font_texture_id;

/// Please make sure `load_palette` was called prior to this
void load_font_tiles() {
  font_texture_id = glLoadTileSet(
    font_tiles,
    8, 8,
    8, 8*128,
    GL_RGB16,
    8, 8*128,
    TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT,
    16,
    &nds_palette,
    &font_file
  );
}

glImage spr_tiles[256];
int spr_texture_id;

void load_sprite_tiles(uint8_t tile_data[SPR_TILES_SIZE]) {
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

  int num_tiles = 128; // Should be 256 but currently only works with 128 (TODO figure out 256 tiles)
  glGenTextures( 1, &spr_texture_id );
  glBindTexture( 0, spr_texture_id );
  glTexImage2D( 0, 0, GL_RGB16, 8, 8 * num_tiles, 0, TEXGEN_TEXCOORD | GL_TEXTURE_COLOR0_TRANSPARENT, tile_data );
  glColorTableEXT( 0, 0, 16, 0, 0, &nds_palette );

  // init sprites texture coords and texture ID
  for (int i = 0; i < num_tiles; i++) {
    spr_tiles[i].width        = 8;
    spr_tiles[i].height       = 8;
    spr_tiles[i].u_off        = 0;
    spr_tiles[i].v_off        = i*8; 
    spr_tiles[i].textureID    = spr_texture_id;
  }

  if (spr_texture_id < 0)
    printf("Failed to load texture: %d\n", spr_texture_id);
}

void unload_graphics() {
  glDeleteTextures(1, &spr_texture_id);
}


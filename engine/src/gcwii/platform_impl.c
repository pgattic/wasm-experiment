#include <grrlib.h>
#include <gccore.h>
#include <fat.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "graphics.h"
#include "keys.h"
#include "../platform.h"
#include "../f_sel.h"
#include "../game.h"
#include "../main.h"

#define WII_SC_W 640
#define WII_SC_H 480

const char FALLBACK_FILE_DIR[256] = "sd:/";

// Offscreen target that matches engine resolution
static GRRLIB_texImg *render_target = NULL;

char* platform_init() {
  GRRLIB_Init();
  init_keys();

  GRRLIB_SetBackgroundColour(0,0,0,0xFF);

  GRRLIB_SetAntiAliasing(false);
  GXRModeObj *rmode = VIDEO_GetPreferredMode(NULL);
  GX_SetCopyFilter(GX_FALSE, rmode->sample_pattern, GX_FALSE, rmode->vfilter);
  GX_SetDither(GX_FALSE);

#ifdef PLATFORM_WII
  if (!fatInitDefault()) return "FAT initialization failed";
#else
  // TODO
#endif

  // Upload palette & font tiles
  load_palette();
  load_font_tiles();

  render_target = GRRLIB_CreateEmptyTextureFmt(WC_SCREEN_WIDTH, WC_SCREEN_HEIGHT, GX_TF_RGBA8);
  if (!render_target) return "Failed to create render_target texture";

  return 0;
}

void platform_prepare_cartridge() {
  load_sprite_tiles(loaded_cartridge.spr_tiles);
}

void platform_begin_frame() {
  // Input first
  collect_keys();

  // Begin offscreen (EFB) composition pass for the *game area only*
  // Everything drawn by platform_* this frame will be captured into render_target.
  GRRLIB_CompoStart();
}

static inline int compute_scale_int(void) {
  int sx = WII_SC_W / WC_SCREEN_WIDTH;
  int sy = WII_SC_H / WC_SCREEN_HEIGHT;
  int s = (sx < sy) ? sx : sy;
  return (s < 1) ? 1 : s;
}

void platform_end_frame() {
  // End offscreen pass: copy the top-left WC sized region into render_target
  GRRLIB_CompoEnd(0, 0, render_target);

  // Now draw the composed game texture scaled & centered on the TV
  int scale = compute_scale_int();
  int dst_w = WC_SCREEN_WIDTH  * scale;
  int dst_h = WC_SCREEN_HEIGHT * scale;
  int dst_x = (WII_SC_W - dst_w) / 2;
  int dst_y = (WII_SC_H - dst_h) / 2;

  // Dim background (like SDL path), *behind* the scaled image
  uint8_t r = (wii_colors[1] >> 24) & 0xFF;
  uint8_t g = (wii_colors[1] >> 16) & 0xFF;
  uint8_t b = (wii_colors[1] >>  8) & 0xFF;
  GRRLIB_FillScreen(RGBA(r>>1, g>>1, b>>1, 0xFF));

  // Draw the offscreen texture to screen
  GRRLIB_DrawImg((f32)dst_x, (f32)dst_y, render_target, 0.0f,
                 (f32)scale, (f32)scale, 0xFFFFFFFF);

  // Present (handles vsync)
  GRRLIB_Render();
}

void platform_deinit() {
  unload_graphics();
  if (render_target) { GRRLIB_FreeTexture(render_target); render_target = NULL; }
  GRRLIB_Exit();
}

uint32_t platform_rand() { return rand(); }

void platform_clear_screen(uint8_t color) {
  // Clear within the *game* canvas (captured by CompoStart/End)
  GRRLIB_Rectangle(0.0f, 0.0f, (f32)WC_SCREEN_WIDTH, (f32)WC_SCREEN_HEIGHT, wii_colors[color], true);
}

void platform_set_pixel(int32_t x, int32_t y, uint8_t color) {
  GRRLIB_Rectangle((f32)x, (f32)y, 1.0f, 1.0f, wii_colors[color], true);
}

void platform_rect_outline(int32_t x, int32_t y, uint32_t w, uint32_t h, uint8_t color) {
  GRRLIB_Rectangle((f32)x, (f32)y, (f32)w, (f32)h, wii_colors[color], false);
}

void platform_rect_fill(int32_t x, int32_t y, uint32_t w, uint32_t h, uint8_t color) {
  GRRLIB_Rectangle((f32)x, (f32)y, (f32)w, (f32)h, wii_colors[color], true);
}

void platform_sprite(int32_t x, int32_t y, uint8_t sprite) {
  if (!spr_tileset) return;
  GRRLIB_DrawTile((f32)x, (f32)y, spr_tileset, 0.0f, 1.0f, 1.0f, 0xFFFFFFFF, sprite);
}

void platform_render_char(int32_t x, int32_t y, char ch) {
  if (!font_tileset) return;
  GRRLIB_DrawTile((f32)x, (f32)y, font_tileset, 0.0f, 1.0f, 1.0f, 0xFFFFFFFF, (unsigned char)ch);
}

void platform_print(int32_t x, int32_t y, char* s) {
  for (int i=0; s[i] != 0; i++) {
    if ((x + i*8) > WC_SCREEN_WIDTH) break;
    platform_render_char(x + i*8, y, s[i]);
  }
}

void platform_tile_map(int32_t draw_x, int32_t draw_y, uint8_t map_x, uint8_t map_y, uint8_t map_w, uint8_t map_h) {
  // TODO
}

bool platform_button(uint8_t b)          { return key_held(b); }
bool platform_button_pressed(uint8_t b)  { return key_pressed(b); }
bool platform_menu_pressed()             { return gcwii_pressed_menu; }

void platform_print_line(const char *text) { printf("%s\n", text); }

// File selection
#include <dirent.h>
#include "../file_list.h"

char* platform_init_fsel_data(const char* path, file_list* fsel_list) {
  DIR* dirp = opendir(path);
  struct dirent* cur;
  if (dirp == NULL) return "Failed to open directory";
  char * error;

  while ((cur = readdir(dirp)) != NULL) {
    if (strlen(cur->d_name) == 0) continue;
    error = insert_file(fsel_list, cur->d_name, cur->d_type == DT_DIR);
    if (error) return error;
  }
  closedir(dirp);
  return 0;
}


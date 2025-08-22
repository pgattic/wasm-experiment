#include <grrlib.h>
#include <wiiuse/wpad.h>
#include <gccore.h>
#include <fat.h>
#include <dirent.h>
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

// Integer scaling to preserve crisp pixels
static int SCALE = 2; // computed in init
static int LEFT_MARGIN = 0;
static int TOP_MARGIN  = 0;

const char FALLBACK_FILE_DIR[256] = "sd:";

char* platform_init() {
  // Init subsystems (order mirrors GRRLIB template)
  if (GRRLIB_Init() != 0) {
    return "GRRLIB init failed";
  }
  WPAD_Init();

  // Optional: black background behind our letterboxing
  GRRLIB_SetBackgroundColour(0,0,0,0xFF);

  // Mount FAT (SD/USB)
  if (!fatInitDefault()) {
    return "FAT initialization failed";
  }

  // Compute integer scale & margins for 240x160 canvas
  int sx = WII_SC_W / WC_SCREEN_WIDTH;
  int sy = WII_SC_H / WC_SCREEN_HEIGHT;
  SCALE = (sx < sy) ? sx : sy;
  if (SCALE < 1) SCALE = 1;

  LEFT_MARGIN = (WII_SC_W - (WC_SCREEN_WIDTH  * SCALE)) / 2;
  TOP_MARGIN  = (WII_SC_H - (WC_SCREEN_HEIGHT * SCALE)) / 2;

  // Upload palette & font tiles
  load_palette();
  load_font_tiles();
  return 0;
}

void platform_prepare_cartridge() {
  load_sprite_tiles(loaded_cartridge.spr_tiles);
}

void platform_begin_frame() {
  // Input first
  collect_keys();

  // Dimmed background (half of color 1) like SDL path
  uint8_t r = (wii_colors[1] >> 24) & 0xFF;
  uint8_t g = (wii_colors[1] >> 16) & 0xFF;
  uint8_t b = (wii_colors[1] >>  8) & 0xFF;
  GRRLIB_FillScreen(((u32)(r>>1) << 24) | ((u32)(g>>1) << 16) | ((u32)(b>>1) << 8) | 0xFF);
}

static inline float sx(int32_t x) { return (float)(LEFT_MARGIN + x * SCALE); }
static inline float sy(int32_t y) { return (float)(TOP_MARGIN  + y * SCALE); }

void platform_end_frame() {
  // Letterbox borders outside the game area are already black via FillScreen.
  GRRLIB_Render();      // push to TV
  VIDEO_WaitVSync();    // block until vsync for stable 60Hz
}

void platform_deinit() {
  unload_graphics();
  GRRLIB_Exit();
}

uint32_t platform_rand() { return rand(); }

void platform_clear_screen(uint8_t color) {
  GRRLIB_Rectangle((float)LEFT_MARGIN, (float)TOP_MARGIN,
                   (float)(WC_SCREEN_WIDTH * SCALE),
                   (float)(WC_SCREEN_HEIGHT * SCALE),
                   wii_colors[color], true);
}

void platform_set_pixel(int32_t x, int32_t y, uint8_t color) {
  // Draw as a SCALE×SCALE filled rect so pixels expand crisply
  GRRLIB_Rectangle(sx(x), sy(y), (float)SCALE, (float)SCALE, wii_colors[color], true);
}

void platform_rect_outline(int32_t x, int32_t y, uint32_t w, uint32_t h, uint8_t color) {
  GRRLIB_Rectangle(sx(x), sy(y), (float)(w * SCALE), (float)(h * SCALE), wii_colors[color], false);
}

void platform_rect_fill(int32_t x, int32_t y, uint32_t w, uint32_t h, uint8_t color) {
  GRRLIB_Rectangle(sx(x), sy(y), (float)(w * SCALE), (float)(h * SCALE), wii_colors[color], true);
}

void platform_sprite(int32_t x, int32_t y, uint8_t sprite) {
  if (!spr_tileset) return;
  // Use tileset-aware draw (1:1 scaled, then SCALE it)
  GRRLIB_DrawTile(sx(x), sy(y), spr_tileset, 0.0f, (float)SCALE, (float)SCALE, 0xFFFFFFFF, sprite);
}

void platform_render_char(int32_t x, int32_t y, char ch) {
  if (!font_tileset) return;
  GRRLIB_DrawTile(sx(x), sy(y), font_tileset, 0.0f, (float)SCALE, (float)SCALE, 0xFFFFFFFF, (unsigned char)ch);
}

void platform_print(int32_t x, int32_t y, char* s) {
  for (int i=0; s[i] != 0; i++) {
    if ((x + i*8) > WC_SCREEN_WIDTH) break;
    platform_render_char(x + i*8, y, s[i]);
  }
}

void platform_tile_map(int32_t draw_x, int32_t draw_y, uint8_t map_x, uint8_t map_y, uint8_t map_w, uint8_t map_h) {
  // TODO: same as other platforms when you wire a map format
}

bool platform_button(uint8_t b)          { return key_held(b); }
bool platform_button_pressed(uint8_t b)  { return key_pressed(b); }
bool platform_menu_pressed()             { return wii_pressed_menu; }

void platform_print_line(const char *text) { printf("%s\n", text); }

// File selection

char* platform_init_fsel_data() {
  // Start in current working dir if available, else SD root
  if (strlen(fsel_path) == 0) {
    strcpy(fsel_path, "sd:/");
  }

  fsel_curr_files_c = 0;
  DIR *dirp = opendir(fsel_path);
  if (!dirp) return "Failed to open directory";
  readdir(dirp); // skip "."

  for (int i = 0; i < 256; i++) {
    struct dirent *cur = readdir(dirp);
    if (!cur || cur->d_name[0] == 0) break;

    strcpy(fsel_curr_files[i], cur->d_name);
    if (cur->d_type == DT_DIR) {
      is_dir[i] = true;
      int slen = (int)strlen(fsel_curr_files[i]);
      fsel_curr_files[i][slen]   = '/';
      fsel_curr_files[i][slen+1] = '\0';
    } else {
      is_dir[i] = false;
    }
    fsel_curr_files_c++;
  }
  closedir(dirp);
  return 0;
}


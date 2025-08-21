#include <nds.h>
#include <stdint.h>
#include <fat.h>
#include "graphics.h"
#include "keys.h"
#include "../platform.h"
#include "../f_sel.h"

#define NDS_SC_W 256
#define NDS_SC_H 192

#define LEFT_MARGIN ((NDS_SC_W - WC_SCREEN_WIDTH) / 2) // 8
#define TOP_MARGIN ((NDS_SC_H - WC_SCREEN_HEIGHT) / 2) // 16

const char FALLBACK_FILE_DIR[256] = "fat:"; // Should never have to be used

char* platform_init() {
  consoleDemoInit();
  videoSetMode(MODE_0_3D);
  glScreen2D();

  vramSetBankA(VRAM_A_TEXTURE);
  vramSetBankE(VRAM_E_TEX_PALETTE);

  load_palette();
  load_font_tiles();

  printf("\n");
  if (!fatInitDefault()) {
    return "FAT initialization failed";
  }
  strcpy(fsel_path, getcwd(NULL, 0));
  return 0;
}

void platform_prepare_cartridge() {
  load_sprite_tiles(loaded_cartridge.spr_tiles);
}

int platform_begin_frame() {
  oamUpdate(&oamMain);
  collect_keys();
  glBegin2D();
  return 0;
}

void platform_end_frame() {
  glBoxFilled(0, 0, NDS_SC_W - 1, TOP_MARGIN - 1, 0); // Top Margin
  glBoxFilled(0, 0, LEFT_MARGIN - 1, NDS_SC_H - 1, 0); // Left Margin
  glBoxFilled(0, NDS_SC_H - TOP_MARGIN, NDS_SC_W - 1, NDS_SC_H - 1, 0); // Bottom Margin
  glBoxFilled(NDS_SC_W - LEFT_MARGIN, 0, NDS_SC_W - 1, NDS_SC_H - 1, 0); // Right Margin

  glEnd2D();
  glFlush(0);
}

void platform_deinit() {
  unload_graphics();
}

uint32_t platform_rand() {
  return rand();
}

void platform_clear_screen(uint8_t color) {
  glBoxFilled(0, 0, 255, 191, nds_palette[color]);
}

void platform_set_pixel(int32_t x, int32_t y, uint8_t color) {
  glPutPixel(LEFT_MARGIN + x, TOP_MARGIN + y, nds_palette[color]);
}

void platform_rect_outline(int32_t x, int32_t y, uint32_t width, uint32_t height, uint8_t color) {
  glBox(LEFT_MARGIN + x, TOP_MARGIN + y, LEFT_MARGIN + x + width - 1, TOP_MARGIN + y + height - 1, nds_palette[color]);
}

void platform_rect_fill(int32_t x, int32_t y, uint32_t width, uint32_t height, uint8_t color) {
  glBoxFilled(LEFT_MARGIN + x, TOP_MARGIN + y, LEFT_MARGIN + x + width - 1, TOP_MARGIN + y + height - 1, nds_palette[color]);
}

void platform_sprite(int32_t x, int32_t y, uint8_t sprite) {
  glSprite(LEFT_MARGIN + x, TOP_MARGIN + y, GL_FLIP_NONE, &spr_tiles[sprite]);
}

void platform_render_char(int32_t x, int32_t y, char ch) {
  glSprite(LEFT_MARGIN + x, TOP_MARGIN + y, GL_FLIP_NONE, &font_tiles[ch]);
}

void platform_print(int32_t x, int32_t y, char* string) {
  for (int curr = 0; string[curr] != 0; curr++) {
    if ((x + curr*8) > WC_SCREEN_WIDTH) { break; } // Early finish
    platform_render_char(x + curr*8, y, string[curr]);
  }
}

void platform_tile_map(int32_t draw_x, int32_t draw_y, uint8_t map_x, uint8_t map_y, uint8_t map_w, uint8_t map_h) {
  // TODO
}

bool platform_button(uint8_t button) {
  return key_held(button);
}

bool platform_button_pressed(uint8_t button) {
  return key_pressed(button);
}

bool platform_menu_pressed() {
  return nds_pressed_menu;
}

void platform_print_line(const char *text) {
  printf("%s\n", text);
}

#include <dirent.h>

char* platform_init_fsel_data() {
  fsel_curr_files_c = 0;
  DIR *dirp = opendir(fsel_path);
  if (dirp == NULL) {
    return "Failed to open directory";
  }
  readdir(dirp); // Skip first entry (".")

  for (int i=0; i < 256; i++) {
    struct dirent *cur = readdir(dirp);
    if (cur == NULL)
      break;

    if (strlen(cur->d_name) == 0)
      break;

    strcpy(fsel_curr_files[i], cur->d_name);
    if (cur->d_type == DT_DIR) {
      is_dir[i] = true;
      int slen = strlen(fsel_curr_files[i]);
      fsel_curr_files[i][slen] = '/';
      fsel_curr_files[i][slen+1] = '\0';
    } else {
      is_dir[i] = false;
    };
    fsel_curr_files_c++;
  }
  closedir(dirp);
  return 0;
}


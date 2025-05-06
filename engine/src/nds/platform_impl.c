#include <nds.h>
#include <stdint.h>
#include <fat.h>
#include "graphics.h"
#include "keys.h"
#include "../platform.h"

#define NDS_SC_W 256
#define NDS_SC_H 192

#define LEFT_MARGIN ((NDS_SC_W - WC_SCREEN_WIDTH) / 2) // 8
#define TOP_MARGIN ((NDS_SC_H - WC_SCREEN_HEIGHT) / 2) // 16

int platform_init() {
  consoleDemoInit();
  videoSetMode(MODE_0_3D);
  glScreen2D();

  printf("\n");
  if (!fatInitDefault()) {
    printf("FAT initialization failed.\n");
    return 1;
  }
  return 0;
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

void platform_prepare_cartridge(Cart *c) {
  vramSetBankA(VRAM_A_TEXTURE);
  vramSetBankE(VRAM_E_TEX_PALETTE);

  load_palette(c->palette);
  load_sprite_tiles(c->spr_tiles);
}

uint32_t platform_rand() {
  return rand();
}

void platform_clear_screen(uint8_t color) {
  glBoxFilled(0, 0, 255, 191, nds_palette[color]);
}

void platform_set_pixel(uint8_t x, uint8_t y, uint8_t color) {
  glPutPixel(LEFT_MARGIN + x, TOP_MARGIN + y, nds_palette[color]);
}

void platform_rect_outline(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color) {
  glBox(LEFT_MARGIN + x, TOP_MARGIN + y, LEFT_MARGIN + x + width - 1, TOP_MARGIN + y + height - 1, nds_palette[color]);
}

void platform_rect_fill(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color) {
  glBoxFilled(LEFT_MARGIN + x, TOP_MARGIN + y, LEFT_MARGIN + x + width - 1, TOP_MARGIN + y + height - 1, nds_palette[color]);
}

void platform_sprite(uint8_t x, uint8_t y, uint8_t sprite) {
  glSprite(LEFT_MARGIN + x, TOP_MARGIN + y, GL_FLIP_NONE, &spr_tiles[sprite]);
}

void platform_tile_map(int16_t draw_x, int16_t draw_y, uint8_t map_x, uint8_t map_y, uint8_t map_w, uint8_t map_h) {
  // TODO
}

bool platform_button(uint8_t button) {
  return key_held(button);
}

bool platform_button_pressed(uint8_t button) {
  return key_pressed(button);
}

void platform_print_line(const char *text) {
  printf("%s\n", text);
}

#include <sys/stat.h>
#include <dirent.h>

int platform_select_file(char *targetFile) {
  // Relative path is safer (would expect "fat:" on NDS, "SD:" on DSi I think, etc.)
  char cwd[100] = "./wasmcarts/";
  DIR *dirp = opendir(cwd);
  if (dirp == NULL) {
    perror("Error");
    return 1;
  }

  char entries[10][20] = {0};
  bool is_dir[10] = {0};
  int num_dirs = 0;

  for (int i=0; i < 10; i++) {
    struct dirent *cur = readdir(dirp);
    if (cur == NULL)
      break;

    if (strlen(cur->d_name) == 0)
      break;

    strcpy(entries[i], cur->d_name);
    if (cur->d_type == DT_DIR) is_dir[i] = 1;
    num_dirs++;
  }
  closedir(dirp);

  int curr = 2;
  bool changed = true;

  while (1) {
    swiWaitForVBlank();

    scanKeys();

    uint32_t keys_down = keysDown();
    if (keys_down & KEY_UP && curr > 2) {
      curr--;
      changed = true;
    }
    if (keys_down & KEY_DOWN && curr < num_dirs-1) {
      curr++;
      changed = true;
    }
    if (keys_down & KEY_A) {
      strcpy(targetFile, cwd);
      strcat(targetFile, entries[curr]);
      return 0;
    }

    if (changed) {
      consoleClear();
      printf("%s\n", cwd);
      for (int i = 2; i < 10; i++) {
        printf("%s %s%s\n", (i==curr ? ">":" "), entries[i], (is_dir[i] ? "/" : " "));
      }
      changed = false;
    }
  }
}


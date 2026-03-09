#include <gccore.h>
#include <fat.h>
#include <ogc/system.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "graphics.h"
#include "keys.h"
#include "../platform.h"
#include "../f_sel.h"
#include "../game.h"
#include "../main.h"

#define GX_FIFO_SIZE (256 * 1024)
#define GAME_TEX_W 256
#define GAME_TEX_H 256

static GXRModeObj *rmode;
static void *xfb[2];
static int xfb_index;
static u8 *gx_fifo;
static u8 *game_texture;
static GXTexObj game_tex_obj;
static Mtx44 projection;

static u32 game_buffer[WC_SCREEN_WIDTH * WC_SCREEN_HEIGHT];

static inline u8 color_r(u32 c) { return (u8)(c >> 24); }
static inline u8 color_g(u32 c) { return (u8)(c >> 16); }
static inline u8 color_b(u32 c) { return (u8)(c >> 8); }
static inline u8 color_a(u32 c) { return (u8)(c); }

static inline void put_pixel_clipped(int x, int y, u32 rgba) {
  if (x < 0 || y < 0 || x >= WC_SCREEN_WIDTH || y >= WC_SCREEN_HEIGHT) {
    return;
  }
  game_buffer[(y * WC_SCREEN_WIDTH) + x] = rgba;
}

static void gx_upload_game_texture(void) {
  memset(game_texture, 0, GAME_TEX_W * GAME_TEX_H * 4u);

  for (int by = 0; by < GAME_TEX_H; by += 4) {
    for (int bx = 0; bx < GAME_TEX_W; bx += 4) {
      size_t block_index = (size_t)((by / 4) * (GAME_TEX_W / 4) + (bx / 4));
      u8 *dst = game_texture + (block_index * 64u);

      for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
          int px = bx + x;
          int py = by + y;

          u32 rgba = 0;
          if (px < WC_SCREEN_WIDTH && py < WC_SCREEN_HEIGHT) {
            rgba = game_buffer[(py * WC_SCREEN_WIDTH) + px];
          }

          int i = (y * 4 + x) * 2;
          dst[i + 0] = color_a(rgba);
          dst[i + 1] = color_r(rgba);
          dst[32 + i + 0] = color_g(rgba);
          dst[32 + i + 1] = color_b(rgba);
        }
      }
    }
  }

  DCFlushRange(game_texture, GAME_TEX_W * GAME_TEX_H * 4u);
}

static inline int compute_scale_int(void) {
  int sx = (int)rmode->fbWidth / WC_SCREEN_WIDTH;
  int sy = (int)rmode->efbHeight / WC_SCREEN_HEIGHT;
  int s = (sx < sy) ? sx : sy;
  return (s < 1) ? 1 : s;
}

static void gx_draw_colored_quad(f32 x, f32 y, f32 w, f32 h, u8 r, u8 g, u8 b, u8 a) {
  GX_ClearVtxDesc();
  GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
  GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);
  GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
  GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);

  GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
  GX_SetNumTexGens(0);

  GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
  GX_Position3f32(x, y, 0.0f);
  GX_Color4u8(r, g, b, a);
  GX_Position3f32(x + w, y, 0.0f);
  GX_Color4u8(r, g, b, a);
  GX_Position3f32(x + w, y + h, 0.0f);
  GX_Color4u8(r, g, b, a);
  GX_Position3f32(x, y + h, 0.0f);
  GX_Color4u8(r, g, b, a);
  GX_End();
}

static void gx_draw_game_quad(f32 x, f32 y, f32 w, f32 h) {
  GX_InitTexObj(&game_tex_obj, game_texture, GAME_TEX_W, GAME_TEX_H, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
  GX_InitTexObjLOD(&game_tex_obj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
  GX_LoadTexObj(&game_tex_obj, GX_TEXMAP0);

  GX_ClearVtxDesc();
  GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
  GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);
  GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);

  GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
  GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
  GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

  GX_SetNumTexGens(1);
  GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
  GX_SetTevOp(GX_TEVSTAGE0, GX_REPLACE);

  GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
  GX_Position3f32(x, y, 0.0f);
  GX_Color4u8(255, 255, 255, 255);
  GX_TexCoord2f32(0.0f, 0.0f);

  GX_Position3f32(x + w, y, 0.0f);
  GX_Color4u8(255, 255, 255, 255);
  GX_TexCoord2f32((f32)WC_SCREEN_WIDTH / (f32)GAME_TEX_W, 0.0f);

  GX_Position3f32(x + w, y + h, 0.0f);
  GX_Color4u8(255, 255, 255, 255);
  GX_TexCoord2f32((f32)WC_SCREEN_WIDTH / (f32)GAME_TEX_W, (f32)WC_SCREEN_HEIGHT / (f32)GAME_TEX_H);

  GX_Position3f32(x, y + h, 0.0f);
  GX_Color4u8(255, 255, 255, 255);
  GX_TexCoord2f32(0.0f, (f32)WC_SCREEN_HEIGHT / (f32)GAME_TEX_H);
  GX_End();
}

char* platform_init() {
  VIDEO_Init();
  init_keys();

  rmode = VIDEO_GetPreferredMode(NULL);
  if (!rmode) {
    return "VIDEO_GetPreferredMode failed";
  }

  xfb[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
  xfb[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
  if (!xfb[0] || !xfb[1]) {
    return "Failed to allocate framebuffer";
  }

  VIDEO_Configure(rmode);
  VIDEO_SetNextFramebuffer(xfb[0]);
  VIDEO_SetBlack(FALSE);
  VIDEO_Flush();
  VIDEO_WaitVSync();
  if (rmode->viTVMode & VI_NON_INTERLACE) {
    VIDEO_WaitVSync();
  }

  gx_fifo = memalign(32, GX_FIFO_SIZE);
  if (!gx_fifo) {
    return "Failed to allocate GX FIFO";
  }
  memset(gx_fifo, 0, GX_FIFO_SIZE);
  GX_Init(gx_fifo, GX_FIFO_SIZE);

  GX_SetViewport(0.0f, 0.0f, (f32)rmode->fbWidth, (f32)rmode->efbHeight, 0.0f, 1.0f);
  GX_SetScissor(0, 0, rmode->fbWidth, rmode->efbHeight);
  GX_SetDispCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight);
  GX_SetDispCopyDst(rmode->fbWidth, rmode->xfbHeight);
  GX_SetDispCopyYScale((f32)rmode->xfbHeight / (f32)rmode->efbHeight);
  GX_SetCopyFilter(rmode->aa, rmode->sample_pattern, GX_FALSE, rmode->vfilter);
  GX_SetFieldMode(rmode->field_rendering, ((rmode->viHeight == 2 * rmode->xfbHeight) ? GX_ENABLE : GX_DISABLE));
  GX_SetCullMode(GX_CULL_NONE);
  GX_SetNumChans(1);
  GX_SetNumTexGens(1);
  GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
  GX_SetTevOp(GX_TEVSTAGE0, GX_REPLACE);
  GX_SetColorUpdate(GX_TRUE);
  GX_SetAlphaUpdate(GX_TRUE);
  GX_SetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);

  Mtx modelview;
  guMtxIdentity(modelview);
  GX_LoadPosMtxImm(modelview, GX_PNMTX0);

  guOrtho(projection, 0.0f, (f32)rmode->efbHeight, 0.0f, (f32)rmode->fbWidth, 0.0f, 1.0f);
  GX_LoadProjectionMtx(projection, GX_ORTHOGRAPHIC);

  game_texture = memalign(32, GAME_TEX_W * GAME_TEX_H * 4u);
  if (!game_texture) {
    return "Failed to allocate game texture";
  }
  memset(game_texture, 0, GAME_TEX_W * GAME_TEX_H * 4u);
  xfb_index = 0;

#ifdef PLATFORM_WII
  if (!fatInitDefault()) {
    return "FAT initialization failed";
  }
#endif

  load_palette();
  load_font_tiles();
  platform_clear_screen(0);

  return NULL;
}

char* platform_set_start_dir(char* path, size_t path_size) {
  (void)path_size;
  strcpy(path, "sd:/");
  return NULL;
}

void platform_prepare_cartridge() {
  load_sprite_tiles(loaded_cartridge.spr_tiles);
}

void platform_begin_frame() {
  collect_keys();
}

void platform_end_frame() {
  gx_upload_game_texture();

  int scale = compute_scale_int();
  int dst_w = WC_SCREEN_WIDTH * scale;
  int dst_h = WC_SCREEN_HEIGHT * scale;
  int dst_x = ((int)rmode->fbWidth - dst_w) / 2;
  int dst_y = ((int)rmode->efbHeight - dst_h) / 2;

  u8 bg_r = color_r(wii_colors[1]) >> 1;
  u8 bg_g = color_g(wii_colors[1]) >> 1;
  u8 bg_b = color_b(wii_colors[1]) >> 1;

  GX_SetCopyClear((GXColor){ bg_r, bg_g, bg_b, 0xFF }, 0x00FFFFFF);
  GX_InvVtxCache();
  GX_InvalidateTexAll();

  gx_draw_colored_quad(0.0f, 0.0f, (f32)rmode->fbWidth, (f32)rmode->efbHeight, bg_r, bg_g, bg_b, 0xFF);
  gx_draw_game_quad((f32)dst_x, (f32)dst_y, (f32)dst_w, (f32)dst_h);

  GX_DrawDone();
  GX_SetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
  GX_CopyDisp(xfb[xfb_index], GX_TRUE);
  GX_Flush();

  VIDEO_SetNextFramebuffer(xfb[xfb_index]);
  VIDEO_Flush();
  VIDEO_WaitVSync();
  xfb_index ^= 1;
}

void platform_deinit() {
  unload_graphics();
  if (game_texture) {
    free(game_texture);
    game_texture = NULL;
  }
  if (gx_fifo) {
    free(gx_fifo);
    gx_fifo = NULL;
  }
}

uint32_t platform_rand() { return rand(); }

void platform_clear_screen(uint8_t color) {
  u32 rgba = wii_colors[color & 0x0F];
  for (size_t i = 0; i < (size_t)(WC_SCREEN_WIDTH * WC_SCREEN_HEIGHT); i++) {
    game_buffer[i] = rgba;
  }
}

void platform_set_pixel(int32_t x, int32_t y, uint8_t color) {
  put_pixel_clipped((int)x, (int)y, wii_colors[color & 0x0F]);
}

void platform_rect_outline(int32_t x, int32_t y, uint32_t w, uint32_t h, uint8_t color) {
  if (w == 0 || h == 0) {
    return;
  }
  u32 rgba = wii_colors[color & 0x0F];
  int x0 = x;
  int y0 = y;
  int x1 = x + (int)w - 1;
  int y1 = y + (int)h - 1;

  for (int px = x0; px <= x1; px++) {
    put_pixel_clipped(px, y0, rgba);
    put_pixel_clipped(px, y1, rgba);
  }
  for (int py = y0; py <= y1; py++) {
    put_pixel_clipped(x0, py, rgba);
    put_pixel_clipped(x1, py, rgba);
  }
}

void platform_rect_fill(int32_t x, int32_t y, uint32_t w, uint32_t h, uint8_t color) {
  if (w == 0 || h == 0) {
    return;
  }
  int x0 = x;
  int y0 = y;
  int x1 = x + (int)w;
  int y1 = y + (int)h;

  if (x0 < 0) x0 = 0;
  if (y0 < 0) y0 = 0;
  if (x1 > WC_SCREEN_WIDTH) x1 = WC_SCREEN_WIDTH;
  if (y1 > WC_SCREEN_HEIGHT) y1 = WC_SCREEN_HEIGHT;
  if (x0 >= x1 || y0 >= y1) {
    return;
  }

  u32 rgba = wii_colors[color & 0x0F];
  for (int py = y0; py < y1; py++) {
    u32 *row = &game_buffer[py * WC_SCREEN_WIDTH + x0];
    for (int px = x0; px < x1; px++) {
      row[px - x0] = rgba;
    }
  }
}

void platform_sprite(int32_t x, int32_t y, uint8_t sprite) {
  const uint8_t *tile = spr_tiles[sprite];
  for (int py = 0; py < 8; py++) {
    for (int px = 0; px < 8; px++) {
      uint8_t idx = tile[py * 8 + px];
      if (idx == 0) {
        continue;
      }
      put_pixel_clipped(x + px, y + py, wii_colors[idx & 0x0F]);
    }
  }
}

void platform_render_char(int32_t x, int32_t y, char ch) {
  const uint8_t *tile = font_tiles[(uint8_t)ch];
  for (int py = 0; py < 8; py++) {
    for (int px = 0; px < 8; px++) {
      uint8_t idx = tile[py * 8 + px];
      if (idx == 0) {
        continue;
      }
      put_pixel_clipped(x + px, y + py, wii_colors[idx & 0x0F]);
    }
  }
}

void platform_print(int32_t x, int32_t y, char* s) {
  for (int i = 0; s[i] != 0; i++) {
    if ((x + i * 8) > WC_SCREEN_WIDTH) {
      break;
    }
    platform_render_char(x + i * 8, y, s[i]);
  }
}

void platform_tile_map(int32_t draw_x, int32_t draw_y, uint8_t map_x, uint8_t map_y, uint8_t map_w, uint8_t map_h) {
  (void)draw_x;
  (void)draw_y;
  (void)map_x;
  (void)map_y;
  (void)map_w;
  (void)map_h;
}

bool platform_button(uint8_t b)          { return key_held(b); }
bool platform_button_pressed(uint8_t b)  { return key_pressed(b); }
bool platform_menu_pressed()             { return gcwii_pressed_menu; }

void platform_print_line(const char *text) { printf("%s\n", text); }

#include <dirent.h>
#include "../file_list.h"

char* platform_init_fsel_data(const char* path, file_list* fsel_list) {
  DIR* dirp = opendir(path);
  struct dirent* cur;
  if (dirp == NULL) return "Failed to open directory";
  char* error;

  while ((cur = readdir(dirp)) != NULL) {
    if (strlen(cur->d_name) == 0) continue;
    error = insert_file(fsel_list, cur->d_name, cur->d_type == DT_DIR);
    if (error) {
      closedir(dirp);
      return error;
    }
  }
  closedir(dirp);
  return NULL;
}


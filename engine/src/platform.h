#pragma once

/*
 * PLATFORM.H
 * Functions required for a given platform to work
 */

#include <stdint.h>
#include <stdbool.h>
#include "cartridge.h"

/* USEFUL CONSTANTS */

#define WC_SCREEN_WIDTH 240
#define WC_SCREEN_HEIGHT 160


// Below are all of the functions that a platform must implement in order to become
// a supported target (see `nds/platform_impl.c` for an example)

/* LOGIC CONTROL */

/// Initialize the system to a ready state. Non-zero return indicates failure.
int platform_init(void);

/// Choose a file to load (sets targetFile to the name of the chosen file).
int platform_select_file(char *targetFile);

/// Perform platform-specific data loading and prep with cartridge data.
/// This may include converting and pre-palletizing the graphics, pre-loading the tile-map, etc.
void platform_prepare_cartridge(Cart *c);

/// Prepare the system graphics for rendering a frame.
/// Commonly includes collecting key inputs.
/// Returns 1 if the game is meant to stop.
int platform_begin_frame(void);

/// End the rendering state, await for next frame.
/// May also include things like swapping/flushing graphics buffers if applicable.
void platform_end_frame(void);

/// De-initialize the system, free memory, prepare to exit.
void platform_deinit(void);


/* API-RELATED FUNCTIONS */

/// Generate a random 32-bit integer.
uint32_t platform_rand();

/// Clear the screen.
void platform_clear_screen(uint8_t color);

/// Set a pixel to a given color.
void platform_set_pixel(int32_t x, int32_t y, uint8_t color);

/// Render a rectangle outline.
void platform_rect_outline(int32_t x, int32_t y, uint32_t width, uint32_t height, uint8_t color);

/// Render a filled rectangle.
void platform_rect_fill(int32_t x, int32_t y, uint32_t width, uint32_t height, uint8_t color);

/// Render a sprite using the sprite tileset.
void platform_sprite(int32_t x, int32_t y, uint8_t sprite);

/// Draw a single character to the screen
void platform_render_char(int32_t x, int32_t y, char ch);

/// Print Text to designated location on screen
void platform_print(int32_t x, int32_t y, char string[]);

/// Draw a tilemap using the tilemap data and the bg tileset.
void platform_tile_map(int32_t draw_x, int32_t draw_y, uint8_t map_x, uint8_t map_y, uint8_t map_w, uint8_t map_h);

/// Check if a button is held down.
bool platform_button(uint8_t button);

/// Check if a button was pressed this frame.
bool platform_button_pressed(uint8_t button);

/// Print a line of text.
void platform_print_line(const char *text);


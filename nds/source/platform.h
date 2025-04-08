#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cartridge.h"

// All of the functions that a platform must implement
// in order to become a supported target


/* LOGIC CONTROL */

/// Initialize the system to a ready state.
void platform_init(void);

/// Choose a file to load (sets targetFile to the name of the chosen file).
int platform_select_file(char *targetFile);

/// Prepare the system graphics for rendering a frame.
/// Commonly includes collecting key inputs.
void platform_begin_frame(void);

/// End the rendering state, await for next frame.
/// May also include things like swapping/flushing graphics buffers if applicable.
void platform_end_frame(void);

/// De-initialize the system, free memory, prepare to exit.
void platform_deinit(void);

/// Perform platform-specific data loading and prep with cartridge data.
/// This may include converting and pre-palletizing the graphics, pre-loading the tile-map, etc.
void platform_prepare_cartridge(Cart *c);


/* API-RELATED FUNCTIONS */

/// Generate a random 32-bit integer.
uint32_t platform_rand();

/// Clear the screen.
void platform_clear_screen(uint8_t color);

/// Set a pixel to a given color.
void platform_set_pixel(uint8_t x, uint8_t y, uint8_t color);

/// Render a rectangle outline.
void platform_rect_outline(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);

/// Render a filled rectangle.
void platform_rect_fill(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);

/// Render a sprite.
void platform_sprite(uint8_t x, uint8_t y, uint8_t sprite);

/// Check if a button is held down.
bool platform_button(uint8_t button);

/// Check if a button was pressed this frame.
bool platform_button_pressed(uint8_t button);

/// Print a line of text.
void platform_print_line(const char *text);


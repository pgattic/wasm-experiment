#pragma once
#include <stdint.h>
#include <stdbool.h>

// Implemented by both `gamecube_keys.c` and `wii_keys.c`

extern bool gcwii_pressed_menu;

void init_keys();
void collect_keys(void);
bool key_held(uint8_t button);
bool key_pressed(uint8_t button);


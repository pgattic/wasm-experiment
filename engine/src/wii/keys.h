#pragma once
#include <stdint.h>
#include <stdbool.h>

extern bool wii_pressed_menu;

void collect_keys(void);
bool key_held(uint8_t button);
bool key_pressed(uint8_t button);


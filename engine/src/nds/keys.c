#include <nds.h>
#include <stdint.h>
#include <stdbool.h>

#define NUM_KEYS 12

bool nds_held_keys[NUM_KEYS] = {0};
bool nds_pressed_keys[NUM_KEYS] = {0};

void collect_keys() {
  scanKeys();

  uint16_t held = keysHeld();
  uint16_t pressed = keysDown();

  nds_held_keys[0] = (held & KEY_LEFT) != 0;
  nds_held_keys[1] = (held & KEY_RIGHT) != 0;
  nds_held_keys[2] = (held & KEY_UP) != 0;
  nds_held_keys[3] = (held & KEY_DOWN) != 0;
  nds_held_keys[4] = (held & KEY_A) != 0;
  nds_held_keys[5] = (held & KEY_B) != 0;
  nds_held_keys[6] = (held & KEY_X) != 0;
  nds_held_keys[7] = (held & KEY_Y) != 0;
  nds_held_keys[8] = (held & KEY_L) != 0;
  nds_held_keys[9] = (held & KEY_R) != 0;
  nds_held_keys[10] = (held & KEY_START) != 0;
  nds_held_keys[11] = (held & KEY_SELECT) != 0;

  nds_pressed_keys[0] = (pressed & KEY_LEFT) != 0;
  nds_pressed_keys[1] = (pressed & KEY_RIGHT) != 0;
  nds_pressed_keys[2] = (pressed & KEY_UP) != 0;
  nds_pressed_keys[3] = (pressed & KEY_DOWN) != 0;
  nds_pressed_keys[4] = (pressed & KEY_A) != 0;
  nds_pressed_keys[5] = (pressed & KEY_B) != 0;
  nds_pressed_keys[6] = (pressed & KEY_X) != 0;
  nds_pressed_keys[7] = (pressed & KEY_Y) != 0;
  nds_pressed_keys[8] = (pressed & KEY_L) != 0;
  nds_pressed_keys[9] = (pressed & KEY_R) != 0;
  nds_pressed_keys[10] = (pressed & KEY_START) != 0;
  nds_pressed_keys[11] = (pressed & KEY_SELECT) != 0;
}

bool key_held(uint8_t button) {
  return (button < NUM_KEYS) ? nds_held_keys[button] : 0;
}

bool key_pressed(uint8_t button) {
  return (button < NUM_KEYS) ? nds_pressed_keys[button] : 0;
}

#include <stdbool.h>
#include <stdint.h>
#include <ogc/pad.h>
#include "keys.h"

// Implements `keys.h`

// So far only player 1 supported

#define NUM_KEYS 12

static bool held[NUM_KEYS]    = {0};
static bool pressed[NUM_KEYS] = {0};
bool gcwii_pressed_menu = false;

void init_keys() {
  PAD_Init();
}

static inline void set_btn(int idx, bool h, bool d) {
  if (idx < 0 || idx >= NUM_KEYS) return;
  held[idx] = h;
  pressed[idx] = d;
  // if (h) { held[idx] = true; } else { held[idx] = false; }
  // if (d) { pressed[idx] = true; held[idx] = true; }
}

void collect_keys(void) {
  // for (int i=0;i<NUM_KEYS;i++) pressed[i] = false;
  gcwii_pressed_menu = false;

  // Scan Wiimote (+ Classic) and GameCube pads
  PAD_ScanPads();

  u16 gh = PAD_ButtonsHeld(0);
  u16 gd = PAD_ButtonsDown(0);

  // D-pad
  set_btn(0, (gh & PAD_BUTTON_LEFT),
             (gd & PAD_BUTTON_LEFT));
  set_btn(1, (gh & PAD_BUTTON_RIGHT),
             (gd & PAD_BUTTON_RIGHT));
  set_btn(2, (gh & PAD_BUTTON_UP),
             (gd & PAD_BUTTON_UP));
  set_btn(3, (gh & PAD_BUTTON_DOWN),
             (gd & PAD_BUTTON_DOWN));

  set_btn(4, (gh & PAD_BUTTON_A),
             (gd & PAD_BUTTON_A));
  set_btn(5, (gh & PAD_BUTTON_B),
             (gd & PAD_BUTTON_B));
  set_btn(6, (gh & PAD_BUTTON_X),
             (gd & PAD_BUTTON_X));
  set_btn(7, (gh & PAD_BUTTON_Y),
             (gd & PAD_BUTTON_Y));

  // Shoulders
  set_btn(8, (gh & PAD_TRIGGER_L),
             (gd & PAD_TRIGGER_L));
  set_btn(9, (gh & PAD_TRIGGER_R),
             (gd & PAD_TRIGGER_R));

  // Start / Select (Classic +/-, GC START, Wiimote +/-)
  set_btn(10, (gh & PAD_BUTTON_START),
              (gd & PAD_BUTTON_START));
  set_btn(11, 0, 0);

  // Menu: START+Z as a convenience
  gcwii_pressed_menu = ((gd & PAD_BUTTON_START) && (gh & PAD_TRIGGER_Z));
}

bool key_held(uint8_t b)    { return (b<NUM_KEYS)? held[b]    : false; }
bool key_pressed(uint8_t b) { return (b<NUM_KEYS)? pressed[b] : false; }


#include <stdbool.h>
#include <stdint.h>
#include <wiiuse/wpad.h>
#include <ogc/pad.h>

// So far only player 1 supported

#define NUM_KEYS 12

static bool held[NUM_KEYS]    = {0};
static bool pressed[NUM_KEYS] = {0};
bool wii_pressed_menu = false;

static inline void set_btn(int idx, bool h, bool d) {
  if (idx < 0 || idx >= NUM_KEYS) return;
  held[idx] = h;
  pressed[idx] = d;
  // if (h) { held[idx] = true; } else { held[idx] = false; }
  // if (d) { pressed[idx] = true; held[idx] = true; }
}

void collect_keys(void) {
  // for (int i=0;i<NUM_KEYS;i++) pressed[i] = false;
  wii_pressed_menu = false;

  // Scan Wiimote (+ Classic) and GameCube pads
  WPAD_ScanPads();
  PAD_ScanPads();

  u32 wh = WPAD_ButtonsHeld(0);
  u32 wd = WPAD_ButtonsDown(0);

  u16 gh = PAD_ButtonsHeld(0);
  u16 gd = PAD_ButtonsDown(0);

  //                Wii Controller      Classic controller                   GameCube Controller
  // D-pad
  set_btn(0, (wh & (WPAD_BUTTON_UP    | WPAD_CLASSIC_BUTTON_LEFT))  || (gh & PAD_BUTTON_LEFT),
             (wd & (WPAD_BUTTON_UP    | WPAD_CLASSIC_BUTTON_LEFT))  || (gd & PAD_BUTTON_LEFT));
  set_btn(1, (wh & (WPAD_BUTTON_DOWN  | WPAD_CLASSIC_BUTTON_RIGHT)) || (gh & PAD_BUTTON_RIGHT),
             (wd & (WPAD_BUTTON_DOWN  | WPAD_CLASSIC_BUTTON_RIGHT)) || (gd & PAD_BUTTON_RIGHT));
  set_btn(2, (wh & (WPAD_BUTTON_RIGHT | WPAD_CLASSIC_BUTTON_UP))    || (gh & PAD_BUTTON_UP),
             (wd & (WPAD_BUTTON_RIGHT | WPAD_CLASSIC_BUTTON_UP))    || (gd & PAD_BUTTON_UP));
  set_btn(3, (wh & (WPAD_BUTTON_LEFT  | WPAD_CLASSIC_BUTTON_DOWN))  || (gh & PAD_BUTTON_DOWN),
             (wd & (WPAD_BUTTON_LEFT  | WPAD_CLASSIC_BUTTON_DOWN))  || (gd & PAD_BUTTON_DOWN));

  // Face: map like SDL: A,S,X,Z keys → here: prefer Classic A/B/X/Y; fallback to Wiimote 2/1/A/B; GC A/B/X/Y
  set_btn(4, (wh & (WPAD_BUTTON_2     | WPAD_CLASSIC_BUTTON_A))     || (gh & PAD_BUTTON_A),
             (wd & (WPAD_BUTTON_2     | WPAD_CLASSIC_BUTTON_A))     || (gd & PAD_BUTTON_A));
  set_btn(5, (wh & (WPAD_BUTTON_1     | WPAD_CLASSIC_BUTTON_B))     || (gh & PAD_BUTTON_B),
             (wd & (WPAD_BUTTON_1     | WPAD_CLASSIC_BUTTON_B))     || (gd & PAD_BUTTON_B));
  set_btn(6, (wh & (WPAD_BUTTON_A     | WPAD_CLASSIC_BUTTON_X))     || (gh & PAD_BUTTON_X),
             (wd & (WPAD_BUTTON_A     | WPAD_CLASSIC_BUTTON_X))     || (gd & PAD_BUTTON_X));
  set_btn(7, (wh & (WPAD_BUTTON_B     | WPAD_CLASSIC_BUTTON_Y))     || (gh & PAD_BUTTON_Y),
             (wd & (WPAD_BUTTON_B     | WPAD_CLASSIC_BUTTON_Y))     || (gd & PAD_BUTTON_Y));

  // Shoulders
  set_btn(8, (wh &                      WPAD_CLASSIC_BUTTON_ZL)     || (gh & PAD_TRIGGER_L),
             (wd &                      WPAD_CLASSIC_BUTTON_ZL)     || (gd & PAD_TRIGGER_L));
  set_btn(9, (wh &                      WPAD_CLASSIC_BUTTON_ZR)     || (gh & PAD_TRIGGER_R),
             (wd &                      WPAD_CLASSIC_BUTTON_ZR)     || (gd & PAD_TRIGGER_R));

  // Start / Select (Classic +/-, GC START, Wiimote +/-)
  set_btn(10, (wh & (WPAD_BUTTON_PLUS | WPAD_CLASSIC_BUTTON_PLUS  ))|| (gh & PAD_BUTTON_START),
              (wd & (WPAD_BUTTON_PLUS | WPAD_CLASSIC_BUTTON_PLUS  ))|| (gd & PAD_BUTTON_START));
  set_btn(11, (wh & (WPAD_BUTTON_MINUS| WPAD_CLASSIC_BUTTON_MINUS ))|| 0,
              (wd & (WPAD_BUTTON_MINUS| WPAD_CLASSIC_BUTTON_MINUS ))|| 0);

  // Menu: HOME on Wiimote/Classic, or GC START+Z as a convenience
  wii_pressed_menu = (wd & (WPAD_BUTTON_HOME|WPAD_CLASSIC_BUTTON_HOME)) ||
                     ((gd & PAD_BUTTON_START) && (gh & PAD_TRIGGER_Z));
}

bool key_held(uint8_t b)    { return (b<NUM_KEYS)? held[b]    : false; }
bool key_pressed(uint8_t b) { return (b<NUM_KEYS)? pressed[b] : false; }


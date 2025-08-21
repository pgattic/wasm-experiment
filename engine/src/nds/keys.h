#include <stdint.h>
#include <gl2d.h>

extern bool nds_pressed_menu;

/// Updates the internal state machine about button information. Should be called once per frame
void collect_keys();

/// Checks if the given key is held, according to the information collected from `collect_keys`
bool key_held(uint8_t button);

/// Checks if the given key is was pressed this frame, according to the information collected from `collect_keys`
bool key_pressed(uint8_t button);


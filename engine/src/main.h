#pragma once

enum Screen {
  FILE_SELECT,
  PLAYING,
  GAME_MENU,
};

extern enum Screen current_screen;
extern char* err_msg;
extern bool running;


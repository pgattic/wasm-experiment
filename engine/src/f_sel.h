#pragma once

#include <stdint.h>

extern char fsel_path[256];
extern size_t fsel_cursor_location;
extern char fsel_curr_files[256][256]; // I might want to figure out how to shrink this
extern size_t fsel_curr_files_c;
extern bool is_dir[256];

char* set_fsel_path(const char* path);
char* update_file_select();


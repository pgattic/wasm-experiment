#pragma once

#include <stdint.h>

#define MAX_FILE_PATH 1024
#define MAX_DIR_ENTRIES 256
#define MAX_FILE_LENGTH 256

extern char fsel_path[MAX_FILE_PATH];
extern size_t fsel_cursor_location;
extern char fsel_curr_files[MAX_DIR_ENTRIES][MAX_FILE_LENGTH];
extern size_t fsel_curr_files_c;
extern bool is_dir[MAX_DIR_ENTRIES];

char* set_fsel_path(const char* path);
char* update_file_select();


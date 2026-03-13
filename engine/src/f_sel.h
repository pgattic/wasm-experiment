#pragma once

#include <stdint.h>
#include "file_list.h"

#define MAX_FILE_PATH 1024

extern char fsel_path[MAX_FILE_PATH];
extern size_t fsel_cursor_locations[MAX_FILE_PATH];
extern size_t fsel_dir_depth;

extern file_list fsel_list;

char* set_fsel_path(const char* path);
void setup_file_select();
char* update_file_select();


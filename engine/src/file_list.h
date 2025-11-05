#pragma once

#include <stdbool.h> // Gives me bool
#include <stddef.h> // Gives me size_t
#include <stdint.h> // Gives me uint32_t etc.


// Alphabetized list of strings (ones ending with `/` being first) that always puts `../` first, and excludes `./`

typedef struct file_list_node file_list_node;
struct file_list_node {
  char* name;
  bool is_dir;
  file_list_node* next;
  file_list_node* prev;
};

typedef struct file_list {
  size_t length;
  file_list_node* first;
  file_list_node* last;
} file_list;

int compare_file_list_node(file_list_node* a, file_list_node* b);
char* insert_file(file_list* list, const char* name, bool is_dir);
void clear_file_list(file_list* list);
file_list_node* file_list_seek(file_list* list, size_t index);


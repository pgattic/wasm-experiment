#include "file_list.h"
#include <stdbool.h>
#include <stdlib.h> // Gives me malloc
#include <string.h>

const bool show_hidden = false;

file_list new_file_list() {
  return (file_list){0, NULL, NULL};
}

int compare_file_list_node(file_list_node* a, file_list_node* b) {
  if (a->is_dir && !(b->is_dir)) { // A goes first
    return -1;
  }
  if (!(a->is_dir) && b->is_dir) { // B goes first
    return 1;
  }
  return strcmp(a->name, b->name); // Compare by name
}

char* insert_file(file_list* list, const char* name, bool is_dir) {
  // Ignore the current directory '.' name
  if (name[0] == '.' && show_hidden == false) return NULL; // hide hidden files
  if (strcmp(name, ".") == 0) {
    return NULL;
  }
  if ((list->first == NULL) != (list->last == NULL)) {
    return "Malformed file list";
  }

  file_list_node* new_node = malloc(sizeof(file_list_node));
  if (new_node == NULL) {
    return "Memory allocation failure";
  }

  if (is_dir) {
    size_t name_len = strlen(name);
    new_node->name = malloc(name_len + 2); // Include room for '/' and '\0'
    if (new_node->name == NULL) {
      free(new_node);
      return "Memory allocation failure";
    }
    strcpy(new_node->name, name);
    new_node->name[name_len] = '/';
    new_node->name[name_len + 1] = '\0';
  } else {
    new_node->name = strdup(name);
    if (new_node->name == NULL) {
      free(new_node);
      return "Memory allocation failure";
    }
  }

  new_node->is_dir = is_dir;
  new_node->prev = NULL;
  new_node->next = NULL;

  // Empty list
  if (list->first == NULL && list->last == NULL) {
    list->first = new_node;
    list->last = new_node;
  }
  // Insertion at the beginning
  else if (compare_file_list_node(new_node, list->first) < 0) {
    file_list_node* old_first = list->first;
    new_node->next = old_first;
    old_first->prev = new_node;
    list->first = new_node;
  } else if (compare_file_list_node(new_node, list->last) > 0) { // Insertion at the end
    file_list_node* old_last = list->last;
    new_node->prev = old_last;
    old_last->next = new_node;
    list->last = new_node;
  } else {
    // Find the correct insertion point
    file_list_node* current = list->first;
    while (current->next != NULL && compare_file_list_node(new_node, current->next) > 0) {
      current = current->next;
    }
    // Insert after 'current'
    file_list_node* current_next = current->next;
    new_node->next = current_next;
    current_next->prev = new_node;
    current->next = new_node;
    new_node->prev = current;
  }

  list->length++;
  return NULL;
}

void clear_file_list(file_list* list) {
  if (list == NULL) {
    return; // Nothing to free
  }

  file_list_node* current = list->first;
  file_list_node* next_node;

  while (current != NULL) {
    next_node = current->next;
    free(current->name);
    free(current);
    current = next_node;
  }

  // Reset the list to a valid empty state
  list->first = NULL;
  list->last = NULL;
  list->length = 0;
}

file_list_node* file_list_seek(file_list* list, size_t index) {
  if (index >= list->length) {
    return NULL;
  }

  if (index < list->length / 2) {
    file_list_node* result = list->first;
    for (size_t i = 0; i < index; ++i) {
      result = result->next;
    }
    return result;
  } else {
    file_list_node* result = list->last;
    for (size_t i = 0; i < list->length - index - 1; ++i) {
      result = result->prev;
    }
    return result;
  }
}


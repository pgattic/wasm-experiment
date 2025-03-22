#include "cartridge.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

Cart *loadCartridge(const char *filepath, size_t *fileSize) {
  FILE *fp = fopen(filepath, "rb");
  if (!fp) {
    printf("Error opening file: %s\n", filepath);
    return NULL;
  }

  struct stat file_stats;
  if (stat(filepath, &file_stats) == -1) {
    printf("Error reading file stats\n");
  }
  *fileSize = file_stats.st_size;

  uint8_t *buffer = (uint8_t*) malloc(*fileSize);
  if (!buffer) {
    printf("Memory allocation failed.\n");
    fclose(fp);
    return NULL;
  }

  size_t bytesRead = fread(buffer, 1, *fileSize, fp);
  if (bytesRead != *fileSize) {
    printf("Error reading file. Expected %zu bytes but got %zu.\n", *fileSize, bytesRead);
    free(buffer);
    fclose(fp);
    return NULL;
  }
  fclose(fp);
  return (Cart *) buffer;
}


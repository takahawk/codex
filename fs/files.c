#include "codex/fs/files.h"
#include <stdio.h>
#include <stdlib.h>

static char* read_to_str(const char *filename)
{
  FILE *file = fopen(filename, "rb");

  if (!file)
    return NULL;

  fseek(file, 0, SEEK_END);
  long len = ftell(file);
  rewind(file);

  char *buf = malloc(len + 1);
  if (!buf) {
    fclose(file);
    return NULL;
  }

  size_t n = fread(buf, 1, len, file);
  buf[n] = '\0';
  fclose(file);

  return buf;
}

const struct _FilesStatic FILES = {
  .read_to_str = read_to_str,
};

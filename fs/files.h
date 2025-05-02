#ifndef CODEX_FILES_
#define CODEX_FILES_

struct _FilesStatic {
  char* (*read_to_str) (const char* filename);
};

extern const struct _FilesStatic FILES;

#endif // !CODEX_FILES_

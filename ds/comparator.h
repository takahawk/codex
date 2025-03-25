#ifndef CODEX_COMPARATOR_H_
#define CODEX_COMPARATOR_H_

typedef struct {
	int (*cb) (const void *, const void*, void *ctx);
	void *ctx;
} Comparator;


extern const Comparator str_comparator;
extern const Comparator uint16_comparator;
#endif
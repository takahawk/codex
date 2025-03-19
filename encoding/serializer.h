#ifndef CODEX_SERIALIZER_H_
#define CODEX_SERIALIZER_H_
#include <stdint.h>

/*interface*/ typedef struct Serializer {
	void *ctx;

	size_t (*estimate_size)    (struct Serializer *self, void *entity);
	size_t (*serialize_to)     (struct Serializer *self, void *entity, uint8_t *buffer);
	size_t (*deserialize_from) (struct Serializer *self, uint8_t *buffer, void **ptr);

	void   (*release)          (struct Serializer **pself);
} Serializer;

#endif
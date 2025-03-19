#ifndef CODEX_SERIALIZER_H_
#define CODEX_SERIALIZER_H_

#include <endian.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*interface*/ typedef struct Serializer {
	void *ctx;

	size_t (*estimate_size)    (struct Serializer *self, void *entity);
	size_t (*serialize_to)     (struct Serializer *self, void *entity, uint8_t *buffer);
	size_t (*deserialize_from) (struct Serializer *self, uint8_t *buffer, void **ptr);

	void   (*release)          (struct Serializer **pself);
} Serializer;

static size_t
serialize_string(char *s, uint8_t *buffer) {
	uint8_t *p = buffer;
	size_t size = 0;
	uint64_t len = strlen(s) + 1;
	uint64_t belen = htobe64(len);
	memcpy(p, &belen, sizeof(belen));
	p += sizeof(belen);
	size += sizeof(belen);

	memcpy(p, s, len);
	size += len;

	return size;
}

static size_t
deserialize_string(uint8_t *buffer, char **s) {
	uint8_t *p = buffer;
	size_t size = 0;
	uint64_t len;
	memcpy(&len, p, sizeof(len));
	p += sizeof(len);
	size += sizeof(len);

	len = be64toh(len);
	*s = malloc(len);
	memcpy(*s, p, len);
	size += len;

	return size;
}

#endif
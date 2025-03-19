#include <endian.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
	Serializer *item_serializer;
} SerializerCtx;

static size_t
array_serializer_estimate_size(Serializer *serializer, void *entity) {
	SerializerCtx *ctx = (SerializerCtx *) serializer->ctx;
	Serializer *is = ctx->item_serializer;
	Array *array = (Array *) entity;

	size_t size = 0;
	size += sizeof(uint64_t); // len
	size += sizeof(uint64_t); // elem_size
	for (size_t i = 0; i < array->len; ++i) {
		size += is->estimate_size(is, array->get(array, i));
	}

	return size;
}

static size_t
array_serializer_serialize_to(Serializer *serializer, void *entity, uint8_t *buffer) {
	SerializerCtx *ctx = (SerializerCtx *) serializer->ctx;
	Serializer *is = ctx->item_serializer;
	Array *array = (Array *) entity;

	size_t size = 0;
	uint8_t *p = buffer;

	uint64_t len = htobe64((uint64_t)array->len);
	memcpy(p, &len, sizeof(len));
	p += sizeof(len);
	size += sizeof(len);

	uint64_t elem_size = htobe64((uint64_t) array->elem_size);
	memcpy(p, &elem_size, sizeof(elem_size));
	p += sizeof(elem_size);
	size += sizeof(len);

	for (size_t i = 0; i < array->len; ++i) {
		void *item = array->get(array, i);
		size_t item_size = is->serialize_to(is, item, p);
		p += item_size;
		size += item_size;
	}

	return size;
}

static size_t
array_serializer_deserialize_from(Serializer *serializer, uint8_t *buffer, void **entity) {
	SerializerCtx *ctx = (SerializerCtx *) serializer->ctx;
	Serializer *is = ctx->item_serializer;

	uint8_t *p = buffer;
	size_t size = 0;

	uint64_t len;
	memcpy(&len, p, sizeof(len));
	len = be64toh(len);
	p += sizeof(len);
	size += sizeof(len);

	if (len > SIZE_MAX) {
		fprintf(stderr, "array length is too big for current platform!\n");
		exit(EXIT_FAILURE);
	}

	uint64_t elem_size;
	memcpy(&elem_size, p, sizeof(elem_size));
	elem_size = be64toh(elem_size);
	p += sizeof(len);
	size += sizeof(len);

	if (elem_size > SIZE_MAX) {
		fprintf(stderr, "element size is too big for current platform!\n");
		exit(EXIT_FAILURE);
	}

	Array *array = form_array(elem_size);

	for (size_t i = 0; i < len; ++i) {
		void *item;
		size_t item_size = is->deserialize_from(is, p, &item);

		array->add(array, item);
		p += item_size;
		size += item_size;
	}

	*entity = array;

	return size;
}

static Serializer*
form_array_serializer(Serializer *item_serializer) {
	SerializerCtx *ctx = malloc(sizeof(SerializerCtx));
	ctx->item_serializer = item_serializer;

	Serializer *serializer = malloc(sizeof(Serializer));
	serializer->ctx = ctx;
	serializer->estimate_size = array_serializer_estimate_size;
	serializer->serialize_to = array_serializer_serialize_to;
	serializer->deserialize_from = array_serializer_deserialize_from;

	return serializer;
}
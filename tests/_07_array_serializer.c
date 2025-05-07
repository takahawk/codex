#include "codex/ds/array.h"
#include "codex/encoding/serializer.h"

#include <endian.h>
#include <string.h>

#include "codex/mem/debug_allocator.h"
#include "codex/testing/assert.h"

typedef struct {
	Allocator *a;

	uint16_t number;
	char *original;
	char *emulacrum;
} EmulacrumTarot;

static void
emulacrum_tarot_serializer_release(Serializer **pself) {
	Serializer *self = *pself;
	Allocator *a = self->a;

	a->free(a, self);
	*pself = NULL;
}

static size_t
emulacrum_tarot_serializer_estimate_size(Serializer *s, void *entity) {
	EmulacrumTarot *et = (EmulacrumTarot *) entity;
	return sizeof(et->number) + 
	       sizeof(uint64_t) + strlen(et->original) + 1 + 
	       sizeof(uint64_t) + strlen(et->emulacrum) + 1; 
}

static size_t
emulacrum_tarot_serializer_serialize_to(Serializer *s, void *entity, uint8_t *buffer) {
	EmulacrumTarot *et = (EmulacrumTarot *) entity;

	size_t size = 0;
	uint8_t *p = buffer;
	uint16_t number = htobe16(et->number);
	memcpy(p, &number, sizeof(number));
	p += sizeof(number);
	size += sizeof(number);

	size_t i = serialize_string(et->original, p);
	p += i;
	size += i;

	i = serialize_string(et->emulacrum, p);
	p += i;
	size += i;

	return size;
}

static size_t
emulacrum_tarot_serializer_deserialize_from(Serializer *s, uint8_t *buffer, void **entity) {
	EmulacrumTarot et;
	et.a = s->a;

	size_t size = 0;
	uint8_t *p = buffer;
	memcpy(&et.number, p, sizeof(et.number));
	et.number = be16toh(et.number);
	p += sizeof(et.number);
	size += sizeof(et.number);

	size_t i = deserialize_string(s->a, p, &et.original);
	p += i;
	size += i;

	i = deserialize_string(s->a, p, &et.emulacrum);
	p += i;
	size += i;

	memcpy(*entity, &et, sizeof(EmulacrumTarot));

	return size;
}

Serializer *form_emulacrum_tarot_serializer(Allocator *all) {
	Serializer *s = all->alloc(all, sizeof(Serializer));

	s->a = all;

	s->estimate_size = emulacrum_tarot_serializer_estimate_size;
	s->serialize_to = emulacrum_tarot_serializer_serialize_to;
	s->deserialize_from = emulacrum_tarot_serializer_deserialize_from;
	s->release = emulacrum_tarot_serializer_release;

	return s;
}

static void
_emulacrum_tarot_release(Allocator *a, void **pself) {
	// TODO: maybe this function should be parameter of serializer
	EmulacrumTarot self = *(EmulacrumTarot *) pself;

	a->free(a, self.original);
	a->free(a, self.emulacrum);
}

Array*/*EmulacrumTarot*/ form_test_array();

int main() {
	Allocator *a = form_debug_allocator(&STD_ALLOCATOR);
	DebugAllocatorCtx *allocCtx = a->ctx;
	Array *arr = form_test_array(a);
	Serializer *is = form_emulacrum_tarot_serializer(a);
	Serializer *s = arr->form_serializer(a, is);

	size_t size = s->estimate_size(s, arr);
	uint8_t *buf = a->alloc(a, size);

	s->serialize_to(s, arr, buf);

	Array *arr2;

	s->deserialize_from(s, buf, (void **) &arr2);
	arr2->release_cb = RELEASE_CB.form(a, _emulacrum_tarot_release);

	assert_bool_equals(arr->len, arr2->len);
	assert_bool_equals(arr->elem_size, arr2->elem_size);
	for (int i = 0; i < arr2->len; ++i) {
		EmulacrumTarot *et = arr->get(arr, i);
		EmulacrumTarot *et2 = arr2->get(arr2, i);
		assert_uint16_equals(et->number, et2->number);
		assert_str_equals(et->original, et2->original);
		assert_str_equals(et->emulacrum, et2->emulacrum);
	}

	a->free(a, buf);
	s->release(&s);
	is->release(&is);

	arr->release(&arr);
	arr2->release(&arr2);

	if (allocCtx->allocations->len != 0) {
		allocCtx->print_allocations(allocCtx);
		return -1;
	}

	return 0;
}

Array*/*EmulacrumTarot*/ 
form_test_array(Allocator *all) {
	Array *a = ARRAY.form_with_allocator(all, sizeof(EmulacrumTarot));

	EmulacrumTarot entry = {
		.number = 16,
		.original = "The Tower",
		.emulacrum = "Fractura"
	};
	a->add(a, &entry);

	entry = (EmulacrumTarot) {
		.number = 0,
		.original = "The Fool",
		.emulacrum = "Zero"
	};
	a->add(a, &entry);

	entry = (EmulacrumTarot) {
		.number = 17,
		.original = "The Star",
		.emulacrum = "1337"
	};
	a->add(a, &entry);

	entry = (EmulacrumTarot) {
		.number = 14,
		.original = "Temperance",
		.emulacrum = "Meridian"
	};
	a->add(a, &entry);

	entry = (EmulacrumTarot) {
		.number = 2,
		.original = "High Priestess",
		.emulacrum = "Reflexia Void"
	};
	a->add(a, &entry);

	entry = (EmulacrumTarot) {
		.number = 1,
		.original = "The Magician",
		.emulacrum = "The One"
	};
	a->add(a, &entry);
	return a;
}

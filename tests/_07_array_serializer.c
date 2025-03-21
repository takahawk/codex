#include "ds/array.h"
#include "encoding/serializer.h"

#include <endian.h>
#include <string.h>

#include "allocators/allocator.h"
#include "testing/assert.h"

typedef struct {
	uint16_t number;
	char *original;
	char *emulacrum;
} EmulacrumTarot;

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
	EmulacrumTarot *et = s->a->alloc(s->a, sizeof(EmulacrumTarot));

	size_t size = 0;
	uint8_t *p = buffer;
	memcpy(&et->number, p, sizeof(et->number));
	et->number = be16toh(et->number);
	p += sizeof(et->number);
	size += sizeof(et->number);

	size_t i = deserialize_string(s->a, p, &et->original);
	p += i;
	size += i;

	i = deserialize_string(s->a, p, &et->emulacrum);
	p += i;
	size += i;

	*entity = et;

	return size;
}

Serializer *form_emulacrum_tarot_serializer(Allocator *all) {
	Serializer *s = all->alloc(all, sizeof(Serializer));

	s->a = all;

	s->estimate_size = emulacrum_tarot_serializer_estimate_size;
	s->serialize_to = emulacrum_tarot_serializer_serialize_to;
	s->deserialize_from = emulacrum_tarot_serializer_deserialize_from;

	return s;
}


Array*/*EmulacrumTarot*/ form_test_array();

int main() {
	Allocator a = form_std_allocator();
	Array *arr = form_test_array(&a);
	Serializer *is = form_emulacrum_tarot_serializer(&a);
	Serializer *s = arr->form_serializer(&a, is);

	size_t size = s->estimate_size(s, arr);
	uint8_t *buf = a.alloc(&a, size);

	s->serialize_to(s, arr, buf);

	Array *arr2;

	s->deserialize_from(s, buf, (void **) &arr2);

	assert_bool_equals(arr->len, arr2->len);
	assert_bool_equals(arr->elem_size, arr2->elem_size);
	for (int i = 0; i < arr2->len; ++i) {
		EmulacrumTarot *et = arr->get(arr, i);
		EmulacrumTarot *et2 = arr2->get(arr2, i);
		assert_uint16_equals(et->number, et2->number);
		assert_str_equals(et->original, et2->original);
		assert_str_equals(et->emulacrum, et2->emulacrum);
	}

	s->release(&s);

	return 0;
}

Array*/*EmulacrumTarot*/ 
form_test_array(Allocator *all) {
	Array *a = form_array(all, sizeof(EmulacrumTarot));

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

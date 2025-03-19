#include "ds/array.h"
#include "encoding/serializer.h"

#include <endian.h>
#include <string.h>

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
	       sizeof(uint32_t) + strlen(et->original) + 1 + 
	       sizeof(uint32_t) + strlen(et->emulacrum) + 1; 
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

	uint32_t len = strlen(et->original) + 1;
	uint32_t belen = htobe32(len);
	memcpy(p, &belen, sizeof(belen));
	p += sizeof(len);
	size += sizeof(len);
	memcpy(p, et->original, len);
	p += len;
	size += len;

	len = strlen(et->emulacrum) + 1;
	belen = htobe32(len);
	memcpy(p, &belen, sizeof(belen));
	p += sizeof(len);
	size += sizeof(len);
	memcpy(p, et->emulacrum, len);
	p += len;
	size += len;

	return size;
}

static size_t
emulacrum_tarot_serializer_deserialize_from(Serializer *s, uint8_t *buffer, void **entity) {
	EmulacrumTarot *et = malloc(sizeof(EmulacrumTarot));

	size_t size = 0;
	uint8_t *p = buffer;
	memcpy(&et->number, p, sizeof(et->number));
	et->number = be16toh(et->number);
	p += sizeof(et->number);
	size += sizeof(et->number);

	uint32_t len;
	memcpy(&len, p, sizeof(len));
	p += sizeof(len);
	size += sizeof(len);

	len = be32toh(len);
	et->original = malloc(len);
	memcpy(et->original, p, len);
	p += len;
	size += len;

	memcpy(&len, p, sizeof(len));
	p += sizeof(len);
	size += sizeof(len);

	len = be32toh(len);
	et->emulacrum = malloc(len);
	memcpy(et->emulacrum, p, len);
	p += len;
	size += len;

	*entity = et;

	return size;
}

Serializer *form_emulacrum_tarot_serializer() {
	Serializer *s = malloc(sizeof(Serializer));

	s->estimate_size = emulacrum_tarot_serializer_estimate_size;
	s->serialize_to = emulacrum_tarot_serializer_serialize_to;
	s->deserialize_from = emulacrum_tarot_serializer_deserialize_from;

	return s;
}


Array*/*EmulacrumTarot*/ form_test_array();

int main() {
	Array *arr = form_test_array();
	Serializer *is = form_emulacrum_tarot_serializer();
	Serializer *s = arr->form_serializer(is);

	size_t size = s->estimate_size(s, arr);
	uint8_t *buf = malloc(size);

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

	return 0;
}

Array*/*EmulacrumTarot*/ 
form_test_array() {
	Array *a = form_array(sizeof(EmulacrumTarot));

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

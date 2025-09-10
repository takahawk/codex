#include "codex/ds/array.h"

#include <string.h>

#include "codex/testing/assert.h"

#include "codex/mem/debug_allocator.h"

typedef struct {
	uint16_t number;
	char *original;
	char *emulacrum;

} EmulacrumTarot;

Array/*EmulacrumTarot*/ form_unsorted();
Array/*EmulacrumTarot*/ form_sorted_by_number();
Array/*EmulacrumTarot*/ form_sorted_by_original();
Array/*EmulacrumTarot*/ form_sorted_by_emulacrum();

static int
compare_by_number(const void *a, const void *b) {
	EmulacrumTarot lhs = *(EmulacrumTarot *) a;
	EmulacrumTarot rhs = *(EmulacrumTarot *) b;

	return lhs.number - rhs.number;
}

static int
compare_by_original(const void *a, const void *b) {
	EmulacrumTarot lhs = *(EmulacrumTarot *) a;
	EmulacrumTarot rhs = *(EmulacrumTarot *) b;

	return strcmp(lhs.original, rhs.original);
}

static int
compare_by_emulacrum(const void *a, const void *b) {
	EmulacrumTarot lhs = *(EmulacrumTarot *) a;
	EmulacrumTarot rhs = *(EmulacrumTarot *) b;

	return strcmp(lhs.emulacrum, rhs.emulacrum);
}

int main() {
	Array arr = form_unsorted();
	Array by_number = form_sorted_by_number();
	Array by_original = form_sorted_by_original();
	Array by_emulacrum = form_sorted_by_emulacrum();

	assert_bool_equals(cdx_array_equals(arr, by_number), false);
	cdx_array_sort(arr, compare_by_number);
	assert_bool_equals(cdx_array_equals(arr, by_number), true);

	assert_bool_equals(cdx_array_equals(arr, by_original), false);
	cdx_array_sort(arr, compare_by_original);
	assert_bool_equals(cdx_array_equals(arr, by_original), true);

	assert_bool_equals(cdx_array_equals(arr, by_emulacrum), false);
	cdx_array_sort(arr, compare_by_emulacrum);
	assert_bool_equals(cdx_array_equals(arr, by_emulacrum), true);

	cdx_array_release(&arr);
	cdx_array_release(&by_number);
	cdx_array_release(&by_original);
	cdx_array_release(&by_emulacrum);

	return 0;
}

Array/*EmulacrumTarot*/
form_unsorted() {
	Array a = cdx_array_form(sizeof(EmulacrumTarot));

	EmulacrumTarot entry = {
		.number = 16,
		.original = "The Tower",
		.emulacrum = "Fractura"
	};
	cdx_array_add(&a, &entry);

	entry = (EmulacrumTarot) {
		.number = 0,
		.original = "The Fool",
		.emulacrum = "Zero"
	};
	cdx_array_add(&a, &entry);

	entry = (EmulacrumTarot) {
		.number = 17,
		.original = "The Star",
		.emulacrum = "1337"
	};
	cdx_array_add(&a, &entry);

	entry = (EmulacrumTarot) {
		.number = 14,
		.original = "Temperance",
		.emulacrum = "Meridian"
	};
	cdx_array_add(&a, &entry);

	entry = (EmulacrumTarot) {
		.number = 2,
		.original = "High Priestess",
		.emulacrum = "Reflexia Void"
	};
	cdx_array_add(&a, &entry);

	entry = (EmulacrumTarot) {
		.number = 1,
		.original = "The Magician",
		.emulacrum = "The One"
	};
	cdx_array_add(&a, &entry);
	return a;

}

Array/*EmulacrumTarot*/
form_sorted_by_number() {
	Array a = cdx_array_form(sizeof(EmulacrumTarot));

	EmulacrumTarot entry = (EmulacrumTarot) {
		.number = 0,
		.original = "The Fool",
		.emulacrum = "Zero"
	};
	cdx_array_add(&a, &entry);

	entry = (EmulacrumTarot) {
		.number = 1,
		.original = "The Magician",
		.emulacrum = "The One"
	};
	cdx_array_add(&a, &entry);

	entry = (EmulacrumTarot) {
		.number = 2,
		.original = "High Priestess",
		.emulacrum = "Reflexia Void"
	};
	cdx_array_add(&a, &entry);

	entry = (EmulacrumTarot) {
		.number = 14,
		.original = "Temperance",
		.emulacrum = "Meridian"
	};
	cdx_array_add(&a, &entry);

	entry = (EmulacrumTarot) {
		.number = 16,
		.original = "The Tower",
		.emulacrum = "Fractura"
	};
	cdx_array_add(&a, &entry);

	entry = (EmulacrumTarot) {
		.number = 17,
		.original = "The Star",
		.emulacrum = "1337"
	};
	cdx_array_add(&a, &entry);

	return a;
}
Array/*EmulacrumTarot*/ form_sorted_by_original() {
	Array a = cdx_array_form(sizeof(EmulacrumTarot));

	EmulacrumTarot entry = (EmulacrumTarot) {
		.number = 2,
		.original = "High Priestess",
		.emulacrum = "Reflexia Void"
	};
	cdx_array_add(&a, &entry);

	entry = (EmulacrumTarot) {
		.number = 14,
		.original = "Temperance",
		.emulacrum = "Meridian"
	};
	cdx_array_add(&a, &entry);

	entry = (EmulacrumTarot) {
		.number = 0,
		.original = "The Fool",
		.emulacrum = "Zero"
	};
	cdx_array_add(&a, &entry);

	entry = (EmulacrumTarot) {
		.number = 1,
		.original = "The Magician",
		.emulacrum = "The One"
	};
	cdx_array_add(&a, &entry);

	entry = (EmulacrumTarot) {
		.number = 17,
		.original = "The Star",
		.emulacrum = "1337"
	};
	cdx_array_add(&a, &entry);

	entry = (EmulacrumTarot) {
		.number = 16,
		.original = "The Tower",
		.emulacrum = "Fractura"
	};
	cdx_array_add(&a, &entry);

	return a;
}

Array/*EmulacrumTarot*/ 
form_sorted_by_emulacrum() {
	Array a = cdx_array_form(sizeof(EmulacrumTarot));

	EmulacrumTarot entry = (EmulacrumTarot) {
		.number = 17,
		.original = "The Star",
		.emulacrum = "1337"
	};
	cdx_array_add(&a, &entry);

	entry = (EmulacrumTarot) {
		.number = 16,
		.original = "The Tower",
		.emulacrum = "Fractura"
	};
	cdx_array_add(&a, &entry);

	entry = (EmulacrumTarot) {
		.number = 14,
		.original = "Temperance",
		.emulacrum = "Meridian"
	};
	cdx_array_add(&a, &entry);

	entry = (EmulacrumTarot) {
		.number = 2,
		.original = "High Priestess",
		.emulacrum = "Reflexia Void"
	};
	cdx_array_add(&a, &entry);

	entry = (EmulacrumTarot) {
		.number = 1,
		.original = "The Magician",
		.emulacrum = "The One"
	};
	cdx_array_add(&a, &entry);

	entry = (EmulacrumTarot) {
		.number = 0,
		.original = "The Fool",
		.emulacrum = "Zero"
	};
	cdx_array_add(&a, &entry);
	
	return a;
}

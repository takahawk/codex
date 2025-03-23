#include "codex/ds/array.h"

#include <string.h>

#include "codex/testing/assert.h"

#include "codex/allocators/debug_allocator.h"

typedef struct {
	uint16_t number;
	char *original;
	char *emulacrum;

} EmulacrumTarot;

Array*/*EmulacrumTarot*/ form_unsorted(Allocator *all);
Array*/*EmulacrumTarot*/ form_sorted_by_number(Allocator *all);
Array*/*EmulacrumTarot*/ form_sorted_by_original(Allocator *all);
Array*/*EmulacrumTarot*/ form_sorted_by_emulacrum(Allocator *all);

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
	Allocator *all = form_debug_allocator(&std_allocator);
	DebugAllocatorCtx *allocCtx = all->ctx;
	Array *arr = form_unsorted(all);
	Array *by_number = form_sorted_by_number(all);
	Array *by_original = form_sorted_by_original(all);
	Array *by_emulacrum = form_sorted_by_emulacrum(all);

	assert_bool_equals(arr->equals(arr, by_number), false);
	arr->sort(arr, compare_by_number);
	assert_bool_equals(arr->equals(arr, by_number), true);

	assert_bool_equals(arr->equals(arr, by_original), false);
	arr->sort(arr, compare_by_original);
	assert_bool_equals(arr->equals(arr, by_original), true);

	assert_bool_equals(arr->equals(arr, by_emulacrum), false);
	arr->sort(arr, compare_by_emulacrum);
	assert_bool_equals(arr->equals(arr, by_emulacrum), true);

	arr->release(&arr);
	by_number->release(&by_number);
	by_original->release(&by_original);
	by_emulacrum->release(&by_emulacrum);

	if (allocCtx->allocations->len != 0) {
		allocCtx->print_allocations(allocCtx);
		return -1;
	}

	return 0;
}

Array*/*EmulacrumTarot*/ 
form_unsorted(Allocator *all) {
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

Array*/*EmulacrumTarot*/ 
form_sorted_by_number(Allocator *all) {
	Array *a = form_array(all, sizeof(EmulacrumTarot));

	EmulacrumTarot entry = (EmulacrumTarot) {
		.number = 0,
		.original = "The Fool",
		.emulacrum = "Zero"
	};
	a->add(a, &entry);

	entry = (EmulacrumTarot) {
		.number = 1,
		.original = "The Magician",
		.emulacrum = "The One"
	};
	a->add(a, &entry);

	entry = (EmulacrumTarot) {
		.number = 2,
		.original = "High Priestess",
		.emulacrum = "Reflexia Void"
	};
	a->add(a, &entry);

	entry = (EmulacrumTarot) {
		.number = 14,
		.original = "Temperance",
		.emulacrum = "Meridian"
	};
	a->add(a, &entry);

	entry = (EmulacrumTarot) {
		.number = 16,
		.original = "The Tower",
		.emulacrum = "Fractura"
	};
	a->add(a, &entry);

	entry = (EmulacrumTarot) {
		.number = 17,
		.original = "The Star",
		.emulacrum = "1337"
	};
	a->add(a, &entry);
	
	return a;
}
Array*/*EmulacrumTarot*/ form_sorted_by_original(Allocator *all) {
	Array *a = form_array(all, sizeof(EmulacrumTarot));

	EmulacrumTarot entry = (EmulacrumTarot) {
		.number = 2,
		.original = "High Priestess",
		.emulacrum = "Reflexia Void"
	};
	a->add(a, &entry);

	entry = (EmulacrumTarot) {
		.number = 14,
		.original = "Temperance",
		.emulacrum = "Meridian"
	};
	a->add(a, &entry);

	entry = (EmulacrumTarot) {
		.number = 0,
		.original = "The Fool",
		.emulacrum = "Zero"
	};
	a->add(a, &entry);

	entry = (EmulacrumTarot) {
		.number = 1,
		.original = "The Magician",
		.emulacrum = "The One"
	};
	a->add(a, &entry);

	entry = (EmulacrumTarot) {
		.number = 17,
		.original = "The Star",
		.emulacrum = "1337"
	};
	a->add(a, &entry);

	entry = (EmulacrumTarot) {
		.number = 16,
		.original = "The Tower",
		.emulacrum = "Fractura"
	};
	a->add(a, &entry);
	
	return a;
}

Array*/*EmulacrumTarot*/ 
form_sorted_by_emulacrum(Allocator *all) {
	Array *a = form_array(all, sizeof(EmulacrumTarot));

	EmulacrumTarot entry = (EmulacrumTarot) {
		.number = 17,
		.original = "The Star",
		.emulacrum = "1337"
	};
	a->add(a, &entry);

	entry = (EmulacrumTarot) {
		.number = 16,
		.original = "The Tower",
		.emulacrum = "Fractura"
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

	entry = (EmulacrumTarot) {
		.number = 0,
		.original = "The Fool",
		.emulacrum = "Zero"
	};
	a->add(a, &entry);
	
	return a;
}

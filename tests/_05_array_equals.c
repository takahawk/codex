#include "ds/array.h"
#include "testing/assert.h"

#include "allocators/debug_allocator.h"

typedef struct {
	uint16_t number;
	char *original;
	char *emulacrum;

} EmulacrumTarot;

Array*/*EmulacrumTarot*/ form_array_1(Allocator *a);
Array*/*EmulacrumTarot*/ form_array_2(Allocator *a);
Array*/*int*/            form_array_3(Allocator *a);


int main() {
	Allocator *a = form_debug_allocator(&std_allocator);
	DebugAllocatorCtx *allocCtx = a->ctx;
	Array *a1 = form_array_1(a);
	Array *a2 = form_array_2(a);
	Array *a3 = form_array_3(a);
	Array *a4 = form_array_1(a);
	Array *a5 = form_array_2(a);
	Array *a6 = form_array_3(a);

	assert_bool_equals(a1->equals(a1, a1), true);
	assert_bool_equals(a1->equals(a1, a2), false);
	assert_bool_equals(a1->equals(a1, a3), false);
	assert_bool_equals(a1->equals(a1, a4), true);

	assert_bool_equals(a2->equals(a2, a1), false);
	assert_bool_equals(a2->equals(a2 ,a2), true);
	assert_bool_equals(a2->equals(a2, a3), false);
	assert_bool_equals(a2->equals(a2, a5), true);

	assert_bool_equals(a3->equals(a3, a1), false);
	assert_bool_equals(a3->equals(a3 ,a2), false);
	assert_bool_equals(a3->equals(a3, a3), true);
	assert_bool_equals(a3->equals(a3, a6), true);

	a1->release(&a1);
	a2->release(&a2);
	a3->release(&a3);
	a4->release(&a4);
	a5->release(&a5);
	a6->release(&a6);

	if (allocCtx->allocations->len != 0) {
		allocCtx->print_allocations(allocCtx);
		return -1;
	}

	return 0;
}

Array*/*EmulacrumTarot*/ 
form_array_1(Allocator *all) {
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
	
	return a;
}

Array*/*EmulacrumTarot*/ 
form_array_2(Allocator *all) {
	Array *a = form_array(all, sizeof(EmulacrumTarot));

	EmulacrumTarot entry = {
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

Array*/*int*/
form_array_3(Allocator *all) {
	Array *a = form_array(all, sizeof(int));

	int entry = 0;
	a->add(a, &entry);
	entry = 1337;
	a->add(a, &entry);
	entry = 1;
	a->add(a, &entry);

	return a;	
}
#include "codex/ds/array.h"
#include "codex/testing/assert.h"

typedef struct {
	uint16_t number;
	char *original;
	char *emulacrum;

} EmulacrumTarot;

Array/*EmulacrumTarot*/ form_array_1();
Array/*EmulacrumTarot*/ form_array_2();
Array/*int*/            form_array_3();


int main() {
  // TODO: allocator
	Array a1 = form_array_1();
	Array a2 = form_array_2();
	Array a3 = form_array_3();
	Array a4 = form_array_1();
	Array a5 = form_array_2();
	Array a6 = form_array_3();

	assert_bool_equals(cdx_array_equals(a1, a1), true);
	assert_bool_equals(cdx_array_equals(a1, a2), false);
	assert_bool_equals(cdx_array_equals(a1, a3), false);
	assert_bool_equals(cdx_array_equals(a1, a4), true);

	assert_bool_equals(cdx_array_equals(a2, a1), false);
	assert_bool_equals(cdx_array_equals(a2 ,a2), true);
	assert_bool_equals(cdx_array_equals(a2, a3), false);
	assert_bool_equals(cdx_array_equals(a2, a5), true);

	assert_bool_equals(cdx_array_equals(a3, a1), false);
	assert_bool_equals(cdx_array_equals(a3 ,a2), false);
	assert_bool_equals(cdx_array_equals(a3, a3), true);
	assert_bool_equals(cdx_array_equals(a3, a6), true);

	cdx_array_release(&a1);
	cdx_array_release(&a2);
	cdx_array_release(&a3);
	cdx_array_release(&a4);
	cdx_array_release(&a5);
	cdx_array_release(&a6);

	return 0;
}

Array/*EmulacrumTarot*/ 
form_array_1() {
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
	
	return a;
}

Array/*EmulacrumTarot*/ 
form_array_2() {
	Array a = cdx_array_form(sizeof(EmulacrumTarot));

	EmulacrumTarot entry = {
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

Array/*int*/
form_array_3() {
	Array a = cdx_array_form(sizeof(int));

	int entry = 0;
	cdx_array_add(&a, &entry);
	entry = 1337;
	cdx_array_add(&a, &entry);
	entry = 1;
	cdx_array_add(&a, &entry);

	return a;
}

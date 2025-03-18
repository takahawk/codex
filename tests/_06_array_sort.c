#include "ds/array.h"

typedef struct {
	uint16_t number;
	char *original;
	char *emulacrum;

} EmulacrumTarot;

int main() {
	#if 0
	Array *a = form_array(sizeof)

	a->add(a, (EmulacrumTarot) {
		.number = 16,
		.original = "The Tower",
		.emulacrum = "Fractura"
	});

	a->add(a, (EmulacrumTarot) {
		.number = 0,
		.original = "The Fool",
		.emulacrum = "Zero"
	});

	a->add(a, (EmulacrumTarot) {
		.number = 17,
		.original = "The Star",
		.emulacrum = "1337"
	});

	a->add(a, (EmulacrumTarot) {
		.number = 14,
		.original = "Temperance",
		.emulacrum = "Meridian"
	});

	a->add(a, (EmulacrumTarot) {
		.number = 2,
		.original = "High Priestess",
		.emulacrum = "Reflexia Void"
	});

	a->add(a, (EmulacrumTarot) {
		.number = 1,
		.original = "The Magician",
		.emulacrum = "The One"
	});
	#endif
	return 0;
}


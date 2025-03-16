#include <string.h>
#include "../parsers/dotenv.h"

int main() {
	char *buffer = "Zero=TheOne\n"
				   "Architect=Galatea\n"
				   "Simulation=Emulation";
	Dotenv *dotenv = parse_dotenv(buffer);

	const char *val = dotenv->get_string(dotenv, "Zero");

	if (strcmp(val, "TheOne") != 0) {
		fprintf(stderr, "actual=%s expected=TheOne\n", val);
	}
	val = dotenv->get_string(dotenv, "Architect");

	if (strcmp(val, "Galatea") != 0) {
		fprintf(stderr, "actual=%s expected=Galatea\n", val);
	}
	val = dotenv->get_string(dotenv, "Simulation");

	if (strcmp(val, "Emulation") != 0) {
		fprintf(stderr, "actual=%s expected=Emulation\n", val);
	}
	return 0;
}
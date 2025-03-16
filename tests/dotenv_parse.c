#include <string.h>
#include "../parsers/dotenv.h"

int main() {
	char *buffer = "Zero=TheOne\n"
				   "Architect=Galatea\n"
				   "Simulation=Emulation";
	Dotenv *dotenv = parse_dotenv(buffer);

	Array *keys = dotenv->keys;
	Array *values = dotenv->values;

	char *key = *(char **) keys->get(keys, 0);
	if (strcmp(key, "Zero") != 0) {
		fprintf(stderr, "actual=%s expected=%s\n", key, "Zero");
		return -1;
	}

	key = *(char **) keys->get(keys, 1);
	if (strcmp(key, "Architect") != 0) {
		fprintf(stderr, "actual=%s expected=%s\n", key, "Architect");
		return -1;
	}

	key = *(char **) keys->get(keys, 2);
	if (strcmp(key, "Simulation") != 0) {
		fprintf(stderr, "actual=%s expected=%s\n", key, "Simulation");
		return -1;
	}

	char *value = *(char **) values->get(values, 0);
	if (strcmp(value, "TheOne") != 0) {
		fprintf(stderr, "actual=%s expected=%s\n", key, "TheOne");
		return -1;
	}

	value = *(char **) values->get(values, 1);
	if (strcmp(value, "Galatea") != 0) {
		fprintf(stderr, "actual=%s expected=%s\n", key, "Galatea");
		return -1;
	}

	value = *(char **) values->get(values, 2);
	if (strcmp(value, "Emulation") != 0) {
		fprintf(stderr, "actual=%s expected=%s\n", key, "Emulation");
		return -1;
	}

	return 0;
}
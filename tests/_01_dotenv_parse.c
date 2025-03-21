#include <string.h>
#include "allocators/allocator.h"
#include "parsers/dotenv.h"
#include "testing/assert.h"


int main() {
	Allocator a = form_std_allocator();
	char *buffer = "Zero=TheOne\n"
				   "Architect=Galatea\n"
				   "Simulation=Emulation";
	Dotenv *dotenv = parse_dotenv(&a, buffer);

	Array *keys = dotenv->keys;
	Array *values = dotenv->values;

	char *key = *(char **) keys->get(keys, 0);
	assert_str_equals(key, "Zero");

	key = *(char **) keys->get(keys, 1);
	assert_str_equals(key, "Architect");

	key = *(char **) keys->get(keys, 2);
	assert_str_equals(key, "Simulation");

	char *value = *(char **) values->get(values, 0);
	assert_str_equals(value, "TheOne");

	value = *(char **) values->get(values, 1);
	assert_str_equals(value, "Galatea");

	value = *(char **) values->get(values, 2);
	assert_str_equals(value, "Emulation");

	return 0;
}
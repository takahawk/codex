#include <string.h>
#include "../parsers/dotenv.h"
#include "../testing/assert.h"

int main() {
	char *buffer = "Zero=TheOne\n"
				   "Architect=Galatea\n"
				   "Simulation=Emulation";
	Dotenv *dotenv = parse_dotenv(buffer);

	const char *val = dotenv->get_string(dotenv, "Zero");
	assert_str_equals(val, "TheOne");

	val = dotenv->get_string(dotenv, "Architect");
	assert_str_equals(val, "Galatea");

	val = dotenv->get_string(dotenv, "Simulation");
	assert_str_equals(val, "Emulation");

	return 0;
}
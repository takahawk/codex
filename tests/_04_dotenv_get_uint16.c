#include <stdbool.h>
#include <string.h>
#include "parsers/dotenv.h"
#include "testing/assert.h"


int main() {
	Allocator a = form_std_allocator();
	char *buffer = "TheStar=1337\n"
				   "Feel=7331\n";
	Dotenv *dotenv = parse_dotenv(&a, buffer);

	uint16_t value = dotenv->get_uint16(dotenv, "TheStar");
	assert_uint16_equals(value, 1337);

	value = dotenv->get_uint16(dotenv, "Feel");
	assert_uint16_equals(value, 7331);
	return 0;
}
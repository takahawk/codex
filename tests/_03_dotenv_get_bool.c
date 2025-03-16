#include <stdbool.h>
#include <string.h>
#include "../parsers/dotenv.h"
#include "../testing/assert.h"

int main() {
	char *buffer = "Existence=true\n"
				   "NonExistence=false\n";
	Dotenv *dotenv = parse_dotenv(buffer);

	bool value = dotenv->get_bool(dotenv, "Existence");
	assert_bool_equals(value, true);

	value = dotenv->get_bool(dotenv, "NonExistence");
	assert_bool_equals(value, false);
	return 0;
}
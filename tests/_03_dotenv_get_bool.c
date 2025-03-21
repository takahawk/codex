#include <stdbool.h>
#include <string.h>

#include "allocators/allocator.h"
#include "parsers/dotenv.h"
#include "testing/assert.h"


int main() {
	Allocator a = form_std_allocator();
	char *buffer = "Existence=true\n"
				   "NonExistence=false\n";
	Dotenv *dotenv = parse_dotenv(&a, buffer);

	bool value = dotenv->get_bool(dotenv, "Existence");
	assert_bool_equals(value, true);

	value = dotenv->get_bool(dotenv, "NonExistence");
	assert_bool_equals(value, false);
	return 0;
}
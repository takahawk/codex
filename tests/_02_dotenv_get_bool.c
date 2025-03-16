#include <stdbool.h>
#include <string.h>
#include "../parsers/dotenv.h"

int main() {
	char *buffer = "Existence=true\n"
				   "NonExistence=false\n";
	Dotenv *dotenv = parse_dotenv(buffer);

	bool value = dotenv->get_bool(dotenv, "Existence");
	if (value != true) {
		fprintf(stderr, "Existence: actual=false expected=true\n");
		return -1;
	}
	value = dotenv->get_bool(dotenv, "NonExistence");
	if (value != false) {
		fprintf(stderr, "NonExistence: actual=true expected=false\n");
		return -1;
	}
	return 0;
}
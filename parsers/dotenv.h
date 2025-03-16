#ifndef PARSERS_DOTENV_H_
#define PARSERS_DOTENV_H_
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "../array.h"

typedef struct Dotenv {
	Array/*char**/ keys;
	Array/*char**/ values;

	void (*release) (struct Dotenv **pself);
} Dotenv;

static void
dotenv_release(Dotenv **pself) {
	Dotenv *self = *pself;
	Array *keys = self->keys;
	Array *values = self->values;
	keys->release(&keys);
	values->release(&values);
	free(self);
	*pself = NULL;
}

static Dotenv*
parse_dotenv(char *buffer) {
	// TODO: quotes
	// TODO: comments
	// TODO: whitespace
	// yeah... now we just want it dead simple
	enum {
		STATE_KEY, 
		STATE_VALUE
	} state;

	Array *keys = form_array(sizeof(char*));
	Array *values = form_array(sizeof(char*));
	char c;

	int j = 0;
	for (int i = 0; (c = buffer[i]) != '\0'; ++i) {
		switch (state) {
		case STATE_KEY:
			if ('=' == c) {
				char *newkey = malloc(i - j + 1);
				memcpy(newkey, buffer + j, i - j);
				newkey[i - j] = '\0';
				j = i + 1;
				keys->add(keys, &newkey);
				state = STATE_VALUE;
			} else if (c == '\n') {
				fprintf(stderr, "unexpected newline (index=%d)", i);
				exit(EXIT_FAILURE);
			} else if (isspace(c)) {
				fprintf(stderr, "spaces (index=%d) are not allowed for Proxima .env files", c, i);
				exit(EXIT_FAILURE);
			} else if (!isalnum(c)) {
				fprintf(stderr, "characted %c (index=%d) is not allowed for Proxima .env files", c, i);
				exit(EXIT_FAILURE);
			}
			break;
		case STATE_VALUE:
			if ('\n' == c) {
				char *newval = malloc(i - j + 1);
				memcpy(newval, buffer + j, i - j);
				newval[i - j] = '\0';
				j = i + 1;
				values->add(values, &newval);
				state = STATE_KEY;
			} else if (isspace(c)) {
				fprintf(stderr, "spaces (index=%d) are not allowed for Proxima .env files", c, i);
				exit(EXIT_FAILURE);
			} else if (!isalnum(c)) {
				fprintf(stderr, "characted %c (index=%d) is not allowed for Proxima .env files as part of value", c, i);
				exit(EXIT_FAILURE);
			}
			break;
		}
	}

	Dotenv *env = malloc(sizeof(Dotenv));
	env->keys = keys;
	env->values = values;

	env->release = dotenv_release;
	return env;
}

#endif
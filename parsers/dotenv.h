#ifndef PARSERS_DOTENV_H_
#define PARSERS_DOTENV_H_
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../array.h"

typedef struct Dotenv {
	// TODO: probably better to use tree... when we implement one in codex
	Array/*char**/ *keys;
	Array/*char**/ *values;

	const char* (*get_string) (struct Dotenv *self, const char *key);
	bool        (*get_bool)   (struct Dotenv *self, const char *key);

	void        (*release)    (struct Dotenv **pself);
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

static const char*
dotenv_get_string(Dotenv *self, const char *key) {
	Array *keys = self->keys;
	for (int i = 0; i < keys->len; ++i) {
		char *dotenv_key = *(char **) keys->get(keys, i);
		if (strcmp(key, dotenv_key) == 0) {
			const char *strval = *(char **) self->values->get(self->values, i);
			return strval;
		}
	}

	fprintf(stderr, "key not found \"%s\"\n", key);
	exit(EXIT_FAILURE);
}

static bool
dotenv_get_bool(Dotenv *self, const char *key) {
	const char *strval = self->get_string(self, key);
	if (strcmp(strval, "true") == 0)
		return true;
	if (strcmp(strval, "false") == 0)
		return false;

	// not a bool value
	fprintf(stderr, "\"%s\" is not a bool value (%s=%s)\n", strval, key, strval);
	exit(EXIT_FAILURE);
}


static Dotenv*
parse_dotenv(char *buffer) {
	// TODO: quotes
	// TODO: comments
	// TODO: whitespace
	// TODO: check for existing keys
	// yeah... now we just want it dead simple
	enum {
		STATE_KEY, 
		STATE_VALUE
	} state = STATE_KEY;

	Array *keys = form_array(sizeof(char*));
	Array *values = form_array(sizeof(char*));
	char c;

	int i = 0;
	int j = 0;
	for (; (c = buffer[i]) != '\0'; ++i) {
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
				fprintf(stderr, "spaces (index=%d) are not allowed for Proxima .env files", i);
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
				fprintf(stderr, "spaces (index=%d) are not allowed for Proxima .env files", i);
				exit(EXIT_FAILURE);
			} else if (!isalnum(c)) {
				fprintf(stderr, "characted %c (index=%d) is not allowed for Proxima .env files as part of value", c, i);
				exit(EXIT_FAILURE);
			}
			break;
		}
	}
	if (STATE_VALUE == state) {
		char *newval = malloc(i - j + 1);
		memcpy(newval, buffer + j, i - j);
		newval[i - j] = '\0';
		values->add(values, &newval);
	}

	Dotenv *env = malloc(sizeof(Dotenv));
	env->keys = keys;
	env->values = values;

	env->get_bool = dotenv_get_bool;
	env->release = dotenv_release;
	env->get_string = dotenv_get_string;
	return env;
}

#endif
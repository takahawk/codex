#include "dotenv.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



static void
dotenv_release(Dotenv **pself) {
	Dotenv *self = *pself;
	Allocator *a = self->a;
	Array *keys = self->keys;
	Array *values = self->values;
	keys->release(&keys);
	values->release(&values);
	a->free(a, self);
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

static uint16_t
dotenv_get_uint16(Dotenv *self, const char *key) {
	const char *strval = self->get_string(self, key);	
	char *endptr;

	long val = strtol(strval, &endptr, 10);

	if (*endptr != '\0') {
		fprintf(stderr, "value is not a number: %s\n", strval);
		exit(EXIT_FAILURE);
	}

	if (errno == ERANGE || val > 65535  || val < 0) {
		fprintf(stderr, "value is out of range (uint16 <- %d...%d): %s", 0, 65535, strval);
		exit(EXIT_FAILURE);
	}

	return (uint16_t) val;
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


const Dotenv DOTENV_PROTOTYPE = {
	.get_string = dotenv_get_string,
	.get_uint16 = dotenv_get_uint16,
	.get_bool = dotenv_get_bool,
	.release = dotenv_release 
};

Dotenv*
parse_dotenv(Allocator *a, const char *buffer) {
	// TODO: quotes
	// TODO: comments
	// TODO: whitespace
	// TODO: check for existing keys
	// yeah... now we just want it dead simple
	enum {
		STATE_KEY, 
		STATE_VALUE
	} state = STATE_KEY;

	Array *keys = form_array(a, sizeof(char*));
	keys->item_release = JUST_FREE_IT;
	Array *values = form_array(a, sizeof(char*));
	values->item_release = JUST_FREE_IT;
	char c;

	int i = 0;
	int j = 0;
	for (; (c = buffer[i]) != '\0'; ++i) {
		switch (state) {
		case STATE_KEY:
			if ('=' == c) {
				char *newkey = a->alloc(a, i - j + 1);
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
				char *newval = a->alloc(a, i - j + 1);
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
		char *newval = a->alloc(a, i - j + 1);
		memcpy(newval, buffer + j, i - j);
		newval[i - j] = '\0';
		values->add(values, &newval);
	}

	Dotenv *env = a->alloc(a, sizeof(Dotenv));
	*env = DOTENV_PROTOTYPE;
	env->a = a;
	env->keys = keys;
	env->values = values;

	return env;
}

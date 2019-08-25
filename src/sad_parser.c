#include <stdio.h>
#include <stdbool.h>
#include "sad.h"
#include "cutils/PS.h"

// Internal functions

static inline struct Sad Sad_parse(PS ps);

// Parse dict value, c must be '{'
static SadDict Sad_parseDict(PS ps);

// Parse array value, c must be '['
static SadArray Sad_parseArray(PS ps);

// Parse string value, c must be '\''
static Str Sad_parseString(PS ps, int quote);

// Parse sad entry, if there is no (more) entry then returned
// entry's key is NULL
static struct SadEntry Sad_parseEntry(PS ps);

// Parse sad value, if there is no (more) value then returned
// value.is = 0
static struct Sad Sad_parseValue(PS ps, int ad);

// Parse a entry key
// key's length is 0 if no more key left
static Str Sad_parseKey(PS ps);

// Skip whict spaces
static inline void PS_skipWhitespace(PS ps);

// Public API Implementation

struct Sad Sad_fromFILE(FILE *file)
{
	return  Sad_parse(PS_newFromFILE(file, PS_SOURCE_ASCII));
}

struct Sad Sad_fromCStr(const char *cstr)
{
	return  Sad_parse(PS_newFromCStr(cstr, PS_SOURCE_ASCII));
}

struct Sad Sad_fromStr(Str str)
{
	return  Sad_parse(PS_newFromStr(str, PS_SOURCE_ASCII));
}

// Internal functions implementation

// TODO error checking and reporting
static inline struct Sad Sad_parse(PS ps)
{
	struct Sad sad = { .type = SAD_UNKNOWN_VALUE_TYPE };
	if (ps == NULL) {
		return sad;
	}
	sad = Sad_parseValue(ps, PS_END);
	PS_free(ps);
	return sad;
}

static struct Sad Sad_parseValue(PS ps, int ad)
{
	struct Sad v = { .type = 0 };
	PS_skipWhitespace(ps);
	int c = PS_curChar(ps);
	switch (c) {
	case '\'': case '\"': // S
		v.str = Sad_parseString(ps, c);
		v.type = SAD_STRING;
		break;
	case '[': // A
		v.array = Sad_parseArray(ps);
		v.type = SAD_ARRAY;
		break;
	case '{': // D
		v.dict = Sad_parseDict(ps);
		v.type = SAD_DICT;
		break;
	default:
		if (c != ad) {
			PS_setErrorCode(ps, SAD_BAD_FORMAT);
		} else {
			// array or dict closing
			PS_next(ps);
		}
		break;
	}
	return v;
}

static SadDict Sad_parseDict(PS ps)
{
	SadDict vd = SadDict_newWithCapacity(0);
	PS_next(ps); // pass '{'
	for (struct SadEntry e = Sad_parseEntry(ps);
	     e.key != NULL;
	     e = Sad_parseEntry(ps)) {
		SadDict_push(vd, e);
	}
	if (PS_getErrorCode(ps)) {
		SadDict_destroy(vd);
		vd = NULL;
	}
	return vd;
}

static SadArray Sad_parseArray(PS ps)
{
	SadArray va = SadArray_newWithCapacity(0);
	PS_next(ps); // pass '['
	for (struct Sad v = Sad_parseValue(ps, ']');
	     v.type != SAD_UNKNOWN_VALUE_TYPE;
	     v = Sad_parseValue(ps, ']')) {
		SadArray_push(va, v);
	}
	if (PS_getErrorCode(ps)) {
		SadArray_destroy(va);
		va = NULL;
	}
	return va;
}

static Str Sad_parseString(PS ps, int quote)
{
	Str vs = Str_newWithCapacity(0);
	int c;
again:
	PS_next(ps);
	c = PS_curChar(ps);
	switch (c) {
	case '\\':
		PS_next(ps);
		c = PS_curChar(ps);
		switch (c) {
		case 't':
			Str_push(vs, '\t');
			break;
		case 'v':
			Str_push(vs, '\v');
			break;
		case 'n':
			Str_push(vs, '\n');
			break;
		case 'a':
			Str_push(vs, '\a');
			break;
		case PS_END:
			goto on_error;
		default:
			Str_push(vs, c);
			break;
		}
		goto again;
	case '\'': case '\"':
		if (quote == c)
			break;
		Str_push(vs, c);
		goto again;
	case -1:
		goto on_error;
	default:
		Str_push(vs, c);
		goto again;
	}
	PS_next(ps); // pass ' or "
	return vs;

on_error:
	PS_setErrorCode(ps, true);
	Str_free(vs);
	return NULL;
}

static Str Sad_parseKey(PS ps)
{
	PS_skipWhitespace(ps);

	Str key = Str_newWithCapacity(0);

	int c = PS_curChar(ps);
	while (c >= 'a' && c <= 'z' ||
	       c >= 'A' && c <= 'Z' ||
	       c >= '0' && c <= '9' ||
	       c == '_' || c == '-') {
		Str_push(key, c);
		PS_next(ps);
		c = PS_curChar(ps);
	}
	return key;
}

static struct SadEntry Sad_parseEntry(PS ps)
{
	struct SadEntry entry = { .key = NULL };

	entry.key = Sad_parseKey(ps);
	if (Str_getLength(entry.key) == 0) {
		if (PS_curChar(ps) == '}') {
			PS_next(ps);
		}
		goto end;
	}

	entry.value = Sad_parseValue(ps, '}');
	if (entry.value.type == SAD_UNKNOWN_VALUE_TYPE) {
		goto end;
	}

	return entry;
end:
	Str_free(entry.key);
	entry.key = NULL;
	Sad_destroy(&entry.value);
	return entry;
}

static inline void PS_skipWhitespace(PS ps)
{
	int c;
again:
	c = PS_curChar(ps);
	switch (c) {
	case ' ': case '\t': case '\v': case '\n': case '\r':
		PS_next(ps);
		goto again;
	}
}


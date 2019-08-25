#include "sad.h"

static void append_str(Str dst, Str s);
static void append_array(Str dst, SadArray a, int64_t indent);
static void append_dict(Str dst, SadDict d, int64_t indent);
static void append_value(Str dst, struct Sad *v, int64_t indent);
static void append_indents(Str dst, int64_t indents);

void Sad_toStr(struct Sad *self, Str str)
{
	Str_clear(str);
	append_value(str, self, 0);
}

static void append_str(Str dst, Str s)
{
	size_t len = Str_getLength(s);
	Str_push(dst, '\"');
	for (size_t i = 0; i < len; i++) {
		uint8_t c = Str_get(s, i);
		switch (c) {
		case '\\':
			Str_appendArray(dst, "\\\\", 2);
			break;
		case '\'':
			Str_appendArray(dst, "\\\'", 2);
			break;
		case '\"':
			Str_appendArray(dst, "\\\"", 2);
			break;
		case '\t':
			Str_appendArray(dst, "\\\t", 2);
			break;
		case '\v':
			Str_appendArray(dst, "\\\v", 2);
			break;
		case '\n':
			Str_appendArray(dst, "\\\n", 2);
			break;
		case '\r':
			Str_appendArray(dst, "\\\r", 2);
			break;
		case '\a':
			Str_appendArray(dst, "\\\a", 2);
			break;
		default:
			Str_push(dst, c);
			break;
		}
	}
	Str_push(dst, '\"');
}

static void append_array(Str dst, SadArray a, int64_t indent)
{
	size_t len = SadArray_getLength(a);
	int64_t new_indent = indent >= 0 ? indent + 1 : 0;
	Str_push(dst, '[');
	for (size_t i = 0; i < len; i++) {
		struct Sad v = SadArray_get(a, i);
		if (v.type == SAD_UNKNOWN_VALUE_TYPE)
			continue;
		append_indents(dst, new_indent);
		append_value(dst, &v, new_indent);
	}
	append_indents(dst, indent);
	Str_push(dst, ']');
}

static void append_dict(Str dst, SadDict d, int64_t indent)
{
	size_t len = SadDict_getLength(d);
	int64_t new_indent = indent >= 0 ? indent + 1 : 0;
	Str_push(dst, '{');
	for (size_t i = 0; i < len; i++) {
		struct SadEntry e = SadDict_get(d, i);
		if (e.key == NULL || e.value.type == SAD_UNKNOWN_VALUE_TYPE)
			continue;
		append_indents(dst, new_indent);
		Str_append(dst, e.key);
		Str_push(dst, ' ');
		append_value(dst, &e.value, new_indent);
	}
	append_indents(dst, indent);
	Str_push(dst, '}');
}

static void append_value(Str dst, struct Sad *v, int64_t indent)
{
	switch (v->type) {
	case SAD_STRING:
		append_str(dst, v->str);
		break;
	case SAD_ARRAY:
		append_array(dst, v->array, indent);
		break;
	case SAD_DICT:
		append_dict(dst, v->dict, indent);
		break;
	}
}

static void append_indents(Str dst, int64_t indents)
{
	if (indents >= 0) {
		Str_push(dst, '\n');
		for (size_t i = 0; i < indents; i++) {
			Str_push(dst, '\t');
		}
	}
}


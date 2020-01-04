// sad: a simple data(String, Array, Dict) file format
// Copyright (C) 2020  Arch Strange <arch.strange@foxmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#include "write.h"

static void write_indents(Str dst, int64_t indents);
static inline void Sad_writeValue(struct Sad sad, Str dst, int64_t indent);
static void Sad_writeString(Str string, Str dst);
static void Sad_writeArray(SadArray array, Str dst, int64_t indent);

void Sad_writeToStr(struct Sad sad, Str dst, bool pretty)
{
	Str_clear(dst);
	Sad_writeValue(sad, dst, pretty ? 0 : -1);
}

static void Sad_writeString(Str string, Str dst)
{
	size_t len = Str_getLength(string);
	const uint8_t *s = Str_data(string);

	Str_push(dst, '\"');
	for (size_t i = 0; i < len; i++) {
		uint8_t c = s[i];
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

static void Sad_writeArray(SadArray array, Str dst, int64_t indent)
{
	size_t len = SadArray_getLength(array);
	const struct Sad *a = SadArray_data(array);

	int64_t new_indent = indent >= 0 ? indent + 1 : 0;
	Str_push(dst, '[');
	for (size_t i = 0; i < len; i++) {
		if (a[i].type == SAD_INVALID)
			continue;
		write_indents(dst, new_indent);
		Sad_writeValue(a[i], dst, new_indent);
	}
	write_indents(dst, indent);
	Str_push(dst, ']');
}

static void Sad_writeDict(SadDict d, Str dst, int64_t indent)
{
	size_t n = SadDict_getLength(d);
	StrVector keys = StrVector_newWithCapacity(n);
	SadArray values = SadArray_newWithCapacity(n);
	SadDict_getKeys(d, keys);
	SadDict_getValues(d, values);
	Str *ks = StrVector_data(keys);
	struct Sad *vs = SadArray_data(values);

	int64_t new_indent = indent >= 0 ? indent + 1 : 0;
	Str_push(dst, '{');
	for (size_t i = 0; i < n; i++) {
		if (ks[i] == NULL || vs[i].type == SAD_INVALID)
			continue;
		write_indents(dst, new_indent);
		Str_append(dst, ks[i]);
		Str_push(dst, ' ');
		Sad_writeValue(vs[i], dst, new_indent);
	}
	write_indents(dst, indent);
	Str_push(dst, '}');
}

static inline void Sad_writeValue(struct Sad v, Str dst, int64_t indent)
{
	switch (v.type) {
	case SAD_STRING:
		Sad_writeString(v.str, dst);
		break;
	case SAD_ARRAY:
		Sad_writeArray(v.array, dst, indent);
		break;
	case SAD_DICT:
		Sad_writeDict(v.dict, dst, indent);
		break;
	}
}

static void write_indents(Str dst, int64_t indents)
{
	if (indents >= 0) {
		Str_push(dst, '\n');
		for (size_t i = 0; i < indents; i++) {
			Str_push(dst, '\t');
		}
	}
}


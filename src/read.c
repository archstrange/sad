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
#include "read.h"
#include "SadArray.h"
#include "SadDict.h"
#include <cutils/LS.h>

enum SL_TTYPE {
	TT_EOS = -1, // end of LS, or error token
	TT_STRING = 128,
	TT_KEY,
	// others: [, ], {, }
};

typedef struct {
	LS ls;
	int ttype; // current token type
	Str token; // current token string
	size_t trow, tcol; // token position
} SL; // struct Sad Lexer

static inline int SL_updateChar(SL *self);
static int SL_nextToken(SL *self);
static void SL_nextString(SL *self);
static void SL_nextKey(SL *self);
static struct Sad Sad_read(SL *sl);

static SadDict Sad_readDict(SL *self);
static SadArray Sad_readArray(SL *self);

struct Sad Sad_readFile(FILE *fp)
{
	SL sl = {
		.ls = LS_newFileSource(fp, false, false, NULL, NULL),
		.token = Str_new(),
	};
	if (sl.ls == NULL) return SAD_DEFAULT_VALUE();
	return Sad_read(&sl);
}

struct Sad Sad_readStr(Str str)
{
	SL sl = {
		.ls = LS_newStrSource(str, false),
		.token = Str_new(),
	};
	if (sl.ls == NULL) return SAD_DEFAULT_VALUE();
	return Sad_read(&sl);
}

struct Sad Sad_readCStr(const char *cstr)
{
	SL sl = {
		.ls = LS_newCStrSource(cstr, false),
		.token = Str_new(),
	};
	if (sl.ls == NULL) return SAD_DEFAULT_VALUE();
	return Sad_read(&sl);
}

static inline int SL_updateChar(SL *self)
{
	LS_position(self->ls, NULL, &self->trow, &self->tcol);
	return LS_char(self->ls);
}

static int SL_nextToken(SL *self)
{
	int c;
again:
	c = SL_updateChar(self);
	switch (c) {
	case '\t': case '\n': case '\r':
	case '\v': case ' ':
		LS_next(self->ls);
		goto again;
	case '[': case ']':
	case '{': case '}':
		self->ttype = c;
		LS_next(self->ls);
		return c;
	case '\'': case '\"':
		SL_nextString(self);
		break;
	default:
		SL_nextKey(self);
		break;
	}
	return self->ttype;
}

static void SL_nextString(SL *self)
{
	int quote = LS_char(self->ls),
	    c;
	Str_clear(self->token);
	LS_next(self->ls);
again:
	c = SL_updateChar(self);
	LS_next(self->ls);
	switch (c) {
	case '\\':
		c = SL_updateChar(self);
		LS_next(self->ls);
		switch (c) {
		case 't':
			Str_push(self->token, '\t');
			break;
		case 'v':
			Str_push(self->token, '\v');
			break;
		case 'n':
			Str_push(self->token, '\n');
			break;
		default:
			if (c < 0) goto endls;
			Str_push(self->token, c);
			break;
		}
		goto again;
	default:
		if (c == quote) break;
		else if (c < 0) goto endls;
		Str_push(self->token, c);
		goto again;
	}

	self->ttype = TT_STRING;
	return;
endls:
	self->ttype = TT_EOS;
}

static void SL_nextKey(SL *self)
{
	int c;
	Str_clear(self->token);
again:
	c = SL_updateChar(self);
	LS_next(self->ls);
	if (c >= 'a' && c <= 'z' ||
	    c >= 'A' && c <= 'Z' ||
	    c >= '0' && c <= '9' ||
	    c == '-' || c == '_') {
		Str_push(self->token, c);
		goto again;
	} else if (Str_getLength(self->token) == 0) {
		self->ttype = TT_EOS;
		return;
	}
	self->ttype = TT_KEY;
}

static struct Sad Sad_read(SL *sl)
{
	struct Sad sad = SAD_DEFAULT_VALUE();
	SadArray a;
	SadDict d;

	switch (SL_nextToken(sl)) {
	case TT_STRING:
		sad = SAD_STRING_VALUE(Str_clone(sl->token));
		break;
	case '[':
		a = Sad_readArray(sl);
		if (a != NULL) {
			sad = SAD_ARRAY_VALUE(a);
		}
		break;
	case '{':
		d = Sad_readDict(sl);
		if (d != NULL) {
			sad = SAD_DICT_VALUE(d);
		}
		break;
	}
	return sad;
}

static SadArray Sad_readArray(SL *self)
{
	SadArray array = SadArray_new();
	struct Sad sad;

	while ((sad = Sad_read(self)).type != SAD_INVALID) {
		SadArray_push(array, sad);
	}
	if (self->ttype != ']') {
		SadArray_free(array);
		array = NULL;
	}
	return array;
}

static SadDict Sad_readDict(SL *self)
{
	struct Sad sad;
	Str key = Str_new();
	SadDict dict = SadDict_new();

	while (SL_nextToken(self) == TT_KEY) {
		Str_copy(key, self->token);
		sad = Sad_read(self);
		if (sad.type == SAD_INVALID) {
			goto onError;
		}
		SadDict_insert(dict, key, sad);
	}

	if (self->ttype != '}') {
		goto onError;
	}
	Str_free(key);
	return dict;
onError:
	Str_free(key);
	SadDict_free(dict);
	return NULL;
}


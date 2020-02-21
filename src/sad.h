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
#ifndef SAD_SAD_H
#define SAD_SAD_H
#include <cutils/Str.h>
#include "SadArray.h"
#include "SadDict.h"

enum SadValueType {
	SAD_INVALID = 0,
	SAD_STRING,
	SAD_ARRAY,
	SAD_DICT,
};

struct Sad {
	int type;
	union {
		Str str;
		SadArray array;
		SadDict dict;
	};
};

static inline struct Sad SAD_DEFAULT_VALUE();
static inline struct Sad SAD_STRING_VALUE(Str s);
static inline struct Sad SAD_ARRAY_VALUE(SadArray a);
static inline struct Sad SAD_DICT_VALUE(SadDict d);
static inline struct Sad Sad_clone(struct Sad src);
static inline void Sad_copy(struct Sad self, struct Sad src);
static inline void Sad_free(struct Sad self);

/// Generic Dict insert macro func
/// @value type:
///   - use copy:
///     - Str *
///     - SadArray *
///     - SadDict *
///   - use assign:
///     - Str
///     - SadArray
///     - SadDict
#define SadDict_insertValue(sd, key, value) \
	__SadDict_insertg(sd, key, \
			 _Generic(value, \
				  Str: SAD_STRING, \
				  SadArray: SAD_ARRAY, \
				  SadDict: SAD_DICT, \
				  Str *: -SAD_STRING, \
				  SadArray *: -SAD_ARRAY, \
				  SadDict *: -SAD_DICT), \
			 value)

/// Generic Array set macro func
/// @value type:
///   - use copy:
///     - Str *
///     - SadArray *
///     - SadDict *
///   - use assign:
///     - Str
///     - SadArray
///     - SadDict
#define SadArray_setValue(sa, index, value) \
	__SadArray_setg(sa, index, \
			 _Generic(value, \
				  Str: SAD_STRING, \
				  SadArray: SAD_ARRAY, \
				  SadDict: SAD_DICT, \
				  Str *: -SAD_STRING, \
				  SadArray *: -SAD_ARRAY, \
				  SadDict *: -SAD_DICT), \
			 value)

/// Generic Array push macro func
/// @value type:
///   - use copy:
///     - Str *
///     - SadArray *
///     - SadDict *
///   - use assign:
///     - Str
///     - SadArray
///     - SadDict
#define SadArray_pushValue(sa, value) \
	__SadArray_pushg(sa, \
			 _Generic(value, \
				  Str: SAD_STRING, \
				  SadArray: SAD_ARRAY, \
				  SadDict: SAD_DICT, \
				  Str *: -SAD_STRING, \
				  SadArray *: -SAD_ARRAY, \
				  SadDict *: -SAD_DICT), \
			 value)

///////////////////////////////////////////////////////////////////////////////
// inline methods impl
///////////////////////////////////////////////////////////////////////////////

static inline struct Sad SAD_DEFAULT_VALUE()
{
	return (struct Sad){ .type = SAD_INVALID };
}

static inline struct Sad SAD_STRING_VALUE(Str s)
{
	return (struct Sad){ .type = SAD_STRING, .str = s };
}

static inline struct Sad SAD_ARRAY_VALUE(SadArray a)
{
	return (struct Sad){ .type = SAD_ARRAY, .array = a };
}

static inline struct Sad SAD_DICT_VALUE(SadDict d)
{
	return (struct Sad){ .type = SAD_DICT, .dict = d };
}

static inline struct Sad Sad_clone(struct Sad src)
{
	struct Sad self = SAD_DEFAULT_VALUE();
	switch (src.type) {
	case SAD_STRING:
		self.str = Str_clone(src.str);
		break;
	case SAD_ARRAY:
		self.array = SadArray_clone(src.array);
		break;
	case SAD_DICT:
		self.dict = SadDict_clone(src.dict);
		break;
	}
	return self;
}

static inline void Sad_copy(struct Sad self, struct Sad src)
{
	if (self.type != src.type) {
		Sad_free(self);
		self = Sad_clone(src);
		return;
	}
	switch (src.type) {
	case SAD_STRING:
		Str_copy(self.str, src.str);
		break;
	case SAD_ARRAY:
		SadArray_copy(self.array, src.array);
		break;
	case SAD_DICT:
		SadDict_copy(self.dict, src.dict);
		break;
	}
}

static inline void Sad_free(struct Sad self)
{
	switch (self.type) {
	case SAD_STRING:
		Str_free(self.str);
		break;
	case SAD_ARRAY:
		SadArray_free(self.array);
		break;
	case SAD_DICT:
		SadDict_free(self.dict);
		break;
	}
	self.type = SAD_INVALID;
}

#define __gSad(type, ptr) \
	struct Sad sad = { 0 }; \
	switch (sad.type = type) { \
	case SAD_STRING: \
	case SAD_ARRAY: \
	case SAD_DICT: \
		sad.dict = ptr; \
		break; \
	case -SAD_STRING: \
	case -SAD_ARRAY: \
	case -SAD_DICT: \
		sad.dict = *((SadDict *)ptr); \
		break; \
	}

static inline void __SadDict_insertg(SadDict sd,
				     Str key,
				     int type,
				     void *ptr)
{
	__gSad(type, ptr);

	if (type < 0) { // use copy
		sad.type = -type;
		SadDict_insert(sd, key, &sad);
	} else {
		SadDict_insert(sd, key, sad);
	}
}

static inline void __SadArray_setg(SadArray sa,
				   uint64_t index,
				   int type,
				   void *ptr)
{
	__gSad(type, ptr);

	if (type < 0) { // use copy
		sad.type = -type;
		SadArray_set(sa, index, &sad);
	} else {
		SadArray_set(sa, index, sad);
	}
}

static inline size_t __SadArray_pushg(SadArray sa, int type, void *ptr)
{
	__gSad(type, ptr);

	if (type < 0) { // use copy
		sad.type = -type;
		return SadArray_push(sa, &sad);
	} else {
		return SadArray_push(sa, sad);
	}
}

#endif


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

#endif


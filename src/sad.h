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


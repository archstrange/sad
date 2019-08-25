#ifndef OHCE_SAD_H
#define OHCE_SAD_H
#include <stdbool.h>
#include <stdio.h>

#include "cutils/Vector/Str.h"
#include "SadArray.h"
#include "SadDict.h"

enum {
	SAD_BAD_FORMAT = -1,
	SAD_UNKNOWN_VALUE_TYPE = 0,
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
	bool dict_sorted;
};

struct SadEntry {
	Str key;
	struct Sad value;
};

struct Sad Sad_create(int type);
void Sad_destroy(struct Sad *self);

void SadArray_destroy(SadArray array);
void SadDict_destroy(SadDict dict);
void SadEntry_destroy(struct SadEntry *entry);

/// Get sad value specified by @key
struct Sad SadDict_getValue(SadDict dict, Str key);

/// Set sad value specified by @key or add it if @key doesn't exist.
void SadDict_setValue(SadDict dict, Str key, struct Sad value);

void Sad_toStr(struct Sad *self, Str string);

int SadEntry_cmp(const void *l, const void *r);

struct Sad Sad_fromFILE(FILE *file);
struct Sad Sad_fromStr(Str str);
struct Sad Sad_fromCStr(const char *cstr);

#endif


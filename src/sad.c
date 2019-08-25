#include "sad.h"
#include <string.h>

struct Sad Sad_create(int type)
{
	struct Sad sad = { .type = type };
	switch (type) {
	case SAD_STRING:
		sad.str = Str_newWithCapacity(0);
		break;
	case SAD_ARRAY:
		sad.array = SadArray_newWithCapacity(0);
		break;
	case SAD_DICT:
		sad.dict = SadDict_newWithCapacity(0);
		break;
	default:
		sad.type = SAD_UNKNOWN_VALUE_TYPE;
		break;
	}
	return sad;
}

void Sad_destroy(struct Sad *self)
{
	switch (self->type) {
	case SAD_STRING:
		Str_free(self->str);
		break;
	case SAD_ARRAY:
		SadArray_destroy(self->array);
		break;
	case SAD_DICT:
		SadDict_destroy(self->dict);
		break;
	}
	memset(self, 0, sizeof(*self));
}

int SadEntry_cmp(const void *l, const void *r)
{
	int ret;
	const struct SadEntry *lhs = l, *rhs = r;

	Str_push(lhs->key, 0);
	Str_push(rhs->key, 0);

	ret = strcmp(Str_data(lhs->key), Str_data(rhs->key));

	Str_pop(lhs->key);
	Str_pop(rhs->key);
	return ret;
}

void SadEntry_destroy(struct SadEntry *entry)
{
	Str_free(entry->key);
	Sad_destroy(&entry->value);
}

void SadArray_destroy(SadArray array)
{
	size_t length = SadArray_getLength(array);
	struct Sad *data = SadArray_data(array);
	for (size_t i = 0; i < length; i++) {
		Sad_destroy(data + i);
	}
	SadArray_free(array);
}

void SadDict_destroy(SadDict dict)
{
	size_t length = SadDict_getLength(dict);
	struct SadEntry *data = SadDict_data(dict);
	for (size_t i = 0; i < length; i++) {
		SadEntry_destroy(data + i);
	}
	SadDict_free(dict);
}

static int Str_cmp(Str l, Str r)
{
	const uint8_t *ls = Str_data(l);
	const uint8_t *rs = Str_data(r);
	size_t ls_len = Str_getLength(l);
	size_t rs_len = Str_getLength(r);
	size_t i = 0;
	for (size_t i = 0; i < ls_len && i < rs_len; i++) {
		int sub = (int)ls[i] - (int)rs[i];
		if (sub != 0)
			return sub;
	}
	if (ls_len < rs_len) {
		return -1;
	}
	return ls_len - rs_len;
}

struct Sad SadDict_getValue(SadDict dict, Str key)
{
	// TODO use better algo
	const struct SadEntry *entries = SadDict_data(dict);
	size_t len = SadDict_getLength(dict);
	for (size_t i = 0; i < len; i++) {
		if (Str_cmp(entries[i].key, key) == 0)
			return entries[i].value;
	}
	return (struct Sad){ .type = SAD_UNKNOWN_VALUE_TYPE };
}

void SadDict_setValue(SadDict dict, Str key, struct Sad value)
{
	// TODO use better algo
	struct SadEntry *entries = SadDict_data(dict);
	size_t len = SadDict_getLength(dict);
	for (size_t i = 0; i < len; i++) {
		if (Str_cmp(entries[i].key, key) == 0) {
			Sad_destroy(&entries[i].value);
			entries[i].value = value;
			return;
		}
	}
	struct SadEntry e = { .key = Str_clone(key), .value = value };
	SadDict_push(dict, e);
}


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
#include <stdio.h>
#include "../src/sad.h"
#include "../src/read.h"
#include "../src/write.h"

void generic_funcs_test()
{
	SadDict sd = SadDict_new();
	SadArray sa = SadArray_new();
	Str key = Str_newFromCStr("key");
	SadDict_insertValue(sd, key, &key);
	SadDict_insertValue(sd, key, &sa);
	SadDict_insertValue(sd, key, Str_new());
	SadDict_insertValue(sd, key, SadArray_new());
	SadDict_insertValue(sd, key, SadDict_new());

	SadArray_pushValue(sa, &key);
	SadArray_pushValue(sa, Str_new());
	SadArray_pushValue(sa, SadArray_new());
	SadArray_pushValue(sa, SadDict_new());
	SadArray_setValue(sa, 0, Str_new());
	SadArray_setValue(sa, 1, SadArray_new());
	SadArray_setValue(sa, 2, SadDict_new());
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Test sad use: %s file.sad\n", argv[0]);
		return 0;
	}

	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL) {
		fprintf(stderr, "Failed to open file %s\n", argv[1]);
		return 1;
	}

	struct Sad sad = Sad_readFile(fp);

	fclose(fp);

	if (sad.type == SAD_INVALID) {
		fprintf(stderr, "%s is a bad format sad file\n", argv[1]);
		return 1;
	}

	Str str = Str_newWithCapacity(0);
	Sad_writeToStr(sad, str, true);

	size_t len = Str_getLength(str);
	if (len == 0) {
		fprintf(stderr, "sad to string failed\n");
		return 1;
	}

	Str_echo(str, stdout);

	Str_free(str);
	Sad_free(sad);
	return 0;
}

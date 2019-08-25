#include <stdio.h>
#include "../src/sad.h"

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

	struct Sad sad = Sad_fromFILE(fp);
	if (sad.type == SAD_UNKNOWN_VALUE_TYPE) {
		fprintf(stderr, "%s is a bad format sad file\n", argv[1]);
		return 1;
	}

	Str str = Str_newWithCapacity(0);
	Sad_toStr(&sad, str);

	size_t len = Str_getLength(str);
	if (len == 0) {
		fprintf(stderr, "sad to string failed\n");
		return 1;
	}

	unsigned char cstr[len + 1];
	Str_dump(str, cstr);
	cstr[len] = 0;
	printf("%s\n", cstr);

	Str_free(str);
	Sad_destroy(&sad);
	fclose(fp);
	return 0;
}

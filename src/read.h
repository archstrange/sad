#ifndef SAD_READ_H
#define SAD_READ_H
#include "sad.h"
#include <stdio.h>
#include <cutils/Str.h>

struct Sad Sad_readFile(FILE *fp);

struct Sad Sad_readStr(Str str);

struct Sad Sad_readCStr(const char *cstr);

#endif


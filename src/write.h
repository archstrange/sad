#ifndef SAD_WRITE_H
#define SAD_WRITE_H
#include "sad.h"
#include <stdio.h>

void Sad_writeToStr(struct Sad sad, Str dst, bool pretty);

static inline void Sad_writeToFile(struct Sad sad, FILE *fp, bool pretty)
{
	Str str = Str_new();
	Sad_writeToStr(sad, str, pretty);
	Str_print(str, fp);
}


#endif


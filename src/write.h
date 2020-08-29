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
	Str_free(str);
}


#endif


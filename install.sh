#!/bin/bash

if [ -z "$1" ]; then
	install_dir=/usr/local
else
	install_dir=$1
fi

include=$install_dir/include/sad
lib=$install_dir/lib

if [ -d $include ]; then
	rm -rf $include
fi
mkdir -p $include
mkdir -p $lib
cp -v src/sad.h src/SadArray.h src/SadDict.h $include/
cp -v build/src/libsad.a $lib/

#!/bin/bash

BUILD_DIR=bin

CLEAN_AND_CMAKE=0

while getopts "c" OPT; do
	case $OPT in
		c) CLEAN_AND_CMAKE=1
	esac
done

if [[ $CLEAN_AND_CMAKE == 1 ]]; then
	rm -rf $BUILD_DIR
	cmake -B $BUILD_DIR -S . -DTARGET=server
fi

make -C $BUILD_DIR firmware
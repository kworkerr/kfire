#!/bin/sh

CC=${CC:-cc}

TARGET=kfire

$CC src/* -o $TARGET

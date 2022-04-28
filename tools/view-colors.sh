#!/bin/sh
grep _RGB keymap.c | grep '^#define COLOR' | while read _ name c; do echo $name; pastel color $c; done

#!/bin/bash
set -eu
src="q5.c"
exec="q5"
rm -f $exec
gcc $src -o $exec -std=c99
./q5

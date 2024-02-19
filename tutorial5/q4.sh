#!/bin/bash
set -eu
src="q4.c"
exec="q4"
rm -f $exec
gcc $src -o $exec -std=c99
./"$exec"

#!/bin/bash
set -eu
src="q3.c"
exec="q3"
rm -f $exec
gcc $src -o $exec -std=c99
./"$exec"

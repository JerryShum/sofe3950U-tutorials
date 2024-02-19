#!/bin/bash
set -eu
src="q2.c"
exec="q2"
rm -f $exec
gcc $src -o $exec -std=c99
./"$exec"

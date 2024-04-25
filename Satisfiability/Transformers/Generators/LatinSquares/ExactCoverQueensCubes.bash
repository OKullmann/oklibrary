#!/usr/bin/env bash
# Oliver Kullmann, 25.4.2024 (Swansea)
# Copyright 2024 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version. */

set -o errexit
set -o nounset

program=$(basename "$0")
errorm="ERROR[${program}]:"
version=0.1.1

if (( $# != 1 )); then
  >&2 echo "$errorm Exactly one parameter (N) needed, but $# provided."
  exit 1
fi

N="$1"

if ! [ "$N" -gt 0 ] 2>/dev/null; then
  >&2 echo -e "$errorm Argument \"$N\" is not a positive natural number."
  exit 1
fi

output=$(CPandiagonal $N "" | clasp 0 | passextractpos.awk | Sort | CP_clasp_first_columns.awk -v N=$N -v mode=1 | ./ExactCoverQueensCubes | tail -1)
outputarr=($output)
if [ "${outputarr[0]}" == "Empty" ]; then
  >&2 echo -e "$errorm N=$N has no pandiagonal solutions."
  exit 1
fi
exec="./${outputarr[4]}"
cfile="./${outputarr[5]}"

eval $output
eval $exec | Sort

rm $cfile $exec

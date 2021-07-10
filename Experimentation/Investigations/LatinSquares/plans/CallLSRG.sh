#!/usr/bin/env bash
# Oleg Zaikin, 9.7.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version. */

# Usage:

# CallLSRG.sh N m

# runs LSRG to generate a random Latin sqaure of order N with m filled cells, then a
# given solving program is used to solve the Euler square completion problem for this
# square.

program="CallLSRG"
version="0.0.1"

if [[ $# == 1 && $1 == "-v" ]]; then
  echo $program
  echo "version" $version
  exit 0
elif [[ $# < 2 || [$# == 1 && $1 == "-h"] ]]; then
  echo "Usage: CallLSRG.sh N m"
  exit 0
fi

N=$1
m=$2

i=0
while true; do
  LSRG $N,2 "-co" "1*0,0,$m;1*0,0,0" 1609092523835210350,1625864956250569213,0,0,$i | ./2mols
  i=$((i+1))
  echo $N $m
done

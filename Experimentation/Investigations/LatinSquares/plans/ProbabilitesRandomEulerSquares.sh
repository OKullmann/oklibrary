#!/usr/bin/env bash
# Oleg Zaikin, 6.7.2021 (Swansea)
# Copyright 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version. */

# Usage:

# ProbabilitesRandomEulerSquares.sh N limsec

# For a given N and a time limit, vary m (the number of filles celld of a random Latin
# square), and for every pair (N, m) run the script CallLSRG with the time limit. The
# script generates a sample of random Latin squares of order N with m filled cells, then
# solves the corresponding Euler square completion problems.

program="ProbabilitesRandomEulerSquares"
version="0.0.1"

if [[ $# == 1 && $1 == "-v" ]]; then
  echo $program
  echo "version" $version
  exit 0
elif [[ $# < 2 || [$# == 1 && $1 == "-h"] ]]; then
  echo "Usage: ProbabilitesRandomEulerSquares.sh N limsec"
  exit 0
fi

N=$1
limsec=$2

echo "N m o t"
for ((m=$N**2; m > 1; --m)); do
  timeout ${limsec}s ./CallLSRG.sh $N $m
done

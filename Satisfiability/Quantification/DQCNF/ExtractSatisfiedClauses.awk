# Oliver Kullmann, 31.3.2020 (Swansea)
# Copyright 2020 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Extracting the assignments x with 1 <= x <= c (the latter specified via
# "-v c=..."), counting them and printing their count and the sorted string
# of these variables, from either Dimacs-output (a final line starting with
# "v "), or in Minisat-style (two lines, the first "SAT", the second
# the assignment as a clause).
# If no v-line was found, nothing is printed.

BEGIN {
  count=0; err=0; found=0
}

found >= 1 && NR > found {err=1; exit err}

/^v/ {
  if (NF <= 1 || $(NF) != 0) {err=1; exit err}
  for (i=2; i<=NF-1; ++i)
    if ($i >= 1 && $i <= c) {
      result[count++] = $i
    }
  found=NR
}

/^SAT/ {
  if (NR != 1) {err=1; exit err}
  getline
  if (NR != 2 || NF == 0 || $(NF) != 0) {err=1; exit err}
  for (i=1; i<=NF-1; ++i)
    if ($i >= 1 && $i <= c) {
      result[count++] = $i
    }
  found=NR
}

END {
  if (err) exit err
  if (found == 0) exit 0
  if (count == 0) print "0  \"\""
  else if (count == 1) print "1 \"" result[0] "\""
  else {
    asort(result)
    printf "%d", count
    printf " \""
    printf "%d", result[1]
    for (i=2; i <= count; ++i) printf " %d", result[i]
    print "\""
  }
}

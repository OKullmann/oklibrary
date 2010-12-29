# Matthew Gwynne, 29.12.2010 (Swansea)
# Copyright 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.


# Awk script which, given a stream on STDIN containing a number of standard
# DIMACS "v-lines", returns a maxima set which represents
# the partial assignment given by the v-line.

# Grab All fields not including the v and the 0.
BEGIN { resstr=""; }
/^v/ { for (i=2; i<=NF-1; i++){ resstr=resstr "," $i; } }
END { print "{" substr(resstr,2) "}"; }

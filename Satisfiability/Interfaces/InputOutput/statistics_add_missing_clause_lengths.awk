# Matthew Gwynne, 16.1.2011 (Swansea)
# Copyright 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Takes a statistics file generate by ExtendedDimacsFullStatistics
# using the "n" or "ni" (i.e. excluding certain clause lengths with 
# zero occurrences) and outputs the same statistics file with 
# any missing clause-lengths added with an occurrence value of 0.
# 
# Parameters :
#     max: The maximum clause length.
#
BEGIN { N=0 }
/^[0-9]+ +[0-9]+ *$/ { for (; N < $1; N++) print N " " 0; N++ }
/^ *$/ { next }
{ print }
END { for (; N < max; N++) print N " " 0 }
# For example:
#
# > echo -e "p cnf 2 2\n1 0\n 1 0\n" | 
# >    ExtendedDimacsFullStatistics-O3-DNDEBUG "n" | 
# >    awk -v max=2 -f partial_statistics2full_statistics.awk
#  n non_taut_c red_l taut_c orig_l comment_count finished_bool
# 2 2 2 0 2 0 1
#  length count
# 0 0
# 1 2
# 2 0
#

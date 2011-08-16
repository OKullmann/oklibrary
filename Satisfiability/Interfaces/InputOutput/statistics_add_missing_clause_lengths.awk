# Matthew Gwynne, 16.1.2011 (Swansea)
# Copyright 2011 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Reads from standard-input the output of ExtendedDimacsFullStatistics
# using option "n" or "ni" (i.e., output where clause lengths,
# for which no clauses occurred of that length, were omitted)
# and adds any missing clause-lengths to the output; clause-lengths
# not in the input are output with occurrences of 0. The maximum
# clause-length is given as the only argument.

BEGIN { N=0 }
/^[0-9]+ +[0-9]+ *$/ { for (; N < $1; N++) print N " " 0; N++ }
/^ *$/ { next }
{ print }
END { for (; N <= max; N++) print N " " 0 }
# For example:
#
# > echo -e "p cnf 2 2\n1 0\n 1 0\n" | 
# >    ExtendedDimacsFullStatistics-O3-DNDEBUG "n" | 
# >    awk -v max=3 -f statistics_add_missing_clause_lengths.awk
#  n non_taut_c red_l taut_c orig_l comment_count finished_bool
# 2 2 2 0 2 0 1
#  length count
# 0 0
# 1 2
# 2 0
#

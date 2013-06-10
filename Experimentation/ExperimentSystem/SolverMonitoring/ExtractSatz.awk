# Matthew Gwynne, 3.6.2011 (Swansea)
# Copyright 2011, 2012, 2013 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.


# Extracts the numerical data from output of satz215, in a single line.

BEGIN {
  rn=0; rc=0; t=0; sat=2; nds=0; r1=0; r2=0; pls=0; file=""; bnds=0; r2la=0
  r3=0; r3la=0; dc=0
}

/^c sat_status/ { sat = $3 }
/^c number_of_variables/ { rn = $3 }
/^c initial_number_of_clauses/ { rc = $3 }
/^c reddiff_number_of_clauses/ { dc = $3 }
/^c running_time\(sec\)/ { t = $3 }
/^c number_of_nodes/ { nds = $3 }
/^c number_of_binary_nodes/ { bnds = $3 }
/^c number_of_pure_literals/ { pls = $3 }
/^c number_of_1-reductions/ { r1 = $3 }
/^c number_of_2-look-ahead/ { r2la = $3 }
/^c number_of_2-reductions/ { r2 = $3 }
/^c number_of_3-look-ahead/ { r3la = $3 }
/^c number_of_3-reductions/ { r3 = $3 }
/^c file_name/ { file = $3 }

END { 
  print rn " " rc " " t " " sat " " nds " " r1 " " r2 " " pls " \"" file "\" " bnds " " r2la " " r3 " " r3la " " dc
}

# The original satz output was of the following form:
#
#   **** The instance is (satisfiable|unsatisfiable). *****
#   NB_MONO= pls, NB_UNIT= r1, NB_BRANCHE= nds, NB_BACK= bnds
#   Program terminated in t seconds.
#   satz215 file t nds bnds r2la r2 sat rn rc dc r3la r3
#

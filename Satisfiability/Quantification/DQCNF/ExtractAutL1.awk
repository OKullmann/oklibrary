# Oliver Kullmann, 24.7.2018 (Swansea)
# Copyright 2018 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.


# Extracts the numerical data from output of autL1, in a single line.
# Can be used on a file with many autL1-outputs, and for this assumes the
# order of lines is as below.

BEGIN {
  filename=""; no=0; co=0; na=0; ne=0; mind=0; maxd=0; nd=0; lo=0; n=0; c=0; l=0; sat="NaN"; t="NaN"
}

/^c file_name / { filename = $3 }
/^c num_variables / { no = $3 }
/^c num_clauses / { co = $3 }
/^c number_a_variables / { na = $3 }
/^c number_e_variables / { ne = $3 }
/^c min_dep_size / { mind = $3 }
/^c max_dep_size / { maxd = $3 }
/^c num_different_dep_sets / { nd = $3 }
/^c num_ae_literal_occurrences / { lo = $3 }
/^c n / { n = $3 }
/^c c / { c = $3 }
/^c num_literal_occurrences / { l = $3 }
/ SATISFIABLE / { sat = 1 }
/^SATISFIABLE/ { sat = 1 }
/UNSATISFIABLE/ { sat = 0 }
/^user / { t = $2 }
/^BLOCKEND / { print filename " " no " " co " " na " " ne " " mind " " maxd " " nd " " lo " " n " " c " " l " " sat " " t; sat="NaN"; t="NaN"}

END { 
  
}

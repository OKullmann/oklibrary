# Oliver Kullmann, 9.7.2013 (Swansea)
# Copyright 2013, 2017, 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Version 1.0.2

#TODOS:

# DONE 1. New system for n, c, l and variants
# (a) As in the input: pn (<-parn), pc (<- parc), taut (<=nt).
# (b) After elimination of tautological clauses and repeated literals:
#     maxn (<- rn), n (not reported), c (<- rc), l (<- rl), mcl (<- mcl).
# (c) Other parameters:
#   ptime -> parsetime (?)
#   nsol -> sol.
# This should be consolidated with the data for the OKsolver and minisat.


# Extracts the numerical data from output of tawSolver, in a single line.

BEGIN {
  maxn=0; c=0; mcl=0; l=0; t=0; sat=2; nds=0; bnds=0; r1=0; pls="NA"; sol="NA"; ptime=0; pn=0; pc=0; taut=0; file=""; opt=""; prog=""; vers=""; wghts=""
}

/^s UNSATISFIABLE/ { sat=0 }
/^s SATISFIABLE/ { sat=1 }
/^s UNKNOWN/ { sat=2 }
/^c max_occurring_variable/ { maxn = $3 }
/^c number_of_clauses/ { c = $3 }
/^c maximal_clause_length/ { mcl=$3 }
/^c number_of_literal_occurrences/ { l=$3 }
/^c running_time\(sec\)/ { t = $3 }
/^c number_of_nodes/ { nds = $3 }
/^c number_of_binary_nodes/ { bnds = $3 }
/^c number_of_1-reductions/ { r1 = $3 }
/^c number_of_pure_literals/ { pls=$3 }
/^c number_of_solutions/ { sol=$3 }
/^c reading-and-set-up_time\(sec\)/ { ptime=$3 }
/^c p_param_variables/ { pn=$3 }
/^c p_param_clauses/ { pc=$3 }
/^c number_tautologies/ { taut=$3 }
/^c file_name/ { file = $3 }
/^c options/ { opt=$3 }
/^c program_name/ { prog=$3 }
/^c version_number/ { vers=$3 }
/^c weights/ { wghts=$3 }

END { 
  print maxn " " c " " l " " mcl " " t " " sat " " nds " " bnds " " r1 " " pls " " sol " " ptime " " pn " " pc " " taut " \"" file "\"" " \"" opt "\"" " \"" prog "\"" " \"" vers "\""" \"" wghts "\""
}

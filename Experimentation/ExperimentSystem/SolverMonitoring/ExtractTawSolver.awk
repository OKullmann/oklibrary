# Oliver Kullmann, 9.7.2013 (Swansea)
# Copyright 2013, 2017, 2021 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Version 1.0.6

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
  maxn=0; c=0; mcl=0; l=0; t=0; maxmb=0; sat=2; nds=0; bnds=0; scnds=0; lvs=0; inds=0; inps=0; r1=0; r1ps=0; r1pn=0; pls="NA"; wtc="NA"; wtcpin="NA"; ti="NA"; ati="NA"; alph="NA"; lmbd="NA"; apn="NA"; sol="NA"; ptime=0; pn=0; pc=0; taut=0; prog=""; vers=""; opt=""; wghts=""; file=""
}

/^s UNSATISFIABLE/ { sat=0 }
/^s SATISFIABLE/ { sat=1 }
/^s UNKNOWN/ { sat=2 }
/^c program_name/ { prog=$3 }
/^c   version_number/ { vers=$3 }
/^c   options/ { opt=$3 }
/^c   weights/ { wghts=$3 }
/^c file_name/ { file=$3 }
/^c   p_param_variables/ { pn=$3 }
/^c   p_param_clauses/ { pc=$3 }
/^c   number_tautologies/ { taut=$3 }
/^c   max_occurring_variable/ { maxn=$3 }
/^c   number_of_clauses/ { c=$3 }
/^c   maximal_clause_length/ { mcl=$3 }
/^c   number_of_literal_occurrences/ { l=$3 }
/^c running_time\(sec\)/ { t=$3 }
/^c   max_memory\(MB\)/ { maxmb=$3 }
/^c number_of_nodes/ { nds=$3 }
/^c   number_of_binary_nodes/ { bnds=$3 }
/^c   number_of_single_child_nodes/ { scnds=$3 }
/^c   number_of_leaves/ { lvs=$3 }
/^c   number_of_internal_nodes/ { inds=$3 }
/^c   inodes_per_second/ { inps=$3 }
/^c number_of_1-reductions/ { r1=$3 }
/^c   1-reductions_per_second/ { r1ps=$3 }
/^c   1-reductions_per_node/ { r1pn=$3 }
/^c number_of_pure_literals/ { pls=$3 }
/^c number_wtau_calls/ { wtc=$3 }
/^c   number_wtau_calls_per_inode/ { wtcpin=$3 }
/^c number_tau_iterations/ { ti=$3 }
/^c   average_tau_iterations/ { ati=$3 }
/^c alpha/ { alph=$3 }
/^c lambda/ { lmbd=$3 }
/^c number_all_pure_nodes/ { apn=$3 }
/^c number_of_solutions/ { sol=$3 }
/^c reading-and-set-up_time\(sec\)/ { ptime=$3 }

END { 
  print maxn " " c " " l " " mcl " " t " " maxmb " " sat " " nds " " bnds " " scnds " " lvs " " inds " " inps " " r1 " " r1ps " " r1pn " " pls " " wtc " " wtcpin " " ti " " ati " " alph " " lmbd " " apn " " sol " " ptime " " pn " " pc " " taut " \"" prog "\"" " \"" vers "\"" " \"" opt "\"" " \"" wghts "\"" " \"" file "\""
}

# Oliver Kullmann, 9.7.2013 (Swansea)
# Copyright 2013, 2017, 2021, 2024 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Version 1.2

# TODOS:
# DONE 1. New system for n, c, l and variants
# (a) As in the input: pn (<-parn), pc (<- parc), taut (<=nt).
# (b) After elimination of tautological clauses and repeated literals:
#     maxn (<- rn), n (not reported), c (<- rc), l (<- rl), mcl (<- mcl).
# (c) Other parameters:
#   ptime -> parsetime (?)
#   nsol -> sol.
# 2. This should be consolidated with the data for the OKsolver and minisat.


# Extracts the numerical data from output of (c)(t)(m)tawSolver,
# in a single line.

BEGIN {
  maxn=0; c=0; mcl=0; l=0; t=0; st=0; wt=0; maxmb=0; sat=2; nds=0; bnds=0; scnds=0; lvs=0; inds=0; inps=0; r1=0; r1ps=0; r1pn=0; pls="NA"; he=0; prj=0; avprj=0; onez=0; fonez=0; bothz=0; fbothz=0; withf=0; fwithf=0; nof=0; fnof=0; minf=0; meanf=0; maxf=0; sdf=0; withs=0; fwiths=0; nos=0; fnos=0; mins=0; means=0; maxs=0; sds=0; wtc="NA"; awtc="NA"; ti="NA"; ati="NA"; alph="NA"; lmbd="NA"; sol="NA"; ptime=0; sptime=0; wptime=0; pn=0; pc=0; taut=0; prog=""; vers=""; opt=""; wghts=""; file=""
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
/^c   system_time/ { st=$3 }
/^c   elapsed_wall_clock/ { wt=$3 }
/^c   max_memory\(MB\)/ { maxmb=$3 }
/^c number_of_nodes/ { nds=$3 }
/^c   number_of_binary_nodes/ { bnds=$3 }
/^c   number_of_single_child_nodes/ { scnds=$3 }
/^c   number_of_leaves/ { lvs=$3 }
/^c   number_of_internal_nodes/ { inds=$3 }
/^c     inodes_per_second/ { inps=$3 }
/^c number_of_1-reductions/ { r1=$3 }
/^c   1-reductions_per_second/ { r1ps=$3 }
/^c   1-reductions_per_node/ { r1pn=$3 }
/^c   number_of_pure_literals/ { pls=$3 }
/^c heuristics_evaluations/ { he=$3 }
/^c   number_projections/ { prj=$3 }
/^c     average_projections/ { avprj=$3 }
/^c     number_onezero/ { onez=$3 }
/^c       freq_onezero/ { fonez=$3 }
/^c     number_bothzero/ { bothz=$3 }
/^c       freq_bothzero/ { fbothz=$3 }
/^c   number_withfirst/ { withf=$3 }
/^c     freq_withfirst/ { fwithf=$3 }
/^c   number_nofirst/ { nof=$3 }
/^c     freq_nofirst/ { fnof=$3 }
/^c   min_first/ { minf=$3 }
/^c     mean_first/ { meanf=$3 }
/^c     max_first/ { maxf=$3 }
/^c     sd_first/ { sdf=$3 }
/^c   number_withsecond/ { withs=$3 }
/^c     freq_withsecond/ { fwiths=$3 }
/^c   number_nosecond/ { nos=$3 }
/^c     freq_nosecond/ { fnos=$3 }
/^c   min_second/ { mins=$3 }
/^c     mean_second/ { means=$3 }
/^c     max_second/ { maxs=$3 }
/^c     sd_second/ { sds=$3 }
/^c   number_wtau_calls/ { wtc=$3 }
/^c     average_wtau_calls/ { awtc=$3 }
/^c   number_tau_iterations/ { ti=$3 }
/^c     average_tau_iterations/ { ati=$3 }
/^c   alpha/ { alph=$3 }
/^c   lambda/ { lmbd=$3 }
/^c number_of_solutions/ { sol=$3 }
/^c reading-and-set-up_time\(sec\)/ { ptime=$3 }
/^c   system/ { sptime=$3 }
/^c   elapsed/ { wptime=$3 }

END { 
  print maxn " " c " " t " " sat " " l " " st " " wt " " maxmb " " mcl " " nds " " bnds " " scnds " " lvs " " inds " " inps " " r1 " " r1ps " " r1pn " " pls " " he " " prj " " avprj " " onez " " fonez " " bothz " " fbothz " " withf " " fwithf " " nof " " fnof " " minf " " meanf " " maxf " " sdf " " withs " " fwiths " " nos " " fnos " " mins " " means " " maxs " " sds " " wtc " " awtc " " ti " " ati " " alph " " lmbd " " sol " " ptime " " sptime " " wptime " " pn " " pc " " taut " \"" prog "\"" " \"" vers "\"" " " opt " \"" wghts "\"" " " file
}

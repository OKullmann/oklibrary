# Matthew Gwynne, 20.5.2011 (Swansea)
# Copyright 2011, 2012 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.


# Extracts the numerical data from output of OKsolver_2002, in a single line.

BEGIN {
  mcl="NA"; n="NA"; c="NA"; l="NA"; t="NA"; sat=2; nds="NA"; r1="NA"; r2="NA";
  pls="NA"; ats="NA"; h="NA"; file="NA"; dmcl="NA"; dn="NA"; dc="NA"; dl="NA";
  n2cr="NA"; snds="NA"; qnds="NA"; mnds="NA"; tel="NA"; oats="NA"; n2cs="NA";
  m2cs="NA"; pa="NA"; ps="NA"; tp="NA"; spd="NA"; spc="NA"; }
/^c sat_status/ { sat=$3; }
/^c initial_maximal_clause_length/ { mcl=$3; }
/^c initial_number_of_variables/ { n=$3; }
/^c initial_number_of_clauses/ { c=$3; }
/^c initial_number_of_literal_occurrences/ { l=$3; }
/^c number_of_initial_unit-eliminations/ { r1=$3; }
/^c reddiff_maximal_clause_length/ { dmcl=$3; }
/^c reddiff_number_of_variables/ { dn=$3; }
/^c reddiff_number_of_clauses/ { dc=$3; }
/^c reddiff_number_of_literal_occurrences/ { dl=$3; }
/^c number_of_2-clauses_after_reduction/ { n2cr=$3; }
/^c running_time\(sec\)/ { t=$3; }
/^c number_of_nodes +/ { nds=$3 }
/^c number_of_single_nodes/ { snds=$3; }
/^c number_of_quasi_single_nodes/ { qnds=$3; }
/^c number_of_2-reductions/ { r2=$3; }
/^c number_of_pure_literals/ { pls=$3; }
/^c number_of_autarkies/ { ats=$3; }
/^c number_of_missed_single_nodes/ { mnds=$3; }
/^c max_tree_depth/ { h=$3; }
/^c proportion_searched/ { pa=$3; }
/^c proportion_single/ { ps=$3; }
/^c total_proportion/ { tp=$3; }
/^c number_of_table_enlargements/ { tel=$3; }
/^c number_of_1-autarkies/ { oats=$3; }
/^c number_of_new_2-clauses/ { n2cs=$3; }
/^c maximal_number_of_added_2-clauses/ { m2cs=$3; }
/^c file_name/ { file="\"" $3 "\""; }
/^c splitting_directory/ { spd="\"" $3 "\""; }
/^c splitting_cases/ { spc=$3; }

END { 
  print n " " c " " l " " mcl " " t " " sat " " nds " " r1 " " r2 " " pls " " ats " " \
    h " " file " " n2cr " " dmcl " " dn " " dc " " dl " " snds " " \
    qnds " " mnds " " pa " " ps " " tp " " tel " " oats " " n2cs " " m2cs " " spd " " spc; }

// Oliver Kullmann, 4.8.2009 (Swansea)
/* Copyright 2009, 2010, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ExperimentSystem/SolverMonitoring/plans/OKsolver.hpp
  \brief General plans monitoring OKsolver_2002


  \todo Handling splitting output
  <ul>
   <li> OKsolver_2002 outputs the fields
   \verbatim
c splitting_directory                   SplitViaOKsolver_D100des_6t4_1base_r5_s1cnf_2012-01-15-231326/Instances
c splitting_cases                       345185
   \endverbatim
   when given the option "-S" but not otherwise.
   </li>
   <li> We must decide how to handle this "dynamic" output for
   OKsolver_2002: Making case-distinctions introduces dependencies and
   combinatorial complications, and thus is best avoided. </li>
   <li> The question of how to handle dynamic output in general is
   discussed in "Handling changing solver output" in
   ExperimentSystem/SolverMonitoring/plans/general.hpp . </li>
   <li> The facts that:
    <ul>
     <li> for general OKsolver_2002 output, splitting doesn't occur, and </li>
     <li> splitting is handled by the special script "SplittingViaOKsolver",
     </li>
    </ul>
   mean that, in this case, the additional complexity of having
   ExtractOKsolver take additional parameters is best. These
   additional parameters then determine which attributes OKsolver
   extracts. OK: Having the value NA, it is always easy to determine which
   case occurs, easier than to ask for the existence/non-existence of
   certain fields. Thus we always should have just *one* table-format
   for a solver, not many.</li>
   <li> The ExtractOKsolver script should be extended to take a
   "-S" option to indicate that the OKsolver_2002 output it is reading
   includes splitting data. OK: As discussed above, this should not be done.
   </li>
  </ul>


  \todo Write extraction tool
  <ul>
   <li> See "Extraction tools" in
   ExperimentSystem/SolverMonitoring/plans/general.hpp. </li>
   <li> See also "Move read_minisat2_output functionality to wrapper script"
   in ExperimentSystem/SolverMonitoring/plans/Minisat2.hpp. </li>
  </ul>


  \todo R-problem with read_oksolver_mon
  <ul>
   <li> Suddenly we get warnings:
   \verbatim
> E=read_oksolver_mon("GreenTao_N_3-2-3-5_581.cnf.mo")
486511
Warnmeldungen:
1: In if (!header) rlabp <- FALSE :
  Bedingung hat Länge > 1 und nur das erste Element wird benutzt
2: In if (header) { :
  Bedingung hat Länge > 1 und nur das erste Element wird benutzt
   \endverbatim
   </li>
   <li> These warnings apparently do not show up directly after starting
   R, but then later; and also a certain length of the file seems to be
   needed. </li>
   <li> The read-table call directly:
   \verbatim
E = read.table(file="GreenTao_N_3-2-3-5_581.cnf.mo",header=T,colClasses=c("integer","integer","numeric","numeric","numeric","integer","integer","integer","numeric"))
   \endverbatim
   </li>
   <li> Contact the R-mailing list. </li>
  </ul>


  \todo Object-oriented syntax
  <ul>
   <li> It would be good if one could just say "plot(E)", "summary(E)" or
   "hist(E)", and the corresponding function "plot_oksolver" etc. would be
   chosen. </li>
   <li>  For this the data frames needed to be "marked". </li>
  </ul>


  \todo Function plot_oksolver_mon_nodes
  <ul>
   <li> Currently it is hardly visible that one observation node might
   have associated 100 nodes, and another observation might have this
   as well, while the former really advances only one observation level,
   while the latter advances more observation levels, say 10, and thus
   in effect the number of associated nodes for it is closer to
   100 / 10 = 10. </li>
  </ul>


  \todo Function summary_oksolver
  <ul>
   <li> The R-standard is to have "summary" just outputting text, while by
   "plot" one gets plots; now we have it combined? </li>
  </ul>


  \todo Function hist_oksolver_mon_nodes
  <ul>
   <li> The current breakpoints don't reflect the value-structure well for
   low node-counts, say up to 8 nodes:
   \verbatim
> log2(seq(0,8))
[1]     -Inf 0.000000 1.000000 1.584963 2.000000 2.321928 2.584963 2.807355
[9] 3.000000
   \endverbatim
   </li>
   <li> Is it possible to adjust the standard setting of breakpoints just for
   these initial values? </li>
   <li> Perhaps one bar for each of the values 0, ..., 8. </li>
   <li> But if there are no values in this range, then no breakpoints are
   needed. </li>
   <li> It would be good if one could first compute the proposed setting of
   breakpoints, and then just adjust them, if necessary. </li>
   <li> However one problem is the grouping (so that one gets to see a
   distribution)?! Somehow the bars should collect "equal" amounts of counts
   --- however should this relate to the original node-counts or to their
   logarithms?? Perhaps the logarithms (as it is now) is fine in principle,
   but the discretisation effects for small node-counts should be repaired.
   </li>
   <li> A fine-grained histogram is created by breaks="Freedman-Diaconis"
   (just "f" is sufficient). For the examples considered, "Scott" (just "sc"
   has the same effect. </li>
   <li> breaks="Sturges" (just "st" is sufficient) uses less bars; this might
   be actually more appropriate, since we need to consider here a
   distribution? The single counts can be viewed by plot(table(E$nodes)) or
   plot(table(log2(E$nodes))). </li>
   <li> If only the first n values and their counts are of interest, use
   table(E$nodes)[1:n] (or the usual other variations). This information can
   also be plotted (just using "plot"). </li>
   <li> We need also more tick-marks. </li>
  </ul>

*/


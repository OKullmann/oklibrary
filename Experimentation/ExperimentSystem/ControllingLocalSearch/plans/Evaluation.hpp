// Oliver Kullmann, 11.9.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ExperimentSystem/ControllingLocalSearch/plans/Evaluation.hpp
  \brief %Tools for evaluating local search algorithms


  \todo Evaluation tools for run_ubcsat
  <ul>
   <li> Functions are needed for standard evaluations.
    <ol>
     <li> At the R-level, we go for convenience, and overloading functions
     like "summary" or "plot" seems be the right way. </li>
     <li> One needs to find out how to do this, how to recognise our
     specific data frames. </li>
     <li> "summary" should show data, while "plot" might run through a
     serious of plots, showing the data from different angles. </li>
    </ol>
   </li>
   <li> Plotting an overview:
    <ol>
     <li> Given the dataframe in E, the best algorithms seems best graphed
     by
     \verbatim
plot(E$alg,E$min)
     \endverbatim
     which plots per algorithm the boxplots of the min-distribution. </li>
     <li> However it is useless if on the x-axis not all algorithms are
     listed. </li>
     <li> And an option for cutoff_min is needed, which removes algorithms
     whose best min-value is at least the cutoff-value. </li>
    </ol>
   </li>
   <li> Sorting the algorithms
    <ol>
     <li> Considering single algorithms by e.g.
     \verbatim
> table(E$min[E$alg=="adaptnoveltyp"])
     \endverbatim
     (note that currently algorithm names are inappropriately handled). </li>
     <li> These tables can be put into a linear order by sorting first
     according to min-value reached (the lower the better), and second by
     count obtained (the higher the better). </li>
     <li> Also the time needed should at least be shown. </li>
     <li> A function should be written which prints out the
     sorted tables in a nice way. </li>
     <li> As a first attempt we have eval_ubcsat_dataframe, which just shows
     all results in table form (sorting needs to be added). </li>
     <li> Perhaps then the (first) evaluation tool just uses
     plot(E$alg,E$min), followed by printing those *sorted* tables. </li>
    </ol>
   </li>
   <li> Also the average number of steps to the optimum is relevant:
   When it is closer to the cutoff, then increasing the cutoff likely
   might yield an improvement, while if not then the algorithm wanders
   randomly around without achieving something (so increasing the cutoff
   seems of little value). </li>
  </ul>

*/

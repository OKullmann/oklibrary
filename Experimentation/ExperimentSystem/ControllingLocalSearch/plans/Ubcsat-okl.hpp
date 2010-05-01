// Oliver Kullmann, 12.9.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ExperimentSystem/ControllingLocalSearch/plans/Ubcsat-okl.hpp
  \brief Plans on the wrapper for Ubcsat


  \todo More readable large numbers:
  <ul>
   <li> See "Contact the developers of Ubcsat" in
   Satisfiability/Algorithms/LocalSearch/Ubcsat/plans/general.hpp. </li>
   <li> Especially for the cutoff value it would be good if number
   representations like "100*10^6" (or "55*11^7") could be used. </li>
   <li> It's awkward to count the zeros. </li>
   <li> So ubcsat-okl needs to parse the parameters, and translates numbers
   like that into their decimal representation. </li>
   <li> In order to avoid misinterpretations, perhaps we only consider
   a "-cutoff" parameter. </li>
   <li> A little awk-script could do the job:
    <ol>
     <li> Its input is the whole parameter line. </li>
     <li> Running through the parameters one searches for "-cutoff". </li>
     <li> The parameter after "-cutoff" is first devided by "*" into (a,x),
     and if x is present, x is divided into (b,c) by "^", and then
     a * b^c (where a, b, c are natural numbers) is computed. </li>
     <li> Returned is the whole parameter line, with the cutoff-value
     replaced. </li>
    </ol>
   </li>
  </ul>


  \todo Better output
  <ul>
   <li> See ExperimentSystem/ControllingLocalSearch/plans/DataCollection.hpp
   for plans on extending resp. using the output-functionality. </li>
   <li> The idea is that ubcsat-okl creates some form of complete output,
   which is directly readable by R (into a dataframe, without further
   processing). </li>
   <li> By appropriate formatting one also might make it better readable
   for the human reader. </li>
   <li> Since we reformat the output, the wrapper should likely reformat
   every single output line individually (as it comes, so that one can
   read into R also partial results (from the intermediate file)). </li>
   <li> The following seems reasonable as a "ubcsat-okl" script:
   \verbatim
# Work out algorithm argument
ALG_ARG_P=1; for arg_p in `seq 1 $#`; do 
  if [[ ${!arg_p} == "-alg" ]] ; then ALG_ARG_P=`expr $arg_p + 1`; fi;
done
ALG=`printf '%34s' ${!ALG_ARG_P}`
echo "       sat  min     osteps     msteps       seed                                alg"
ubcsat -rclean -r out stdout run,found,best,beststep,steps,seed -r stats stdout numclauses,numvars,numlits,fps,beststep[mean],steps[mean+max],percentsolve,best[min+max+mean+median] $* | sed -e "s/^\\(\\( \\+[0-9]\\+\\)\\{6\\} *\\)$/\\1${ALG}/"
   \endverbatim
   This appends the algorithm as a column to the data, line by line. </li>
   <li> Should this script also take into account that the user may want to
   specify different output parameters, and then provide different headers for
   when this is the case, or is this functionality beyond the scope of
   ubcsat-okl, and the output parameters are fixed? </li>
   <li> We also need the DIMACS output codes (10 for satisfying assignment
   found, 0 for unknown).
   <li> Another point is how to handle statistics as they are only printed
   at the end (and this in a form incompatible with the R-format). </li>
   <li> DONE
   A problem appending columns line by line to ubcsat output is that 
   ubcsat version 1.0.0 doesn't flush data, and so one would have to wait until
   ubcsat has finished before viewing results. See "Ubcsat does not flush the
   output-buffer" below for how to repair this. </li>
  </ul>

*/


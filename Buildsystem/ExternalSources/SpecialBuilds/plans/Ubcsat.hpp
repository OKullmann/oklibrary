// Oliver Kullmann, 28.6.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Ubcsat.hpp
  \brief Plans regarding installation of the Ubcsat package


  \todo Update to newer version
  <ul>
   <li> The post-1.1.0-version should support 64bit machines, so we wait
   for that. </li>
   <li> We should download the available documentation. </li>
   <li> OK and MG must get in contact with the Ubcsat-group. </li>
   <li> See "Contact the developers of Ubcsat" in
   Satisfiability/Algorithms/LocalSearch/plans/general.hpp. </li>
   <li> If really needed, one could use the executable provided with 1.1.0:
   We get a segfault when finally analysing the data (and computing the maximum
   number of steps), but this is not such a big problem (the data analysis
   then is done using R). </li>
  </ul>


  \todo ubcsat-okl
  <ul>
   <li> More readable large numbers:
    <ol>
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
    </ol>
   </li>
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
   <li> We also need the DIMACS output codes (10 for satisfying assignment
   found, 0 for unknown).
   <li> DONE
   A problem appending columns line by line to ubcsat output is that 
   ubcsat version 1.0.0 doesn't flush data, and so one would have to wait until
   ubcsat has finished before viewing results. See "Ubcsat does not flush the
   output-buffer" below for how to repair this. </li>
   <li> Another point is how to handle statistics as they are only printed
   at the end. </li>
  </ul>


  \bug Cutoff value etc. should be 64 bits on a 64-bit machine
  <ul>
   <li> Yet "unsigned int" is used for example for the cutoff-value,
   not allowing big experimentation. </li>
   <li> Tell the Ubcsat people. </li>
  </ul>


  \todo Investigating the weak performance of Ubcsat on OK's 32-bit machine
  <ul>
   <li> See "Bounds on NR([q,q],2)" in
   Experimentation/Investigations/plans/RamseyProblems.hpp on some data
   on the problem. </li>
   <li> We should send the Ramsey-instance to the Ubcsat-people, to see
   whether they think the performance on csltok is pretty bad (or perhaps
   the performance on cs-wsok is pretty good). </li>
  </ul>


  \todo Building Ubcsat (OK, ML)
  <ul>
   <li> The Ubcsat installation seems pretty up-to-date, but we should
   check the details (only concerning 1.0.0).
    <ol>
     <li> Make-variables should be renamed, replacing "-" by "_". </li>
    </ol>
   </li>
   <li> See "Installation of UBCSAT completed" in
   Satisfiability/Algorithms/LocalSearch/plans/general.hpp. </li>
   <li> DONE (updated to the new system, where only the "recommended"
   version number is provided)
   Yet the installation of a different version than the "recommended"
   version wouldn't work, since the directory-names contain the recommended
   version number (and not the one dictated by the target).
    <ol>
     <li> One could leave it like that --- if a version different from
     the recommended shall be used then the recommended version needs
     to be overridden. </li>
     <li> But then we don't need to provide different targets. </li>
    </ol>
   </li>
   <li> DONE Remove the hardcoded version-numbers. </li>
   <li> DONE (removed)
   What is the meaning of "tag" and "tag_" ??? The names should tell. </li>
   <li> No file modification in the external sources. DONE </li>
  </ul>


  \todo Speed
  <ul>
   <li> We should find out about the speed of the various versions (1.0.0
   versus 1.1.0, the provided executable versus the one build by our system
   versus one build with "-m32", 32-bit versus 64-bit). </li>
   <li> Yet it seems the only difference is between 32 and 64 bits. </li>
   <li> Let's use
   \verbatim
ubcsat -alg samd -i Ramsey_5_2_40.cnf -runs 10 -cutoff 5000 
   \endverbatim
   as the benchmark (reading off the "FlipsPerSecond" (FPS)). </li>
   <li> On csltok (32 bit) we have the problem that, apparently due to heating
   problems (real or fake), the processor runs with various clock-frequencies,
   and this not controllable (under Suse 9.2). But it seems that the slowest
   execution is constantly ~ 270 FPS, while the fastest execution is
   constantly ~ 1200 FPS (which should correspond to 1700 MHz). </li>
   <li> On cs-wsok (64 bit) we have between 2400 and 2800 FPS for our
   executable (1.0.0), between 2100 and 2200 FPS both for 1.1.0 with -m32
   or as provided with the distribution --- so native compilation yields
   better results (between 10%-30%)! </li>
   <li> On cscarme (64-bit, 2.8 GHz) we have around 2500 FPS (for 1.1.0-
   executable). </li>
  </ul>

*/


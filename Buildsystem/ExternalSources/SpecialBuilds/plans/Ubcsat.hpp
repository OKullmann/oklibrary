// Oliver Kullmann, 28.6.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Ubcsat.hpp
  \brief Plans regarding installation of the Ubcsat package


  \todo Make clear the required package for dos2unix
  <ul>
   <li> Some linux distributions (such as Ubuntu, and (therefore)
   presumably Debian) do not include dos2unix by default
   and the only packaged versions of the utility they have (in 
   particular the tofrodos package which until recently created
   dos2unix and unix2dos aliases - 
   http://www.thefreecountry.com/tofrodos/index.shtml) do not match the 
   "correct" version, as used by the OKlibrary. </li>
   <li> As such alternative packages, even when creating aliases oneself,
   do not have certain options available, as well as the fact that the 
   "correct" dos2unix package is not clear from a standard google search, 
   we should provide a link to the package home page somewhere within the 
   documentation or plans to make it clear where to obtain this package. </li>
   <li> The correct package is available at 
   http://www.xs4all.nl/~waterlan/dos2unix.html . </li>
  </ul>


  \todo Update to newer version : DONE (we wait for version 1.2.0)
  <ul>
   <li> The post-1.1.0-version should support 64bit machines, so we wait
   for that. </li>
   <li> It seems that version 2.0 should come out soon (SAT 2010?). </li>
   <li> With version 1.0 we are experiencing a segmentation fault (see
   Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/VanderWaerden_3-4-4-4.hpp),
   which doesn't seem to occur with the precompiled 1.1-version. </li>
   <li> We should download the available documentation. </li>
   <li> OK and MG must get in contact with the Ubcsat-group. </li>
   <li> See "Contact the developers of Ubcsat" in
   Satisfiability/Algorithms/LocalSearch/Ubcsat/plans/general.hpp. </li>
   <li> If really needed, one could use the executable provided with 1.1.0:
   We get a segfault when finally analysing the data (and computing the maximum
   number of steps), but this is not such a big problem (the data analysis
   then is done using R). </li>
  </ul>


  \bug DONE (transferred to "Contact the developers of Ubcsat" in
  Satisfiability/Algorithms/LocalSearch/Ubcsat/plans/general.hpp)
  Cutoff value etc. should be 64 bits on a 64-bit machine
  <ul>
   <li> Yet "unsigned int" is used for example for the cutoff-value,
   not allowing big experimentation. </li>
   <li> Tell the Ubcsat people. </li>
  </ul>


  \todo DONE (this has been transferred to "Speed" below)
  Investigating the weak performance of Ubcsat on OK's 32-bit machine
  <ul>
   <li> See "Bounds on NR([q,q],2)" in
   Experimentation/Investigations/plans/RamseyProblems.hpp on some data
   on the problem. </li>
   <li> We should send the Ramsey-instance to the Ubcsat-people, to see
   whether they think the performance on csltok is pretty bad (or perhaps
   the performance on cs-wsok is pretty good). </li>
  </ul>


  \todo Building Ubcsat : DONE (the topics in
  Satisfiability/Algorithms/LocalSearch/Ubcsat/plans/general.hpp are to be
  addressed when installing Ubcsat version 1.2.0)
  <ul>
   <li> DONE
   The Ubcsat installation seems pretty up-to-date, but we should
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


  \todo Speed : DONE (transferred to "Contact the developers of Ubcsat" in
  Satisfiability/Algorithms/LocalSearch/Ubcsat/plans/general.hpp)
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


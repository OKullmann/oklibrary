// Oliver Kullmann, 28.6.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Ubcsat.hpp
  \brief Plans regarding installation of the Ubcsat package


  \todo Update to version 1.2.0
  <ul>
   <li> We created a new package "ubcsat-1-2-0-beta.tar.bz2", cloning
   https://github.com/dtompkins/ubcsat.git, checking out branch beta. </li>
   <li> DONE
   We also changed "#define MAXPARMLINELEN 4096" to
   "#define MAXPARMLINELEN 16384". </li>
   <li> "new-ubcsat-okl" is the wrapper-script to access the new version
   (besides the old version). </li>
   <li> Update to a newer version via
   \verbatim
> ubcsat-1-2-0-beta> git pull https://github.com/dtompkins/ubcsat.git beta
   \endverbatim
   inside the extracted archive (then archiving it again). </li>
   <li> Version 1-2-0 is slower than our version 1-0-0:
   \verbatim
> ubcsat-okl -alg rots -runs 10 -i VanDerWaerden_2-3-36_1256.cnf
...
FlipsPerSecond = 15225

> new-ubcsat-okl -alg rots -runs 10 -i VanDerWaerden_2-3-36_1256.cnf
...
FlipsPerSecond = 14102
   \endverbatim
   </li>
   <li> DONE (we didn't check out the beta-version)
   The current version is not usable at all; e-mail sent to
   ubcsat-developers. </li>
   <li>  DONE (corrected the bug)
   The new version still seems not usable; bug report sent to
   ubcsat-developers. </li>
   <li> DONE (32-bit types were defined as "long", which is 64 bits)
   After update the problem looks like that:
   \verbatim
ubcsat-1-2-0-beta> make ubcsat_debug
ubcsat-1-2-0-beta> valgrind --track-origins=yes ./ubcsat_debug -alg rots -runs 10 -i VanDerWaerden_2-3-36_1256.cnf
==32044== Conditional jump or move depends on uninitialised value(s)
==32044==    at 0x40AAFD: PickRoTS (rots.c:123)
==32044==    by 0x454A38: ubcsatmain (ubcsat.c:95)
==32044==    by 0x454EBA: main (ubcsat.c:149)
==32044==  Uninitialised value was created by a heap allocation
==32044==    at 0x4B1B16E: malloc (vg_replace_malloc.c:236)
==32044==    by 0x41D60D: AllocateRAM (ubcsat-mem.c:71)
==32044==    by 0x41D949: SetString (ubcsat-mem.c:98)
==32044==    by 0x41BD51: SetupUBCSAT (ubcsat-internal.c:1847)
==32044==    by 0x4544B2: ubcsatmain (ubcsat.c:33)
==32044==    by 0x454EBA: main (ubcsat.c:149)
   \endverbatim
   </li>
  </ul>


  \bug UBCSAT 1.0.0 reports wrong minimum number of falsified clauses
  <ul>
   <li> Using the test file:
   \verbatim
> cat test.cnf 
p cnf 2 2
1 2 0
1 -2 0
   \endverbatim
   and running UBCSAT with "gsat", "rnovelty+" or possibly other
   solvers, yields 3 falsified clauses as the "best" or "min" 
   despite the fact that there are only 2 clauses in the clause
   set AND those runs are said to have found a satisfying assignment:
   \verbatim
> ubcsat-okl -alg gsat -i test.cnf -seed 1845608494
# -rclean -r out stdout run,found,best,beststep,steps,seed -r stats stdout numclauses,numvars,numlits,fps,beststep[mean],steps[mean+max],percentsolve,best[min+max+mean+median] -runs 10 -cutoff 100000 -alg gsat -i test.cnf -seed 1845608494
       sat  min     osteps     msteps       seed
      1 1     0          2          2 1845608494 
      2 1     3          0          1 2475891066 
      3 1     3          0          1 1321093004 
      4 1     3          0          1 3320110952 
      5 1     0          2          2 3736165511 
      6 1     0          2          2 2062463429 
      7 1     3          0          1 3199736854 
      8 1     0          2          2 3920935571 
      9 1     3          0          1  495225504 
     10 1     0          2          2 3620363783 


Clauses = 2
Variables = 2
TotalLiterals = 4
FlipsPerSecond = inf    
BestStep_Mean = 1.000000
Steps_Mean = 1.500000
Steps_Max = 2.000000
PercentSuccess = 100.00
BestSolution_Mean = 1.500000
BestSolution_Median = 1.500000
BestSolution_Min = 0.000000
BestSolution_Max = 3.000000
   \endverbatim
   </li>
  </ul>


  \bug UBCSAT 1.0.0 segfaults with weighted MaxSAT instances
  <ul>
   <li> Given the input file ("test.wcnf")
   \verbatim
p wcnf 2 4
200 1 2 0
200 1 -2 0
1 1 0
1 2 0 
   \endverbatim
   and running
   \verbatim
ubcsat-okl -alg gsat -w -i test.wcnf
   \endverbatim
   or
   \verbatim
ubcsat -alg gsat -w -i test.wcnf
   \endverbatim
   segfaults on MG's machine.
   </li>
   <li> Using ubcsat 1.1.0 doesn't segfault. </li>
   <li> Typically this behaviour depends on the algorithms, so gsat can't be
   used, but hopefully other algorithms. </li>
   <li> With ubcsat version 1.2 we hope that this problem disappears. </li>
  </ul>


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

*/


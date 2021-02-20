// Oliver Kullmann, 18.8.2007 (Swansea)
/* Copyright 2007, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Solvers/OKsolver/plans/general.hpp
  \brief Plans for the development of the OKsolver-family

  The OKsolver is meant as the general purpose SAT solver
  (at the time) realised by the OKlibrary.


  \todo Segmentation fault on SAT_P_opt_snake_p11.pddl_36.cnf.
  <ul>
   <li> Occurs after 65 seconds on the CNF SAT_P_opt_snake_p11.pddl_36.cnf from the
   directory /data/benchmarks/SAT/SATCompetition/2020/planning/ </li>
   <li> Occurs in the course of reading the CNF. </li>
   <li> CNF's literals number is 458598850. </li>
   <li> The first error found by valgrind is:
   Invalid write of size 4
   at 0x40D4CB: uebernehmenKlausel() (Einlesen.c:225)
   by 0x407963: Einlesen(_IO_FILE*, unsigned int) (Einlesen.c:441)
   by 0x406787: main (OKsolver_2002_lnk.c:1506)
   Address 0xb341d708 is 0 bytes after a block of size 1,499,109,064 alloc'd
   at 0x4C2E2DF: malloc (in /usr/lib64/valgrind/vgpreload_memcheck-amd64-linux.so)
   by 0x4029C4: xmalloc(unsigned long) (Speicher.c:31)
   by 0x407403: Einlesen(_IO_FILE*, unsigned int) (Einlesen.c:335)
   by 0x406787: main (OKsolver_2002_lnk.c:1506)
   </li>
  </ul>


  \todo Update namespaces.


  \todo How to refer to the different versions of OKsolver ?
  <ul>
   <li> Perhaps the main name is "OKsolver2002" for the old OKsolver,
   OKsolver2009 for the new version etc. </li>
   <li> Naming the old OKsolver:
    <ol>
     <li> Yet we use "OKsolver_2002", but the underscore seems superfluous
     (and creates problems when writing the name). </li>
     <li> Also "OKsolver-2002" is used. Somehow this looks nicer? </li>
    </ol>
   </li>
   <li> A problem then is how to call the re-programmed original
   OKsolver? </li>
   <li> Perhaps this is only an internal version, and so the name doesn't
   matter so much. </li>
   <li> So once the plans for the new OKsolver (currently "OKsolver_2_0")
   are completed, the year of its first appearance should be estimated
   (perhaps 2013), and the module "OKsolver2013" (or else) has to be created.
   </li>
   <li> Perhaps then "SAT2002" should be renamed to "OKsolver2002" --- or
   perhaps we just call the modules "2002", "2013", etc., according to
   our strategy to use the full path name? </li>
   <li> Then we get a bit of a problem with the namespaces and their aliases;
   the modules could be called "Y2002", "Y2013", etc., with aliases
   "Y02", "Y13". Solvers still "OKsolver2002" etc. </li>
   <li> Should the versions appear in the name? Better not. </li>
   <li> The version of each OKsolver is the version of its module. </li>
  </ul>


  \todo Combination of OKsolver with Minisat
  <ul>
   <li> The current, very simple combination of OKsolver-2002 and minisat-2.2.0
   via SplittingViaOKsolver is very successful; see
   Investigations/RamseyTheory/VanderWaerdenProblems/plans/Parallelisation/general.hpp
   for a general overview. </li>
   <li> The current abortion criterion, the size of the partial assignment
   leading to the node, is not too bad, but to handle hard problems more
   efficiently, a more "intelligent" method is needed. </li>
   <li> Writing all sub-problems as partial assignments to file is good for
   distributed solving, however it makes more flexible splitting hard to
   realise. </li>
   <li> Though one can imagine a recursive directory structure, where hard
   problems, where minisat takes more than, say, 5 seconds, are then further
   split. </li>
   <li> Or one creates a combination of minisat with OKsolver, where first
   minisat is run for a few seconds, and if this doesn't solve the problem,
   then OKsolver is used to split up the problem, and for these sub-problems
   this combined solver is run again. </li>
  </ul>

*/


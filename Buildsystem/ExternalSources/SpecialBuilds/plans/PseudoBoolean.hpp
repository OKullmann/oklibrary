// Oliver Kullmann, 26.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/PseudoBoolean.hpp
  \brief Plans regarding building of pseudo-boolean solvers and libraries


  \todo Install pseudo-boolean solvers
  <ul>
   <li> We offer translations to pseudo-boolean instances
   (Satisfiability/Interfaces/InputOutput/MinOnes2PseudoBoolean.cpp)
   but only one pseudo-boolean solver (minisat+). </li>
   <li> We should add all known pseudo-boolean solvers to the library.
   </li>
  </ul>


  \todo minisat+ input format
  <ul>
   <li> The minisat+ input format is the OPB format from
   PB05; see http://www.cril.univ-artois.fr/PB05/solver_req.html.
   </li>
   <li> Since PB06, the "*" between a variable and it's weight has
   been removed. </li>
   <li> So instead of writing a pseudo-boolean constraint like:
   \verbatim
+1 * x1 +2 * x2 >= 1;
   \endverbatim
   the format is now:
   \verbatim
+1 x1 +2 x2 >= 1;
   \endverbatim
   </li>
   <li> There are several options:
    <ul>
     <li> A patch for minisat+ which allows it to read the new format. </li>
     <li> A wrapper script around minisat+ which converts the new OPB format
     to the old. </li>
     <li> A script or manual conversion of new OPB files to old, before using
     minisat+. </li>
    </ul>
   </li>
   <li> The best option is to patch minisat+, as the old format is no longer
   used and we, as users of minisat+, shouldn't have to worry about such
   things. </li>
  </ul>


  \todo borg-pb
  <ul>
   <li> Available at
   http://nn.cs.utexas.edu/pages/research/borg/resources.html . </li>
   <li> Github repository at
   https://github.com/borg-project/pb-competition-borg . </li>
  </ul>


  \todo clasp:
  <ul>
   <li> Available at http://www.cs.uni-potsdam.de/clasp/ . </li>
   <li> See ExternalSources/SAT/Potassco/ . </li>
   <li> Replaces pbclasp. </li>
  </ul>


  \todo SAT4J:
  <ul>
   <li> Available at http://www.sat4j.org/ . </li>
   <li> Offers both pseudo-boolean and MaxSAT variants. </li>
   <li> See "SAT4J". </li>
  </ul>


  \todo BoolVar/PB
  <ul>
   <li> http://arxiv.org/abs/1103.3954 describes a java library for translating
   pseudo-boolean constraints to CNF. </li>
   <li> The website is at http://boolvar.sourceforge.net/. </li>
   <li> BoolVar is a Java library with which one constructs a pseudo-boolean
   problem in Java, and then translates this to either SAT or OPB (the
   pseudo-boolean competition format). </li>
   <li> The ".jar" file is a collection of class files packaged together.
   It is already compiled. </li>
   <li> BoolVar does not support the reading in of OPB files and output
   of a SAT problem as an application. </li>
   <li> To convert OPB files to SAT, we would have to write an OPB file
   parser and then use this to construct a pseudo-boolean problem in
   the BoolVar system. </li>
   <li> The source is available at
   http://sourceforge.net/projects/boolvar/files/ . </li>
   <li> Hopefully the source can be compiled by gcj, and then
   we can use the applications created. </li>
   <li> To compile BoolVar with gcj, we first need a working gcj; see
   "gcj doesn't compile Java files" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/Gcc.hpp. </li>
  </ul>

*/

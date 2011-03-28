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
   but no pseudo-boolean solvers. </li>
   <li> We should add all known pseudo-boolean solvers to the library. 
   </li>
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
   <li> Hopefully it can be compiled by gcj, and then we can use the
   applications created. </li>
  </ul>

*/

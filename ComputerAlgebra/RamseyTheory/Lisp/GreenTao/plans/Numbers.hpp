// Oliver Kullmann, 17.4.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/RamseyTheory/Lisp/GreenTao/plans/Numbers.hpp
  \brief Plans regarding Green-Tao numbers


  \todo Connections to other modules
  <ul>
   <li> In RamseyTheory/Lisp/GreenTao/Numbers.mac we need to provide
   everything known about van-der-Waerden numbers. </li>
   <li> See
   "Architectures of the systems for showing/computing Ramsey-type numbers" in
   ComputerAlgebra/RamseyTheory/Lisp/plans/general.hpp. </li>
   <li> And see
   Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/general.hpp.
   </li>
  </ul>


  \todo Update the number-system
  <ul>
   <li> ComputerAlgebra/RamseyTheory/Lisp/GreenTao/Numbers.mac needs a complete
   update. </li>
   <li> Regarding the general system (where
   ComputerAlgebra/RamseyTheory/Lisp/VanderWaerden/Numbers.mac can be taken as
   role model). </li>
   <li> And regarding the data. </li>
  </ul>


  \todo Computation of initial sequences of transversal GT-numbers
  <ul>
   <li> initial_sequence_GTt(k) should proceed until the first result returned
   by greentaot(m,k) appears which is not an integer. </li>
   <li> This should also make the case-distinction superfluous. </li>
   <li> The computation is very slow: vanderwaerdent(m,k) needs a more
   efficient algorithm. </li>
  </ul>

*/

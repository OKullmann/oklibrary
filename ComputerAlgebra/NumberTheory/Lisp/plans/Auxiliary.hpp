// Oliver Kullmann, 6.12.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/NumberTheory/Lisp/plans/Auxiliary.hpp
  \brief Plans regarding helper functions with number-theoretical character


  \todo Floors of logarithms
  <ul>
   <li> Currently we have fld, which is a special case of floorlog, can
   only handle integers, and which is also slower, but which can handle
   arguments of arbitrary size. </li>
   <li> Should "floorlog" be changed to "flog"? </li>
   <li> floorlog could be made excepting arbitrary large inputs if arbitrary
   precision would be used --- perhaps a case distinction should be performed?
   </li>
   <li> The use of numeric logarithms will always involve a risk of false
   results, so precise algorithms are sought. </li>
  </ul>

*/


// Oliver Kullmann, 23.1.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Numerical/Lisp/plans/general.hpp
  \brief Plans for numerical Maxima/Lisp functions in general


  \todo Function "min_scanning" 
  (ComputerAlgebra/Numerical/Lisp/Minimisation.mac)
  <ul>
   <li> See "Local variables" in ComputerAlgebra/plans/MaximaTechniques.hpp.
   </li>
   <li> Is it avoidable that argument-names like "f8765" are captured by
   function definitions (so that in the presence of a function with
   name "f8765" min_scanning is not working)?? </li>
  </ul>


  \todo Implement the "downhill simplex method in multidimensions"
  (according to Nelder and Mead).

*/


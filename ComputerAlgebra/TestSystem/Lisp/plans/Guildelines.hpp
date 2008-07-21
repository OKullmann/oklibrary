// Oliver Kullmann, 21.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/TestSystem/Lisp/plans/Guidelines.hpp
  \brief Plans on providing guidelines on the test system


  \todo Calling sub-tests
  <ul>
   <li> When a testfunction okltest_f calls another testfunction okltest_g,
   then it seems sensible to do this via "assert(okltest_g(...))", in order
   to make sure that the test was passed. </li>
   <li> In principle this might not be necessary, since in case of failure
   an exception is thrown; though it should be safer to check also the
   return value. </li>
   <li> A disadvantage is that the assert failure then happens at the
   place where the sub-test is called, and no information on the failing
   subtest is available. </li>
   <li> In order to get this information, "assert(okltest_g(...))" needs to
   be changed to (simply) "okltest_g(...)". </li>
  </ul>

*/


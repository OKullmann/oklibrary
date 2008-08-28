// Oliver Kullmann, 9.11.2006 (Swansea)
/* Copyright 2006 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Database/plans/general.hpp
  \brief Plans for the module concerning the formulas-database


  \bug We should avoid any rounding errors in the output of CreateStatistics.cpp
  <ul>
   <li> Scientific notation perhaps is better avoided. </li>
   <li> And floating point numbers should be output at a high
   precision. </li>
  </ul>


  \todo General databases
  <ul>
   <li> Compare "The general framework" in Experimentation/plans/general.hpp.
   </li>
   <li> A relational database likely is too specialised to provide a common
   framework for all the data relevant in the context of SAT (e.g., special
   minimally unsatisfiable clause-sets, interesting satisfying assignments
   for hard problems, data on formula classes, data on the behaviour of
   algorithms. </li>
   <li> Perhaps we have something common "root", simply file-based, which
   contains then, as usual in sub-directories, the more specialised
   databases. </li>
  </ul>


  \todo Documentation
  <ul>
   <li> Add minimal doxygen-documentation to the C++ files. </li>
  </ul>


  \todo make
  <ul>
   <li> Shall we use the local postgresql-installation for compilation
   (as include-source)? Perhaps as the default. </li>
  </ul>


  \todo Other files
  <ul>
   <li> How to treat the .dtd and .sql files? </li>
  </ul>


*/



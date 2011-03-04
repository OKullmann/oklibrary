// Oliver Kullmann, 29.6.2008 (Swansea)
/* Copyright 2008, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/docus/MaximaTechniques.hpp
  \brief Documentation on how to write programs in Maxima/Lisp


  <h1> Programming at the Maxima/Lisp level in the OKlibrary </h1>


  <h2> Naming conventions </h2>

  <h3> File names </h3>

  <ul>
   <li> All Maxima-code is placed in "Filename.mac"-files. </li>
   <li> Exceptions are only demo-files (placed in demos-subdirectories),
   which have the suffix ".dem". </li>
   <li> Lisp-files have suffix ".lisp". </li>
  </ul>


  <h2> Testing and checking </h2>

  <h3> Input checking </h3>

  <ul>
   <li> No input validation is performed (at the Maxima/Lisp level), since
    <ol>
     <li> it makes the code hard to write, read, and maintain, </li>
     <li> creates considerable runtime overhead, </li>
     <li> and is inadequate at this level, where experimentation and
     interaction is the focus, </li>
     <li> and finally, Maxima represents the set-theoretical level, the
     (full) mathematical world. </li>
    </ol>
   </li>
   <li> For a definition "D" of a special type of Maxima-objects, for
   examples graphs or clause-sets, predicates "D_p" (for example
   "g_p" or "cs_p") are provided, which can be used in the (higher-order)
   unit-tests. </li>
  </ul>

*/


// Oliver Kullmann, 16.1.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/docus/MaximaTutorial.hpp
  \brief Tutorial Maxima/Lisp


  <h1> Tutorial for Maxima in the OKlibrary </h1>

  <ol>
   <li> To make use of the special OKlibrary Maxima functions, first they have to be loaded. One possibility is to load individual files seperately using:
   \verbatim
(%i1) oklib_load(filename)
   \endverbatim
   Alternatively, every function from the OKlibrary can be loaded
   simultaneously by using the oklib_load_all() function.
   \verbatim
(%i2) oklib_load_all()$
0 errors, 0 warnings
   \endverbatim
   </li> 
   <li> %Graphs
    <ol>
     <li> We provide graphs (directed and undirected, simple and general)
     in the direct mathematical definition. </li>
     <li> See the tutorial ComputerAlgebra/Graphs/Lisp/docus/Tutorial.hpp. </li>
    </ol>
   </li>
   <li> %Satisfiability
    <ol>
     <li> Similarly, we provide clause-sets (boolean, and generalised)
      according to the mathematical definition. </li>
     <li> See the tutorial
      ComputerAlgebra/Satisfiability/Lisp/docus/Tutorial.hpp. </li>
    </ol>
   </li>
   <li> %Trees
    <ol>
     <li> We provide trees (directed, rooted, labelled and unlabelled, binary 
     and general) in the direct mathematical definition. </li>
     <li> See the tutorial ComputerAlgebra/Trees/Lisp/docus/Tutorial.hpp. </li>
    </ol>
   </li>
  </ol>

*/


// Oliver Kullmann, 11.6.2007 (Swansea)
/* Copyright 2007, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/plans/milestones.hpp
  
  \module_version Transformers/Generators 0.0.8 (28.11.2010)


  \par Version 0.0.9

  \par
   In Satisfiability/Transformers/Generators/plans/GreenTao.hpp the following 
   topics are handled:
    - Improving SAT translations


  \par Version 0.0.10

  \par
   In Transformers/Generators/plans/general.hpp the following
   topics are handled:
    - Update


  \par Version 0.0.11

  \par
   In Satisfiability/Transformers/Generators/plans/VanderWaerden.hpp the
   following topics are handled:
    - Update

  \par
   In Satisfiability/Transformers/Generators/plans/Ramsey.hpp the following 
   topics are handled:
    - Strengthen connection to Maxima/Lisp level
    - Move todos to this file


  \par Version 0.0.12

  \par
   In Satisfiability/Transformers/Generators/plans/GreenTao.hpp the following 
   topics are handled:
    - Connections and scope
    - The basic generators
    - Improving GTTransversals
    - Improving GTTransversalsInc


  \par Version 0.0.13

  \par
   In Transformers/Generators/LinInequal.hpp the following topics are handled:
    - Update function names
    - Complete LinInequal.cpp
    - Also handle ">=" and "=" (besides "<=")

  \par
   In Satisfiability/Transformers/Generators/plans/Ramsey.hpp the following 
   topics are handled:
    - Write unit tests


  \par Version 0.0.14

  \par
   In Transformers/Generators/LinInequal.hpp the following topics are handled:
    - Create Doxygen documentation
    - Improve VdWTransversals and VdWTransversalsInc
    - Write docus

  \par
   In Satisfiability/Transformers/Generators/plans/Ramsey.hpp the following 
   topics are handled:
    - Add generator to generate general %Ramsey problems


  \par Version 0.0.15

  \par
   In Transformers/Generators/plans/FaronsPuzzle.hpp the following
   topics are handled:
    - bug : When using symmetry breaking
    - Comments in output

  \par
   In Transformers/Generators/LinInequal.hpp the following topics are handled:
    - Restore old "assignment-application"

  \par
   Create new milestones.


  \par Version 0.0.16

  \par
   In Transformers/Generators/plans/general.hpp the following
   topics are handled:
    - Sudoku

  \par
   In Transformers/Generators/plans/FaronsPuzzle.hpp the following
   topics are handled:
    - Create doxygen-documentation
    - Use an CLSAdaptor for output
    - Test.cpp
    - Write docus



  -------------------------------------------------------------------------------------------------------------------------------------

  \par Version history

   - 0.0.5 : 11.6.2007; initial version number (several plans, and two (old) implementations in OKsolver/Experimental/Transformationen).
   - 0.0.6 : 10.11.2007; update of plans and restructuring.
   - 0.0.7 : 6.12.2009; tools for computing transversal numbers for Green-Tao and van-der-Waerden problems, tools for cardinality constraints, SAT generators for Hindman- and GreenTao-problems.
   - 0.0.8 : 28.11.2010; generator for UCP-testing, various corrections and improvements, extensions of existing generators, new generator for palindromic vdW-problems.

*/


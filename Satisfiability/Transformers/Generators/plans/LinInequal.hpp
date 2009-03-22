// Oliver Kullmann, 14.3.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Transformers/Generators/plans/LinInequal.hpp
  \brief Plans regarding encoding linear inequalities as CNF


  \todo DONE
  Create application
  <ul>
   <li> DONE (restored and updated old code)
   The application was contained (as commented code)
   in the old Transformers/Generators/Test.cpp, which was removed in
   commit 7ad4d9916dea861abab631f2904e49841a86892f (10.11.2007). </li>
   <li> DONE (completed the main application)
   Now one needs to find out what the code is doing:
    <ol>
     <li> The parameters from the command line are unsigned integers, where
     the last is the "level", while the others are "capacities". </li>
     <li> From standard input first unsigned integers L, B are read, where
     L apparently yields variables V1, ..., VL; B should be the upper bound.
     </li>
     <li> Then apparently from standard input triples
     (identifier, choice, preference) are read, where choice and preference
     are unsigned integers. </li>
    </ol>
   </li>
  </ul>


  \todo Update function names
  <ul>
   <li> Generators/LinInequal.hpp yet uses class-like names for functions;
   this should be changed. </li>
  </ul>


  \todo Complete LinInequal.cpp
  <ul>
   <li> DONE (see 'Restore old "assignment-application"' below)
   It remains to describe what happens if two or more parameters are
   given.
    <ol>
     <li> The main point is to find out what LinInequal::Assignment is doing.
     </li>
     <li> Likely, this should then be moved into its own application. </li>
    </ol>
   </li>
   <li> DONE
   Once we removed this case, the current reading from standard input
   likely should be abolished, and command line parameters used. </li>
   <li> We should have the alternative to demand not just the upper bound,
   but also the lower bound or the strict equality.
    <ol>
     <li> See 'Also handle ">=" and "=" (besides "<=")' below. </li>
     <li> Only ">=" is missing now. </li>
     <li> DONE
     This then can be controlled by an additional command-line parameter.
     </li>
     <li> DONE
     So we have three required parameters: n, bound, comp, where comp
     is currently one of the strings "==", "<=". </li>
     <li> DONE
     A fourth optional parameter is the variable-prefix. </li>
    </ol>
   </li>
  </ul>


  \todo Also handle ">=" and "=" (besides "<=")
  <ul>
   <li> This is possible by two further variations on function
   LinInequal::CompVar. </li>
   <li> DONE
   The name "CompVar" then should be changed, say, "compvar_le". </li>
   <li> The new functions are called "compvar_ge" and "compvar_eq". </li>
   <li> compvar_eq : DONE
    <ol>
     <li> The only problem is what to do if the clause-set to be generated is
     unsatisfiable (Dimacs doesn't allow empty clauses). </li>
     <li> Simplest is to throw an exception. </li>
     <li> Easiest from a standard exception class (since this code needs to
     be refactored anyway). </li>
    </ol>
   </li>
  </ul>


  \todo Create Doxygen documentation
  <ul>
   <li> It remains to document LinInequal::Assignment. </li>
  </ul>


  \todo Restore old "assignment-application"
  <ul>
   <li> The (now removed) code handling the special "assignment"-case is
   \code
if (argc > 2) {
  LinInequal::ui_vec Cap;
  Cap.reserve(argc-2);
  for (unsigned int i = 1; i < argc-1; ++i)
    Cap.push_back(StringHandling::fromString<unsigned int>(argv[i]));
  const unsigned int level =
   StringHandling::fromString<unsigned int>(argv[argc-1]);
  LinInequal::Assignment(Cap, std::cin, level, std::cout);
}
   \endcode
   </li>
   <li> First it has to be established what this is doing. </li>
  </ul>

  \todo Improve VdWTransversals and VdWTransversalsInc
  <ul>
   <li> Once available as application, the translation of extended into strict
   Dimacs format together with correcting the parameter-line should be used
   (instead of yet using the OKsolver together with awk). See "Improve
   application InputOutput/ExtendedToStrictDimacs.cpp" in
   Interfaces/InputOutput/plans/general.hpp. </li>
   <li> The generalisation of these scripts to a transformer "hypergraph
   transversal -> SAT" and to a sequence generator for transversal numbers
   of monotonically increasing hypergraph sequences should be made
   available. </li>
  </ul>


  \todo Write docus
  <ul>
   <li> Also the applications VdWTransversals and VdWTransversalsInc need
   documentation. </li>
  </ul>

*/

// Oliver Kullmann, 7.11.2006 (Swansea)
/* Copyright 2006 - 2007, 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Algorithms/LocalSearch/plans/general.hpp
  \brief Plans for the general module on local search


  \todo Using Ubcsat as a library
  <ul>
   <li> Once the successor of version 1.1.0 is out (and we successfully
   installed it), we need to re-evaluate the possibility of using Ubcsat as
   an ordinary library. </li>
   <li> Relevant build-system-variables are ubcsat_installsrc_okl,
   ubcsat_include_option_okl, ubcsat_link_okl and ubcsat_link_option_okl. </li>
   <li> Yet we have to use the deprecated gcc-option "-I-" (see variable
   ubcsat_include_option_okl; the application using this is
   Satisfiability/Algorithms/Autarkies/Search/AnalyseTotalAssignment.cpp).
   </li>
  </ul>


  \todo Relations to other modules
  <ul>
   <li> See ComputerAlgebra/Satisfiability/Lisp/LocalSearch/plans/general.hpp.
   </li>
  </ul>


  \todo Installation of UBCSAT completed
  <ol>
   <li> See "Building Ubcsat" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/Ubcsat.hpp. </li>
   <li> Some documentation is needed (goes to
   Buildsystem/docus/ExternalSources.hpp):
    <ul>
     <li> how to use the binary </li>
     <li> how to use the library files </li>
     <li> what are those library files </li>
     <li> how to use LocalSearch/Ubcsat. </li>
    </ul>
   </li>
   <li> Temporary build-directory:
    <ol>
     <li> We could get rid off the temporary build-directory by adding
     \code
-I- -I$(OKsystem)/OKlib/LocalSearch/Ubcsat/corrected
     \endcode
     to the build-compilation. </li>
     <li> However this option is deprecated with gcc version 4.2. </li>
     <li> We should try to convince the Ubcsat-developers to use a standard
     directory structure. </li>
    </ol>
   </li>
  </ol>


  \todo Contact the developers of Ubcsat
  <ul>
   <li> See "Update to version 1.1.0" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/Ubcsat.hpp. </li>
   <li> Wish-list for a new Ubcsat-release 1.2.0:
    <ol>
     <li> Weak performance on 32-bit machines (with version 1.0.0): See
     "Speed" in Buildsystem/ExternalSources/SpecialBuilds/plans/Ubcsat.hpp.
     </li>
     <li> Higher cutoffs: See "Cutoff value etc. should be 64 bits on a
     64-bit machine" in
     Buildsystem/ExternalSources/SpecialBuilds/plans/Ubcsat.hpp. </li>
     <li> Input of large numbers: See "More readable large numbers:" in
     ExperimentSystem/ControllingLocalSearch/plans/Ubcsat-okl.hpp. </li>
     <li> Use of C99: nowadays every C-compiler is a C99 compiler, and using
     the C99 possibilities (like the new parts of the library) would enhance
     the code-quality (and our user-experience). </li>
    </ol>
   </li>
  </ul>


  \todo Create further milestones (for 0.0.7)


  \todo Framework for the analysis of search space structures
  <ul>
   <li> The task is to implement various methods for studying search space
   structures, so that local search algorithms can be adapted to specific
   instances and/or instance classes. </li>
   <li> First we develop these methods at the Maxima/Lisp level; see
   "Examing search landscapes" in
   ComputerAlgebra/Satisfiability/Lisp/LocalSearch/plans/general.hpp. </li>
   <li> At the Maxima/Lisp level we concentrate on exact computations, and
   for methods based on sampling on clean and basic methods; for more
   practically oriented methods R should be used. </li>
  </ul>


*/

/*!
  \namespace OKlib::Satisfiability::Algorithms::LocalSearch
  \brief Components for supporting local search

  Alias "LSe".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Algorithms {
      namespace LocalSearch {
      }
      namespace LSe = LocalSearch;
    }
  }
}


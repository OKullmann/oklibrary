// Oliver Kullmann, 14.6.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Applications/RamseyTheory/MinimumTransversals_VanderWaerden.cpp
  \brief Application for computing minimum transversal of hypergraphs of arithmetic progressions

  <ul>
   <li> The two command line parameters are N and k, the maximal number of
   vertices and the progression-length. </li>
   <li> Provides the generator for
   Combinatorics/Hypergraphs/Transversals/Bounded/MinimumTransversalsMongen.cpp
   (there the main-function is to be found), and thus that library-file has to
   be compiled before creating the full application here. </li>
   <li> Since currently this dependency can not be expressed by our build
   system, this application is not compiled automatically, but has to be
   be compiled on demand by using
   \verbatim
RamseyTheory> oklib all programs=MinimumTransversals_VanderWaerden
   \endverbatim
   </li>
  </ul>

*/

#include <set>
#include <vector>
#include <stdexcept>

#include <OKlib/Combinatorics/Hypergraphs/Generators/VanderWaerden.hpp>

#include <OKlib/Combinatorics/Hypergraphs/Transversals/Bounded/MinimumTransversalsMongen.hpp>

namespace OKlib {
 namespace Combinatorics {
  namespace Hypergraphs {
   namespace Transversals {
    namespace Bounded {

      OKlib::Combinatorics::Hypergraphs::Generators::Arithprog_finish<hyperedge_type, hyperedge_list_type, vertex_type> prog_gen;

      void initialise(const vertex_type, const parameter_type& P) {
        if (P.empty())
          throw std::runtime_error
            ("ERROR[MinimumTransversals_VanderWaerden]: "
             "The progression-length must be provided as parameter.");
        prog_gen.set(P.front());
      }

      hyperedge_list_type generator(const vertex_type n) {
        return prog_gen(n);
      }

    }
   }
  }
 }
}


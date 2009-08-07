// Oliver Kullmann, 30.7.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Combinatorics/Hypergraphs/Generators/Hindman.hpp
  \brief %Tools for generating Hindman hypergraphs
*/

#ifndef HINDMANHYPERGRAPH_jjdhsTfr435R
#define HINDMANHYPERGRAPH_jjdhsTfr435R

#include <vector>

namespace OKlib {
  namespace Combinatorics {
    namespace Hypergraphs {
      namespace Generators {

        /*!
          \class Hindman2_hypergraph
          \brief Functor for creating Hindman hypergraphs with k=2

          Specification: hindman_k2_ohg and hindmani_k2_ohg in
          ComputerAlgebra/Hypergraphs/Lisp/Generators/Hindman.mac.

          \todo To be implemented.
        */

        template <typename Int = int>
        class Hindman2_hypergraph {

          Hindman2_hypergraph(const Int a, const Int n, const bool inj = false) {

          }
        };

      }
    }
  }
}

#endif

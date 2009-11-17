// Oliver Kullmann, 15.11.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Applications/RamseyTheory/MinimumTransversals_GreenTao.cpp
  \brief Application for computing minimum transversal of hypergraphs of arithmetic progressions in the primes

  <ul>
   <li> The two command line parameters are N and k, where k is
   the progression-length, while N is the maximal number of vertices (i.e.,
   prime numbers). </li>
   <li> Provides the generator for
   Combinatorics/Hypergraphs/Transversals/Bounded/MinimumTransversalsMongen.cpp
   (there the "main"-function is to be found), and thus that library-file has
   to be compiled before creating the full application here. </li>
   <li> Since currently this dependency can not be expressed by our build
   system, this application is not compiled automatically, but has to be
   be compiled on demand by using
   \verbatim
RamseyTheory> oklib all programs=MinimumTransversals_GreenTao
   \endverbatim
   </li>
  </ul>

*/

#include <set>
#include <vector>
#include <stdexcept>
#include <cassert>

#include <OKlib/Combinatorics/Hypergraphs/Generators/GreenTao.hpp>
#include <OKlib/Combinatorics/Hypergraphs/Transversals/Bounded/VertexBranching.hpp>

#include <OKlib/Combinatorics/Hypergraphs/Transversals/Bounded/MinimumTransversalsMongen.hpp>

namespace OKlib {
 namespace Combinatorics {
  namespace Hypergraphs {
   namespace Transversals {
    namespace Bounded {
    
      typedef OKlib::Combinatorics::Hypergraphs::Generators::GreenTao<size_type> GT_hypergraph_type;
      typedef GT_hypergraph_type::set_system_type set_system_type;

      typedef OKlib::Combinatorics::Hypergraphs::Transversals::Bounded::DirectStratification<set_system_type, size_type> Strata_t;

      struct Wrapper {
        Wrapper() : S(0) {}
        void set(const size_type k, const size_type N) {
          if (S) delete S;
          GT_hypergraph_type G(k, N);
          S = new Strata_t(G.hyperedge_set(), G.vertex_set());  
        }
        hyperedge_list_type operator()(const size_type n) const {
          assert(S);
          return S -> operator()(n);
        }
        ~Wrapper() { delete S; }

        private :
          const Strata_t* S;
      };

      Wrapper prog_gen;


      void initialise(const size_type N, const parameter_type& P) {
        if (P.size() < 1)
          throw std::runtime_error
            ("ERROR[MinimumTransversals_GreenTao]: "
             "The progression-length must be provided as parameter.");
        prog_gen.set(P[0], N);
      }

      hyperedge_list_type generator(const size_type n) {
        return prog_gen(n);
      }

    }
   }
  }
 }
}


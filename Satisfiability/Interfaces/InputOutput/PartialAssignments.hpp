// Oliver Kullmann, 11.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Interfaces/InputOutput/PartialAssignments.hpp
  \brief Input and output of partial assignments

*/

#ifndef PARTIALASSIGNMENTS_hB32wQwqs12
#define PARTIALASSIGNMENTS

#include <istream>
#include <set>
#include <string>
#include <cassert>

#include <OKlib/Satisfiability/ProblemInstances/Literals/var.hpp>

namespace OKlib {
  namespace Satisfiability {
    namespace Interfaces {
      namespace InputOutput {

        struct Error_readpass_missing_v {};
        struct Error_readpass_stream {};
        struct Error_readpass_nonint {};
        struct Error_readpass_contradiction {};

        /*!
          \class ReadPass
          \brief Reading a partial assignment from an input stream

          <ul>
           <li> The stream starts with "v ", followed by non-zero integers
           separated by space-symbols, representing the boolean literals to be
           set to true, and concluded by "0". </li>
           <li> Space-symbols (also leading and trailing ones) are ignored.
           </li>
           <li> After construction, in data-member litset the set of literals
           set to true is obtained, in data-member pa the corresponding
           partial assignment, and in data-member max_var the maximal absolute
           value of literals (i.e., the maximal variable-index). </li>
           <li> Only integral literals are possible. </li>
           <li> Besides syntax-errors the only error is contradictory literals.
           </li>
           <li> Repeated literals are ignored. </li>
          </ul>
        */

        template <typename Lit, class Pass>
        struct ReadPass {
          typedef Lit literal_type;
          typedef Pass pass_type;
          typedef std::set<literal_type> litset_t;
          typedef typename litset_t::const_iterator iterator;

          const litset_t litset;
          const pass_type pa;
          const literal_type max_var;

          ReadPass(std::istream& in) : litset(fill_litset(in)), pa(litset.begin(),litset.end()), max_var(maxvar()) {}

        private :

          litset_t fill_litset(std::istream& in) const {
            {std::string token;
             in >> token;
             if (not in) throw Error_readpass_stream();
             if (token != "v") throw Error_readpass_missing_v();
            }
            litset_t litset;
            do {
              assert(in);
              literal_type x;
              in >> x;
              if (not in) throw Error_readpass_nonint();
              if (x == 0) break;
              const iterator fc = litset.find(-x);
              if (fc != litset.end()) throw Error_readpass_contradiction();
              litset.insert(x);
            } while (true);
            return litset;
          }

          literal_type maxvar() const {
            literal_type m = 0;
            const iterator lsend = litset.end();
            for (iterator i = litset.begin(); i != lsend; ++i) {
              const literal_type x = OKlib::Literals::var(*i);
              if (x > m) m = x;
            }
            return m;
          }

        };
        
      }
    }
  }
}

#endif

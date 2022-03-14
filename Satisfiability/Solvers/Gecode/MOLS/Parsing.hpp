// Oliver Kullmann, 8.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Parsing of the problem specification


1. Example:

--------------------
squares A B c10 # comment; k=3

# empty lines possible (always optional)

A        rls cls symm # like the simple format for graphs
at B     cls diag # these are conditions for at(B)
c231 A   idem rred # these variants will be introduced (basic encoding)
A        uni cls # as with programming languages, space-symbols are collapsed

= c213 c10  c321 A
= B B # we allow that (in the basic encoding)
= A  c213 A

ortho A  c213 B  at A
# ortho A # this is an error
# ortho # this is an error
--------------------


The order above is irrelvant except of that "squares" acts like a declaration
in programming languages (old-fashioned way: only one declaration at the top).

The search for three MOLS is formulated as:

squares A B C
A rls cls
B rls cls
C rls cls
ortho A B C


A doubly self-orthogonal ls:

squares A
ortho A c213 A
ortho A at A


Totally symmetric Latin square: all six conjugates are equal:

squares A
A symm
= A c312 A
= A c321 A
= A c213 A
= A c231 A
= A c132 A


QUESTION: it seems that for N>=3 one can not have both diag and antidiag
at the same time? But easiest to allow such combinations (and such tests
can be useful for performance evaluation).


1. Conditions not included:

    - Semisymmetric Latin square: it is equal to (at least) three of its
      conjugates, which must necessarily include the (1, 2, 3), (3, 1, 2) and
      (2, 3, 1)-conjugates.

      Due to the "or" we don't handle that.

*/

#ifndef PARSING_W8d7RNyiKu
#define PARSING_W8d7RNyiKu

#include <istream>
#include <vector>
#include <exception>
#include <string>
#include <optional>
#include <sstream>

#include <cassert>

#include <ProgramOptions/Strings.hpp>

#include "Conditions.hpp"

namespace Parsing {

  namespace CD = Conditions;

  using CD::size_t;

  struct Error : std::runtime_error {
    Error(const std::string s) : std::runtime_error("ERROR[Parsing(Conditions)]:: " + s) {}
  };


  class ReadAC {

    typedef Environment::tokens_t tokens_t;
    typedef Environment::indstr_t indstr_t;

    std::optional<CD::Square> read_sq(const tokens_t line, size_t& j,
                                      const indstr_t& is) const noexcept {
      const size_t N = line.size();
      if (j >= N) return {};
      const CD::VS vs = CD::toVS(line[j]);
      if (vs != CD::VS::id) ++j;
      if (not is.second.contains(line[j])) return {};
      return CD::Square{(*is.second.find(line[j++])).second, vs};
    }


    CD::AConditions operator()(std::istream& in) const {
      assert(not in.bad());
      const auto content = Environment::split2_cutoff(in, '\n', '#');
      const size_t numlines = content.size();
      if (numlines == 0) throw Error("File empty.");
      assert(not content[0].empty());
      if (content[0][0] != CD::AConditions::decl_keyword)
        throw Error("First line, first entry does not cotain \"" +
                    std::string(CD::AConditions::decl_keyword) + "\", but \"" +
                    content[0][0] + "\"");
      const indstr_t is =
        Environment::indexing_strings(content[0].cbegin()+1,
                                      content[0].end(), false);
      const size_t k = is.first.size();
      CD::AConditions res(k);
      if (k == 0) throw Error("No squares declared.");
      for (size_t i = 1; i < numlines; ++i) {
        const auto& line = content[i];
        assert(not line.empty());
        if (line[0] == "=") {
          size_t j = 1;
          const auto sq1 = read_sq(line, j, is);
          if (not sq1) throw Error("Bad first square \"" + line[j] +
                                   "\" in eq-line " +
                                   std::to_string(i) + " at position " +
                                   std::to_string(j));
          const auto sq2 = read_sq(line, j, is);
          if (not sq2) throw Error("Bad second square \"" + line[j] +
                                   "\" in eq-line " +
                                   std::to_string(i) + " at position " +
                                   std::to_string(j));
          if (j < line.size())
            throw Error("Eq-line " + std::to_string(i) +
                        " has trailing content, size = " +
                        std::to_string(line.size()));
          const CD::Square s1 = sq1.value(), s2 = sq2.value();
          res.insert(s1); res.insert(s2);
          res.insert(CD::Equation(s1, s2));
        }
        else if (line[0] == CD::AConditions::orth_keyword) {
          size_t j = 1;
          const size_t size = line.size();
          std::set<CD::Square> ssqs;
          while (j < size) {
            const auto sq = read_sq(line, j, is);
            if (not sq) throw Error("Bad square \"" + line[j] +
                                    "\" in orth-line " +
                                   std::to_string(i) + " at position " +
                                   std::to_string(j));
            const CD::Square s = sq.value();
            if (not ssqs.insert(s).second) {
              std::ostringstream ss;
              ss << "Repeated square " << s << " in orth-line " << i <<
                " at position " << j << ".";
              throw Error(ss.str());
            }
            res.insert(s);
          }
          if (ssqs.size() <= 1)
            throw Error("Orth-line " + std::to_string(i) + " has only " +
                        std::to_string(ssqs.size()) + " squares.");
          res.insert(ssqs);
        }
        else {
          // XXX
        }
      }
      return res;
    }

  };

}

#endif

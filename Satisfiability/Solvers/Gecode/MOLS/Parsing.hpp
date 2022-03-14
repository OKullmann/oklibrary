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
#include <utility>

#include <cassert>

#include <ProgramOptions/Strings.hpp>

#include "Conditions.hpp"

namespace Parsing {

  namespace CD = Conditions;

  using CD::size_t;

  struct Error : std::runtime_error {
    Error(const std::string s) : std::runtime_error("ERROR[Parsing(Conditions)]:: " + s) {}
  };


  // Condition-Type:
  enum class CT {
    unknown = 0,
    unary = 1,
    equation = 2,
    prod_equation = 3
  };
  typedef std::pair<CT, size_t> cl_t;
  cl_t classify(const std::string& s) noexcept {
    assert(not s.empty());
    if (s.front() == '=') return {CT::equation,0};
    else if (const size_t uc = size_t(CD::toUC(s)); uc != 0)
      return {CT::unary, uc};
    else if (const size_t pt = size_t(CD::toPT(s)); pt != 0)
      return {CT::prod_equation, pt};
    else return {CT::unknown,0};
  }


  class ReadAC {

    typedef Environment::tokens_t tokens_t;
    typedef Environment::indstr_t indstr_t;

    void out_line(std::ostream& out, const tokens_t& t) const {
      const size_t size = t.size();
      assert(size != 0);
      out << t[0];
      for (size_t i = 1; i < size; ++i) out << " " << t[i];
    }

    std::optional<CD::Square> read_sq(const tokens_t& line, size_t& j,
                                      const indstr_t& is) const noexcept {
      const size_t N = line.size();
      if (j >= N) return {};
      const CD::VS vs = CD::toVS(line[j]);
      if (vs != CD::VS::id) ++j;
      if (not is.second.contains(line[j])) return {};
      return CD::Square{(*is.second.find(line[j++])).second, vs};
    }

    std::vector<CD::Square> read_sqs(const size_t k,
                                     const tokens_t& line, size_t& j,
                                     const indstr_t& is) const {
      assert(not line.empty());
      std::vector<CD::Square> res; res.reserve(k);
      for (size_t i = 0; i < k; ++i) {
        const auto sq = read_sq(line, j, is);
        if (not sq) {
          std::ostringstream ss;
          ss << "Bad square number " << i << " at position " << j <<
            "in line\n \"";
          out_line(ss, line); ss << "\"";
          throw Error(ss.str());
        }
        res.push_back(sq.value());
      }
      if (j < line.size()) {
        std::ostringstream ss;
        ss << "The following line contains more than " << k
           << " squares:\n \"";
        out_line(ss, line); ss << "\"";
        throw Error(ss.str());
      }
      assert(res.size() == k);
      return res;
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
        const auto [ct, index] = classify(line[0]);
        switch (ct) {
        case CT::unknown :
          throw Error("Unknown key-word \"" + line[0] + "\" in line " +
                      std::to_string(i));
        case CT::equation : {
          size_t j = 1;
          const auto sqs = read_sqs(2, line, j, is);
          res.insert(sqs[0]); res.insert(sqs[1]);
          res.insert(CD::Equation(sqs[0], sqs[1]));
          break;
        }
        case CT::prod_equation : {
          size_t j = 1;
          const auto sqs = read_sqs(3, line, j, is);
          res.insert(sqs[0]); res.insert(sqs[1]); res.insert(sqs[2]);
          res.insert(CD::ProdEq(sqs[0], sqs[1], sqs[2], CD::PT(index)));
          break;
        }
        case CT::unary : {
          
        }}
        return res;
      }
    }

  };

}

#endif

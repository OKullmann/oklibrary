// Oliver Kullmann, 28.2.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <sstream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "DimacsTools.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.5",
        "28.3.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/TestDimacsTools.cpp",
        "GPL v3"};

  using namespace DimacsTools;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {assert(eqp(var(Clause{}),{}));
   assert(eqp(var(Clause{Lit(), Lit(1,-1), Lit(1,1)}), {Var(0), Var(1)}));
   assert(eqp(var(ClauseList{}),{}));
   assert(eqp(var(ClauseList{{}, {Lit(2,-1)}, {Lit(2,1), Lit(3,1)}}),
              {Var(2), Var(3)}));
   assert(eqp(var(DimacsClauseList{{0,0},
                      {{Lit()}, {Lit(2,-1)}, {Lit(2,1), Lit(3,1)}}}),
              {Var(), Var(2), Var(3)}));
  }

  {std::stringstream ss;
   ss.str("p cnf 0 0\n");
   assert(eqp(read_strict_dimacs_pars(ss), {0,0}));
   ss.str("p cnf 55 77\n");
   assert(eqp(read_strict_dimacs_pars(ss), {55,77}));
   ss.str("c\nc xx\np cnf 0 88\n");
   assert(eqp(read_strict_dimacs_pars(ss), {0,88}));
   assert(ss); assert(not ss.eof()); assert(ss.peek() == -1);
  }

  {std::stringstream ss;
   ss.str("0\n");
   assert((read_strict_literal(ss) == Lit{0,1}));
   ss.str("4 ");
   assert((read_strict_literal(ss) == Lit{4,1}));
   ss.str("-77 ");
   assert((read_strict_literal(ss) == Lit{77,-1}));
  }

  {std::stringstream ss;
   ss.str("0\n");
   assert(eqp(read_strict_clause(ss), {}));
   ss.str("3 -4 5 -6 0\n");
   assert(eqp(read_strict_clause(ss), {Lit{3,1}, Lit{4,-1}, Lit{5,1}, Lit{6,-1}}));
   assert(ss); assert(not ss.eof()); assert(ss.peek() == -1);
  }

  {std::stringstream ss;
   ss.str("p cnf 0 0\n");
   assert(eqp(read_strict_Dimacs(ss), {{0,0},{}}));
   ss.str("p cnf 5 0\n");
   assert(eqp(read_strict_Dimacs(ss), {{5,0},{}}));
   ss.str("p cnf 5 1\n0\n");
   assert(eqp(read_strict_Dimacs(ss), {{5,1},{{}}}));
   ss.str("p cnf 5 1\n1 2 -3 0\n");
   assert(eqp(read_strict_Dimacs(ss), {{5,1},{{Lit{1,1}, Lit{2,1}, Lit{3,-1}}}}));
   ss.str("p cnf 5 2\n1 2 -3 0\n0\n");
   assert(eqp(read_strict_Dimacs(ss), {{5,2},{{Lit{1,1}, Lit{2,1}, Lit{3,-1}},{}}}));
   ss.str("p cnf 5 2\n1 2 -3 0\n-4 5 0\n");
   assert(eqp(read_strict_Dimacs(ss), {{5,2},{{Lit{1,1}, Lit{2,1}, Lit{3,-1}},{Lit{4,-1},Lit{5,1}}}}));
   ss.str("p cnf 5 3\n1 2 -3 0\n0\n-4 5 0\n");
   assert(eqp(read_strict_Dimacs(ss), {{5,3},{{Lit{1,1}, Lit{2,1}, Lit{3,-1}},{},{Lit{4,-1},Lit{5,1}}}}));
   ss.str("p cnf 5 4\n1 2 -3 0\n0\n-4 5 0\n0\n");
   assert(eqp(read_strict_Dimacs(ss), {{5,4},{{Lit{1,1}, Lit{2,1}, Lit{3,-1}},{},{Lit{4,-1},Lit{5,1}},{}}}));
   ss.str("p cnf 5 4\n1 2 -3 0\n1 5 0\n-4 5 0\n0\n");
   assert(eqp(read_strict_Dimacs(ss), {{5,4},{{Lit{1,1}, Lit{2,1}, Lit{3,-1}},{Lit{1,1},Lit{5,1}},{Lit{4,-1},Lit{5,1}},{}}}));
   ss.str("p cnf 5 1\n1 2 3 0\n");
   assert(eqp(read_strict_Dimacs(ss), {{5,1},{{Lit{1,1}, Lit{2,1}, Lit{3,1}}}}));
   assert(ss); assert(not ss.eof()); assert(ss.peek() == -1);
  }

  {GslicedCNF F;
   assert(valid(F));
   F.O() = DimacsClauseList{{0,1},{{}}};
   assert(valid(F.O()));
   assert(F.O().first.c == 1);
   F.G() = DimacsClauseList{{1,1},{{Lit(1,1)}}};
   assert(valid(F.G()));
   assert(F.G().first.c == 1);
   assert(valid_slicedcnf(F.SF));
   assert(F.V.empty());
   assert(not valid(F));
   F.V = {Var(1)};
   assert(valid(F));
  }

  {DimacsClauseList F{{0,0},{}};
   {const auto res = minisat_call(F);
    assert(res.stats.sr == SolverR::sat);
    assert(eqp(res.pa, {}));}
   {const auto res = minisat_call(DimacsClauseListref_put(F));
    assert(res.stats.sr == SolverR::sat);
    assert(eqp(res.pa, {}));}
  }
  {DimacsClauseList F{{1,2}, { {Lit(1,1)}, {Lit(1,-1)} } };
   {const auto res = minisat_call(F);
    assert(res.stats.sr == SolverR::unsat);
    assert(eqp(res.pa, {}));}
   {const auto res = minisat_call(DimacsClauseListref_put(F));
    assert(res.stats.sr == SolverR::unsat);
    assert(eqp(res.pa, {}));}
  }
  {DimacsClauseList F{{2,2}, {  {Lit{1,1},Lit{2,1}}, {Lit{1,-1}} } };
   {assert(F.second.size() == 2);
    const auto res = minisat_call(F, [](const Lit x){return x.v.v!=1;});
    assert(res.stats.sr == SolverR::sat);
    assert(eqp(res.pa, {Lit{2,1}}));}
   {assert(F.second.size() == 2);
    const auto res = minisat_call(DimacsClauseListref_put(F),
                                  [](const Lit x){return x.v.v!=1;});
    assert(res.stats.sr == SolverR::sat);
    assert(eqp(res.pa, {Lit{2,1}}));}
  }

  {bool caught = false;
   // In the following case, minisat does not create an output-file:
   try { const auto res = minisat_call("X", triv_filter, "--help"); }
   catch(const std::runtime_error& e) {
     const auto lines = Environment::split(e.what(), '\n');
     assert(lines.size() >= 4);
     assert(lines[0] ==
            "DimacsTools::minisat_call: Error when calling SAT-solver by");
     assert(lines[3].starts_with(
       "DimacsTools::minisat_call(file): error when removing file"));
     caught = true;
    }
   assert(caught);
  }

  {std::istringstream ss;
   ss.str("0 1");
   assert(read_strict_variable(ss) == Var(0));
   assert(read_strict_variable(ss) == Var(1));
   assert(ss); assert(ss.eof());
  }
  {std::istringstream ss;
   ss.str("a 0\n");
   assert(eqp(read_strict_aline(ss), {}));
   assert(ss); assert(not ss.eof()); assert(ss.peek() == -1);
   ss.str("a 55 12 0\n"); ss.clear();
   assert(eqp(read_strict_aline(ss), {Var(55), Var(12)}));
   assert(ss); assert(not ss.eof()); assert(ss.peek() == -1);
  }
  {std::istringstream ss;
   ss.str("e 0\n2 -1 0\n");
   skip_strict_eline(ss);
   assert(eqp(read_strict_clause(ss), {Lit(2), -Lit(1)}));
   assert(ss); assert(not ss.eof()); assert(ss.peek() == -1);
  }
  {std::istringstream ss;
   ss.str("a 0\n");
   assert(eqp(read_strict_gline(ss), {}));
   assert(ss); assert(ss.eof());
   ss.clear(); ss.str("a 0\na 12 0\ne 13 0\n");
   assert(eqp(read_strict_gline(ss), {}));
   assert(ss); assert(ss.eof());
   ss.clear(); ss.str("a 55 12 0\n");
   assert(eqp(read_strict_gline(ss), {Var(55), Var(12)}));
   assert(ss); assert(ss.eof());
   ss.clear(); ss.str("a 77 1 0\na 9 0\ne 2 3 0\nx");
   assert(eqp(read_strict_gline(ss), {Var(77), Var(1)}));
   assert(ss); assert(not ss.eof()); assert(ss.peek() == 'x');
   ss.clear(); ss.str("e 2 3 0\na 88 0\nx");
   assert(eqp(read_strict_gline(ss), {}));
   assert(ss); assert(not ss.eof()); assert(ss.peek() == 'x');
   ss.clear(); ss.str("e 2 3 0\na 88 0\n");
   assert(eqp(read_strict_gline(ss), {}));
   assert(ss); assert(ss.eof());
  }
  {std::istringstream is; std::ostringstream os;
   is.str("p cnf 0 0\na 0\ne 0\n");
   extract_apart_strict2qcnf(is, os);
   assert(os.str() == "p cnf 0 0\n");
  }
  {std::istringstream is; std::ostringstream os;
   is.str("p cnf 0 0\na 0\n");
   extract_gpart_strictqcnf(is, os);
   assert(os.str() == "p cnf 0 0\n");
  }
  {std::istringstream is; std::ostringstream os;
   is.str("p cnf 0 0\ne 0\n");
   extract_gpart_strictqcnf(is, os);
   assert(os.str() == "p cnf 0 0\n");
  }
  {std::istringstream is; std::ostringstream os;
   is.str("p cnf 0 2\na 0\ne 0\n0\n0\n");
   extract_apart_strict2qcnf(is, os);
   assert(os.str() == "p cnf 0 2\n0\n0\n");
  }
  {std::istringstream is; std::ostringstream os;
   is.str("p cnf 0 2\na 0\ne 0\n0\n0\n");
   extract_gpart_strictqcnf(is, os);
   assert(os.str() == "p cnf 0 0\n");
  }
  {std::istringstream is; std::ostringstream os;
   is.str("p cnf 5 2\ne 1 2 3 4 5 0\n0\n0\n");
   extract_gpart_strictqcnf(is, os);
   assert(os.str() == "p cnf 0 0\n");
  }
  {std::istringstream is; std::ostringstream os;
   is.str("p cnf 5 2\na 2 0\ne 1 2 3 4 5 0\n0\n0\n");
   extract_apart_strict2qcnf(is, os);
   assert(os.str() == "p cnf 1 2\n0\n0\n");
  }
  {std::istringstream is; std::ostringstream os;
   is.str("p cnf 5 2\na 2 0\ne 1 2 3 4 5 0\n0\n0\n");
   extract_gpart_strictqcnf(is, os);
   assert(os.str() == "p cnf 1 2\n0\n0\n");
  }

  {std::istringstream is;
   is.str("1 -2 3 -4 5 0\n");
   const auto res = read_strict_clause_split(is, {Var(2),Var(3)});
   assert(eqp(res[0], {Lit(-2), Lit(3)}));
   assert(eqp(res[1], {Lit(1), Lit(-4), Lit(5)}));
  }
  {std::istringstream is;
   is.str("p cnf 111 4\n"
          "a 3 7 5 8  111 0\n"
          "e  2 1  55  0\n"
          "a 4  9 10 6 0\n"
          "-1  10 0\n"
          " -5 3 -3 0\n"
          "-8 4 -10  3 0\n"
          "-7  10 -1  6 0\n");
   const GslicedCNF F = read_strict_GslicedCNF(is);
   assert(eqp(F.V, {Var(3), Var(5), Var(7), Var(8)}));
   assert(eqp(F.T, {Var(111)}));
   std::ostringstream os;
   os << F;
   assert(os.str() ==
          "p cnf 10 4\n"
          "a 3 5 7 8 0\n"
          "e  2 1  55  0\n"
          "a 4  9 10 6 0\n"
          "-1 10 0\n"
          "-3 3 -5 0\n"
          "4 -10 3 -8 0\n"
          "-1 6 10 -7 0\n");
  }

  {const Clause C{Lit(1), Lit(-3), Lit(0), Lit(-6)};
   const varmap_t m = list2map({Var(1),Var(3),Var(4)});
   assert((rename(C,m) == Clause{Lit(1),Lit(-2),Lit(0)}));
  }

}

// Oliver Kullmann, 20.12.2019 (Swansea)
/* Copyright 2019, 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  See subdirectory docus.

BUGS:

0. DONE Due to the proginfo-object located in file ProgramInfo.hpp, the macro
   __FILE__ can no longer be used (since no the program-name is wrong).
   - Doesn't seem a problem to hardcode the filename.

1. Variables enc(i,0,i,p) -> false, i != 0, p != 0
  - Due to euler-amo for row 0 in ls 0, and row i in ls 0 having value i.
  - So for ls-variables enc(i,j,eps,p) additionally also
      p >= 1:
        j=0 => eps != i
    is required.
  - nls is reduced by (k-1) * (N-1).
  - What happened to these variables before?

Consider the ls p >= 1, field (i,0), i >= 1.
Currently here all enc(i,0,y,p) for y != 0 are there (while we should also
have y != i). Together with the three constraint-types.
  So what are the constraints on enc(i,0,i,p)?

Which enc(i,0,{x,y},{0,p}) are there?
  Currently y != 0, y != i,
            x = i, enc(i,0,{i,y},{0,p}) = enc(i,0,y,q).
enc(i,0,{i,y},{0,p}) is part of euler-amo for all y' != 0,i.

So it seems enc(i,0,i,p) is not part of any euler-constraint, and can
be set true??

For example consider N=4, k=2, p=1, i=1, eps=1.
This is the first ls-variable in ls 1: v := enc(1,0,1,1).
ls 0 has 3*7=21 ls-variables, so enc(1,0,1,1)=22.
ls 1 has 3*11=33 ls-variables, which makes nls=57.
And indeed v has only 3*3=9 in the three euler-constraints.
Setting v->1 yields an unsatisfiable instance (via 2 r_2-reductions).
  Is this always so?


2. Wrong counts for reduced form:
> ./Mols_debug 10 3 "" "" "" "" ""
Output to file "MOLS2SAT_BASIC_10_3_r_L0_fE_fP.dimacs".
Mols_debug: Mols.cpp:557: void {anonymous}::orthogonality(std::ostream&, const Encoding::VarEncoding&): Assertion `running_counter == enc.nvc.c' failed.
Aborted (core dumped)

Counting the current values:
> grep "^p" MOLS2SAT_BASIC_10_3_r_L0_fE_fP.dimacs
p cnf 27135 129186

27135 is indeed the max-occurring variable-index, but the total number
of variables is lower, and the number of clauses is lower:
> tawSolver MOLS2SAT_BASIC_10_3_r_L0_fE_fP.dimacs
^Cs UNKNOWN
c max_occurring_variable                27135
c number_of_clauses                     128754
c maximal_clause_length                 9
c number_of_literal_occurrences         279567
c p_param_variables                     27135
c p_param_clauses                       129186
c number_tautologies                    0
c file_name                             MOLS2SAT_BASIC_10_3_r_L0_fE_fP.dimacs
c options                               ""

> OKsolver2002 MOLS2SAT_BASIC_10_3_r_L0_fE_fP.dimacs
^Cs UNKNOWN
c initial_maximal_clause_length         9
c initial_number_of_variables           27127
c initial_number_of_clauses             128754
c initial_number_of_literal_occurrences 279567
c number_of_initial_unit-eliminations   0
c number_of_2-clauses_after_reduction   111123
c file_name                             MOLS2SAT_BASIC_10_3_r_L0_fE_fP.dimacs

This is likely due to the computation of "cbls2" in Statistics.hpp
not complete (see the "???" there).



TODOS:

1. Eliminating the primary es-variables (euler-equivalences)
 - The guess is that minisat-preprocessing eliminates those (and that
   seems important for minisat-solving). (Though behaviour for
   SplittingViaOKsolver for example in data/8_3_r is strange;
   hopefully this becomes clearer once this has been implemented.)
 - It seems these variables aren't powerful, at least in the current
   setting (not much is done with them).
 - We extend the "r,f"-option by "wr,wf", with "w" for "without".
 - Replacing these variables (v <-> a & b) is easy for eulo-amo.
 - But not for Lv, and for Lp only with Lpu (so Lv, Lp, Lb are forbidden
   for wr,wf).

2. Move functionality to own modules
 - Perhaps Options.hpp DONE
 - Statistics.hpp DONE
 - And Encoding.hpp.
 - GeneralRepresentations.hpp.
 - LSRepresentations.hpp.
 - InputOutput.hpp.

*/

#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <optional>

#include <cstdlib>

#include <ProgramOptions/Environment.hpp>

#include "../Random/ClauseSets.hpp"

#include "ProgramInfo.hpp"
#include "Errors.hpp"
#include "Options.hpp"
#include "Statistics.hpp"
#include "Encoding.hpp"

namespace {

  using namespace ProgramInfo;
  using namespace Errors;
  using namespace Options;
  using namespace Statistics;
  using namespace Encoding;

  std::string default_filestem() {
    return "MOLS2SAT_BASIC";
  }

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg <<
    " [N=" << N_default << ",>=1]"
    " [k=1]\n"
    "   [symopt=" << Environment::WRP<SymP>{} << ",\"\"]"
    " [primopt=" << Environment::WRP<PrimeP>{} << ",\"\"]\n"
    "   [output=-cout,\"\",-nil,NAME]\n"
    "computes the SAT-translation for a latin square of order N.\n"
    "\n> " << proginfo.prg <<
    " [N=>1]"
    " [k>=2]\n"
    "   [symopt=" << Environment::WRP<SymP>{} << ",\"\"]\n"
    "   [ealoopt=" << Environment::WRP<EAloP>{} << ",\"\"]\n"
    "   [eulopt=" << Environment::WRP<EulP>{} << ",\"\"]\n"
    "   [primopt=" << Environment::WRP<PrimeP>{} << ",\"\"]\n"
    "   [output=-cout,\"\",-nil,NAME]\n"
    "computes the SAT-translation for k MOLS of order N:\n\n"
    "  - The arguments are positional, not named (the names are used here only"
    " for communication).\n"
    "  - Trailing arguments can be left out, then using their default-values"
    " (the first given value).\n"
    "  - For the four options-arguments, the value \"\" yields also the"
    " default-values.\n"
    "  - \"\" for the output however yields the default output-filename:\n"
    "     k=1: \"" << default_filestem() << "_N_1_so_po.dimacs\"\n"
    "     k>1: \"" << default_filestem() << "_N_k_so_eao_euo_po.dimacs\".\n"
    "  - \"-nil\" for the output means no output of clauses (only information).\n"
;
    return true;
  }






  /* Production of clauses */
#ifndef NDEBUG
  var_t running_counter = 0;
# define INCCLAUSE ++running_counter;
#else
# define INCCLAUSE
#endif

  using RandGen::Var;
  using RandGen::Lit;
  using RandGen::Clause;

  void amo_primes(std::ostream& out, const Clause& C) {
    if (C.size() >= 2) {
      auto current_end = C.cbegin(); ++current_end;
      do {
        const Lit y = -*current_end;
        for (auto i = C.cbegin(); i != current_end; ++i) {
          out << Clause{-*i, y}; INCCLAUSE;
        }
      } while (++current_end != C.end());
    }
  }
  void alo_primes(std::ostream& out, const Clause& C) {
    out << C; INCCLAUSE;
  }
  void eo_primes(std::ostream& out, const Clause& C) {
    amo_primes(out, C);
    alo_primes(out, C);
  }

  // The disjunction over B implies w:
  inline void disj_impl(std::ostream& out, const Clause& B, const Lit w) {
    for (const Lit x : B) { out << Clause{-x, w}; INCCLAUSE; }
  }
  // As seco_amov2cl(L,V) in CardinalityConstraints.mac:
  void amo_seco(std::ostream& out, Clause C, const VarEncoding& enc) {
    Clause B(3);
    while (C.size() > 4) {
      const Lit w{enc(), 1};
      B.assign(C.end()-3, C.end());
      C.resize(C.size()-3);
      C.push_back(w);
      amo_primes(out, B);
      disj_impl(out, B, w);
    }
    amo_primes(out, C);
  }
  // The disjunction over B is equivalent to w:
  inline void disj_equiv(std::ostream& out, const Clause& B, const Lit w) {
    disj_impl(out, B, w);
    out << -w << " "; out << B; INCCLAUSE;
  }
  // Combining seco_amovuep2cl(L,V) and seco_amouep_co(L) from CardinalityConstraints.mac:
  var_t amouep_seco(std::ostream& out, Clause C, const VarEncoding& enc) {
    var_t final_v = 0;
    Clause B(3);
    while (C.size() > 4) {
      final_v = enc();
      const Lit w{final_v, 1};
      B.assign(C.end()-3, C.end());
      C.resize(C.size()-3);
      C.push_back(w);
      amo_primes(out, B);
      disj_equiv(out, B, w);
    }
    amo_primes(out, C);
    return final_v;
  }
  void eo_seco(std::ostream& out, const Clause& C, const VarEncoding& enc) {
    assert(has_pair(enc.ealoopt));
    if (enc.ealoopt == EAloP::pair or enc.ealoopt == EAloP::both) {
      amo_seco(out, C, enc);
      out << C; INCCLAUSE;
    }
    else {
      assert(enc.ealoopt == EAloP::pairuep or enc.ealoopt == EAloP::bothuep);
      const var_t final_v = amouep_seco(out, C, enc);
      if (final_v == 0) {
        assert(C.size() <= 4);
        out << C; INCCLAUSE;
      }
      else {
        assert(C.size() >= 5);
        const var_t missing = C.size()%2==1 ? 2 : 3;
        out << Lit{final_v,1} << " " << Clause(C.begin(), C.begin()+missing);
        INCCLAUSE;
      }
    }
  }

  // x -> (y and z):
  void implication(std::ostream& out, const Lit x, const Lit y, const Lit z) {
    out << Clause{-x, y} << Clause{-x, z}; INCCLAUSE; INCCLAUSE;
  }
  // x <- (y and z):
  void back_implication(std::ostream& out, const Lit x, const Lit y, const Lit z) {
    out << Clause{x, -y, -z}; INCCLAUSE;
  }
  // x <-> (y and z):
  void definition(std::ostream& out, const Lit x, const Lit y, const Lit z) {
    implication(out,x,y,z);
    back_implication(out,x,y,z);
  }


  void ls(std::ostream& out, const VarEncoding& enc) {
#ifndef NDEBUG
    assert(running_counter == 0);
#endif
    if (enc.symopt == SymP::full) {

      for (dim_t p = 0; p < enc.k; ++p) {
        // EO(i,j,-,p) :
        for (dim_t i = 0; i < enc.N; ++i)
          for (dim_t j = 0; j < enc.N; ++j) {
            Clause C; C.reserve(enc.N);
            for (dim_t eps = 0; eps < enc.N; ++eps)
              C.push_back({enc(i,j,eps,p),1});
            if (enc.primopt == PrimeP::full)
              eo_primes(out, C);
            else
              alo_primes(out, C);
          }
        // EO(i,-,eps,p) :
        for (dim_t i = 0; i < enc.N; ++i)
          for (dim_t eps = 0; eps < enc.N; ++eps) {
            Clause C; C.reserve(enc.N);
            for (dim_t j = 0; j < enc.N; ++j)
              C.push_back({enc(i,j,eps,p),1});
            if (enc.primopt == PrimeP::full)
              eo_primes(out, C);
            else
              amo_primes(out, C);
          }
        // EO(-,j,eps,p) :
        for (dim_t j = 0; j < enc.N; ++j)
          for (dim_t eps = 0; eps < enc.N; ++eps) {
            Clause C; C.reserve(enc.N);
            for (dim_t i = 0; i < enc.N; ++i)
              C.push_back({enc(i,j,eps,p),1});
            if (enc.primopt == PrimeP::full)
              eo_primes(out, C);
            else
              amo_primes(out, C);
          }
      }

    } else {
      assert(enc.symopt == SymP::reduced);
      // EO(i,j,-,0) :
      for (dim_t i = 1; i < enc.N; ++i)
        for (dim_t j = 1; j < enc.N; ++j) {
          Clause C;
          for (dim_t eps = 0; eps < enc.N; ++eps)
            if (eps != i and eps != j)
              C.push_back({enc(i,j,eps,0),1});
          if (enc.primopt == PrimeP::full)
            eo_primes(out, C);
          else
            alo_primes(out, C);
        }
      // EO(i,-,eps,0) :
      for (dim_t i = 1; i < enc.N; ++i)
        for (dim_t eps = 0; eps < enc.N; ++eps) {
          if (eps == i) continue;
          Clause C;
          for (dim_t j = 1; j < enc.N; ++j)
            if (eps != j)
              C.push_back({enc(i,j,eps,0),1});
          if (enc.primopt == PrimeP::full)
            eo_primes(out, C);
          else
            amo_primes(out, C);
        }
      // EO(-,j,eps,0) :
      for (dim_t j = 1; j < enc.N; ++j)
        for (dim_t eps = 0; eps < enc.N; ++eps) {
          if (eps == j) continue;
          Clause C;
          for (dim_t i = 1; i < enc.N; ++i)
            if (eps != i)
              C.push_back({enc(i,j,eps,0),1});
          if (enc.primopt == PrimeP::full)
            eo_primes(out, C);
          else
            amo_primes(out, C);
        }

      for (dim_t p = 1; p < enc.k; ++p) {
        // EO(i,j,-,p) :
        for (dim_t i = 1; i < enc.N; ++i)
          for (dim_t j = 0; j < enc.N; ++j) {
            Clause C;
            for (dim_t eps = 0; eps < enc.N; ++eps)
              if (eps != j and (j != 0 or eps != i))
                C.push_back({enc(i,j,eps,p),1});
            if (enc.primopt == PrimeP::full)
              eo_primes(out, C);
            else
              alo_primes(out, C);
          }
        // EO(i,-,eps,p) :
        for (dim_t i = 1; i < enc.N; ++i)
          for (dim_t eps = 0; eps < enc.N; ++eps) {
            Clause C;
            for (dim_t j = 0; j < enc.N; ++j)
              if (eps != j and (j != 0 or eps != i))
                C.push_back({enc(i,j,eps,p),1});
            if (enc.primopt == PrimeP::full)
              eo_primes(out, C);
            else
              amo_primes(out, C);
          }
        // EO(-,j,eps,p) :
        for (dim_t j = 0; j < enc.N; ++j)
          for (dim_t eps = 0; eps < enc.N; ++eps) {
            if (eps == j) continue;
            Clause C;
            for (dim_t i = 1; i < enc.N; ++i)
              if (j != 0 or eps != i)
                C.push_back({enc(i,j,eps,p),1});
            if (enc.primopt == PrimeP::full)
              eo_primes(out, C);
            else
              amo_primes(out, C);
          }
      }
    }
#ifndef NDEBUG
    assert(running_counter == enc.nvc.cls);
#endif
  }


  void es_defs(std::ostream& out, const VarEncoding& enc) {
#ifndef NDEBUG
    assert(running_counter == enc.nvc.cls);
#endif
    if (enc.symopt == SymP::full) {
      for (dim_t q = 1; q < enc.k; ++q)
        for (dim_t p = 0; p < q; ++p)
          for (dim_t i = 0; i < enc.N; ++i)
            for (dim_t j = 0; j < enc.N; ++j)
              for (dim_t x = 0; x < enc.N; ++x)
                for (dim_t y = 0; y < enc.N; ++y)
                  if (enc.eulopt == EulP::full)
                    definition(out,{enc(i,j,{x,y},{p,q}),1},
                                   {enc(i,j,x,p),1}, {enc(i,j,y,q),1});
                  else
                    back_implication(out,{enc(i,j,{x,y},{p,q}),1},
                                         {enc(i,j,x,p),1}, {enc(i,j,y,q),1});
    }
    else {
      assert(enc.symopt == SymP::reduced);
      for (dim_t q = 1; q < enc.k; ++q) {
        // p = 0:
        for (dim_t i = 1; i < enc.N; ++i)
          for (dim_t j = 1; j < enc.N; ++j)
            for (dim_t x = 0; x < enc.N; ++x) {
              if (x == i or x == j) continue;
              for (dim_t y = 0; y < enc.N; ++y) {
                if (y == j) continue;
                if (y == x) {
                  const Lit a{enc(i,j,x,0),1}, b{enc(i,j,y,q),1};
                  out << Clause{-a, -b}; INCCLAUSE;
                } else
                    if (enc.eulopt == EulP::full)
                      definition(out,{enc(i,j,{x,y},{0,q}),1},
                                     {enc(i,j,x,0),1},{enc(i,j,y,q),1});
                    else
                      back_implication(out,{enc(i,j,{x,y},{0,q}),1},
                                           {enc(i,j,x,0),1},{enc(i,j,y,q),1});

              }
            }
        // p >= 1:
        for (dim_t p = 1; p < q; ++p)
          for (dim_t i = 1; i < enc.N; ++i)
            for (dim_t j = 0; j < enc.N; ++j)
              for (dim_t x = 0; x < enc.N; ++x) {
                if (x == j or (j == 0 and x == i)) continue;
                for (dim_t y = 0; y < enc.N; ++y) {
                  if (y == j or (j == 0 and y == i)) continue;
                  if (y == x) {
                    if (j != 0) {
                      const Lit a{enc(i,j,x,p),1}, b{enc(i,j,y,q),1};
                      out << Clause{-a, -b}; INCCLAUSE;
                    }
                  } else
                      if (enc.eulopt == EulP::full)
                        definition(out,{enc(i,j,{x,y},{p,q}),1},
                                       {enc(i,j,x,p),1},{enc(i,j,y,q),1});
                      else
                        back_implication(out,{enc(i,j,{x,y},{p,q}),1},
                                             {enc(i,j,x,p),1},{enc(i,j,y,q),1});

                }
              }
      }
    }
  }


  void es_values(std::ostream& out, const VarEncoding& enc) {
    if (not has_val(enc.ealoopt)) return;
    if (enc.symopt == SymP::full) {
      for (dim_t q = 1; q < enc.k; ++q)
          for (dim_t p = 0; p < q; ++p)
            for (dim_t i = 0; i < enc.N; ++i)
              for (dim_t j = 0; j < enc.N; ++j) {
                Clause C; C.reserve(enc.N * enc.N);
                for (dim_t x = 0; x < enc.N; ++x)
                  for (dim_t y = 0; y < enc.N; ++y)
                    C.push_back({enc(i,j,{x,y},{p,q}),1});
                out << C; INCCLAUSE;
              }
    }
    else {
      assert(enc.symopt == SymP::reduced);
      assert(enc.N >= 2);
      for (dim_t q = 1; q < enc.k; ++q) {
        // p = 0:
        for (dim_t i = 1; i < enc.N; ++i)
          for (dim_t j = 1; j < enc.N; ++j) {
            Clause C;
            if (i != j) C.reserve((var_t(enc.N)-2) * (var_t(enc.N)-2));
            else C.reserve((var_t(enc.N)-1) * (var_t(enc.N)-2));
            for (dim_t x = 0; x < enc.N; ++x) {
              if (x == i or x == j) continue;
              for (dim_t y = 0; y < enc.N; ++y) {
                if (y == j or y == x) continue;
                C.push_back({enc(i,j,{x,y},{0,q}),1});
              }
            }
            assert((i!=j and C.size()==(var_t(enc.N)-2)*(var_t(enc.N)-2)) or
                   (i==j and C.size()==(var_t(enc.N)-1)*(var_t(enc.N)-2)));
            out << C; INCCLAUSE;
          }
        // p >= 1:
        for (dim_t p = 1; p < q; ++p)
          for (dim_t i = 1; i < enc.N; ++i)
            for (dim_t j = 0; j < enc.N; ++j) {
              Clause C; C.reserve((var_t(enc.N)-1) * (var_t(enc.N)-2));
              for (dim_t x = 0; x < enc.N; ++x) {
                if (x == j) continue;
                for (dim_t y = 0; y < enc.N; ++y) {
                  if (y == j or y == x) continue;
                  if (j == 0 and (x == i or y == i)) continue;
                  C.push_back({enc(i,j,{x,y},{p,q}),1});
                }
              }
              assert(C.size()==(var_t(enc.N)-1)*(var_t(enc.N)-2));
              out << C; INCCLAUSE;
            }
      }
    }
  }

  void orthogonality(std::ostream& out, const VarEncoding& enc) {
    if (enc.symopt == SymP::full) {
      for (dim_t q = 1; q < enc.k; ++q)
        for (dim_t p = 0; p < q; ++p)
          for (dim_t x = 0; x < enc.N; ++x)
            for (dim_t y = 0; y < enc.N; ++y) {
              Clause C; C.reserve(enc.N * enc.N);
              for (dim_t i = 0; i < enc.N; ++i)
                for (dim_t j = 0; j < enc.N; ++j)
                  C.push_back({enc(i,j,{x,y},{p,q}),1});
              if (not has_pair(enc.ealoopt)) amo_seco(out, C, enc);
              else eo_seco(out, C, enc);
            }
    }
    else {
      assert(enc.symopt == SymP::reduced);
      for (dim_t q = 1; q < enc.k; ++q) {
        // p = 0:
        [[maybe_unused]] const auto length0 = [](const dim_t x, const dim_t y, const var_t N) {
          if (x == 0) return (N-2)*(N-1);
          else if (y == 0) return (N-2)*(N-2);
          else return (N-3)*(N-2) + 1;
        };
        for (dim_t x = 0; x < enc.N; ++x)
          for (dim_t y = 0; y < enc.N; ++y) {
            if (y == x) continue;
            Clause C; C.reserve((var_t(enc.N)-1) * enc.N);
            for (dim_t j = 0; j < enc.N; ++j) {
              if (j == x or j == y) continue; // ???
              for (dim_t i = 1; i < enc.N; ++i) {
                if ((i==x and j!=0) or (i!=x and j==0)) continue;
                C.push_back({enc(i,j,{x,y},{0,q}),1});
              }
            }
            assert(C.size() == length0(x,y,enc.N));
            if (not has_pair(enc.ealoopt)) amo_seco(out, C, enc);
            else eo_seco(out, C, enc);
          }
        // p >= 1:
        [[maybe_unused]] const auto length1 = [](const dim_t x, const dim_t y, const var_t N) {
          return (N-2)*(N-1) - (x==0 or y==0 ? 0 : 2);
        };
        for (dim_t p = 1; p < q; ++p)
          for (dim_t x = 0; x < enc.N; ++x)
            for (dim_t y = 0; y < enc.N; ++y) {
              if (y == x) continue;
              Clause C; C.reserve((var_t(enc.N)-1) * enc.N);
              for (dim_t j = 0; j < enc.N; ++j) {
                if (j == x or j == y) continue;
                for (dim_t i = 1; i < enc.N; ++i)
                  if (j != 0 or (x != i and y != i))
                    C.push_back({enc(i,j,{x,y},{p,q}),1});
              }
              assert(C.size() == length1(x,y,enc.N));
              if (not has_pair(enc.ealoopt)) amo_seco(out, C, enc);
              else eo_seco(out, C, enc);
            }
      }
    }
#ifndef NDEBUG
    assert(running_counter == enc.nvc.c);
#endif
  }


  inline std::string default_param(const VarEncoding e) {
    using Environment::RegistrationPolicies;
    if (e.k == 1)
      return std::to_string(e.N) + "_" + std::to_string(e.k) + "_" +
        RegistrationPolicies<SymP>::string[int(e.symopt)] + "_" +
        RegistrationPolicies<PrimeP>::string[int(e.primopt)];
    else
      return std::to_string(e.N) + "_" + std::to_string(e.k) + "_" +
        RegistrationPolicies<SymP>::string[int(e.symopt)] + "_" +
        RegistrationPolicies<EAloP>::string[int(e.ealoopt)] + "_" +
        RegistrationPolicies<EulP>::string[int(e.eulopt)] + "_" +
        RegistrationPolicies<PrimeP>::string[int(e.primopt)];
  }
  inline std::string default_filesuffix() {
    return ".dimacs";
  }
  inline std::string default_filename(const VarEncoding e) {
    return default_filestem() + "_" + default_param(e) + default_filesuffix();
  }


  dim_t read_dim(const std::string arg) {
    unsigned long d;
    std::size_t converted;
    try { d = std::stoul(arg, &converted); }
    catch (const std::invalid_argument& e) {
      std::cerr << error << "The argument \"" << arg << "\" is not a valid integer.\n";
      std::exit(int(Error::conversion));
    }
    catch (const std::out_of_range& e) {
      std::cerr << error << "The argument \"" << arg << "\" is too big for unsigned long.\n";
      std::exit(int(Error::too_big));
    }
    if (converted != arg.size()) {
      std::cerr << error << "The argument \"" << arg << "\" contains trailing characters: \""
        << arg.substr(converted) << "\".\n";
      std::exit(int(Error::conversion));
    }
    if (d == 0) {
      std::cerr << error << "An argument is 0.\n";
      std::exit(int(Error::too_small));
    }
    const dim_t cd = d;
    if (cd != d) {
      std::cerr << error << "The argument \"" << arg << "\" is too big for dim_t (max 65535).\n";
      std::exit(int(Error::too_big));
    }
    return cd;
  }

  bool special(const std::string_view s) noexcept {
    return s == "-cout" or s == "-nil";
  }
}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  Environment::Index index;

  const dim_t N = argc <= index ? N_default : read_dim(argv[index++]);
  const dim_t k = argc <= index ? k_default : read_dim(argv[index++]);
  const Param p{N,k};

  const std::optional<SymP> rsymopt = argc <= index ? SymP{} : Environment::read<SymP>(argv[index++]);
  if (not rsymopt) {
    std::cerr << error << "Bad option-argument w.r.t. symmetry: \"" << argv[index-1] << "\".\n";
    return int(Error::sym_par);
  }
  const SymP symopt = rsymopt.value();

  const std::optional<EAloP> realoopt = k==1 ? EAloP{-1} :
    argc <= index ? EAloP{} : Environment::read<EAloP>(argv[index++]);
  if (not realoopt) {
    std::cerr << error << "Bad option-argument w.r.t. Euler-ALO: \"" << argv[index-1] << "\".\n";
    return int(Error::ealo_par);
  }
  const EAloP ealoopt = realoopt.value();

  const std::optional<EulP> reulopt = k==1 ? EulP{-1} :
    argc <= index ? EulP{} : Environment::read<EulP>(argv[index++]);
  if (not reulopt) {
    std::cerr << error << "Bad option-argument w.r.t. Euler-form: \"" << argv[index-1] << "\".\n";
    return int(Error::eul_par);
  }
  const EulP eulopt = reulopt.value();

  const std::optional<PrimeP> rprimopt = argc <= index ? PrimeP{} : Environment::read<PrimeP>(argv[index++]);
  if (not rprimopt) {
    std::cerr << error << "Bad option-argument w.r.t. Prime-form: \"" << argv[index-1] << "\".\n";
    return int(Error::p_par);
  }
  const PrimeP primopt = rprimopt.value();

  const VarEncoding enc(p, symopt, ealoopt, eulopt, primopt);

  std::ofstream out;
  std::string filename;
  if (index == argc or special(argv[index])) {
    out.basic_ios<char>::rdbuf(std::cout.rdbuf());
    filename = index==argc ? "-cout" : argv[index];
  }
  else {
    filename = argv[index];
    if (filename.empty()) filename = default_filename(enc);
    out.open(filename);
    if (not out) {
      std::cerr << error << "Can't open file \"" << filename << "\"\n";
      return int(Error::file_open);
    }
    std::cout << "Output to file \"" << filename << "\".\n";
  }
  index++;

  index.deactivate();


  out << Environment::Wrap(proginfo, Environment::OP::dimacs);
  using Environment::DHW;
  using Environment::DWW;
  using Environment::qu;
  out << DHW{"Parameters"}
            << DWW{"command-line"};
  Environment::args_output(out, argc, argv);
  out << "\n"
            << DWW{"N"} << N << "\n"
            << DWW{"k"} << k << "\n"
            << DWW{"sym_handling"} << symopt << "\n"
            << DWW{"Euler_ALO"} << ealoopt << "\n"
            << DWW{"Euler_vars"} << eulopt << "\n"
            << DWW{"Prime_clauses"} << primopt << "\n"
            << DWW{"output"} << qu(filename) << "\n"
      << DHW{"Sizes"};
  if (k == 1) {
    enc.nls(out);
    enc.cls(out);
  }
  else {
    enc.nls(out);
    enc.npes(out);
    out     << DWW{"n0=nls+npes"} << enc.nvc.n0 << "\n";
    enc.naux(out);
    out     << DWW{"n=n0+naux"} << enc.nvc.n << "\n";
    enc.cls(out);
    enc.ces(out);
    out     << DWW{"c=cls+ces"} << enc.nvc.c << "\n";
  }

  if (filename == "-nil") return 0;
  out << dimacs_pars{enc.nvc.n, enc.nvc.c};
  ls(out, enc);
  es_defs(out, enc);
  es_values(out, enc);
  orthogonality(out, enc);
}

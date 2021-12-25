// Oleg Zaikin, 22.12.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  USAGE:

  EXAMPLES:

$MOLS cat data/weights/testN6 | ./Euler_BBOpt_debug 1 1 1 1 data/weights/Para0 val dom
  (1,100,100,100),78


More precisely:

MOLS> cat data/weights/testN6 | /usr/bin/time ./Euler_BBOpt_debug 1 1 1 1 data/weights/Para0 val dom
(1,100,100,100),78
18.56user 0.54system 0:19.11elapsed 100%CPU (0avgtext+0avgdata 69928maxresident)k
0inputs+0outputs (0major+16693minor)pagefaults 0swaps

With output of SearchStats:

MOLS> cat data/weights/testN6 | ./Euler_BBOpt 1 1 1 1 data/weights/Para0 val dom
2 3 4 5        253       110       143         0
1 3 4 5        221        94       127         0
100 3 4 5       1182       390       792         0
1 1 4 5        369       145       224         0
1 100 4 5        223       100       123         0
1 100 1 5        218        98       120         0
1 100 100 5        204        92       112         0
1 100 100 1        216        98       118         0
1 100 100 100        132        54        78         0
(1,100,100,100),78

   Remark: In debug-mode the first line is printed two more times, due to
   asserts checking the result.


TODOS:

1. There is an enormous code-duplication, which needs to be cleaned-up.

BUGS:

0. MEMORY LEAK

Running

MOLS> time cat ./data/weights/testN6 | ./Euler_BBOpt_debug 10 2 3 1 data/weights/Para2 la,val dom

had to be aborted when reaching memory-consumption of 14G.
Likely the resources used by the solver are never released.

Here the version of Para2 contains "10" in all middle-positins, and thus a
large number of cases is created.


1.
MOLS> cat data/weights/testN6 | ./Euler_BBOpt_debug 1 1 1 2 data/weights/Para0 la,val dom
Segmentation fault (core dumped)

All the global variables need to be removed.

More details:

MOLS> cat ./data/weights/testN6 | valgrind ./Euler_BBOpt_debug 1 1 1 2 data/weights/Para0 la,val dom
==15737== Memcheck, a memory error detector
==15737== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==15737== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==15737== Command: ./Euler_BBOpt_debug 1 1 1 2 data/weights/Para0 la,val dom
==15737== 

Euler_BBOpt_debug: ../../../../Satisfiability/Solvers/Gecode/Statistics.hpp:44: void Statistics::SearchStat::reset(): Assertion `valid()' failed.

The problem here is the global variable

  // XXX no global variables in header-files !!! XXX
  Statistics::SearchStat global_stat;

in Lookahead.cpp (line 235).
This needs to be removed.

General remark: Only in very special case should global variables be used.

*/


#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <vector>
#include <array>
#include <tuple>
#include <iomanip>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>
#include <Numerics/Optimisation.hpp>
#include <Transformers/Generators/Random/LatinSquares.hpp>
#include <Transformers/Generators/Random/LSRG.hpp>
#include <Solvers/Gecode/Lookahead.hpp>
#include <Solvers/Gecode/Statistics.hpp>

#include "gecode/driver.hh"
#include "gecode/int.hh"
#include "gecode/search.hh"

#include "Euler.hpp"

namespace {

  using namespace Euler;

  namespace GC = Gecode;
  namespace LA = Lookahead;
  namespace LS = LatinSquares;
  namespace RG = RandGen;

  typedef std::vector<int> gecode_intvec_t;
  typedef std::vector<GC::IntVar> gecode_intvarvec_t;
  typedef std::vector<std::string> partial_ls_t;

  constexpr LS::ls_dim_t N_default = 0;
  constexpr LS::ls_dim_t k_default = 2;


  LS::ls_dim_t read_N(const std::string& s, const std::string& error) noexcept {
    if (s.empty()) return N_default;
    const LS::ls_dim_t N = FloatingPoint::touint(s);
    if (not LS::valid(N) and N != 0) {
      std::cerr << error << "N must be a nonnegative integer in [0,"
                << LS::max_dim-1 << "]" << ", but N=" << N << ".\n";
      std::exit(int(RG::Error::domain));
    }
    return N;
  }
  LS::ls_dim_t read_N(const std::string& error) noexcept {
    std::string s;
    std::cin >> s;
    return read_N(s, error);
  }

  LS::ls_dim_t read_k(const std::string& s,
                      const std::string& error) noexcept {
    if (s.empty()) return k_default;
    const LS::ls_dim_t k = FloatingPoint::touint(s);
    if (not LS::valid(k) and k != 0) {
      std::cerr << error << "k must be a nonnegative integer in [0,"
                << LS::max_dim-1 << "]" << ", but k=" << k << ".\n";
      std::exit(int(RG::Error::domain));
    }
    return k;
  }
  LS::ls_dim_t read_k(const std::string& error) noexcept {
    std::string s;
    std::cin >> s;
    return read_k(s, error);
  }

  gecode_intvec_t read_partial_ls(const LS::ls_dim_t N) noexcept {
    assert(N > 0);
    const LS::ls_dim_t size = N*N;
    gecode_intvec_t partial_ls(size);
    std::string s;
    partial_ls_t ls_s;
    do {
      std::cin >> s;
      if (s.empty()) continue;
      if (N > 10) ls_s.push_back(s);
      else for (auto c : s) ls_s.push_back(std::string(1,c));
      assert(ls_s.size() <= size);
    } while (ls_s.size() != size);
    for (LS::ls_dim_t i=0; i < size; ++i) {
      assert(i < partial_ls.size() and i < ls_s.size());
      partial_ls[i] = (ls_s[i] == "*") ? -1 : std::stoi(ls_s[i]);
    }
    return partial_ls;
  }

  class TWO_MOLS : public GC::Space {
    const LS::ls_dim_t N;
    const LA::option_t alg_options;
    const gecode_option_t gecode_options;
    const Optimisation::vec_t wghts;
    GC::IntVarArray x, y, z, V;
    inline LA::size_t x_index(const LA::size_t i) const noexcept { return i; }
    inline LA::size_t y_index(const LA::size_t i) const noexcept { return i + LA::tr(x.size()); }
    inline LA::size_t z_index(const LA::size_t i) const noexcept {
      return i + LA::tr(x.size()) + LA::tr(y.size());
    }

    inline GC::IntPropLevel prop_level(const gecode_option_t gc_options) const noexcept {
      GC::IntPropLevel ipl = GC::IPL_DEF;
      const auto gc_option = std::get<PropO>(gc_options);
      switch( gc_option ) {
      case PropO::val:
          ipl = GC::IPL_VAL;
          break;
      case PropO::bnd:
          ipl = GC::IPL_BND;
          break;
      case PropO::dom:
          ipl = GC::IPL_DOM;
          break;
      case PropO::def:
          ipl = GC::IPL_DEF;
          break;
      default:
          ipl = GC::IPL_DOM;
          break;
      }
      return ipl;
    }
  public:
    TWO_MOLS(const LS::ls_dim_t N, const LA::option_t alg_options,
             const gecode_option_t gecode_options,
             const gecode_intvec_t ls1_partial = {},
             const gecode_intvec_t ls2_partial = {},
             const Optimisation::vec_t wghts = {}) :
      N(N), alg_options(alg_options), gecode_options(gecode_options),
      x(*this, N*N, 0, N - 1),
      y(*this, N*N, 0, N - 1),
      z(*this, N*N, 0, N - 1),
      V(*this, x.size() + y.size() + z.size(), 0, N - 1) {
      assert(valid());
      // Determine propagation level:
      GC::IntPropLevel prp_lvl = prop_level(gecode_options);
      // Use an umbrella variable array for all variables:
      for (LA::size_t i = 0; i < LA::tr(x.size()); ++i) V[x_index(i)] = x[i];
      for (LA::size_t i = 0; i < LA::tr(y.size()); ++i) V[y_index(i)] = y[i];
      for (LA::size_t i = 0; i < LA::tr(z.size()); ++i) V[z_index(i)] = z[i];
      // Known cells of partially filled Latin squares:
      if (not ls1_partial.empty() and not ls2_partial.empty()) {
        assert(ls1_partial.size() == N*N and ls2_partial.size() == N*N);
        for(LS::ls_dim_t i = 0; i < N; ++i) {
          for(LS::ls_dim_t j = 0; j < N; ++j) {
            assert(i*N + j < ls1_partial.size());
            if (ls1_partial[i*N + j] >= 0) {
              dom(*this, x[i*N + j], ls1_partial[i*N + j], ls1_partial[i*N + j],
                  prp_lvl);
            }
            assert(i*N + j < ls2_partial.size());
            if (ls2_partial[i*N + j] >= 0) {
              dom(*this, y[i*N + j], ls2_partial[i*N + j], ls2_partial[i*N + j],
                  prp_lvl);
            }
          }
        }
      }

      // Latin property in rows of X:
      for (LS::ls_dim_t i = 0; i < N; ++i) {
        gecode_intvarvec_t rows_x;
        for (LS::ls_dim_t j = 0; j < N; ++j) rows_x.push_back(x[i*N + j]);
        GC::distinct(*this, rows_x, prp_lvl);
      }
      // Latin property in cols of X:
      for (LS::ls_dim_t i = 0; i < N; ++i) {
        gecode_intvarvec_t cols_x;
        for (LS::ls_dim_t j = 0; j < N; ++j) cols_x.push_back(x[j*N + i]);
        GC::distinct(*this, cols_x, prp_lvl);
      }
      // Latin property in rows of Y:
      for (LS::ls_dim_t i = 0; i < N; ++i) {
        gecode_intvarvec_t rows_y;
        for (LS::ls_dim_t j = 0; j < N; ++j) rows_y.push_back(y[i*N + j]);
        GC::distinct(*this, rows_y, prp_lvl);
      }
      // Latin property in cols of Y:
      for (LS::ls_dim_t i = 0; i < N; ++i) {
        gecode_intvarvec_t cols_y;
        for (LS::ls_dim_t j = 0; j < N; ++j) cols_y.push_back(y[j*N + i]);
        GC::distinct(*this, cols_y, prp_lvl);
      }
      // Row uniqueness of Z:
      for (LS::ls_dim_t i = 0; i < N; ++i) {
        gecode_intvarvec_t rows_z;
        for (LS::ls_dim_t j = 0; j < N; ++j) rows_z.push_back(z[i*N + j]);
        GC::distinct(*this, rows_z, prp_lvl);
      }
      // Column uniqueness of Z:
      for (LS::ls_dim_t i = 0; i < N; ++i) {
        gecode_intvarvec_t cols_z;
        for (LS::ls_dim_t j = 0; j < N; ++j) cols_z.push_back(z[j*N + i]);
        GC::distinct(*this, cols_z, prp_lvl);
      }
      // Enforce element constraints on Z, X, Y:
      for (LS::ls_dim_t i = 0; i < N; ++i) {
        gecode_intvarvec_t Zvec_i;
        for (LS::ls_dim_t j = 0; j < N; ++j) Zvec_i.push_back(z[i*N + j]);
        for (LS::ls_dim_t j = 0; j < N; ++j) {
          GC::element(*this, GC::IntVarArgs(Zvec_i), x[i*N + j],
                      y[i*N + j], prp_lvl);
        }
      }

      if (not this->failed()) {
        assert(wghts.size() == N-2);
        LA::post_branching<TWO_MOLS>(*this, V, alg_options, wghts);
      }

    }

    TWO_MOLS(TWO_MOLS& T) : GC::Space(T), N(T.N), alg_options(T.alg_options),
             gecode_options(T.gecode_options), wghts(T.wghts) {
      assert(T.valid());
      x.update(*this, T.x);
      y.update(*this, T.y);
      z.update(*this, T.z);
      V.update(*this, T.V);
      assert(valid(V));
    }
    virtual GC::Space* copy() {
      return new TWO_MOLS(*this);
    }

    inline bool valid () const noexcept {return N > 0 and valid(V);}
    inline bool valid (const GC::IntVarArray V) const noexcept {
      return x.size() > 0 and V.size() == x.size() + y.size() + z.size();
    }
    inline bool valid (const LA::size_t i) const noexcept {return i<LA::tr(V.size());}

    inline GC::IntVar at(const LA::size_t i) const noexcept {
      assert(valid()); assert(valid(i));
      return V[i];
    }
    inline GC::IntVarArray at() const noexcept { assert(valid()); return V; }

    LA::option_t branching_options() const noexcept { assert(valid()); return alg_options; }

    void print() {
      assert(valid());
      for (LS::ls_dim_t i = 0; i < N; ++i) {
        for (LS::ls_dim_t j = 0; j < N; ++j) {
          std::cout << x[i*N + j];
          if (j < N-1) std::cout << " ";
        }
        std::cout << std::endl;
      }
      std::cout << std::endl;
      for (LS::ls_dim_t i = 0; i < N; ++i) {
        for (LS::ls_dim_t j = 0; j < N; ++j) {
          std::cout << y[i*N + j];
          if (j < N-1) std::cout << " ";
        }
        std::cout << std::endl;
      }
    }
    void print(std::ostream& os) const noexcept {
      assert(valid(V)); os << V << std::endl;
    }

  };

}

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.2",
        "25.12.2021",
        __FILE__,
        "Oliver Kullmann and Oleg Zaikin",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/Numerics/Euler_BBOpt.cpp",
        "GPL v3"};

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
      "> "<<proginfo.prg<< " M R S T optFILE [parameters for opt]\n\n"
      " - M = partition-size\n"
      " - R = number of rounds (running through all coordinates once)\n"
      " - S = number of shrinkings (on top of the rounds)\n"
      " - T = number of threads\n"
      " - optFILE : contains a line with 5 numbers per coordinate (allows formatting)\n"
      " - all following arguments are passed to the functor to be optimised.\n"
 ;
    return true;
  }

  namespace LA = Lookahead;
  namespace LS = LatinSquares;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  struct Func {
     // Finding an Euler square:
    LS::ls_dim_t N;
    LA::option_t alg_options;
    gecode_option_t gecode_options;
    gecode_intvec_t ls1_partial;
    gecode_intvec_t ls2_partial;

    void init(const int argc, const char* const argv[]) noexcept {
      Environment::Index index;
      // Read algorithm options:
      alg_options = argc <= index ? LA::option_t{} :
        Environment::translate<LA::option_t>()(argv[index-1], LA::sep);
      // Read gecode options:
      gecode_options = argc <= index ? Euler::gecode_option_t{Euler::PropO::dom} :
        Environment::translate<Euler::gecode_option_t>()(argv[index++], LA::sep);
      N = read_N(error);
      [[maybe_unused]] const auto k = read_k(error);
      assert(N > 0 and k > 0);
      ls1_partial = read_partial_ls(N);
      ls2_partial = read_partial_ls(N);
      assert(not ls1_partial.empty() and not ls2_partial.empty());
    }

    Optimisation::y_t func(const Optimisation::vec_t& v) noexcept {
      assert(not v.empty());
      assert(v.size() == N-2);
      const std::shared_ptr<TWO_MOLS> p(new TWO_MOLS(N,
        alg_options, gecode_options, ls1_partial, ls2_partial, v));
      Statistics::SearchStat stat = LA::solve<TWO_MOLS>(p);
      assert(p.use_count() == 1);
for (const auto x : v) std::cerr << x << " ";
std::cerr << stat << "\n";
      const auto leaves = stat.solutions + stat.unsat_leaves;
      return leaves;
    }
  };

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  FloatingPoint::fullprec_float80(std::cout);
  const auto result = Optimisation::bbopt_rounds_app(argc, argv, Func());
  std::cout << result << "\n";
}

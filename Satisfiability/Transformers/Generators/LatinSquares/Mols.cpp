// Oliver Kullmann, 20.12.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*


Examples:

Fuer 1 <= N <= 11:

https://oeis.org/A002860 :
Number of Latin squares of order n; or labeled quasigroups
1, 2, 12, 576, 161280,
812851200, 61479419904000, 108776032459082956800, 5524751496156892842531225600, 9982437658213039871725064756920320000,
776966836171770144107444346734230682311065600000

Use

> ./Mols N 1 f


https://oeis.org/A000315
Number of reduced Latin squares of order n; also number of labeled loops (quasigroups with an identity element) with a fixed identity element.
1, 1, 1, 4, 56,
9408, 16942080, 535281401856, 377597570964258816, 7580721483160132811489280,
5363937773277371298119673540771840

The "full" numbers (above) are obtained by multiplication with n! * (n-1)!.

Use

> ./Mols N 1

or

> ./Mols N 1 r


https://oeis.org/A072377
Number of pairs of orthogonal Latin squares of order n.
Multiplied with 2 (except for N=1):
1, 0, 72, 6912, 6220800,
0, 6263668776960000, 64324116731941355520000, 38166908141096565278370693120000

Use

> ./Mols N 2 f


*/

#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <optional>
#include <utility>

#include <cassert>
#include <cstdint>
#include <cstdlib>

#include <ProgramOptions/Environment.hpp>
#include "../Random/ClauseSets.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.2",
        "31.12.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/LatinSquares/Mols.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  enum class Error {
    conversion = 11,
    too_big = 12,
    too_small = 13,
    file_open = 14,
    sym_par = 15,
  };

  enum class SymP { reduced=0, full=1 };
  std::ostream& operator <<(std::ostream& out, const SymP opt) {
    switch (opt) {
    case SymP::reduced: return out << "reduced";
    case SymP::full: return out << "full";
    default : return out << "SymP::unknown";}
  }
  enum class EAloP { none=0, val=1, pair=2, both=3 };
  std::ostream& operator <<(std::ostream& out, const EAloP opt) {
    switch (opt) {
    case EAloP::none: return out << "none";
    case EAloP::val: return out << "values";
    case EAloP::pair: return out << "pairs";
    case EAloP::both: return out << "values_pairs";
    default : return out << "EAloP::unknown";}
  }
}
namespace Environment {
  template <>
  struct RegistrationPolicies<SymP> {
    static constexpr int size = int(SymP::full) + 1;
    static constexpr std::array<const char*, size> string
      {"r", "f"};
  };
  template <>
  struct RegistrationPolicies<EAloP> {
    static constexpr int size = int(EAloP::both) + 1;
    static constexpr std::array<const char*, size> string
      {"L0", "Lv", "Lp", "Lb"};
  };
}
namespace {

  std::string default_filestem() {
    return "MOLS2SAT_BASIC";
  }


  typedef std::uint16_t dim_t;
  typedef std::uint64_t var_t;

  constexpr dim_t N_default = 3;
  constexpr dim_t k_default = 1;


  bool show_usage(const int argc, const char* const argv[]) {
    assert(argc >= 1);
    if (argc != 2 or not Environment::is_help_string(argv[1])) return false;
    const std::string& program = proginfo.prg;
    std::cout << "USAGE:\n"
    "> " << program << " [-v | --version]\n"
    " shows version information and exits.\n"
    "> " << program << " [-h | --help]\n"
    " shows help information and exits.\n"
    "\n> " << program <<
    " [N=" << N_default << ",>=1]"
    " [k=" << k_default << ",>=1]"
    " [symopt=" << Environment::WRP<SymP>{} << ",\"\"]"
    " [ealoopt=" << Environment::WRP<EAloP>{} << ",\"\"]"
    " [output=-cout,\"\",-nil,NAME]\n"
    "\n computes the SAT-translation for k MOLS of order N:\n"
    "  - Trailing arguments can be left out, thus using their default-values"
    " (the first given value).\n"
    "  - For the two options-arguments, \"\" means also the default-values.\n"
    "  - \"\" for the output however means the default output-filename:\n"
    "        \"" << default_filestem() << "_N_k_s_e.dimacs\".\n"
    "  - \"-nil\" for the output means no output of clauses (only information).\n"
;
    return true;
  }


  struct Param {
    dim_t N;
    dim_t k;
  };
  std::ostream& operator <<(std::ostream& out, const Param p) {
    return out << p.N << "," << p.k;
  }

  struct NumVarsCls {
    var_t nbls1, nbls2, // number of variables per single latin-square,
                        // first instance and others
          nls, // number of variables for all single latin-squares
          nbes, // number of primary variables for single euler-square
          nes, // number of primary variables for all euler-squares
          n0, // number of all primary variables
          n; // number of variables in total (including auxiliary variables)
    var_t cls, ces, c;
  };
  struct fNumVarsCls {
    FloatingPoint::float80 nbls1, nbls2, nls, nbes, nes, n0, n, cls, ces, c;
    constexpr bool valid() const noexcept {
      return FloatingPoint::isUInt({nbls1, nbls2, nls, nbes, nes, n0, n, cls, ces, c});
    }
    constexpr operator NumVarsCls() const noexcept {
      assert(valid());
      return {var_t(nbls1), var_t(nbls2), var_t(nls), var_t(nbes), var_t(nes), var_t(n0), var_t(n), var_t(cls), var_t(ces), var_t(c)};
    }
  };

  // Handling of pairs n, c, handling overflows by embedding into
  // FloatingPoint::float80
  using RandGen::dimacs_pars;
  struct fdimacs_pars {
    FloatingPoint::float80 n, c;
    constexpr fdimacs_pars() noexcept : n(0), c(0) {}
    constexpr fdimacs_pars(const var_t n, const var_t c) noexcept : n(n), c(c) {}
    constexpr fdimacs_pars(const FloatingPoint::float80 n, const FloatingPoint::float80 c) noexcept : n(n), c(c) {}
    constexpr bool valid() const noexcept {
      return FloatingPoint::isUInt({n,c});
    }
    constexpr operator dimacs_pars() const noexcept {
      assert(valid());
      return {var_t(n), var_t(c)};
    }
  };


  constexpr fdimacs_pars pars_eo_primes(const var_t m) noexcept {
    return {0, 1 + FloatingPoint::fbinomial_coeff(m,2)};
  }
  constexpr fdimacs_pars pars_amo_seco(const var_t m) noexcept {
    if (m <= 1) return {};
    if (m == 2) return {var_t(0),var_t(1)};
    return {(m-1)/2-1L, 3L*m-6};
  }


  constexpr NumVarsCls numvarscls(const Param p, const SymP symopt, const EAloP ealoopt) noexcept {
    const FloatingPoint::float80 N = p.N;
    const auto N2 = N*N;
    const auto N3 = N2*N;
    const auto N4 = N2*N2;
    using FloatingPoint::fbinomial_coeff;

    if (symopt == SymP::full) {
      fNumVarsCls r{};
      r.nbls1 = N3;
      r.nls = r.nbls1 * p.k;
      r.nbes = N4;
      r.nes = r.nbes * fbinomial_coeff(p.k, 2);
      r.n0 = r.nls + r.nes;
      r.n = r.n0;
      if (p.k >= 2)
        r.n += fbinomial_coeff(p.k, 2) * N2 * pars_amo_seco(N2).n;
      if (r.n >= FloatingPoint::P264) {
        std::cerr << error << "Parameters " << p << " yield total number of variables >= 2^64.\n";
        std::exit(int(Error::too_big));
      }

      const auto cbls = 3 * N2 * pars_eo_primes(N).c;
      r.cls = cbls * p.k;
      const auto cbes = 3*N4 +
        ((ealoopt == EAloP::pair or ealoopt == EAloP::both) ?
          N2 : 0) +
        ((ealoopt == EAloP::val or ealoopt == EAloP::both) ?
          N2 : 0);
      r.ces = cbes * fbinomial_coeff(p.k, 2);
      if (p.k >= 2)
        r.ces += fbinomial_coeff(p.k, 2) * N2 * pars_amo_seco(N2).c;
      r.c = r.cls + r.ces;
      if (r.c >= FloatingPoint::P264) {
        std::cerr << error << "Parameters " << p << " yield total number of clauses >= 2^64.\n";
        std::exit(int(Error::too_big));
      }
      return r;
    } else { // SymP::reduced

      [[unimplemented]] fNumVarsCls r{};
      r.nbls1 = (N-1)*((N-1) + (N-2)*(N-2));
      r.nbls2 = (N-1)*N*(N-1);
      r.nls = r.nbls1 + r.nbls2 * (p.k - 1);
      r.nbes = 0;
      r.nes = r.nbes * fbinomial_coeff(p.k, 2);
      r.n0 = r.nls + r.nes; r.n = r.n0;
      if (r.n >= FloatingPoint::P264) {
        std::cerr << error << "Parameters " << p << " yield total number of variables >= 2^64.\n";
        std::exit(int(Error::too_big));
      }

      const auto peop1 = pars_eo_primes(N-1).c;
      const auto peop2 = pars_eo_primes(N-2).c;
      const auto cbls1 = ((N-1)*(N-1) - (N-1)) * peop2 + (N-1) * peop1 +
                         2 * (N-1) * ((N-2) * peop2 + peop1);
      const auto cbls2 = 3 * (N-1) * N * peop1;
      r.cls = cbls1 + cbls2 * (p.k - 1);
      r.c = r.cls;
      if (r.c >= FloatingPoint::P264) {
        std::cerr << error << "Parameters " << p << " yield total number of clauses >= 2^64.\n";
        std::exit(int(Error::too_big));
      }
      return r;
    }
  }


  // The indices of Euler-pairs, and their colexicographical ordering:
  struct IndexEuler {
    dim_t p, q; // p < q
  };
  constexpr bool operator <(const IndexEuler e1, const IndexEuler e2) noexcept {
    return e1.q < e2.q or (e1.q == e2.q and e1.p < e2.p);
  }
  static_assert(IndexEuler{0,1} < IndexEuler{0,2});
  static_assert(IndexEuler{0,2} < IndexEuler{1,2});
  static_assert(IndexEuler{1,2} < IndexEuler{0,3});

  constexpr var_t index(const IndexEuler e) noexcept {
    if (e.q % 2 == 0) return e.p + var_t(e.q/2) * (e.q-1);
    else return e.p + var_t(e.q) * ((e.q-1) / 2);
  }
  static_assert(index({0,1}) == 0);
  static_assert(index({0,2}) == 1);
  static_assert(index({1,2}) == 2);
  static_assert(index({0,3}) == 3);
  static_assert(index({65534, 65535}) == 2147450879);
  static_assert(index({65533, 65534}) == 2147385344);


  struct ValPair {
    dim_t x, y; // x, y < N
  };
  constexpr var_t index(const ValPair eps, const dim_t N) noexcept {
    assert(eps.x < N);
    assert(eps.y < N);
    return var_t(eps.x) * N + eps.y;
  }
  static_assert(index({0,0}, 10) == 0);
  static_assert(index({0,1}, 10) == 1);
  static_assert(index({1,0}, 10) == 10);
  static_assert(index({9,9}, 10) == 99);


  struct Encoding {
    const var_t N;
    const var_t k;
    const NumVarsCls nv;
    const SymP symopt;
    const EAloP ealoopt;

    const var_t N2 = N*N;
    const var_t N3 = N2 * N;

  private :
    mutable var_t next = nv.n0;
  public :

    constexpr Encoding(const Param ps, const SymP s, const EAloP e = EAloP::none) noexcept : N(ps.N), k(ps.k), nv(numvarscls(ps,s,e)), symopt(s), ealoopt(e) {}

    var_t operator()() const noexcept {
      assert(next < nv.n);
      return ++next;
    }

    constexpr var_t operator()(const dim_t i, const dim_t j, const dim_t eps, const dim_t p) const noexcept {
      assert(i < N);
      assert(j < N);
      assert(eps < N);
      assert(p < k);

      switch (symopt) {

      case SymP::reduced:
        if (p == 0) {
          assert(i != 0);
          assert(j != 0);
          assert(eps != i);
          assert(eps != j);
          const var_t n_prev_lines = (i-1) * ((N-2)*(N-2) + (N-1));
          const var_t n_prev_cells = i>=j ? (j-1)*(N-2) : (j-2)*(N-2) + (N-1);
          const var_t v = 1 + n_prev_lines + n_prev_cells + eps_adj(i,j,eps);
          assert(v <= nv.nls);
          return v;
        }
        else {
          assert(i != 0);
          assert(eps != j);
          const var_t n_prev_lines = (i-1) * N*(N-1);
          const var_t n_prev_cells = j * (N-1);
          const var_t v = 1 + n_prev_lines + n_prev_cells + eps_adj(j,eps);
          assert(v <= nv.nls);
          return v;
        }

      default : {
          const var_t v = 1 + i * N2 + j * N + eps + p * nv.nbls1;
          assert(v <= nv.nls);
          return v;
      }}
    }

    constexpr var_t operator()(const dim_t i, const dim_t j, const ValPair eps, const IndexEuler pq) const noexcept {
      assert(i < N);
      assert(j < N);
      assert(eps.x < N);
      assert(eps.y < N);
      assert(pq.p < pq.q);
      assert(pq.q < k);
      const var_t v = nv.nls + 1 + i * N3 + j * N2 + index(eps,N) + index(pq) * nv.nbes;
      assert(nv.nls < v and v <= nv.n0);
      return v;
    }

    // Using var_t for the arguments to avoid implicit conversions:
    static constexpr var_t eps_adj(var_t i, var_t j, const var_t eps) noexcept {
      if (i == j) {
        if (eps < i) return eps;
        else return eps-1;
      }
      if (i > j) std::swap(i,j);
      if (eps > j) return eps-2;
      else if (eps > i) return eps-1;
      else return eps;
    }
    static constexpr var_t eps_adj(const var_t j, const var_t eps) noexcept {
      if (eps > j) return eps-1;
      else return eps;
    }

  };
  /* The following creates ICE's for gcc 9.2:
  static_assert(Encoding({1,1},SymP::full)(0,0,0,0) == 1);
  static_assert(Encoding({2,1},SymP::full)(0,0,0,0) == 1);
  static_assert(Encoding({2,1},SymP::full)(0,0,1,0) == 2);
  static_assert(Encoding({2,1},SymP::full)(0,1,0,0) == 3);
  static_assert(Encoding({2,1},SymP::full)(0,1,1,0) == 4);
  static_assert(Encoding({2,1},SymP::full)(1,0,0,0) == 5);
  static_assert(Encoding({2,1},SymP::full)(1,0,1,0) == 6);
  static_assert(Encoding({2,1},SymP::full)(1,1,0,0) == 7);
  static_assert(Encoding({2,1},SymP::full)(1,1,1,0) == 8);
  static_assert(Encoding({2,2},SymP::full)(0,0,0,1) == 9);
  static_assert(Encoding({2,2},SymP::full)(0,0,{0,0},{0,1}) == 17);
  static_assert(Encoding({2,2},SymP::full)(0,0,{0,1},{0,1}) == 18);
  static_assert(Encoding({2,3},SymP::full)(1,1,{1,1},{1,2}) == 72);
  */


  using RandGen::Var;
  using RandGen::Lit;
  using RandGen::Clause;

  void amo_primes(std::ostream& out, const Clause& C) {
    if (C.size() >= 2) {
      auto current_end = C.cbegin(); ++current_end;
      do {
        const Lit y = -*current_end;
        for (auto i = C.cbegin(); i != current_end; ++i)
          out << Clause{-*i, y};
      } while (++current_end != C.end());
    }
  }
  void eo_primes(std::ostream& out, const Clause& C) {
    amo_primes(out, C);
    out << C;
  }

  // As seco_amov2cl(L,V) in CardinalityConstraints.mac:
  void amo_seco(std::ostream& out, Clause C, const Encoding& enc) {
    Clause B(3);
    while (C.size() > 4) {
      const Lit w{enc(), 1};
      B.assign(C.end()-3, C.end());
      C.resize(C.size()-3);
      C.push_back(w);
      amo_primes(out, B);
      for (const Lit x : B) out << Clause{-x,w};
    }
    amo_primes(out, C);
  }
  void eo_seco(std::ostream& out, Clause C, const Encoding& enc) {
    amo_seco(out, C, enc);
    out << C;
  }

  // x <-> (y and z)
  void definition(std::ostream& out, const Lit x, const Lit y, const Lit z) {
    out << Clause{-x, y} << Clause{-x, z} << Clause{x, -y, -z};
  }


  void ls(std::ostream& out, const Encoding& enc) {
    if (enc.symopt == SymP::full) {

      for (dim_t p = 0; p < enc.k; ++p) {
        // EO(i,j,-,p) :
        for (dim_t i = 0; i < enc.N; ++i)
          for (dim_t j = 0; j < enc.N; ++j) {
            Clause C;
            for (dim_t eps = 0; eps < enc.N; ++eps)
              C.push_back({enc(i,j,eps,p),1});
            eo_primes(out, C);
          }
        // EO(i,-,eps,p) :
        for (dim_t i = 0; i < enc.N; ++i)
          for (dim_t eps = 0; eps < enc.N; ++eps) {
            Clause C;
            for (dim_t j = 0; j < enc.N; ++j)
              C.push_back({enc(i,j,eps,p),1});
            eo_primes(out, C);
          }
        // EO(-,j,eps,p) :
        for (dim_t j = 0; j < enc.N; ++j)
          for (dim_t eps = 0; eps < enc.N; ++eps) {
            Clause C;
            for (dim_t i = 0; i < enc.N; ++i)
              C.push_back({enc(i,j,eps,p),1});
            eo_primes(out, C);
          }
      }

    } else { // SymP::reduced

      // EO(i,j,-,0) :
      for (dim_t i = 1; i < enc.N; ++i)
        for (dim_t j = 1; j < enc.N; ++j) {
          Clause C;
          for (dim_t eps = 0; eps < enc.N; ++eps)
            if (eps != i and eps != j)
              C.push_back({enc(i,j,eps,0),1});
          eo_primes(out, C);
        }
      // EO(i,-,eps,0) :
      for (dim_t i = 1; i < enc.N; ++i)
        for (dim_t eps = 0; eps < enc.N; ++eps) {
          if (eps == i) continue;
          Clause C;
          for (dim_t j = 1; j < enc.N; ++j)
            if (eps != j)
              C.push_back({enc(i,j,eps,0),1});
          eo_primes(out, C);
        }
      // EO(-,j,eps,0) :
      for (dim_t j = 1; j < enc.N; ++j)
        for (dim_t eps = 0; eps < enc.N; ++eps) {
          if (eps == j) continue;
          Clause C;
          for (dim_t i = 1; i < enc.N; ++i)
            if (eps != i)
              C.push_back({enc(i,j,eps,0),1});
          eo_primes(out, C);
        }

      for (dim_t p = 1; p < enc.k; ++p) {
        // EO(i,j,-,p) :
        for (dim_t i = 1; i < enc.N; ++i)
          for (dim_t j = 0; j < enc.N; ++j) {
            Clause C;
            for (dim_t eps = 0; eps < enc.N; ++eps)
              if (eps != j)
                C.push_back({enc(i,j,eps,p),1});
            eo_primes(out, C);
          }
        // EO(i,-,eps,p) :
        for (dim_t i = 1; i < enc.N; ++i)
          for (dim_t eps = 0; eps < enc.N; ++eps) {
            Clause C;
            for (dim_t j = 0; j < enc.N; ++j)
              if (eps != j)
                C.push_back({enc(i,j,eps,p),1});
            eo_primes(out, C);
          }
        // EO(-,j,eps,p) :
        for (dim_t j = 0; j < enc.N; ++j)
          for (dim_t eps = 0; eps < enc.N; ++eps) {
            if (eps == j) continue;
            Clause C;
            for (dim_t i = 1; i < enc.N; ++i)
              C.push_back({enc(i,j,eps,p),1});
            eo_primes(out, C);
          }
      }
    }
  }


  void es_defs(std::ostream& out, const Encoding& enc) {
    if (enc.symopt == SymP::full) {
      for (dim_t q = 1; q < enc.k; ++q)
        for (dim_t p = 0; p < q; ++p)
          for (dim_t i = 0; i < enc.N; ++i)
            for (dim_t j = 0; j < enc.N; ++j)
              for (dim_t x = 0; x < enc.N; ++x)
                for (dim_t y = 0; y < enc.N; ++y)
                  definition(out, {enc(i,j,{x,y},{p,q}),1}, {enc(i,j,x,p),1}, {enc(i,j,y,q),1});
    }
  }


  void es_values(std::ostream& out, const Encoding& enc) {
    if (enc.ealoopt == EAloP::none or enc.ealoopt == EAloP::pair) return;
    Clause C; C.reserve(enc.N * enc.N);
    for (dim_t q = 1; q < enc.k; ++q)
        for (dim_t p = 0; p < q; ++p)
          for (dim_t i = 0; i < enc.N; ++i)
            for (dim_t j = 0; j < enc.N; ++j) {
              C.clear();
              for (dim_t x = 0; x < enc.N; ++x)
                for (dim_t y = 0; y < enc.N; ++y)
                  C.push_back({enc(i,j,{x,y},{p,q}),1});
              out << C;
            }
  }

  void orthogonality(std::ostream& out, const Encoding& enc) {
    if (enc.symopt == SymP::full) {
      Clause C; C.reserve(enc.N * enc.N);
      for (dim_t q = 1; q < enc.k; ++q)
        for (dim_t p = 0; p < q; ++p)
          for (dim_t x = 0; x < enc.N; ++x)
            for (dim_t y = 0; y < enc.N; ++y) {
              C.clear();
              for (dim_t i = 0; i < enc.N; ++i)
                for (dim_t j = 0; j < enc.N; ++j)
                  C.push_back({enc(i,j,{x,y},{p,q}),1});
              if (enc.ealoopt == EAloP::none or enc.ealoopt == EAloP::val)
                amo_seco(out, C, enc);
              else
                eo_seco(out, C, enc);
            }
    }
  }


  inline std::string default_param(const Encoding e) {
    return std::to_string(e.N) + "_" + std::to_string(e.k) + "_" + Environment::RegistrationPolicies<SymP>::string[int(e.symopt)] + "_" + Environment::RegistrationPolicies<EAloP>::string[int(e.ealoopt)];
  }
  inline std::string default_filesuffix() {
    return ".dimacs";
  }
  inline std::string default_filename(const Encoding e) {
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

  const std::optional<EAloP> realoopt = argc <= index ? EAloP{} : Environment::read<EAloP>(argv[index++]);
  if (not realoopt) {
    std::cerr << error << "Bad option-argument w.r.t. Euler-ALO: \"" << argv[index-1] << "\".\n";
    return int(Error::sym_par);
  }
  const EAloP ealoopt = realoopt.value();

  const Encoding enc(p, symopt, ealoopt);

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
            << DWW{"output"} << qu(filename) << "\n"
      << DHW{"Sizes"}
            << DWW{"nls"} << enc.nv.nls << "\n"
            << DWW{"nes"} << enc .nv.nes << "\n"
            << DWW{"n0"} << enc.nv.n0 << "\n"
            << DWW{"n"} << enc.nv.n << "\n"
            << DWW{"cls"} << enc.nv.cls << "\n"
            << DWW{"ces"} << enc.nv.ces << "\n"
            << DWW{"c"} << enc.nv.c << "\n"
;

  if (filename == "-nil") return 0;
  out << dimacs_pars{enc.nv.n, enc.nv.c};
  ls(out, enc);
  es_defs(out, enc);
  es_values(out, enc);
  orthogonality(out, enc);
}

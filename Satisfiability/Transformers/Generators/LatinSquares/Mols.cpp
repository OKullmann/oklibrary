// Oliver Kullmann, 20.12.2019 (Swansea)
/* Copyright 2019, 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  See subdirectory docus.

BUGS:

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
 - Perhaps Options.hpp ?
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
#include <utility>

#include <cstdint>
#include <cstdlib>

#include <ProgramOptions/Environment.hpp>
#include "../Random/ClauseSets.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.9.7",
        "25.3.2020",
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
    ealo_par = 16,
    eul_par = 17,
    p_par = 18,
  };


  enum class SymP { reduced=0, full=1 };
  std::ostream& operator <<(std::ostream& out, const SymP opt) {
    switch (opt) {
    case SymP::reduced: return out << "reduced";
    case SymP::full: return out << "full";
    default : return out << "SymP::unknown=" << int(opt);}
  }

  enum class EAloP { inactive=-1,
                     none=0, val=1, pair=2, pairuep=3, both=4, bothuep=5 };
  inline constexpr bool has_pair(const EAloP o) {
    return o != EAloP::none and o != EAloP::val;
  }
  inline constexpr bool has_val(const EAloP o) {
    return o == EAloP::val or o == EAloP::both or o == EAloP::bothuep;
  }
  inline constexpr bool has_uep(const EAloP o) {
    return o == EAloP::pairuep or o == EAloP::bothuep;
  }
  std::ostream& operator <<(std::ostream& out, const EAloP opt) {
    switch (opt) {
    case EAloP::none: return out << "none";
    case EAloP::val: return out << "values";
    case EAloP::pair: return out << "pairs";
    case EAloP::pairuep : return out << "pairs_uep";
    case EAloP::both: return out << "values_pairs";
    case EAloP::bothuep: return out << "values_pairs_uep";
    case EAloP::inactive: return out << "inactive";
    default : return out << "EAloP::unknown=" << int(opt);}
  }

  enum class EulP { inactive=-1, full=0, positive=1, };
  std::ostream& operator <<(std::ostream& out, const EulP opt) {
    switch (opt) {
    case EulP::full: return out << "full";
    case EulP::positive: return out << "positive";
    case EulP::inactive: return out << "inactive";
    default : return out << "EulP::unknown=" << int(opt);}
  }

  enum class PrimeP { full=0, min=1 };
  std::ostream& operator <<(std::ostream& out, const PrimeP opt) {
    switch (opt) {
    case PrimeP::full: return out << "full";
    case PrimeP::min: return out << "minimal";
    default : return out << "PrimeP::unknown=" << int(opt);}
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
    static constexpr int size = int(EAloP::bothuep) + 1;
    static constexpr std::array<const char*, size> string
      {"L0", "Lv", "Lp", "Lpu", "Lb", "Lbu"};
  };
  template <>
  struct RegistrationPolicies<EulP> {
    static constexpr int size = int(EulP::positive) + 1;
    static constexpr std::array<const char*, size> string
      {"fE", "pE"};
  };
  template <>
  struct RegistrationPolicies<PrimeP> {
    static constexpr int size = int(PrimeP::min) + 1;
    static constexpr std::array<const char*, size> string
      {"fP", "mP"};
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
          nls, // total number of variables for all single latin-squares
          nbes1, nbes2, // number of primary variables for single euler-square,
                        // for p=0 and p>=1
          npes, // total number of primary variables for all euler-squares
          n0, // number of all primary variables
          nbaux1, nbaux2, // number of auxiliary variables for single es
          naux, // number of all auxiliary variables (amo/eo for euler-squares)
          n; // number of variables in total
    var_t cls, ces, c;
  };
  struct fNumVarsCls {
    FloatingPoint::float80 nbls1, nbls2, nls, nbes1, nbes2, npes, n0, nbaux1, nbaux2, naux, n, cls, ces, c;
    constexpr bool valid() const noexcept {
      return FloatingPoint::isUInt({nbls1, nbls2, nls, nbes1, nbes2, npes, n0, nbaux1, nbaux2, naux, n, cls, ces, c});
    }
    constexpr operator NumVarsCls() const noexcept {
      assert(valid());
      return {var_t(nbls1), var_t(nbls2), var_t(nls), var_t(nbes1), var_t(nbes2), var_t(npes), var_t(n0), var_t(nbaux1), var_t(nbaux2), var_t(naux), var_t(n), var_t(cls), var_t(ces), var_t(c)};
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
  constexpr fdimacs_pars pars_amo_primes(const var_t m) noexcept {
    return {0, FloatingPoint::fbinomial_coeff(m,2)};
  }
  constexpr fdimacs_pars pars_alo_primes(const var_t) noexcept {
    return {0, 1.0L};
  }

  constexpr FloatingPoint::float80 n_amo_seco(const var_t m) noexcept {
    if (m <= 2) return 0;
    return (m-1)/2-1L;
  }
  constexpr FloatingPoint::float80 c_amo_seco(const var_t m, const EAloP ealoopt) noexcept {
    if (m <= 1) return 0;
    if (m == 2) return 1;
    const FloatingPoint::float80 c = 3L*m - 6;
    if (not has_uep(ealoopt)) return c;
    else return c + n_amo_seco(m);
  }


  constexpr NumVarsCls numvarscls(const Param p, const SymP symopt, const EAloP ealoopt, const EulP eulopt, const PrimeP primopt) noexcept {
    const FloatingPoint::float80 N = p.N, k = p.k;
    const auto N2 = N*N;
    const auto N3 = N2*N;
    const auto N4 = N2*N2;
    const var_t vk{p.k};
    using FloatingPoint::fbinomial_coeff;

    if (symopt == SymP::full) {
      fNumVarsCls r{};
      r.nbls1 = N3;
      r.nls = r.nbls1 * k;
      r.nbes1 = N4;
      r.npes = r.nbes1 * fbinomial_coeff(p.k, 2);
      r.n0 = r.nls + r.npes;
      r.n = r.n0;
      r.nbaux1 = N2 * n_amo_seco(N2);
      r.naux = r.nbaux1 * fbinomial_coeff(p.k, 2);
      r.n = r.n0 + r.naux;
      if (r.n >= FloatingPoint::P264) {
        std::cerr << error << "Parameters " << p << " yield total number of variables >= 2^64.\n";
        std::exit(int(Error::too_big));
      }

      const auto cbls = primopt==PrimeP::full ? 3 * N2 * pars_eo_primes(N).c :
        N2 * pars_alo_primes(N).c + 2 * N2 * pars_amo_primes(N).c;
      r.cls = cbls * k;
      const auto cbes = (eulopt==EulP::full ? 3 : 1) * N4 +
        (has_pair(ealoopt) ?
          N2 : 0) +
        (has_val(ealoopt) ?
          N2 : 0);
      r.ces = cbes * fbinomial_coeff(p.k, 2);
      r.ces += fbinomial_coeff(p.k, 2) * N2 * c_amo_seco(N2, ealoopt);
      r.c = r.cls + r.ces;
      if (r.c >= FloatingPoint::P264) {
        std::cerr << error << "Parameters " << p << " yield total number of clauses >= 2^64.\n";
        std::exit(int(Error::too_big));
      }
      return r;
    }
    else { // SymP::reduced

      fNumVarsCls r{};
      r.nbls1 = (N-1)*((N-1) + (N-2)*(N-2));
      r.nbls2 = (N-1)*(N*(N-1) - 1);
      r.nls = r.nbls1 + r.nbls2 * (k - 1);
      r.nbes1 = (N-2)*(N-1)*(N2 - 3*N + 3);
      r.nbes2 = (N-2)*(N-2)*(N2 - 1);
      //r.nbes2 = (N-1)*(N-1)*N*(N-2) - (N-1)*(2*N-5);
      r.npes = r.nbes1 * (k-1) + r.nbes2 * fbinomial_coeff(vk-1, 2);
      r.n0 = r.nls + r.npes;
      r.nbaux1 = (N-1) * n_amo_seco((N-1)*(N-2)) +
                 (N-1) * n_amo_seco((N-2)*(N-2)) +
                 (N-1)*(N-2) * n_amo_seco((N-3)*(N-2) + 1);
      r.nbaux2 = 2*(N-1) * n_amo_seco((N-1)*(N-2)) +
                 (N-1)*(N-2) * n_amo_seco(N*(N-3));
      r.naux = (k - 1) * r.nbaux1 + fbinomial_coeff(vk-1, 2) * r.nbaux2;
      r.n = r.n0 + r.naux;
      if (r.n >= FloatingPoint::P264) {
        std::cerr << error << "Parameters " << p << " yield total number of variables >= 2^64.\n";
        std::exit(int(Error::too_big));
      }

      const auto peop1 = pars_eo_primes(N-1).c;
      const auto palop1 = pars_alo_primes(N-1).c;
      const auto pamop1 = pars_amo_primes(N-1).c;
      const auto peop2 = pars_eo_primes(N-2).c;
      const auto palop2 = pars_alo_primes(N-2).c;
      const auto pamop2 = pars_amo_primes(N-2).c;
      const auto cbls1 = primopt==PrimeP::full ?
        (N-1)*peop1 + (N-1)*(N-2)*peop2 + 2*(N-1)*(peop1+(N-2)*peop2) :
        (N-1)*((N-2)*palop2+palop1)+2*(N-1)*((N-2)*pamop2+pamop1);
      const auto cbls2 = primopt==PrimeP::full ?
        (N-1)*peop2 + (N-1)*(N-1)*peop1 + 2*(N-1)*(peop2+(N-1)*peop1) :
        (N-1) * N * (palop1 + 2 * pamop1); // ???
      r.cls = cbls1 + cbls2 * (k - 1);
      const auto cdefs = (eulopt==EulP::full ? 3 : 1) * r.npes + // ???
        (k-1) * ((N-1)*(N-2)*(N-2) + (N-1)*(N-1)) +
        fbinomial_coeff(vk-1, 2) * (N-1)*(N-1)*(N-1);
      const auto cbes1 = (has_val(ealoopt) ? (N-1)*(N-1) : 0) +
                         (has_pair(ealoopt) ? N*(N-1) : 0);
      const auto cbes2 = (has_val(ealoopt) ? (N-1)*N : 0) +
                         (has_pair(ealoopt) ? N*(N-1) : 0);
      r.ces = cdefs + cbes1 * (vk-1) + cbes2 * fbinomial_coeff(vk-1, 2);
      r.ces += (k - 1) * // ???
                ((N-1) * c_amo_seco((N-2)*(N-1), ealoopt) +
                 (N-1) * c_amo_seco((N-2)*(N-2), ealoopt) +
                 (N-1)*(N-2) * c_amo_seco((N-3)*(N-2) + 1, ealoopt));
      r.ces += fbinomial_coeff(vk-1, 2) * (N2 - N) * c_amo_seco((N-2)*(N-1), ealoopt);
      r.c = r.cls + r.ces;
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
  /*std::ostream& operator <<(std::ostream& out, const IndexEuler e) {
    return out << e.p << "<" << e.q;
  }*/

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
  /*std::ostream& operator <<(std::ostream& out, const ValPair p) {
    return out << p.x << "," << p.y;
  }*/

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
    const NumVarsCls nvc;
    const SymP symopt;
    const EAloP ealoopt;
    const EulP eulopt;
    const PrimeP primopt;

    const var_t N2 = N*N;
    const var_t N3 = N2 * N;

  private :
    mutable var_t next = nvc.n0;
  public :

    constexpr Encoding(const Param ps, const SymP s, const EAloP e, const EulP eul, const PrimeP prim) noexcept : N(ps.N), k(ps.k), nvc(numvarscls(ps,s,e,eul,prim)), symopt(s), ealoopt(e), eulopt(eul), primopt(prim) {}

    void nls(std::ostream& out) const {
      if (symopt == SymP::full)
        if (k >= 2)
          out << Environment::DWW{"  nls=kN3"} << nvc.nls << "\n";
        else
          out << Environment::DWW{"  nls=N3"} << nvc.nls << "\n";
      else
        if (k >= 2)
          out << Environment::DWW{"  nls=kN3-(2k+2)N2+6N+k-4"} << nvc.nls << "\n";
        else
          out << Environment::DWW{"  nls=N3-4N2+6N-3"} << nvc.nls << "\n";
    }
    void npes(std::ostream& out) const {
      if (symopt == SymP::full)
        out << Environment::DWW{"  npes=0.5k(k-1)N4"} << nvc.npes << "\n";
      else
        out << Environment::DWW{"  npes=0.5k(k-1)(N-1)(N-2)*"} << "\n"
            << Environment::DWW{"       (N2-(1+4/k)N-2+10/k)"} << nvc.npes << "\n";
    }
    void naux(std::ostream& out) const {
      out << Environment::DWW{"  naux~0.5npes"} << nvc.naux << "\n";
    }

    void cls(std::ostream& out) const {
      if (symopt == SymP::full) {
        if (primopt == PrimeP::full)
          if (k >= 2)
            out << Environment::DWW{"  cls=1.5kN2(N2-N+2)"} << nvc.cls << "\n";
          else
            out << Environment::DWW{"  cls=1.5N2(N2-N+2)"} << nvc.cls << "\n";
        else
          if (k >= 2)
            out << Environment::DWW{"  cls=kN2(N2-N+1)"} << nvc.cls << "\n";
          else
            out << Environment::DWW{"  cls=N2(N2-N+1)"} << nvc.cls << "\n";
      }
      else {
        if (primopt == PrimeP::full)
          if (k >= 2)
            out << Environment::DWW{"  cls=1.5(N-1)*"} << "\n"
                << Environment::DWW{"      (kN3-(3k+3)N2+(2k+13)N+4k-16)"}
                << nvc.cls << "\n";
          else
            out << Environment::DWW{"  cls=1.5(N-1)(N3-6N2+15N-12)"} << nvc.cls
                << "\n";
        else
          if (k >= 2)
            out << Environment::DWW{"  cls=k(N-1)*"} << "\n"
                << Environment::DWW{"      (N3-3(1+1/k)N2+(3+11/k)N-11/k)"}
                << nvc.cls << "\n";
          else
            out << Environment::DWW{"  cls=(N-1)(N3-6N2+14N-11)"}
                << nvc.cls << "\n";
      }
    }
    void ces(std::ostream& out) const {
      if (not has_uep(ealoopt)) {
        if (eulopt == EulP::full)
          out << Environment::DWW{"  ces~6npes"} << nvc.ces << "\n";
        else
          out << Environment::DWW{"  ces~4npes"} << nvc.ces << "\n";
      }
      else {
        if (eulopt == EulP::full)
          out << Environment::DWW{"  ces~6.5npes"} << nvc.ces << "\n";
        else
          out << Environment::DWW{"  ces~4.5npes"} << nvc.ces << "\n";
      }
    }

    var_t operator()() const noexcept {
      assert(next < nvc.n);
      return ++next;
    }

    constexpr var_t operator()(const dim_t i, const dim_t j, const dim_t eps, const dim_t p) const noexcept {
      assert(i < N);
      assert(j < N);
      assert(eps < N);
      assert(p < k);

      if (symopt == SymP::full) {
        assert(symopt == SymP::full);
        const var_t v = 1 + p * nvc.nbls1 + i * N2 + j * N + eps;
        assert(v <= nvc.nls);
        return v;
      }
      else {
        assert(symopt == SymP::reduced);
        if (p == 0) {
          assert(i != 0);
          assert(j != 0);
          assert(eps != i);
          assert(eps != j);
          const var_t n_prev_lines = (i-1) * ((N-2)*(N-2) + (N-1));
          const var_t n_prev_cells = i>=j ? (j-1)*(N-2) : (j-2)*(N-2) + (N-1);
          const var_t v = 1 + n_prev_lines + n_prev_cells + eps_adj(i,j,eps);
          assert(v <= nvc.nls);
          return v;
        }
        else {
          assert(i != 0);
          assert(eps != j);
          assert(j != 0 or eps != i);
          const var_t n_prev_ls = nvc.nbls1 + (p-1) * nvc.nbls2;
          const var_t n_prev_lines = (i-1) * (N*(N-1) - 1);
          const var_t n_prev_cells = j * (N-1) - (j==0 ? 0 : 1);
          const var_t v = 1 + n_prev_ls + n_prev_lines + n_prev_cells +
            (j==0 ? eps_adj(i,j,eps) : eps_adj(j,eps));
          assert(v <= nvc.nls);
          return v;
        }
      }
    }

    constexpr var_t operator()(const dim_t i, const dim_t j, const ValPair eps, const IndexEuler pq) const noexcept {
      assert(i < N);
      assert(j < N);
      assert(eps.x < N);
      assert(eps.y < N);
      assert(pq.p < pq.q);
      assert(pq.q < k);

      if (symopt == SymP::full) {
        const var_t n_prev_es = index(pq) * nvc.nbes1;
        const var_t n_prev_lines = i * N3;
        const var_t n_prev_cells = j * N2;
        const var_t v = 1+nvc.nls + n_prev_es + n_prev_lines + n_prev_cells + index(eps,N);
        assert(nvc.nls < v and v <= nvc.n0);
        return v;
      }

      else {
        assert(symopt == SymP::reduced);
        assert(i != 0);
        assert(eps.x != j);
        assert(eps.y != j);
        assert(eps.x != eps.y);
        const var_t p = pq.p, q = pq.q;
        if (p >= 1) {
          assert(j != 0 or (eps.x != i and eps.y != i));
          const var_t n_prev_es = q * nvc.nbes1 + (index(pq)-q) * nvc.nbes2;
          const var_t n_prev_lines = (i-1) * N * (N-1) * (N-2) - (i-1) * 2 * (N-2);
          const var_t n_prev_cells = j==0 ? 0  : j * (N-1) * (N-2) - 2 * (N-2);
          const var_t v = 1+nvc.nls + n_prev_es + n_prev_lines + n_prev_cells
            + (j==0 ? index_adj2(i,j,eps) : index_adj(j, eps));
          assert(nvc.nls < v and v <= nvc.n0);
          return v;
        }
        else {
          assert(p == 0);
          if (j == 0) {
            assert(eps.x == i);
            return operator()(i,0,eps.y,pq.q);
          }
          assert(eps.x != i);
          const var_t n_prev_es = (q-1) * nvc.nbes1 + (index(pq)-(q-1)) * nvc.nbes2;
          const var_t n_prev_lines = (i-1) * ((N-2)*(N-2)*(N-2) + (N-1)*(N-2));
          const var_t n_prev_cells = j<=i ? (j-1) * (N-2) * (N-2) :
                                           (N-1)*(N-2) + (j-2) * (N-2) * (N-2);
          const var_t v = 1+nvc.nls + n_prev_es + n_prev_lines + n_prev_cells + index_adj(i, j, eps);
          assert(nvc.nls < v and v <= nvc.n0);
          return v;
        }
      }
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

    constexpr var_t index_adj(const var_t j, const ValPair eps) const noexcept {
      var_t res = index(eps, N);
      if (eps.x < j) {
        res -= eps.x;
        if (eps.y > j) --res;
        if (eps.x <= eps.y) res -= eps.x+1;
        else res -= eps.x;
      } else if (eps.x == j) {
        res -= eps.x + (eps.y + 1);
        res -= eps.x;
      } else {
        res -= N + (eps.x - 1);
        if (eps.y > j) --res;
        if (eps.x <= eps.y) res -= eps.x;
        else res -= eps.x - 1;
      }
      return res;
    }
    constexpr var_t index_adj(const var_t i, const var_t j, const ValPair eps) const noexcept {
      /* The relevant constraints are: x, y != j, x != i, x != y.
          For i!=j there are (N-2)^2 variables in total, namely N-2 rows
            (minus i,j), each with N-2 elements (minus j, minus diagonal).
          and for i=j there are (N-1)*(N-2) variables in total, namely N-1
            rows (minus i=j), each with N-2 elements (minus j, minus diagonal).
      */
      const var_t xadj = eps_adj(i,j, eps.x);
      const var_t yadj = eps_adj(eps.x,j, eps.y);
      return xadj * (N-2) + yadj;
    }
    constexpr var_t index_adj2(const var_t i, const var_t j, const ValPair eps) const noexcept {
      assert(i != 0 and j == 0);
      /* The relevant constraints are: x,y >= 1, x,y != i; x != y. */
      return index_adj(i,j,eps) - (eps.x-1) - (eps.y > i ? 1 : 0);
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
  void amo_seco(std::ostream& out, Clause C, const Encoding& enc) {
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
  var_t amouep_seco(std::ostream& out, Clause C, const Encoding& enc) {
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
  void eo_seco(std::ostream& out, const Clause& C, const Encoding& enc) {
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


  void ls(std::ostream& out, const Encoding& enc) {
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


  void es_defs(std::ostream& out, const Encoding& enc) {
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


  void es_values(std::ostream& out, const Encoding& enc) {
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

  void orthogonality(std::ostream& out, const Encoding& enc) {
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


  inline std::string default_param(const Encoding e) {
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

  const Encoding enc(p, symopt, ealoopt, eulopt, primopt);

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

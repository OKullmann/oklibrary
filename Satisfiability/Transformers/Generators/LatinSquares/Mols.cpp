// Oliver Kullmann, 20.12.2019 (Swansea)
/* Copyright 2019, 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

The encoding:

For each latin square 0 <= p < k, each field 0 <= i, j < N and each value
0 <= eps < N, there is a (boolean) variable

  enc(i,j,eps,p),

with the meaning: field (i,j) of ls p carries value eps.
This makes N^2 * N = N^3 variables per latin square, for the "full form".
In total that makes

  nls = k * N^3.

The primary variables for the euler-squares are

  enc(i,j,{x,y},{p,q}),

where now 0 <= x, y < N yield the values of a square (with indices i,j),
while 0 <= p < q < k is the index of the euler-square.
(Note that here the curly brackets are used as in the C++ code, not as
in Mathematics (for sets).)
So per euler-square there are N^2 * N^2 = N^4 primary variables (again for
the full form). In total that makes

  npes = binomial(k, 2) * N^4.

The clauses for the latin squares are
  eo ("exactly one") for the fields, rows, columns,
  using the representation by prime clauses.

That makes

  cls = k * 3 * N^2 * (1+binomial(N,2)).

The clauses for the euler-squares are:

 (a) The defining equivalences ("euler-equivalences")

       enc(i,j,{x,y},{p,q}) <-> enc(i,j,x,p) && enc(i,j,y,q);

     which yields 3 * N^4 * binomial(k,2) clauses.

 (b) For all {x,y} and all {p,q} ("euler-amo")

       amo for all i, j : enc({i,j,{x,y},{p,q}).

     Here the "sequential command-encoding" is used, which uses (roughly),
     for amo over m literals:
      - m/2 auxiliary (new) variables
      - 3m clauses.

That makes

  ces ~ 6 * N^4 * binomial(k,2) = 6 * npes

  naux ~ 1/2 * N^4 * binomial(k,2) = 1/2 * npes.

In total:

  n ~ k*N^3 + 3/2 binomial(k,2)*N^4
  c ~ 3 k*N^2*(1+binomial(N,2)) + 6 N^4*binomial(k,2).


In case of "reduced form", the first ls (p=0) is (fully) reduced, the others
are "half-reduced", that is, only the first row (not the first column) is
required to be in standard form:

The following old ls-variables are set:

  first row has value j for column j, for all p:
    enc(0,j,j,p) -> true for all 0 <= p < k and 0 <= j < N

  first column has value i for row i, for p=0:
    enc(i,0,i,0) -> true for all 0 <= j < N.

This yields for the ls-variables:

  first row has no value different from j for column j, for all p
  (due to value-amo):
    enc(0,j,j',p) -> false for all j and j' != j
  all columns j don't have value j for non-first rows i, for all p
  (due to column-amo):
    enc(i,j,j,p)  -> false for all i != 0

  first column has no value different from i for row i, for p=0
  (due to value-amo):
    enc(i,0,i',0) -> false for all i' != i
  all rows i don't have value i for non-first columns j, for p=0
  (due to row-amo):
    enc(i,j,i,0)  -> false for all j != 0

For the es-variables ("euler-squares") this yields:

  Via the euler-definitions, for all {p,q}:

    enc(0,j,{j,j},{p,q}) -> true for all j
    enc(0,j,{i,j},{p,q}) -> false for all i != j

    enc(i,j,{j,y},{p,q}) -> false for i != 0 and all j, y
    enc(i,j,{x,j},{p,q}) -> false for i != 0 and all j, x


  Via euler-amo, for all {p,q}:

    enc(i,j,{x,x},{p,q}) -> false for all i != 0, and all j, x

  This yields by the euler-definitions (now treated as additional constraints):

    not enc(i,j,x,p) or not enc(i,j,x,q) for all i,j != 0, and all x != j,
                                             where in case p=0 also x != i.


  Via the euler-definitions, for p=0:

    enc(i,0,{i,y},{0,q}) = enc(i,0,y,q) for all i != 0 and all y != 0
    enc(i,0,{x,y},{0,q}) -> false for all i != 0 and x != i

    enc(i,j,{i,y},{0,q}) -> false for all i,j != 0 and all y.


So the new ls-variables enc(i,j,eps,p) (the remaining ones) for N >= 3 are:

  p = 0:
    i, j != 0,
    eps != i, j

  p >= 1:
    i != 0,
    eps != j.

That yields now

  nls = (N-1) * (k * N^2 - k * N - 2 N + 3)

  (N-1) rows, with
    +kN^2, for N columns, N values, k squares initially,
    -kN since each field has at least one variable missing,
    -(N-1) since each field outside the diagonal has another variable missing
    -(N-2) for the first column, p=0 (two variables already counted).

  nls = kN^3 - 2(k+1)N^2 + (k+5)N - 3

And the new es-variables enc(i,j,(x,y),(p,q)) are:

  i != 0
  x, y != j
  x != y

In case of p = 0 we additionally have:

  x != i
  j = 0: enc(i,0,(x,y),(0,q)) only for x=i, where it becomes
         enc(i,0,(i,y),(0,q)) = enc(i,0,y,q) (substitution);
         so all variables for j=0 are dispensed, but in the constraints
         involving enc(i,0,(i,y),(0,q)) use enc(i,0,y,q) instead.

That makes

  npes = 1/2 (k-1)(N-2)(N-1) (kN^2 - k*N - 4*N + 6).

As before, we can use

  naux ~ 1/2 npes

since the auxiliary variables come from a scheme, which due to being roughly
size-linear, can be considered size-wise as a big amo over the primary
es-variables, where the amo-seco-scheme uses roughly m/2 auxiliary variables
for an input of m literals.

Also as before, we can use

  ces ~ 6 npes

since the main bulk is given by the 3 clauses for the euler-equivalences
per primary es-variable, and, as above, by the aggregated amo over these
variables, which make again 3*npes clauses.


The second option determining the clauses of the translation has the possibilities
 - L0  : none
 - Lv  : values
 - Lp  : pairs
 - Lb  : values_pairs ("both")
 - Lpu : pairs_uep
 - Lbu : values_pairs_uep

This concerns ALO ("at-least-one") in two forms for the es-variables:
 - "values": each field (i,j) of the euler-square has one of the N^2 values
             {x,y}
 - "pairs": each pair {x,y} is used in some field (i,j) of the euler-square.
This forms thus add N^2 many long clauses, with N^2 literals for the full
form (for the reduced form roughly "N" is replaced by "N-1").

"uep" means "unique extension-property", and strengthens euler-amo by adding
a clause per auxiliary variable v: the amo-translation (alone) uses
implications
  a or b or c -> v
(which yields 3 clauses), which has not uep in case a=b=c=0 (then v can be
0 or 1), and so "uep" adds the reverse
  v -> a or b or c.
"Lpu" uses then the final auxiliary variables v to express ALO (which yields
a clause of length 4 instead of N^2).
"Lbu" additionally to Lpu uses Lv (so we get again the long clauses).

The estimation ces ~ 6 npes has to be updated for the uep-forms, since
here we get 0.5 * npes many additional clauses, and thus here we use

  ces ~ 6.5 npes.


Examples:

Maximum range is 1 <= N <= 11.


A) Single latin squares

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


B) Pairs of MOLS

https://oeis.org/A072377
Number of pairs of orthogonal Latin squares of order n.
Multiplied with 2 (except for N=1):
1, 0, 72, 6912, 6220800,
0, 6263668776960000, 64324116731941355520000, 38166908141096565278370693120000
Use
> ./Mols N 2 f


https://oeis.org/A266166
Number of reduced pairs of orthogonal Latin squares.
1, 0, 1, 2, 18,
0, 342480, 7850589120, 7188534981260640
Use
> ./Mols N 2 r


C) Triples of MOLS

OEIS only contains
https://oeis.org/A266172
Number of species of triples of orthogonal Latin squares
1, 0, 0, 1, 1,
0, 1, 39, 371

Number of triples of orthogonal latin squares of order N:
Use
> ./Mols N 3 f
N<= 3: 0
N=4:   165888 (ctawSolver)
N=5:   1492992000 (computed from reduced solution below)
N=6:   0
Use
> ./Mols N 3 f


Number of reduced triples of orthogonal latin squares of order N
(obtain the above full numbers by multiplication with (N!)^3*(N-1)!):
N<= 3: 0
N=4:   2 (ctawSolver)
N=5:   36 (ctawSolver)
N=6:   0 (minisat)
Use
> .Mols N 3 r


D) Quadruples of MOLS

OEIS only contains
https://oeis.org/A266173
Number of species of sets of four orthogonal Latin squares
1, 0, 0, 0, 1,
0, 1, 1, 96


Number of quadruples of orthogonal latin squares of order N:
Use
> ./Mols N 4 f
N<= 4: 0
N=5:   179159040000 (computed from reduced solution below)
N=6:   0
Use
> ./Mols N 4 f


Number of reduced quadruples of orthogonal latin squares of order N
(obtain the above full numbers by multiplication with (N!)^4*(N-1)!):
N<= 4: 0
N=5:   36 (ctawSolver)
N=6:   0 (minisat)
Use
> ./Mols N 5 r


TODOS:

1. A minimal representation (for local search)

 - The euler-equivalences can be just implications: we have even uep due to
   euler-amo.
 - For the latin-squares we don't need eo three times, but it suffices
   to have here alo for the fields, while for rows/columns only amo (if a field
   has two values, then for columns and rows amo is violated).

2. Eliminating the primary es-variables (euler-equivalences)
 - The guess is that minisat-preprocessing eliminates those (and that
   seems important for minisat-solving). (Though behaviour for
   SplittingViaOKsolver for example in data/8_3_r is strange;
   hopefully this becomes clearer once this has been implemented.)
 - It seems these variables aren't powerful, at least in the current
   setting (not much is done with them).
 - We extend the "r,f"-option by "wr,wf", with "w" for "without".
 - Replaving these variables (v <-> a & b) is easy for eulo-amo.
 - But not for Lv, and for Lp only with Lpu (so Lv, Lp, Lb are forbidden
   for wr,wf).

3. Move functionality to own modules
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

#include <cassert>
#include <cstdint>
#include <cstdlib>

#include <ProgramOptions/Environment.hpp>
#include "../Random/ClauseSets.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.6.3",
        "12.1.2020",
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
  };

  enum class SymP { reduced=0, full=1 };
  std::ostream& operator <<(std::ostream& out, const SymP opt) {
    switch (opt) {
    case SymP::reduced: return out << "reduced";
    case SymP::full: return out << "full";
    default : return out << "SymP::unknown";}
  }
  enum class EAloP { none=0, val=1, pair=2, pairuep=3, both=4, bothuep=5 };
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
    static constexpr int size = int(EAloP::bothuep) + 1;
    static constexpr std::array<const char*, size> string
      {"L0", "Lv", "Lp", "Lpu", "Lb", "Lbu"};
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
          nls, // total number of variables for all single latin-squares
          nbes1, nbes2, // number of primary variables for single euler-square,
                        // for p=0 and p>=1
          npes, // total number of primary variables for all euler-squares
          n0, // number of all primary variables
          naux, // number of all auxiliary variables (amo/eo for euler-squares)
          n; // number of variables in total
    var_t cls, ces, c;
  };
  struct fNumVarsCls {
    FloatingPoint::float80 nbls1, nbls2, nls, nbes1, nbes2, npes, n0, naux, n, cls, ces, c;
    constexpr bool valid() const noexcept {
      return FloatingPoint::isUInt({nbls1, nbls2, nls, nbes1, nbes2, npes, n0, naux, n, cls, ces, c});
    }
    constexpr operator NumVarsCls() const noexcept {
      assert(valid());
      return {var_t(nbls1), var_t(nbls2), var_t(nls), var_t(nbes1), var_t(nbes2), var_t(npes), var_t(n0), var_t(naux), var_t(n), var_t(cls), var_t(ces), var_t(c)};
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


  constexpr NumVarsCls numvarscls(const Param p, const SymP symopt, const EAloP ealoopt) noexcept {
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
      r.naux = fbinomial_coeff(p.k, 2) * N2 * n_amo_seco(N2);
      r.n = r.n0 + r.naux;
      if (r.n >= FloatingPoint::P264) {
        std::cerr << error << "Parameters " << p << " yield total number of variables >= 2^64.\n";
        std::exit(int(Error::too_big));
      }

      const auto cbls = 3 * N2 * pars_eo_primes(N).c;
      r.cls = cbls * k;
      const auto cbes = 3*N4 +
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
      r.nbls2 = (N-1)*N*(N-1);
      r.nls = r.nbls1 + r.nbls2 * (k - 1);
      r.nbes1 = (N-1)*(N-2)*(N-2)*(N-2) + (N-1)*(N-1)*(N-2);
      r.nbes2 = (N-1)*(N-1)*N*(N-2);
      r.npes = r.nbes1 * (k-1) + r.nbes2 * fbinomial_coeff(vk-1, 2);
      r.n0 = r.nls + r.npes;
      const auto amoruns_x0 = N-1;
      const auto amoruns_y0 = N-1;
      const auto amoruns_else = N2 - N - amoruns_x0 - amoruns_y0;
      r.naux = (k - 1) *
                (amoruns_x0 * n_amo_seco((N-2)*(N-1)) +
                 amoruns_y0 * n_amo_seco((N-2)*(N-2)) +
                 amoruns_else * n_amo_seco((N-3)*(N-2) + 1));
      r.naux += fbinomial_coeff(vk-1, 2) * (N2 - N) * n_amo_seco((N-2)*(N-1));
      r.n = r.n0 + r.naux;
      if (r.n >= FloatingPoint::P264) {
        std::cerr << error << "Parameters " << p << " yield total number of variables >= 2^64.\n";
        std::exit(int(Error::too_big));
      }

      const auto peop1 = pars_eo_primes(N-1).c;
      const auto peop2 = pars_eo_primes(N-2).c;
      const auto cbls1 = ((N-1)*(N-1) - (N-1)) * peop2 + (N-1) * peop1 +
                         2 * (N-1) * ((N-2) * peop2 + peop1);
      const auto cbls2 = 3 * (N-1) * N * peop1;
      r.cls = cbls1 + cbls2 * (k - 1);
      const auto cdefs = 3 * r.npes + (k-1)*((N-1)*(N-2)*(N-2) + (N-1)*(N-1)) + fbinomial_coeff(vk-1, 2) * (N-1)*(N-1)*(N-1);
      const auto cbes1 = (has_val(ealoopt) ? (N-1)*(N-1) : 0) +
                         (has_pair(ealoopt) ? N*(N-1) : 0);
      const auto cbes2 = (has_val(ealoopt) ? (N-1)*N : 0) +
                         (has_pair(ealoopt) ? N*(N-1) : 0);
      r.ces = cdefs + cbes1 * (vk-1) + cbes2 * fbinomial_coeff(vk-1, 2);
      r.ces += (k - 1) *
                (amoruns_x0 * c_amo_seco((N-2)*(N-1), ealoopt) +
                 amoruns_y0 * c_amo_seco((N-2)*(N-2), ealoopt) +
                 amoruns_else * c_amo_seco((N-3)*(N-2) + 1, ealoopt));
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

    const var_t N2 = N*N;
    const var_t N3 = N2 * N;

  private :
    mutable var_t next = nvc.n0;
  public :

    constexpr Encoding(const Param ps, const SymP s, const EAloP e = EAloP::none) noexcept : N(ps.N), k(ps.k), nvc(numvarscls(ps,s,e)), symopt(s), ealoopt(e) {}

    void nls(std::ostream& out) const {
      if (symopt == SymP::full)
        out << Environment::DWW{"nls=kN3"} << nvc.nls << "\n";
      else
        out << Environment::DWW{"nls=kN3-2(k+1)N2+(k+5)N-3"} << nvc.nls << "\n";
    }
    void npes(std::ostream& out) const {
      if (symopt == SymP::full)
        out << Environment::DWW{"npes=1/2k(k-1)N4"} << nvc.npes << "\n";
      else
        out << Environment::DWW{"npes=1/2k(k-1)*"} << "\n"
            << Environment::DWW{"     (N2-3N+2)(N2-(1+4/k)N+6/k)"} << nvc.npes << "\n";
    }
    void naux(std::ostream& out) const {
      out << Environment::DWW{"naux~1/2npes"} << nvc.naux << "\n";
    }
    void cls(std::ostream& out) const {
      if (symopt == SymP::full)
        out << Environment::DWW{"cls=3/2k(N3(N-1)+2N2)"} << nvc.cls << "\n";
      else
        out << Environment::DWW{"cls=3/2k(N-1)*"} << "\n"
            << Environment::DWW{"    (N3-3(1+1/k)N2+(4+11/k)N-12/k)"} << nvc.cls << "\n";
    }
    void ces(std::ostream& out) const {
      if (not has_uep(ealoopt))
        out << Environment::DWW{"ces~6npes"} << nvc.ces << "\n";
      else
        out << Environment::DWW{"ces~6.5npes"} << nvc.ces << "\n";
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
          const var_t n_prev_ls = nvc.nbls1 + (p-1) * nvc.nbls2;
          const var_t n_prev_lines = (i-1) * N*(N-1);
          const var_t n_prev_cells = j * (N-1);
          const var_t v = 1 + n_prev_ls + n_prev_lines + n_prev_cells + eps_adj(j,eps) ;
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
          const var_t n_prev_es = q * nvc.nbes1 + (index(pq)-q) * nvc.nbes2;
          const var_t n_prev_lines = (i-1) * N * (N-1) * (N-2);
          const var_t n_prev_cells = j * (N-1) * (N-2);
          const var_t v = 1+nvc.nls + n_prev_es + n_prev_lines + n_prev_cells + index_adj(j, eps);
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

  // The disjunction over B implies w:
  inline void disj_impl(std::ostream& out, const Clause& B, const Lit w) {
    for (const Lit x : B) out << Clause{-x, w};
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
    out << -w << " ";
    out << B;
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
      out << C;
    }
    else {
      assert(enc.ealoopt == EAloP::pairuep or enc.ealoopt == EAloP::bothuep);
      const var_t final_v = amouep_seco(out, C, enc);
      if (final_v == 0) {
        assert(C.size() <= 4);
        out << C;
      }
      else {
        assert(C.size() >= 5);
        const var_t missing = C.size()%2==1 ? 2 : 3;
        out << Lit{final_v,1} << " " << Clause(C.begin(), C.begin()+missing);
      }
    }
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
            Clause C; C.reserve(enc.N);
            for (dim_t eps = 0; eps < enc.N; ++eps)
              C.push_back({enc(i,j,eps,p),1});
            eo_primes(out, C);
          }
        // EO(i,-,eps,p) :
        for (dim_t i = 0; i < enc.N; ++i)
          for (dim_t eps = 0; eps < enc.N; ++eps) {
            Clause C; C.reserve(enc.N);
            for (dim_t j = 0; j < enc.N; ++j)
              C.push_back({enc(i,j,eps,p),1});
            eo_primes(out, C);
          }
        // EO(-,j,eps,p) :
        for (dim_t j = 0; j < enc.N; ++j)
          for (dim_t eps = 0; eps < enc.N; ++eps) {
            Clause C; C.reserve(enc.N);
            for (dim_t i = 0; i < enc.N; ++i)
              C.push_back({enc(i,j,eps,p),1});
            eo_primes(out, C);
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
                  out << Clause{-a, -b};
                } else
                  definition(out, {enc(i,j,{x,y},{0,q}),1}, {enc(i,j,x,0),1},
                                  {enc(i,j,y,q),1});
              }
            }
        // p >= 1:
        for (dim_t p = 1; p < q; ++p)
          for (dim_t i = 1; i < enc.N; ++i)
            for (dim_t j = 0; j < enc.N; ++j)
              for (dim_t x = 0; x < enc.N; ++x) {
                if (x == j) continue;
                for (dim_t y = 0; y < enc.N; ++y) {
                  if (y == j) continue;
                  if (y == x) {
                    if (j != 0) {
                      const Lit a{enc(i,j,x,p),1}, b{enc(i,j,y,q),1};
                      out << Clause{-a, -b};
                    }
                  } else
                    definition(out, {enc(i,j,{x,y},{p,q}),1}, {enc(i,j,x,p),1},
                                    {enc(i,j,y,q),1});
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
                out << C;
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
            out << C;
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
                  C.push_back({enc(i,j,{x,y},{p,q}),1});
                }
              }
              assert(C.size()==(var_t(enc.N)-1)*(var_t(enc.N)-2));
              out << C;
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
              if (j == x or j == y) continue;
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
        [[maybe_unused]] const auto length1 = [](const var_t N) {
          return (N-2)*(N-1);
        };
        for (dim_t p = 1; p < q; ++p)
          for (dim_t x = 0; x < enc.N; ++x)
            for (dim_t y = 0; y < enc.N; ++y) {
              if (y == x) continue;
              Clause C; C.reserve((var_t(enc.N)-1) * enc.N);
              for (dim_t j = 0; j < enc.N; ++j) {
                if (j == x or j == y) continue;
                for (dim_t i = 1; i < enc.N; ++i)
                  C.push_back({enc(i,j,{x,y},{p,q}),1});
              }
              assert(C.size() == length1(enc.N));
              if (not has_pair(enc.ealoopt)) amo_seco(out, C, enc);
              else eo_seco(out, C, enc);
            }
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
    return int(Error::ealo_par);
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
      << DHW{"Sizes"};
  enc.nls(out); enc.npes(out);
  out       << DWW{"n0=nls+npes"} << enc.nvc.n0 << "\n";
  enc.naux(out);
  out       << DWW{"n=n0+naux"} << enc.nvc.n << "\n";
  enc.cls(out); enc.ces(out);
  out       << DWW{"c=cls+ces"} << enc.nvc.c << "\n"
;

  if (filename == "-nil") return 0;
  out << dimacs_pars{enc.nvc.n, enc.nvc.c};
  ls(out, enc);
  es_defs(out, enc);
  es_values(out, enc);
  orthogonality(out, enc);
}

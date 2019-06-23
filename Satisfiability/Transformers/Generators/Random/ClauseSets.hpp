// Oliver Kullmann, 17.4.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Random clause-sets

DESIGN:

I Seeding

Accidentally using the same seeds should be avoided.

 1. So all parameters go into the seed-sequence. DONE
 2. Using always extended seed-values (64 bits). DONE
 3. DONE
    In decreasing order of generality; starting with the "type" of seeding.

    Using four values:
     - main type (e.g., constant-density clause-sets),
     - sub-type (e.g., could be the global options)
     - number of blocks
     - thread-index (default: 0).

    Called "type-seed".

 4. The user-provided seeds (arbitrarily many, including none) are appended.
     - Via "t" one gets a timestamp (as many as asked, without any further
       editing).
     - Via "r" one gets a random value via std::random_device().
 5. DONE (extended now, with syntax "c * n,k,p")
    For the standard model after the first type-seed come (in brackets
    the number of related seed-values):
     1. n (2)
     2. k (1)
     3. c (1)
     4. p (2)
    This is one block(n,k,c,p).
 6. DONE
    Should this also accommodate the extension to DQCNF? Then one also needs
    the number of quantifier blocks (1 for CNF). NO:
    And there are the dependency-specifications. Perhaps this generator is
    conceptually similar to the DQCNF, but actually the instances should be
    independent (already guaranteed by the type-seed), and thus there is no
    need to anticipate here the bigger seed-space for the DQCNF-versions.


II Blocks of parameters DONE (extended)

One block of parameter describes the construction of one clause-set.
One can have several such blocks (at least one), where the clause-sets
are appended, and the Dimacs-parameter-value reflect the whole construct
(summing up the number of clauses, and taking the maximum over the sets of
variables).

The blocks each have their parameters (n,k,c,p) as seeds.
"n", as discussed below, can be an interval.

Actually one can allow zero blocks (for the empty clause-set).



III Global options DONE

 - We have two dimensions:
  - "sorted" (with removal of duplications), stronger "sorted with rejection"
    (making sure that the numbers c of clauses are reached)
  - rename (eliminating gaps and non-occurring variables).
 - This makes 3*2=6 possibilities, realised as a pair of
   enum-classes, in class GParam.
  - In order for that to work with the system in ProgramOptions::Environment,
    one needed to wrap that pair in a class which makes it look like an
    enum-class. This means that in can be constructed from an int, and
    converted to an int.


IV Variables

For a clause-block: Either parameter n: 1, ..., n, or n1,n2: n1, ..., n2.
Represented as a pair n1, n2 (with n1=1 in the first case).

Allowed is the full range for 64-bit; literals are pairs (var,sign).
A comment specifies the smallest bit-number from 8,16,32,64,128 which
safely contains n+1 (- - (n+1) == n+1).


V Sign probabilities

A Prob64 value p, by default 1/2, for the probability of a positive literal.
The input of probabilities is always as a quotient, also for 0 = 0/1 and
1 = 1/1.

Alternatively one can use an integer s, which if >= 0 means the number
of positive literals in a clause, and if negative, then -s means the
number of negative literals in a clause:
 - Likely best to immediately translated this into the number of positive
   literals, so that the internal representation is unsigned.
 - DONE The current parameter handling in RParam needs to be generalised.
 - DONE One could use std::variant.
 - DONE For RParam::add_seeds one can represent s as the pair (0,s), which
   only for s=1 can be produced by a probability (namely p=1), in which
   case one could use (2,1).
 - One motivation here is to have the possibility to specify mixed binary
   clauses (with s=1).


VI The number of clauses DONE

One 64-bit value c.


VII The clause-length DONE

One 64-bit value k.


VIII The random-number generators DONE

Using one RandGen_t g1 for the construction of the clauses.
And one RandGen_t g2 for the signs (which uses bernoulli_low in the case
p = 1/2).
 - Is having two generators really useful? It seems to only complicate things.
 - One argument is the use of bernoulli_low: it generates a continuous stream
   of bits, and this is more efficient when not interrupted inbetween clauses.
 - Is it really worth, to use bernoulli_low?
 - For parallelisation via threads, the result will depend (in a well-defined
   way) on the number of threads, and for each thread one generator is used
   (with its own seed).


IX The CDRCLS-object

   DONE : it seems that currently just using appropriately either
     const DimacsClauseList F = random(g, par);
   in case par.gp != "u|o", and
     rand_clauselist(out, g, par.vp);
   otherwise, is good enough.

 - Class-name perhaps CDRCLS (constant-density random cls).
   Or CoDeRCLS. Or CRCNF.
 - DONE For one parameter-block we have a class RParam.
 - DONE
   While class Param contains the global parameters and a vector of RParam's.
 - DONE Besides Prob64 we introduce VarInterval, which is just a pair of
   64-bit uints (a,b) with 1 <= a <= b.
 - The constructor then takes an Param-object.
 - Accepting the empty list: the empty clause-list (0,0,0,1/2).
 - Additional parameter is the comments as strings.
 - And the seed-sequence (as 64-bit).
 - While the parameter-line of the output is computed by the class.
 - Yet nothing constructed.
 - If one of the global options is on, then the computation creates
   a clause-list held as data-member; otherwise in memory there are only
   single clauses.
 - There is an output-function on an output-stream.
 - There is also an output-iterator available for the clauses (as sorted
   vectors of literals), creating the clauses on-the-fly.
  - Perhaps easier to just have a member-function next_clause() ?
  - If there is no further clause, then the empty clause (empty vector)
    is returned, while every created clause is non-empty.
 - Only one of the output-facilities should be chosen; how to arrange that?
   One could have a guard-variable, and throwing an exception in the
   error-case.
 - If the clause-set is available in memory, then perhaps it should be
   created at construction, and then the two output-facilities (output-stream
   and iterator) are both available. But otherwise only one of the two
   is available, and can only be used once.


X The filename of the output

It is helpful to provide a standardised filename, as default.
Otherwise one can say "-cout" for output to standard-output.

The standardised filename containing the full parameter-sequence
seems awkward here (too long).

Perhaps the two Dimacs-parameters, as specified, plus the number of
clause-blocks and the number of user-seeds. Suffixed by a timestamp.
Like

  BRG_1000_10000_3_7_1560911646161114107.dimacs

Perhaps by "-nt" one can specify "no timestamp".

The default (no argument) is "-cout", with the empty string one gets
the above default-filename, and otherwise "-nt" or the filename is
allowed.


XI The input-format

DONE The seed-sequence is given by a comma-separated list (no spaces),
with "r" for "random", and "t" for timestamp, e.g.

  44,22,r,1,t,12,t,4,r

(generating 9 extended seed-values).
The default is the empty sequence.

For the clause-blocks we require quotation, and allow optional spaces:

  "10 * 3-6,2 | 4,2,0 | 8,3,1/2 ; 20 * 1-4,2,1/3 | 8,3 | 5,2"

means two clause-blocks as follows:

{
  { { {3,6},2,1/2 }, { {1,4},2,0 }, { {1,8},3,1/2 } }, 10 },
  { { {1,4},2,1/3 }, { {1,8},3,1/2 }, { {1,5},2,1/2 } }
}

Just one block by

  "c * n,k,p"

The "|" separates the clause-parts, the ";" separates the clause-blocks.

So the whole input is e.g.

> BlockRandomGenerator r|o "400*100,3" t,1000 -cout

With the third and fourth parameters optional, and the first parameter
allowing "" for the default argument.
While the second parameter does not have a default value, or?
The current standard is always to allow no command-line parameters, with
values then used for profiling.
Perhaps it's also "" (no clause-block), so that one can see the comments.


XII Comments for the Dimacs-output

Showing
 - standard version-information
   (which includes a timestamp with information on it)
 - information on the true random-generator, as used by "r"
 - a reproduction of the command-line
 - the derived full parameter-values, including the full seed-sequence.


XIII For building the program one has to allow also building outside of
the context of the OKlibrary. Then the Git-id is just hardcoded.


*/

#ifndef CLAUSESETS_UUIoaKXj2K
#define CLAUSESETS_UUIoaKXj2K

#include <stdexcept>
#include <utility>
#include <variant>
#include <ostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

#include <ProgramOptions/Environment.hpp>

#include "Distributions.hpp"
#include "Algorithms.hpp"

namespace RandGen {

  /* ***********************
     * Handling parameters *
     ***********************
  */

  class VarInterval {
    gen_uint_t a_, b_;
  public :
    constexpr gen_uint_t a() const noexcept { return a_; }
    constexpr gen_uint_t b() const noexcept { return b_; }

    constexpr VarInterval(const gen_uint_t n) : VarInterval(1,n) {
      if (n == 0) throw std::domain_error("VarInterval(gen_uint_t): n = 0");
    }
    constexpr VarInterval(const int n) : VarInterval(1,n) {
      if (n < 0) throw std::domain_error("VarInterval(int): n < 0");
    }
    VarInterval(FloatingPoint::float80) = delete;
    VarInterval(double) = delete;
    VarInterval(float) = delete;

    constexpr VarInterval(const gen_uint_t a, gen_uint_t b) : a_(a), b_(b) {
      if (a > b)
        throw std::domain_error("VarInterval(gen_uint_t,gen_uint_t): a > b");
      if (a == 0)
        throw std::domain_error("VarInterval(gen_uint_t,gen_uint_t): a = 0");
    }
    explicit constexpr VarInterval(const pair64 p) : VarInterval(p.first, p.second) {}

    explicit VarInterval(const std::string_view s) : VarInterval(s2p(s)) {}

    explicit constexpr operator pair64() const noexcept { return {a_,b_}; }

    constexpr gen_uint_t size() const noexcept { return (b_ - a_) + 1; }

    constexpr bool element(const gen_uint_t x) const noexcept {
      return x >= a_ and x <= b_;
    }
    constexpr gen_uint_t operator[](const gen_uint_t i) const noexcept {
      assert(i < size());
      return a_ + i;
    }
    // Assuming that g(n) creates a random number in {0, ..., n-1}:
    template <class RG>
    gen_uint_t random_element(RG&& g) const noexcept { return a_ + g(size()); }

    friend constexpr bool operator ==(const VarInterval lhs, const VarInterval rhs) noexcept {
      return lhs.a_ == rhs.a_ and lhs.b_ == rhs.b_;
    }
    friend constexpr bool operator !=(const VarInterval lhs, const VarInterval rhs) noexcept {
      return not(lhs == rhs);
    }
    friend std::ostream& operator <<(std::ostream& out, const VarInterval n) {
      return out << n.a_ << "-" << n.b_;
    }

    static pair64 s2p(const std::string_view s) {
      const auto parts = Environment::split(s,'-');
      if (parts.empty()) throw std::domain_error("RandGen::VarIntervall(string_view): empty");
      const auto size = parts.size();
      if (size > 2) throw std::domain_error("RandGen::VarIntervall(string_view): size = " + std::to_string(size));
      if (size == 1) return {1,to_gen_uint_t(parts[0], false)};
      return {to_gen_uint_t(parts[0],false), to_gen_uint_t(parts[1],false)};
    }
  };
  static_assert(VarInterval(1,2).a() == 1 and VarInterval(1,2).b() == 2);
  static_assert(VarInterval(10) == VarInterval(1,10));
  static_assert(VarInterval(5,8) == VarInterval(pair64{5,8}));
  static_assert(pair64(VarInterval(11)) == pair64(1,11));
  static_assert(VarInterval(5,5).size() == 1);
  static_assert(VarInterval(3,10).size() == 8);
  static_assert(VarInterval(5,5).element(5));
  static_assert(not VarInterval(5,6).element(4));
  static_assert(VarInterval(77,78)[0] == 77);
  static_assert(VarInterval(100,110)[10] == 110);


  // The probability of a positive sign, or the number of positive literals
  // in a clause:
  typedef std::variant<Prob64, gen_uint_t> SignDist;
  std::ostream& operator <<(std::ostream& out, const SignDist s) {
    if (s.index() == 0) return out << std::get<0>(s);
    else return out << std::get<1>(s);
  }

  // The parameters of a clause-part:
  struct ClausePart {
    const VarInterval n;
    const gen_uint_t k;
    const SignDist p{Prob64{1,2}};
  };
  constexpr bool operator ==(const ClausePart& lhs, const ClausePart& rhs) noexcept {
    return lhs.n == rhs.n and lhs.k == rhs.k and lhs.p == rhs.p;
  }
  constexpr bool operator !=(const ClausePart& lhs, const ClausePart& rhs) noexcept {
    return not(lhs == rhs);
  }
  static_assert((ClausePart{10,3,Prob64{0,1}} != ClausePart{10,3,0}));
  constexpr bool valid(const ClausePart& rp) noexcept {
    return (rp.k <= rp.n.size()) and
      (rp.p.index() == 0 or std::get<1>(rp.p) <= rp.k);
  }
  static_assert(not valid({{3,5},4}));
  static_assert(valid({{3,6},4}));
  static_assert(valid({{3,6},4,Prob64{0,1}}));
  static_assert(not valid({{3,6},4,5}));
  static_assert(valid({{3,6},4,4}));

  std::ostream& operator <<(std::ostream& out, const ClausePart& cp) {
    return out << "{" << cp.n << "," << cp.k << "," << cp.p << "}";
  }

  typedef std::vector<ClausePart> clausepart_v;
  gen_uint_t size(const clausepart_v& v) noexcept {
    gen_uint_t sum = 0;
    for (const ClausePart& cp : v) sum += cp.k;
    return sum;
  }
  std::ostream& operator <<(std::ostream& out, const clausepart_v& cps) {
    assert(not cps.empty());
    out << cps.front();
    for (clausepart_v::size_type i = 1; i < cps.size(); ++i)
      out << " | " << cps[i];
    return out;
  }

  // The parameters of a clause-block:
  struct RParam {
    const clausepart_v cps;
    const gen_uint_t c;
  };
  inline bool operator ==(const RParam& lhs, const RParam& rhs) noexcept {
    return lhs.cps == rhs.cps and lhs.c == rhs.c;
  }
  inline bool operator !=(const RParam& lhs, const RParam& rhs) noexcept {
    return not(lhs == rhs);
  }
  bool valid(const RParam& rp) noexcept {
    if (rp.cps.empty()) return false;
    for (const ClausePart& cp : rp.cps)
      if (not valid(cp)) return false;
    return true;
  }
  std::ostream& operator <<(std::ostream& out, const RParam& rpar) {
    return out << rpar.c << " * " << rpar.cps;
  }

  typedef std::vector<RParam> rparam_v;
  bool valid(const rparam_v& pv) noexcept {
    for (const auto& rp : pv) if (not valid(rp)) return false;
    return true;
  }
  std::ostream& operator <<(std::ostream& out, const rparam_v& parv) {
    if (parv.empty()) return out << "empty";
    out << parv.front();
    for (rparam_v::size_type i = 1; i < parv.size(); ++i)
      out << " ; " << parv[i];
    return out;
  }


  typedef std::pair<gen_uint_t, gen_uint_t> dimacs_pars;
  inline dimacs_pars extract_parameters(const rparam_v par) noexcept {
    gen_uint_t n = 0, c = 0;
    for (const RParam& pa : par) {
      c += pa.c;
      for (const ClausePart& cp : pa.cps)
        n = std::max(n, cp.n.b());
    }
    return {n,c};
  }
  std::ostream& operator <<(std::ostream& out, const dimacs_pars pa) {
    return out << "p cnf " << pa.first << " " << pa.second << "\n";
  }

  rparam_v read_rparam_v(std::string s) {
    Environment::remove_spaces(s);
    if (s.empty()) return {};
    const auto clause_blocks = Environment::split(s, ';');
    const auto num_blocks = clause_blocks.size();
    rparam_v result; result.reserve(num_blocks);
    for (const std::string& clause : clause_blocks) {
      const auto two_parts = Environment::split(clause, '*');
      if (two_parts.size() != 2) throw 0;
      const unsigned long long cll = std::stoull(two_parts[0]);
      if (cll > randgen_max) throw 1;
      const gen_uint_t c = cll;
      const auto clause_parts = Environment::split(two_parts[1], '|');
      if (clause_parts.empty()) throw 2;
      clausepart_v cps; cps.reserve(clause_parts.size());
      for (const std::string& cp : clause_parts) {
        const auto par = Environment::split(cp, ',');
        const auto size = par.size();
        if (size < 2) throw 3;
        if (size > 3) throw 4;
        const VarInterval n{par[0]};
        const gen_uint_t k = to_gen_uint_t(par[1],false);
        if (size == 2) cps.push_back({n,k});
        else {
          const auto p{toProb64(par[2])};
          if (p) cps.push_back({n,k,p.value()});
          else cps.push_back({n,k,to_gen_uint_t(par[2],false)});
        }
      }
      result.push_back({std::move(cps),c});
  }
   return result;
  }


  // The global parameters:
  enum class SortO { filtered=0, sorted=1, unsorted=2 }; // f, s, u
  enum class RenameO { renamed=0, maxindex=1, original=2 }; // r, m, o

  typedef std::tuple<SortO,RenameO> option_t;
  constexpr char sep = ',';

  // Packing both global parameters, providing index-access (with SortO
  // running fast):
  struct GParam {
    SortO s_;
    RenameO r_;
    constexpr static int size_s = 3;
    constexpr static int size_r = 3;
    constexpr static int size = size_s * size_r;
    constexpr SortO s() const noexcept { return s_; }
    constexpr RenameO r() const noexcept { return r_; }

    constexpr GParam() noexcept : s_(SortO::filtered), r_(RenameO::renamed) {}
    constexpr GParam(const SortO s, const RenameO r) noexcept : s_(s), r_(r) {}
    constexpr GParam(const option_t o) noexcept : s_(std::get<0>(o)), r_(std::get<1>(o)) {}
    explicit constexpr GParam(const int i) : s_(SortO(i % size_s)), r_(RenameO(i / size_s)) {
      if (i < 0) throw std::domain_error("GParam(int): i < 0");
      if (i >= size) throw std::domain_error("GParam(int): i >= size");
    }

    explicit constexpr operator int() const noexcept {
      return int(s_) + int(r_) * size_s;
    }

    typedef std::pair<SortO,RenameO> pair_t;
    constexpr operator pair_t() const noexcept { return {s_,r_}; }

  };
  constexpr bool operator ==(const GParam lhs, const GParam rhs) noexcept {
    return lhs.s_ == rhs.s_ and lhs.r_ == rhs.r_;
  }
  constexpr bool operator !=(const GParam lhs, const GParam rhs) noexcept {
    return not(lhs == rhs);
  }

  static_assert(GParam::size == 3*3);
  constexpr bool check_GParam() noexcept {
    for (int i = 0; i < GParam::size; ++i)
      if (int(GParam(i)) != i) return false;
    return true;
  }
  static_assert(check_GParam());
  static_assert(GParam() == GParam(SortO::filtered, RenameO::renamed));
  static_assert(GParam(0) == GParam());
}

namespace Environment {
  template <>
  struct RegistrationPolicies<RandGen::SortO> {
    static constexpr int size = RandGen::GParam::size_s;
    static constexpr std::array<const char*, size> string
      {"f", "s", "u"};
  };
  template <>
  struct RegistrationPolicies<RandGen::RenameO> {
    static constexpr int size = RandGen::GParam::size_r;
    static constexpr std::array<const char*, size> string
      {"r", "m", "o"};
  };
}

namespace RandGen {

  std::ostream& operator <<(std::ostream& out, const SortO s) {
    switch (s) {
    case SortO::unsorted : return out << "unsorted";
    case SortO::sorted : return out << "sorted";
    default : return out << "filtered";}
  }
  std::ostream& operator <<(std::ostream& out, const RenameO r) {
    switch (r) {
    case RenameO::original : return out << "original";
    case RenameO::maxindex : return out << "maxindex";
    default : return out << "renamed";}
  }
  std::ostream& operator <<(std::ostream& out, const GParam p) {
    return out << p.s_ << sep << p.r_;
  }


  // Packinging all parameters:
  struct Param {
    GParam gp;
    rparam_v vp;

    Param(const GParam gp, const rparam_v& v) : gp(gp), vp(v) {}
    Param(const GParam gp, rparam_v&& v) noexcept : gp(gp), vp(v) {}
  };


  /* ***********************
     * Computing the seeds *
     ***********************
  */

  enum class MainType : gen_uint_t {
    block_uniform_cnf = 0,
    block_uniform_dqcnf_dimacs = 1,
    block_uniform_dqcnf_qdimacs = 2,
    block_uniform_dqcnf_dqdimacs = 3
  };

  const unsigned int default_thread_index = 0;

  const gen_uint_t size_type_eseed = 4;
  const gen_uint_t size_cpart_eseed = 2 + 1 + 2;

  // Compute the seeds for for clause-parameter-block, and add to v:
  gen_uint_t add_seeds(const RParam& par, vec_eseed_t& v) {
    const gen_uint_t add_seeds = 1 + 1 + par.cps.size() * size_cpart_eseed;
    const gen_uint_t curr_size = v.size();
    v.reserve(curr_size + add_seeds);
    v.push_back(par.c);
    v.push_back(par.cps.size());
    for (const ClausePart& cp : par.cps) {
      {const pair64 n_(cp.n);
       v.push_back(n_.first); v.push_back(n_.second);}
      v.push_back(cp.k);
      if (cp.p.index() == 0) {
        const pair64 p_{std::get<0>(cp.p)};
        v.push_back(p_.first); v.push_back(p_.second);
      }
      else {
        const gen_uint_t s{std::get<1>(cp.p)};
        v.push_back(s==1 ? 2 : 0); v.push_back(s);
      }
    }
    assert(v.size() == curr_size + add_seeds);
    return add_seeds;
  }

  // The complete seed-sequence corresponding to the parameters:
  vec_eseed_t seeds(const Param& par) {
    vec_eseed_t v; v.reserve(size_type_eseed);

    v.push_back(gen_uint_t(MainType::block_uniform_cnf));
    v.push_back(gen_uint_t(int(par.gp)));
    v.push_back(par.vp.size());
    v.push_back(default_thread_index);
    assert(v.size() == size_type_eseed);

    for (const auto p : par.vp) add_seeds(p,v);
    return v;
  }

  // Adding the seeds from the command-line:
  gen_uint_t add_seeds(const char* const argv[], const int begin, const int end, vec_eseed_t& v) {
    const int size = end - begin;
    v.reserve(v.size() + size);
    for (int i = begin; i < end; ++i) v.push_back(to_eseed(argv[i]));
    return size;
  }


  /* ********************************
     * Variables, literals, clauses *
     ********************************
  */

  struct Var {
    gen_uint_t v;
  };
  struct Lit {
    Var v;
    signed char sign;
  };
  inline constexpr bool valid(const Var v) noexcept { return v.v >= 1; }
  inline constexpr bool valid(const Lit x) noexcept {
    return valid(x.v) and (x.sign == -1 or x.sign == +1);
  }
  static_assert(not valid(Var{0}));
  static_assert(valid(Var{1}));
  static_assert(not valid(Lit{1,0}));
  static_assert(valid(Lit{1,-1}));
  static_assert(valid(Lit{1,1}));

  inline constexpr bool operator ==(const Var v, const Var w) noexcept {
    return v.v == w.v;
  }
  inline constexpr bool operator !=(const Var v, const Var w) noexcept {
    return not(v == w);
  }
  inline constexpr bool operator ==(const Lit x, const Lit y) noexcept {
    return x.v == y.v and x.sign == y.sign;
  }
  inline constexpr bool operator !=(const Lit x, const Lit y) noexcept {
    return not (x == y);
  }
  inline constexpr bool operator <(const Lit x, const Lit y) noexcept {
    return (x.v.v < y.v.v) or (x.v.v == y.v.v and x.sign < y.sign);
  }
  static_assert(Lit{0,-2} < Lit{0,-1});
  static_assert(Lit{1,1} < Lit{2,-1});

  std::ostream& operator <<(std::ostream& out, const Lit x) {
    if (x.sign == -1) out << "-";
    return out << x.v.v;
  }

  typedef std::vector<Lit> Clause;
  std::ostream& operator <<(std::ostream& out, const Clause& C) {
    for (const Lit x : C) out << x << " ";
    return out << "0\n";
  }
  inline bool operator <(const Clause& C, const Clause& D) noexcept {
    return std::lexicographical_compare(C.rbegin(), C.rend(), D.rbegin(), D.rend());
  }

  template <class CLS>
  gen_uint_t max_var_index(const CLS& F, const bool sorted = false) noexcept {
    if (F.empty()) return 0;
    if (sorted) {
      const Clause& C = *F.rbegin();
      assert(not C.empty());
      return C.back().v.v;
    }
    else {
      gen_uint_t max = 0;
      for (const Clause& C : F) {
        assert(not C.empty());
        max = std::max(max, C.back().v.v);
      }
      return max;
    }
  }
  template <class CLS>
  gen_uint_t rename_clauselist(CLS& F, const bool sorted = false) {
    const gen_uint_t max = max_var_index(F, sorted);
    assert(max + 1 != 0);
    std::vector<gen_uint_t> indices(max+1);
    for (const Clause& C : F)
      for (const Lit x : C)
        indices[x.v.v] = 1;
    gen_uint_t next_index = 0;
    for (gen_uint_t i = 1; i <= max; ++i)
      if (indices[i] == 1) indices[i] = ++next_index;
    for (Clause& C : F)
      for (Lit& x : C)
        x.v.v = indices[x.v.v];
    return next_index;
  }

  typedef std::vector<Clause> ClauseList;
  std::ostream& operator <<(std::ostream& out, const ClauseList& F) {
    for (const Clause& C : F) out << C;
    return out;
  }
  typedef std::set<Clause> ClauseSet;
  std::ostream& operator <<(std::ostream& out, const ClauseSet& F) {
    for (const Clause& C : F) out << C;
    return out;
  }

  typedef std::pair<dimacs_pars, ClauseList> DimacsClauseList;
  typedef std::pair<dimacs_pars, ClauseSet> DimacsClauseSet;
  template <class CLS>
  std::ostream& operator <<(std::ostream& out, const std::pair<dimacs_pars, CLS>& F) {
    return out << F.first << F.second;
  }

  struct DimacsComments {
    typedef std::vector<std::string> comments_v;
    comments_v v;
  };
  std::ostream& operator <<(std::ostream& out, const DimacsComments& com) {
    for (const std::string& s : com.v) out << "c " << s << "\n";
    return out;
  }
  template <class DCLS>
  std::ostream& operator <<(std::ostream& out, const std::pair<DimacsComments, DCLS>& D) {
    return out << D.first << D.second;
  }


  /* *************************
     * The random generation *
     *************************
  */

  // Append a sorted random clause with k literals over the variables from n
  // to the given clause C, with sign-distribution given by p; ignoring
  // the possibility of clashes or duplications w.r.t. the given clauses in C:
  inline void rand_clause(RandGen_t& g, Clause& C, const VarInterval n, const gen_uint_t k, const SignDist p) {
    assert(k >= 1);
    assert(k <= n.size());
    const auto varvec = choose_kn(k, n.size(), g, true);
    assert(varvec.size() == k);
    if (p.index() == 0) {
      const Prob64 p0 = std::get<0>(p);
      if (p0 == Prob64{1,2})
        for (const auto i : varvec)
          C.push_back({n[i], bool2schar(bernoulli_high(g))});
      else {
        Bernoulli b(g, p0);
        for (const auto i : varvec)
          C.push_back({n[i], bool2schar(b())});
      }
    }
    else {
      const gen_uint_t s = std::get<1>(p);
      assert(s <= k);
      if (s == k)
        for (const auto i : varvec) C.push_back({n[i], 1});
      else if (s == 0)
        for (const auto i : varvec) C.push_back({n[i], -1});
      else {
        std::vector<signed char> signs(k, -1);
        {const auto posvec = choose_kn(s, k, g, true);
         assert(posvec.size() == s);
         for (const auto i : posvec) signs[i] = 1;
        }
        for (gen_uint_t i = 0; i < k; ++i)
          C.push_back({n[varvec[i]], signs[i]});
      }
    }
  }

  // Output the created clauses directly on out:
  void rand_clauselist(std::ostream& out, RandGen_t& g, const rparam_v& par) {
    {const auto dp = extract_parameters(par);
     out << dp; if (dp.second == 0) return;}
    for (const RParam& pa : par)
      for (gen_uint_t i = 0; i < pa.c; ++i) {
        Clause C; C.reserve(size(pa.cps));
        for (const ClausePart& cp : pa.cps)
          rand_clause(g, C, cp.n, cp.k, cp.p);
        out << C;
      }
  }

  // Similar to rand_clauselist, but output into a clause-list, and handling
  // renaming-policies:
  DimacsClauseList rand_clauselist(RandGen_t& g, const rparam_v& par, const RenameO r = RenameO::original) {
    if (par.empty()) return {{0,0},{}};
    ClauseList F;
    const auto [n,c] = extract_parameters(par);
    F.reserve(c);
    for (const RParam& pa : par)
      for (gen_uint_t i = 0; i < pa.c; ++i) {
        Clause C; C.reserve(size(pa.cps));
        for (const ClausePart& cp : pa.cps)
          rand_clause(g, C, cp.n, cp.k, cp.p);
        F.push_back(std::move(C));
      }
    assert(F.size() == c);
    switch (r) {
    case RenameO::original : return {{n,c}, F};
    case RenameO::maxindex : return {{max_var_index(F),c}, F};
    default : const gen_uint_t max = rename_clauselist(F);
              return {{max,c}, F}; }
  }

  // Similar to rand_clauselist, but sort the result, and remove duplicated
  // clauses:
  DimacsClauseList rand_sortedclauselist(RandGen_t& g, const rparam_v& par, const RenameO r = RenameO::original) {
    if (par.empty()) return {{0,0},{}};
    ClauseList F;
    const auto [n,c] = extract_parameters(par);
    F.reserve(c);
    for (const RParam& pa : par)
      for (gen_uint_t i = 0; i < pa.c; ++i) {
        Clause C; C.reserve(size(pa.cps));
        for (const ClausePart& cp : pa.cps)
          rand_clause(g, C, cp.n, cp.k, cp.p);
        std::sort(C.begin(), C.end());
        F.push_back(std::move(C));
      }
    assert(F.size() == c);
    std::sort(F.begin(), F.end());
    F.erase(std::unique(F.begin(), F.end()), F.end());
    switch (r) {
    case RenameO::original : return {{n,F.size()}, F};
    case RenameO::maxindex : return {{max_var_index(F,true),F.size()}, F};
    default : const gen_uint_t max = rename_clauselist(F,true);
              return {{max,c}, F}; }
  }

  // Similar to rand_sortedclauselist, but now reject duplicated clauses
  // directly after creation, and thus the clause-set has actually the
  // number of clauses as given:
  DimacsClauseList rand_clauseset(RandGen_t& g, const rparam_v& par, const RenameO r = RenameO::original) {
    if (par.empty()) return {{0,0},{}};
    ClauseSet F;
    const auto [n,c] = extract_parameters(par);
    for (const RParam& pa : par)
      for (gen_uint_t i = 0; i < pa.c; ++i) {
        Clause C; C.reserve(size(pa.cps));
        do {
          C.clear();
          for (const ClausePart& cp : pa.cps)
            rand_clause(g, C, cp.n, cp.k, cp.p);
          std::sort(C.begin(), C.end());
        } while (not F.insert(std::move(C)).second);
      }
    assert(F.size() == c);
    ClauseList F2;
    for (auto it = F.begin(); it != F.end(); )
      F2.push_back(std::move(F.extract(it++).value()));
    assert(F.empty() and F2.size() == c);
    switch (r) {
    case RenameO::original : return {{n,c}, F2};
    case RenameO::maxindex : return {{max_var_index(F2,true),c}, F2};
    default : const gen_uint_t max = rename_clauselist(F2,true);
              return {{max,c}, F2}; }
  }

  DimacsClauseList random(RandGen_t& g, const Param& par) {
    const auto [spar, rpar] = par.gp;
    switch (spar) {
    case SortO::unsorted : return rand_clauselist(g, par.vp, rpar);
    case SortO::sorted : return rand_sortedclauselist(g, par.vp, rpar);
    default : return rand_clauseset(g, par.vp, rpar); }
  }

}

#endif

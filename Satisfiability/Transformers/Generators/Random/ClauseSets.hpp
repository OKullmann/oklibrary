// Oliver Kullmann, 17.4.2019 (Swansea)
/* Copyright 2019, 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Components for random clause-sets

 - Handling parameters:
  - class VarInterval
  - typedef SignDist
  - struct ClausePart
  - clausepart_v as vector of ClausePart's
  - struct RParam for the parameters of a clause-block
  - rparam_v as vector of RParam's
  - typedef dimacs_pars for the two Dimacs-parameters
  - function extract_parameters(rparam_v) to compute the formal
    Dimacs-parameters
  - function read_rparam_v(string)
  - scoped enum SortO, RenameO for the two option-types
  - struct GParam, containing these two options
  - struct Param for all parameters.

 - Computing the seeds:
  - scoped enum MainType for the gen_uint_t-seeds for the major types of
    generators
  - function add_seeds for adding seeds according to one RParam-value, to an
    extended-seeds vector
  - seeds(Param) for computing the complete sequence of seeds encoding the
    parameter-values
  - function add_seeds for adding the user-specified seeds from the
    command-line.

 - Variables, literals, clauses, clause-sets:
  - structs Var, Lit
  - typedef Clause as vector of Lit's
  - typedef ClauseList as vector of Clause's
  - typedef ClauseSet as std::set of Clause's
  - function max_var_index for computing the maximal var-index in a list
    of clauses
  - function rename_clauselist for renaming the variables in a clause-list
  - typedefs DimacsClauseList, DimacsClauseSet, containing also the
    Dimacs-parameter-values.


 - The generation of random clauses:

  - rand_clause(g, C, n, k, p) for adding a random clause to a given clause C

  - rand_clauselist(out, g, par) for direct output to out, for option-value
    GParam(-1) (the completely unrestriced form)
  - rand_clauselist_core(out, g, par) does not output the Dimacs-line

  - rand_clauselist(g, par, RenameO) now computing a clause-list, and handling
    renaming
  - rand_sortedclauselist(g, par, RenameO) extends rand_clauselist(g,...) by
    sorting the output, and removing duplicated clauses
  - rand_clauseset(g, par, RenameO) for the filtered version (rejecting
    duplicated clauses right away)

  - random(g, par) selects one of the previous three functions (for
    option-values != GParam(-1)),


 - Input and output:
  - default_filestem(MainType), default_filesuffix(MainType)
  - default_dimacs(dimacs_pars)
  - default_seeds(vec_eseed_t)
  - default_filename(MainType, dimacs_pars, vec_eseed_t)
  - scoped enum Error for error-codes.

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
#include <numeric>

#include <cassert>

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
      if (n < 0) throw std::domain_error("VarInterval(int): n = " + std::to_string(n) + " < 0");
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
      if (parts.empty()) throw std::domain_error("RandGen::VarIntervall(string): empty string for VarInterval");
      const auto size = parts.size();
      if (size > 2) throw std::domain_error("RandGen::VarIntervall(string): incorrect string \"" + std::string(s) + "\" for VarInterval");
      if (size == 1) return {1,to_gen_uint_t(parts[0], false)};
      return {to_gen_uint_t(parts[0],false), to_gen_uint_t(parts[1],false)};
    }

    struct iterator {
      gen_uint_t m;
      constexpr gen_uint_t operator *() const noexcept {return m;}
      void operator ++() noexcept {++m;}
      friend constexpr bool operator !=(const iterator lhs, const iterator rhs) noexcept {
        return lhs.m != rhs.m;
      }
    };
    constexpr iterator begin() const noexcept { return {a_}; }
    constexpr iterator end() const noexcept { return {b_+1}; }

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
  static_assert(*VarInterval(5,7).begin() == 5);
  static_assert(*VarInterval(5,7).end() == 8);


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


  struct dimacs_pars {
    gen_uint_t n, c;
  };
  // Extracting the formal parameters from the clause-blocks:
  inline dimacs_pars extract_parameters(const rparam_v par) noexcept {
    gen_uint_t n = 0, c = 0;
    for (const RParam& pa : par) {
      c += pa.c;
      for (const ClausePart& cp : pa.cps)
        n = std::max(n, cp.n.b()); // ignoring that possibly pa.c == 0
    }
    return {n,c};
  }
  std::ostream& operator <<(std::ostream& out, const dimacs_pars pa) {
    return out << "p cnf " << pa.n << " " << pa.c << "\n";
  }

  rparam_v read_rparam_v(std::string s) {
    Environment::remove_spaces(s);
    if (s.empty()) return {};
    const auto clause_blocks = Environment::split(s, ';');
    const auto num_blocks = clause_blocks.size();
    rparam_v result; result.reserve(num_blocks);
    for (const std::string& clause : clause_blocks) {
      if (clause.empty()) throw std::domain_error("read_rparam_v: empty clause-description for clauses-parameter \"" + s + "\"");
      if (clause.back() == '*') throw std::domain_error("read_rparam_v: trailing \"*\" in clause-description \"" + clause + "\"");
      const auto two_parts = Environment::split(clause, '*');
      assert(not two_parts.empty());
      if (two_parts.size() == 1) throw std::domain_error("read_rparam_v: no \"*\" in clause-description \"" + clause + "\"");
      if (two_parts.size() > 2) throw std::domain_error("read_rparam_v: more than one \"*\" in clause-description \"" + clause + "\"");
      const gen_uint_t c = to_gen_uint_t(two_parts[0], false);
      const auto clause_parts = Environment::split(two_parts[1], '|');
      assert(not clause_parts.empty());
      clausepart_v cps; cps.reserve(clause_parts.size());
      for (const std::string& cp : clause_parts) {
        const auto par = Environment::split(cp, ',');
        const auto size = par.size();
        if (size < 2) throw std::domain_error("read_rparam_v: missing parameter in clause-part \"" + cp + "\"");
        if (size > 3) throw std::domain_error("read_rparam_v: too many parameters in clause-part \"" + cp + "\"");
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
    explicit constexpr GParam(const int i) :
      s_(i==-1 ? SortO::unsorted : SortO(i % size_s)),
      r_(i==-1 ? RenameO::original : RenameO(i / size_s)) {
      if (i < -1) throw std::domain_error("GParam(int): i = " + std::to_string(i) + " < -1");
      if (i >= size) throw std::domain_error("GParam(int): i = " + std::to_string(i) + " >= size");
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
  static_assert(GParam(-1) == GParam(SortO::unsorted, RenameO::original));
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
    return out << "\"" << p.s_ << sep << p.r_ << "\"";
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
    block_uniform_qcnf = 1,
    block_uniform_dqcnf = 2,
    block_uniform_dqcnf_planteda1 = 3,
    block_uniform_dqcnf_plantede1 = 4,
  };

  const unsigned int default_thread_index = 0;

  const gen_uint_t size_type_eseed = 4;
  const gen_uint_t size_cpart_eseed = 2 + 1 + 2;

  // Compute the seeds for for clause-parameter-block, and add to v, returning
  // the number of elements added to v (to the back):
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
  gen_uint_t add_seeds(const std::string_view s, vec_eseed_t& v) {
    const auto seeds = Environment::split(s, ',');
    const auto size = seeds.size();
    v.reserve(v.size() + size);
    for (const auto& x : seeds) v.push_back(to_eseed(x));
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

  inline constexpr Lit operator -(const Lit x) noexcept {
    return {x.v, x.sign==1 ? (signed char)-1 : (signed char)1};
  }
  static_assert(-Lit{0,-1} == Lit{0,1});
  static_assert(-Lit{0,1} == Lit{0,-1});
  static_assert(-Lit{0,2} == Lit{0,1});

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
      if (C.empty()) return 0;
      else return C.back().v.v;
    }
    else {
      gen_uint_t max = 0;
      for (const Clause& C : F) {
        if (not C.empty()) max = std::max(max, C.back().v.v);
      }
      return max;
    }
  }

  /*
     Renaming F (monotonically), so that the variable-set is then
     {1,...,max_new}, with max >= 0, and all elements used.
     The renaming-vector rv has size max_old+1, where max_old
     is the maximum of variable-indices used in the input-F,
     and such that for 1 <= v <= max_old holds:
      - rv[v] = 0 iff v does not occur in the original F
      - otherwise rv[v] in {1,...,max_new} is the new variable-index.
     An empty renaming-vector means that no renaming took place.
  */
  /* First the type for presenting information on changes according to
     the possibilities of RenameO, assuming that at least one variable is
     to be shown in the prefix:
      - original: (0,{})
      - maxindex: (max_index >= 1, {})
      - renamed : (max_index >= 1, renaming-vector != {})

     Here max_index is the new maximum of occurring indices, which is for
     the renamed form also the number of occurring variables.
  */
  typedef std::vector<gen_uint_t> rename_vt;
  typedef std::pair<gen_uint_t, rename_vt> rename_info_t;
  // Changing F; sorted = true means the clauses themselves and the
  // list of clauses are sorted:
  template <class CLS>
  rename_info_t rename_clauselist(CLS& F, const bool sorted = false) {
    const gen_uint_t old_max = max_var_index(F, sorted);
    assert(old_max + 1 != 0);
    rename_vt indices(old_max+1);
    for (const Clause& C : F)
      for (const Lit x : C)
        indices[x.v.v] = 1;
    gen_uint_t new_max = 0;
    for (gen_uint_t i = 1; i <= old_max; ++i)
      if (indices[i] == 1) indices[i] = ++new_max;
    for (Clause& C : F)
      for (Lit& x : C)
        x.v.v = indices[x.v.v];
    return {new_max, indices};
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

  typedef std::pair<DimacsClauseList, rename_info_t> RDimacsClauseList;

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
    if (k == 0) return;
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
  void rand_clauselist_core(std::ostream& out, RandGen_t& g, const rparam_v& par) {
    for (const RParam& pa : par)
      for (gen_uint_t i = 0; i < pa.c; ++i) {
        Clause C; C.reserve(size(pa.cps));
        for (const ClausePart& cp : pa.cps)
          rand_clause(g, C, cp.n, cp.k, cp.p);
        out << C;
      }
  }
  void rand_clauselist(std::ostream& out, RandGen_t& g, const rparam_v& par) {
    const auto dp = extract_parameters(par);
    out << dp; if (dp.c == 0) return;
    rand_clauselist_core(out,g,par);
  }

  // Similar to rand_clauselist, but output into a clause-list, and handling
  // renaming-policies:
  RDimacsClauseList rand_clauselist(RandGen_t& g, const rparam_v& par, const RenameO r = RenameO::original) {
    if (par.empty()) return {{{0,0},{}}, {}};
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
    case RenameO::original : return {{{n,c}, F}, {}};
    case RenameO::maxindex : {
      const auto new_max = max_var_index(F);
      return {{{new_max,c}, F}, {new_max,{}}};
    }
    default : const auto R = rename_clauselist(F);
              return {{{R.first,c}, F}, R}; }
  }

  // Similar to rand_clauselist, but sort the result, and remove duplicated
  // clauses:
  RDimacsClauseList rand_sortedclauselist(RandGen_t& g, const rparam_v& par, const RenameO r = RenameO::original) {
    if (par.empty()) return {{{0,0},{}}, {}};
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
    const auto newc = F.size();
    assert(newc <= c);
    switch (r) {
    case RenameO::original : return {{{n, newc}, F}, {}};
    case RenameO::maxindex : {
      const auto new_max = max_var_index(F,true);
      return {{{new_max, newc}, F}, {new_max,{}}};
    }
    default : const auto R = rename_clauselist(F,true);
              assert(F.size() == newc);
              return {{{R.first, newc}, F}, R}; }
  }

  // Similar to rand_sortedclauselist, but now reject duplicated clauses
  // directly after creation, and thus the clause-set has actually exactly the
  // number of clauses as given:
  RDimacsClauseList rand_clauseset(RandGen_t& g, const rparam_v& par, const RenameO r = RenameO::original) {
    if (par.empty()) return {{{0,0},{}}, {}};
    ClauseSet F;
    const auto [n,c] = extract_parameters(par);
    // Testing whether there is enough memory (temporary solution; better
    // to use a custome-allocator, which actually uses the allocated memory):
    F.get_allocator().deallocate(F.get_allocator().allocate(c), c);
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
    // Remark: no F2.reserve(c), to minimise memory-duplication:
    for (auto it = F.begin(); it != F.end(); )
      F2.push_back(std::move(F.extract(it++).value()));
    // That F is now empty, is not guaranteed by the standard, but is
    // reasonable to expect:
    assert(F.empty() and F2.size() == c);
    switch (r) {
    case RenameO::original : return {{{n,c}, F2}, {}};
    case RenameO::maxindex : {
      const auto new_max = max_var_index(F2,true);
      return {{{new_max,c}, F2}, {new_max,{}}};
    }
    default : const auto R = rename_clauselist(F2,true);
              return {{{R.first,c}, F2}, R}; }
  }

  RDimacsClauseList random(RandGen_t& g, const Param& par) {
    const auto [spar, rpar] = par.gp;
    switch (spar) {
    case SortO::unsorted : return rand_clauselist(g, par.vp, rpar);
    case SortO::sorted : return rand_sortedclauselist(g, par.vp, rpar);
    default : return rand_clauseset(g, par.vp, rpar); }
  }


  /* ********************
     * Input and output *
     ********************
  */

  std::string default_filestem(const MainType t) {
    switch (t) {
    case MainType::block_uniform_cnf : return "BlRaGe";
    case MainType::block_uniform_qcnf : return "QuBlRaGe";
    case MainType::block_uniform_dqcnf : return "DeQuBlRaGe";
    default : return "NOT_IMPLEMENTED";
    }
  }
  std::string default_filesuffix(const MainType t) {
    switch (t) {
    case MainType::block_uniform_cnf : return ".dimacs";
    case MainType::block_uniform_qcnf : return ".qdimacs";
    case MainType::block_uniform_dqcnf : return ".dqdimacs";
    default : return "NOT_IMPLEMENTED";
    }
  }
  std::string default_dimacs(const dimacs_pars dp) {
    return std::to_string(dp.n) + "_" + std::to_string(dp.c);
  }
  std::string default_seeds(const vec_eseed_t& s) {
    return std::to_string(std::accumulate(s.begin(), s.end(), gen_uint_t(0)));
  }
  std::string default_filename(const MainType t, const dimacs_pars dp, const vec_eseed_t& s) {
    return default_filestem(t) + "_" + default_dimacs(dp) + "_" + default_seeds(s) + default_filesuffix(t);
  }

  enum class Error {
    domain = 30,
    invalid = 31,
    out_of_range = 32,
    alloc = 40,
    except = 50,
    invalid_clauses = 60,
    file_open = 61,
  };

}

#endif

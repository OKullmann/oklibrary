// Oliver Kullmann, 9.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Conditions on sets of latin squares

  Helper-components:

   - typedef size_t

   - scoped enum UC (unary conditions)
   - constant maxUC
   - array strUC
   - output-streaming for UC
   - mapUC (auxiliary, produces the std::map from strings to UC)
   - toUC(string) -> UC (interface to map)

   - scoped enum VS (versions of a square)
   - constant maxVS
   - array strVS
   - output-streaming for VS
   - mapVS (produces the std::map for VS)
   - toVS(string) -> VS
   - class Versions, wrapping a set of VS's (always containing the id-version),
     with output-streaming

   - scoped enum PT (product-type, i.e, row- or column-product)
   - constant maxPT
   - array strPT
   - output-streaming
   - toPT(string) -> PT

   - class Square, wraps index and VS, and a static indexing of names
   - output-streaming for Square (using the names if available)
   - class Squares, wrapping a set of Square's,
     with output-streaming

   - class Equation (wraps two squares)
   - output-streaming

   - class ProdEq (wraps three squares and a PT)
   - output-streaming

  The main class:

   - class AConditions, containing
    - all squares (primary squares and their versions)
    - for each unary condition the related squares
    - all equations
    - all product-equations
    with output-streaming.


TODOS:

1. The current 7 versions should be extended to 12 versions:
    - The 6 conjugates and their 6 transposed forms.
    - Then we should also have all their 11 self-forms as unary conditions.

*/

#ifndef CONDITIONS_QeAOpMULKB
#define CONDITIONS_QeAOpMULKB

#include <vector>
#include <map>
#include <set>
#include <utility>
#include <initializer_list>
#include <array>
#include <ostream>
#include <string>
#include <optional>
#include <exception>
#include <algorithm>

#include <cassert>
#include <cstdint>

#include <ProgramOptions/Strings.hpp>
#include <Numerics/NumInOut.hpp>

namespace Conditions {

  typedef std::uint64_t size_t;

  // Unary conditions:
  enum class UC : size_t {
    rls = 1,
    cls = 2,
    ls = 3,
    diag = 4,
    antidiag = 5,
    uni = 6,
    antiuni = 7,
    idem = 8,
    antiidem = 9,
    rred = 10,
    orred = 11, // opposite
    cred = 12,
    ocred = 13,
    red = 14,
    ored = 15,
    box = 16,
    symm = 17,
    antisymm = 18
  };
  constexpr size_t maxUC = size_t(UC::antisymm);

  constexpr std::array<const char*, maxUC+1>
    strUC{"UNDEF", "rls", "cls", "ls", "diag", "antidiag", "uni", "antiuni",
      "idem", "antiidem", "rred", "orred", "cred", "ocred", "red", "ored",
      "box", "symm", "antisymm"};
  std::ostream& operator <<(std::ostream& out, const UC uc) {
    if (size_t(uc) <= maxUC) {
      assert(strUC[size_t(uc)]);
      return out << strUC[size_t(uc)];
    }
    else return out << "UNKNOWN[Conditions::UC]:" << size_t(uc);
  }
  std::map<std::string, UC> mapUC() {
    std::map<std::string, UC> res;
    for (size_t i = 1; i <= maxUC; ++i) {
      assert(strUC[i]);
      res.insert({strUC[i], UC(i)});
    }
    return res;
  }
  // Returns UC(0) if s does not correspond to a unary-condition-string:
  UC toUC(const std::string& s) noexcept {
    static const std::map<std::string, UC> m = mapUC();
    static const auto end = m.end();
    const auto f = m.find(s);
    return f==end ? UC(0) : f->second;
  }


  // Versions of a square (in the basic encoding, every version is presented
  // as an extra square, connected to the original square via equality- and
  // elementship-relations):
  enum class VS : size_t {
    id = 0, // c123
      c213 = 1,   // t circ id (transposed)
      at = 2,    // at circ id (antitransposed)
      atc213 = 3, // t circ at circ id = p circ id (point-reflection)
    c231 = 4,
      c132 = 5,    // t circ c231
      atc231 = 6, // at circ c231
      atc132 = 7,  // p circ c231
    c312 = 8,
      c321 = 9,    // c circ c312
      atc312 = 10,// at circ c312
      atc321 = 11  // p circ c312
  };
  constexpr size_t maxVS = size_t(VS::atc321);

  constexpr std::array<const char*, maxVS+1>
    strVS{
      "id",   "c213", "at",     "atc213",
      "c231", "c132", "atc231", "atc132",
      "c312", "c321", "atc312", "atc321"};
  std::ostream& operator <<(std::ostream& out, const VS vs) {
    if (size_t(vs) <= maxVS) return out << strVS[size_t(vs)];
    else return out << "UNKNOWN[Conditions::VS]:" << size_t(vs);
  }
  std::map<std::string, VS> mapVS() {
    std::map<std::string, VS> res;
    for (size_t i = 0; i <= maxVS; ++i)
      res.insert({strVS[i], VS(i)});
    return res;
  }
  // Returns VS::id if s does not correspond to a version-string:
  VS toVS(const std::string& s) noexcept {
    static const std::map<std::string, VS> m = mapVS();
    static const auto end = m.end();
    const auto f = m.find(s);
    return f==end ? VS::id : f->second;
  }

  inline constexpr bool is_main_rep(const VS vs) noexcept {
    return size_t(vs) % 4 == 0;
  }
  inline constexpr VS main_rep(const VS vs) noexcept {
    const size_t index = size_t(vs);
    assert(index <= maxVS);
    return VS((index / 4) * 4);
  }
  inline constexpr bool with_t(const VS vs) noexcept {
    const size_t mod = size_t(vs) % 4;
    return mod == 1 or mod == 3;
  }
  inline constexpr bool with_at(const VS vs) noexcept {
    const size_t mod = size_t(vs) % 4;
    return mod == 2 or mod == 3;
  }


  // Only stores the representatives:
  struct Versions {
    typedef std::set<VS> choices_t;
  private :
    choices_t choices_;
  public :
    Versions() noexcept : choices_({VS::id}) {}
    Versions(const std::initializer_list<VS>& L) {
      for (const VS vs : L) insert(vs);
      choices_.insert(VS::id);
    }

    size_t size() const noexcept { return choices_.size(); }
    const choices_t& choices() const noexcept { return choices_; }
    bool insert(const VS v) {
      return choices_.insert(main_rep(v)).second;
    }
    bool contains(const VS v) const noexcept {
      return choices_.contains(main_rep(v));
    }
    // 0 <= index < size(); return size() iff v is not contained:
    size_t index(const VS v) const noexcept {
      return std::distance(choices_.begin(), choices_.find(main_rep(v)));
    }
    bool operator ==(const Versions&) const noexcept = default;
    auto operator <=>(const Versions&) const noexcept = default;
  };
  std::ostream& operator <<(std::ostream& out, const Versions& vs) {
    Environment::out_line(out, vs.choices());
    return out;
  }


  // Product-type:
  enum class PT : size_t { rprod = 1, cprod = 2};
  constexpr size_t maxPT = size_t(PT::cprod);
  constexpr std::array<const char*, maxPT+1>
    strPT{"UNDEF", "rprod", "cprod"};
  std::ostream& operator <<(std::ostream& out, const PT pt) {
    if (size_t(pt) <= maxPT) return out << strPT[size_t(pt)];
    else return out << "UNKNOWN[Conditions::PT]:" << size_t(pt);
  }
  // Returns PT(0) for unrecognised s:
  PT toPT(const std::string& s) noexcept {
    if (s == strPT[1]) return PT(1);
    else if (s == strPT[2]) return PT(2);
    else return PT(0);
  }


  struct Square {
    size_t i;
    VS v;
    constexpr Square(const size_t i, const VS v = VS::id)
      : i(i), v(v) {}
    bool operator ==(const Square&) const noexcept = default;
    auto operator <=>(const Square&) const noexcept = default;

    bool primary() const noexcept { return v == VS::id; }

    constexpr static std::string_view decl_keyword  = "squares";

    static bool allowed(const std::string& s) noexcept {
      assert(not s.empty() and not Environment::starts_with_space(s) and
             not Environment::ends_with_space(s));
      return s != decl_keyword and s != "=" and
        toUC(s) == UC(0) and
        toVS(s) == VS::id and s != "id" and
        toPT(s) == PT(0);
    }
    typedef Environment::indstr_t indstr_t;
    inline static indstr_t is;

    typedef Environment::tokens_t tokens_t;
    typedef std::optional<Square> osq_t;
    // Uses natural numbers for names if is empty:
    static osq_t read(
        const tokens_t& line, size_t& j) noexcept {
      const size_t N = line.size();
      if (j >= N or line[j] == "id") return {};
      const VS vs = toVS(line[j]);
      if (vs != VS::id) { ++j; if (j >= N) return {}; }
      if (is.first.empty()) {
        size_t i;
        try { i = FloatingPoint::to_UInt(line[j]); }
        catch (std::exception&) { return {}; }
        ++j; return Square(i,vs);
      }
      else {
        const auto& name = line[j];
        assert(Square::allowed(name));
        const auto f = is.second.find(name);
        if (f == is.second.end()) return {};
        else {++j; return Square(f->second, vs);}
      }
    }

  };
  std::ostream& operator <<(std::ostream& out, const Square& s) {
    if (not Square::is.first.empty()) {
      assert(s.i < Square::is.first.size());
      const auto& name = Square::is.first[s.i];
      if (s.v == VS::id) return out << name;
      else return out << s.v << " " << name;
    }
    else {
      if (s.v == VS::id) return out << s.i;
      else return out << s.v << " " << s.i;
    }
  }

  struct Squares {
    typedef std::set<Square> set_t;
  private :
    set_t sqs_;
  public :
    Squares() noexcept = default;
    Squares(const std::initializer_list<Square>& L)
      : sqs_(L.begin(), L.end()) {}

    bool empty() const noexcept { return sqs_.empty(); }
    size_t size() const noexcept { return sqs_.size(); }
    const set_t& sqs() const noexcept { return sqs_; }
    bool insert(const Square s) {
      return sqs_.insert(s).second;
    }
    bool contains(const Square s) const noexcept {
      return sqs_.contains(s);
    }
    bool operator ==(const Squares&) const noexcept = default;
    auto operator <=>(const Squares&) const noexcept = default;
  };
  std::ostream& operator <<(std::ostream& out, const Squares& S) {
    Environment::out_line(out, S.sqs(), "  ");
    return out;
  }


  class Equation {
    Square s1, s2; // invariant s1 <= s2
  public :
    constexpr Equation(const Square s1, const Square s2) noexcept
      : s1(std::min(s1,s2)), s2(std::max(s1,s2)) {}
    Square lhs() const noexcept { return s1; }
    Square rhs() const noexcept { return s2; }
    bool operator ==(const Equation&) const noexcept = default;
    auto operator <=>(const Equation&) const noexcept = default;
  };
  std::ostream& operator <<(std::ostream& out, const Equation& e) {
    return out << "= " << e.lhs() << "  " << e.rhs();
  }


  class ProdEq {
    Square r_, f2_, f1_; // r = f2 * f1
    PT pt_;
  public :
    constexpr ProdEq(const Square r, const Square f2,
                     const Square f1, const PT pt = PT::rprod) noexcept
    : r_(r), f2_(f2), f1_(f1), pt_(pt) {}
    PT pt() const noexcept { return pt_; }
    Square f1() const noexcept { return f1_; }
    Square f2() const noexcept { return f2_; }
    Square r() const noexcept { return r_; }
    bool operator ==(const ProdEq&) const noexcept = default;
    auto operator <=>(const ProdEq&) const noexcept = default;
  };
  std::ostream& operator <<(std::ostream& out, const ProdEq& e) {
    return out << e.pt() << " " << e.r() << "  " << e.f2() << "  " << e.f1();
  }


  struct AConditions {
    typedef std::vector<Versions> vv_t;
    typedef std::map<UC, Squares> map_t;
    typedef std::set<Equation> set_eq_t;
    typedef std::set<ProdEq> set_peq_t;

    const size_t k; // the number of primary squaress
  private :
    vv_t versions_;    // versions_.size() == k
    map_t m_;          // all squares for which a given condition is required
    set_eq_t eq_;      // the equalities
    set_peq_t peq_;    // the product-equalities

  public :
    explicit AConditions(const size_t k) noexcept
      : k(k), versions_(k) {}

    const vv_t& versions() const noexcept { return versions_; }
    const map_t& map() const noexcept { return m_; }
    const set_eq_t& eq() const noexcept { return eq_; }
    const set_peq_t& peq() const noexcept { return peq_; }

    const Squares& sqs(const UC uc) const noexcept {
      static const Squares empty;
      const auto f = m_.find(uc);
      if (f == m_.end()) return empty;
      else return f->second;
    }

    bool valid(const Square s) const noexcept {
      return s.i < k;
    }
    bool valid(const Equation e) const noexcept {
      return contains(e.lhs()) and contains(e.rhs());
    }
    bool valid(const ProdEq pe) const noexcept {
      return contains(pe.r()) and contains(pe.f1()) and contains(pe.f2());
    }

    size_t num_squares() const noexcept {
      size_t sum = 0;
      for (const auto& v : versions_) sum += v.size();
      return sum;
    }
    // 0 <= index < num_squares():
    size_t index(const Square s) const noexcept {
      assert(contains(s));
      size_t sum = 0;
      for (size_t j = 0; j < s.i; ++j) sum += versions_[j].size();
      return sum + versions_[s.i].index(s.v);
    }

    bool insert(const Square s) {
      assert(valid(s));
      return versions_[s.i].insert(s.v);
    }
    bool contains(const Square s) const noexcept {
      if (not valid(s)) return false;
      else return versions_[s.i].contains(s.v);
    }

    bool insert(const UC c, const Square s) {
      insert(s);
      return m_[c].insert(s);
    }
    bool contains(const UC c, const Square s) const noexcept {
      const auto f = m_.find(c);
      if (f == m_.end()) return false;
      else return f->second.contains(s);
    }

    // Insert equality-condition:
    bool insert(const Equation e) {
      insert(e.lhs()); insert(e.rhs());
      return eq_.insert(e).second;
    }
    bool contains(const Equation e) const noexcept {
      return eq_.contains(e);
    }

    // Insert Product-equalities:
    bool insert(const ProdEq pe) {
      insert(pe.r()); insert(pe.f1()); insert(pe.f2());
      return peq_.insert(pe).second;
    }
    bool contains(const ProdEq pe) const noexcept {
      return peq_.contains(pe);
    }


    void out_sq_key(std::ostream& out) const {
      out << Square::decl_keyword;
    }
    void out_squares(std::ostream& out) const {
      out_sq_key(out);
      for (size_t i = 0; i < k; ++i) out << " " << Square(i);
      out << "\n";
    }
    void out_conditions(std::ostream& out) const {
      for (const auto& p : m_) {
        assert(not p.second.empty());
        out << p.first << " \t" << p.second << "\n";
      }
    }
    void out_equations(std::ostream& out) const {
      for (const auto& eq : eq_)
        out << eq << "\n";
    }
    void out_pequations(std::ostream& out) const {
      for (const auto& peq : peq_)
        out << peq << "\n";
    }

    bool operator ==(const AConditions&) const noexcept = default;

  };
  std::ostream& operator <<(std::ostream& out, const AConditions& AC) {
    AC.out_squares(out); AC.out_conditions(out); AC.out_equations(out);
    AC.out_pequations(out);
    return out;
  }

}

#endif

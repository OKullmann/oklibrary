// Oliver Kullmann, 9.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Conditions on sets of latin squares

*/

#ifndef CONDITIONS_QeAOpMULKB
#define CONDITIONS_QeAOpMULKB

#include <set>
#include <vector>
#include <map>
#include <utility>
#include <initializer_list>
#include <array>
#include <ostream>
#include <string>

#include <cassert>
#include <cstdint>

namespace Conditions {

  typedef std::uint64_t size_t;

  // Unary conditions (all combinations possible):
  enum class UC : size_t {
    rls = 1,
    cls = 2,
    diag = 3,
    antidiag = 4,
    uni = 5,
    antiuni = 6,
    idem = 7,
    antiidem = 8,
    rred = 9,
    orred = 10, // opposite
    cred = 11,
    ocred = 12,
    symm = 13,
    antisymm = 14
  };

  constexpr size_t maxUC = size_t(UC::antisymm);
  constexpr std::array<const char*, maxUC+1>
    strUC{"UNDEF", "rls", "cls", "diag", "antidiag", "uni", "antiuni", "idem",
      "antiidem", "rred", "orred", "cred", "ocred", "symm", "antisymm"};
  std::ostream& operator <<(std::ostream& out, const UC uc) {
    if (size_t(uc) <= maxUC) return out << strUC[size_t(uc)];
    else return out << "UNKNOWN[Conditions::UC]:" << size_t(uc);
  }
  std::map<std::string, UC> mapUC() {
    std::map<std::string, UC> res;
    for (size_t i = 1; i <= maxUC; ++i)
      res.insert({strUC[i], UC(i)});
    return res;
  }
  // Returns UC(0) if s does not correspond to a unary-condition-string:
  UC toUC(const std::string& s) noexcept {
    static const std::map<std::string, UC> m = mapUC();
    static const auto end = m.end();
    const auto f = m.find(s);
    return f==end ? UC(0) : f->second;
  }


  struct UConditions {
    typedef std::set<UC> cond_t;
  private :
    cond_t cond_;
  public :
    UConditions() noexcept = default;
    UConditions(const std::initializer_list<UC>& L) : cond_(L.begin(), L.end()) {}

    bool empty() const noexcept { return cond_.empty(); }
    const cond_t& cond() const noexcept { return cond_; }
    bool insert(const UC c) {
      return cond_.insert(c).second;
    }
    bool contains(const UC c) const noexcept {
      return cond_.contains(c);
    }
    bool operator ==(const UConditions&) const noexcept = default;
    auto operator <=>(const UConditions&) const noexcept = default;
  };
  std::ostream& operator <<(std::ostream& out, const UConditions& uc) {
    if (uc.cond().empty()) return out;
    auto it = uc.cond().begin(); const auto end = uc.cond().end();
    out << *it; ++it;
    for (; it != end; ++it) out << " " << *it;
    return out;
  }


  // Versions of a square (in the basic encoding, every version is presented
  // as an extra square, connected to the original square via equality- and
  // elementship-relations):
  enum class VS : size_t {
    id = 0,
    c213 = 1,
    c312 = 2,
    c321 = 3,
    c132 = 4,
    c231 = 5,
    at = 6
  };
  constexpr size_t maxVS = size_t(VS::at);
  constexpr std::array<const char*, maxVS+1>
    strVS{"id", "c213", "c312", "c321", "c132", "c231", "at"};
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


  struct Versions {
    typedef std::set<VS> choices_t;
  private :
    choices_t choices_;
  public :
    Versions() noexcept : choices_({VS::id}) {}
    Versions(const std::initializer_list<VS>& L)
      : choices_(L.begin(), L.end()) {
      choices_.insert(VS::id);
    }

    bool empty() const noexcept { return choices_.empty(); }
    const choices_t& choices() const noexcept { return choices_; }
    bool insert(const VS v) {
      if (v == VS::id) return false;
      else return choices_.insert(v).second;
    }
    bool contains(const VS v) const noexcept {
      if (v == VS::id) return true;
      else return choices_.contains(v);
    }
    bool operator ==(const Versions&) const noexcept = default;
    auto operator <=>(const Versions&) const noexcept = default;
  };
  std::ostream& operator <<(std::ostream& out, const Versions& vs) {
    if (vs.choices().empty()) return out;
    auto it = vs.choices().begin(); const auto end = vs.choices().end();
    out << *it; ++it;
    for (; it != end; ++it) out << " " << *it;
    return out;
  }


  struct Square {
    size_t i;
    VS v;
    constexpr Square(const size_t i, const VS v = VS::id)
      : i(i), v(v) {}
    bool operator ==(const Square&) const noexcept = default;
    auto operator <=>(const Square&) const noexcept = default;
  };
  std::ostream& operator <<(std::ostream& out, const Square& s) {
    if (s.v == VS::id) return out << s.i;
    else return out << s.v << " " << s.i;
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
    return out << "= " << e.lhs() << " " << e.rhs();
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
  PT toPT(const std::string& s) noexcept {
    if (s == strPT[1]) return PT(1);
    else if (s == strPT[2]) return PT(2);
    else return PT(0);
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
    return out << e.pt() << " " << e.r() << " " << e.f2() << " " << e.f1();
  }


  struct AConditions {
    typedef std::vector<Versions> vv_t;
    typedef std::map<Square, UConditions> map_t;
    typedef std::set<Equation> set_eq_t;
    typedef std::set<Square> orth_t;
    typedef std::set<orth_t> set_orth_t;

    const size_t k; // the number of primary ls's
  private :
    vv_t versions_; // versions_.size() == k
    map_t m_; // maps exactly the squares according to versions_
    set_eq_t eq_; // the equalities
    set_orth_t orth_; // the orthogonal sets of squares

  public :
    explicit AConditions(const size_t k) noexcept
      : k(k), versions_(k), m_(id_versions(k)) {}
    static map_t id_versions(const size_t k) {
      map_t res;
      for (size_t i = 0; i < k; ++i) res.insert({{i,VS::id},{}});
      return res;
    }

    const vv_t& versions() const noexcept { return versions_; }
    const map_t& map() const noexcept { return m_; }
    const set_eq_t& eq() const noexcept { return eq_; }
    const set_orth_t& orth() const noexcept { return orth_; }

    const UConditions& cond(const Square s) const noexcept {
      assert(contains(s));
      const auto f = m_.find(s);
      assert(f != m_.end());
      return f->second;
    }

    bool valid(const Square s) const noexcept {
      return s.i < k;
    }
    bool valid(const Equation e) const noexcept {
      return contains(e.lhs()) and contains(e.rhs());
    }
    bool valid(const orth_t& o) const noexcept {
      return std::all_of(o.begin(), o.end(),
                         [this](const Square s){return contains(s);});
    }

    size_t num_squares() const noexcept { return m_.size(); }

    // Insert version v for primary square i:
    bool insert(const Square s) {
      assert(valid(s));
      const bool res = versions_[s.i].insert(s.v);
      if (res) m_.insert({s,{}});
      return res;
    }
    bool contains(const Square s) const noexcept {
      if (not valid(s)) return false;
      else return versions_[s.i].contains(s.v);
    }

    // Insert condition c for square s:
    bool insert(const Square s, const UC c) {
      assert(contains(s));
      return m_[s].insert(c);
    }
    bool contains(const Square s, const UC c) const noexcept {
      const auto f = m_.find(s);
      if (f == m_.end()) return false;
      else return f->second.contains(c);
    }

    // Insert equality-condition:
    bool insert(const Equation e) {
      assert(valid(e));
      return eq_.insert(e).second;
    }
    bool contains(const Equation e) const noexcept {
      return eq_.contains(e);
    }

    // Insert orthogonality-condition for a set of squares:
    bool insert(const orth_t& o) {
      assert(valid(o));
      return orth_.insert(o).second;
    }


    constexpr static std::string_view decl_keyword  = "squares";
    constexpr static std::string_view orth_keyword  = "ortho";
    void out_sq_key(std::ostream& out) const {
      out << decl_keyword;
    }
    void out_squares(std::ostream& out) const {
      out_sq_key(out);
      for (size_t i = 0; i < k; ++i) out << " " << i;
      out << "\n";
    }
    void out_conditions(std::ostream& out) const {
      for (const auto& p : m_) {
        if (p.second.empty()) continue;
        out << p.first << " \t" << p.second << "\n";
      }
    }
    void out_equations(std::ostream& out) const {
      for (const auto& eq : eq_)
        out << eq << "\n";
    }
    void out_ortho_key(std::ostream& out) const {
      out << orth_keyword;
    }
    void out_orth(std::ostream& out, const orth_t& o) const {
      if (o.empty()) return;
      auto it = o.begin(); const auto end = o.end();
      out << *it; ++it;
      for (; it != end; ++it) out << " " << *it;
    }
    void out_orthogonality(std::ostream& out) const {
      for (const auto& o : orth_) {
        if (o.size() <= 1) continue;
        out_ortho_key(out); out << " \t";
        out_orth(out, o);
        out << "\n";
      }
    }

    bool operator ==(const AConditions&) const noexcept = default;

  };
  std::ostream& operator <<(std::ostream& out, const AConditions& AC) {
    AC.out_squares(out); AC.out_conditions(out); AC.out_equations(out);
    AC.out_orthogonality(out);
    return out;
  }

}

#endif

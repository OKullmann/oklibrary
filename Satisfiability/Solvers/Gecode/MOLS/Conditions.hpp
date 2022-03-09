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

#include <cassert>
#include <cstdint>

namespace Conditions {

  typedef std::uint64_t size_t;

  // Unary conditions for lines (all combinations possible):
  enum class UCL : size_t {
    diag = 1,
    antidiag = 2,
    uni = 3,
    antiuni = 4,
    idem = 5,
    rred = 6,
    cred = 7
  };

  struct UConditions  {
    typedef std::set<UCL> cond_t;
  private :
    cond_t cond_;
  public :
    UConditions() noexcept = default;
    const cond_t& cond() const noexcept { return cond_; }
    bool insert(const UCL c) {
      return cond_.insert(c).second;
    }
    bool contains(const UCL c) const noexcept {
      return cond_.contains(c);
    }
    bool operator ==(const UConditions&) const noexcept = default;
    auto operator <=>(const UConditions&) const noexcept = default;
  };


  // Versions of a square:
  enum class VS : size_t {
    id = 0,
    c213 = 1,
    c312 = 2,
    c321 = 3,
    c132 = 4,
    c231 = 5,
    at = 6
  };

  struct Versions {
    typedef std::set<VS> choices_t;
  private :
    choices_t choices_;
  public :
    Versions() noexcept : choices_({VS::id}) {}
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


  struct Square {
    size_t i;
    VS v;
    constexpr Square(const size_t i, const VS v = VS::id)
      : i(i), v(v) {}
    bool operator ==(const Square&) const noexcept = default;
    auto operator <=>(const Square&) const noexcept = default;
  };


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
    bool insert(const Square s, const UCL c) {
      assert(contains(s));
      return m_[s].insert(c);
    }
    bool contains(const Square s, const UCL c) const noexcept {
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

  };

}

#endif

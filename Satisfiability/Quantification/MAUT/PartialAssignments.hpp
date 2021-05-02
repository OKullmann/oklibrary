// Oliver Kullmann, 29.4.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Partial assignments

*/

#ifndef PARTIALASSIGNMENTS_hicvHzc11p
#define PARTIALASSIGNMENTS_hicvHzc11p

#include <vector>
#include <exception>
#include <set>
#include <map>

#include <cassert>

#include "VarLit.hpp"
#include "ClauseSets.hpp"

namespace MAUT {

  enum class PA : LIT {o=0, t=1,tp=2, f=-1,fp=-2, b=3 };
  inline constexpr PA operator -(const PA p) noexcept {
    return p != PA::b ? PA(-LIT(p)) : p;
  }
  static_assert(-PA::o == PA::o);
  static_assert(-PA::t == PA::f);
  static_assert(-PA::tp == PA::fp);
  static_assert(-PA::f == PA::t);
  static_assert(-PA::fp == PA::tp);
  static_assert(-PA::b == PA::b);

  inline constexpr PA neg(const PA p, const LIT x) noexcept {
    if (x > 0) return p;
    else if (x == 0) return PA::o;
    else return -p;
  }

  inline constexpr bool pp(const PA p) noexcept {
    return p == PA::t or p == PA::tp;
  }
  inline constexpr bool np(const PA p) noexcept {
    return p == PA::f or p == PA::fp;
  }


  typedef std::vector<PA> PASS;

  PASS cl2pass(const VAR n, const CL& C) {
    assert(valid(C));
    PASS pa(n+1);
    for (const LIT x : C) {
      const VAR v = var(x);
      if (v > n) break;
      pa[v] = x>0 ? PA::t : PA::f;
    }
    return pa;
  }


  struct Pass {

    const VAR n;

    explicit Pass(const VAR n) : n(n), vec(n+1) {}
    Pass(const VAR n, const CL& C) : n(n), vec(cl2pass(n,C)) {}

    PA operator[] (const VAR v) const noexcept {
      assert(valid(v));
      if (v > n) return PA::o;
      return vec[v];
    }
    PA& operator[] (const VAR v) noexcept {
      assert(valid(v) and v <= n);
      return vec[v];
    }
    PA operator() (const LIT x) const noexcept {
      assert(valid(x));
      const VAR v = var(x);
      if (v > n) return PA::o;
      return neg(vec[v], x);
    }

    void set(const LIT x, const PA p) noexcept {
      assert(valid(x));
      assert(var(x) <= n);
      if (x >= 0) vec[var(x)] = p;
      else vec[var(x)] = -p;
    }
    typedef std::pair<LIT, PA> assignment_t;
    void set(const std::vector<assignment_t>& v) noexcept {
      for (const auto [x,p] : v) set(x,p);
    }
    void set(const CL& C, const bool positive = true) noexcept {
      assert(valid(C));
      for (const LIT x : C) {
        const VAR v = var(x);
        if (v > n) break;
        if (positive) set(x, PA::t);
        else set(x, PA::f);
      }
    }

    void clear() noexcept { for (PA& p : vec) p = PA::o; }

    PA at(const VAR v) const {
      assert(valid(v));
      if (v > n)
        throw std::out_of_range(
          "Pass::at, v=" + std::to_string(v) + ", n=" + std::to_string(n));
      return vec[v];
    }
    PA& at(const VAR v) {
      assert(valid(v));
      if (v > n)
        throw std::out_of_range(
          "Pass::at, v=" + std::to_string(v) + ", n=" + std::to_string(n));
      return vec[v];
    }

    size_t size() const noexcept {
      return std::count_if(vec.begin(), vec.end(), [](PA p){return p!=PA::o;});
    }

    friend bool operator ==(const Pass& lhs, const Pass& rhs) noexcept {
      return lhs.n == rhs.n and lhs.vec == rhs.vec;
    }

  private :

    PASS vec;

  };


  bool sat(const Pass& pa, const CL& C) noexcept {
    return std::any_of(C.begin(), C.end(),
                       [&pa](const LIT x){return pp(pa(x));});
  }
  bool falsified(const Pass& pa, const CL& C) noexcept {
    return std::all_of(C.begin(), C.end(),
                       [&pa](const LIT x){return np(pa(x));});
  }

  bool sat(const Pass& pa, const CLS& F) noexcept {
    return std::all_of(F.begin(), F.end(),
                       [&pa](const CL& C){return sat(pa,C);});
  }


  bool occurs(const LIT x, const Pass& pa, const ClauseSet& F) noexcept {
    assert(valid(x) and not singular(x));
    assert(var(x) <= F.occ.n);
    if (var(x) <= pa.n and pa(var(x)) != PA::o) return false;
    for (const LitOcc Cp : F.occ[x]) if (not sat(pa, *Cp)) return true;
    return false;
  }

  size_t degree(const LIT x, const Pass& pa, const ClauseSet& F) noexcept {
    assert(valid(x) and not singular(x));
    assert(var(x) <= F.occ.n);
    if (var(x) <= pa.n and pa(var(x)) != PA::o) return 0;
    size_t deg = 0;
    for (const LitOcc Cp : F.occ[x]) deg += not sat(pa, *Cp);
    return deg;
  }


  size_t add_pure(Pass& pa, const ClauseSet& F) {
    const VAR n = F.dp.n;
    assert(n == F.occ.n and n <= pa.n);
    size_t count = 0;
    for (VAR v = 1; v <= n; ++v)
      if (pa[v] == PA::o) {
        if (occurs(lit(v,1), pa, F)) {
          if (not occurs(lit(v,-1), pa, F)) { pa[v] = PA::fp; ++count; }
        }
        else if (occurs(lit(v,-1), pa, F)) { pa[v] = PA::tp; ++count; }
        else { pa[v] = PA::b; ++count; }
      }
    return count;
  }


  size_t countb(const Pass& pa, const ClauseSet& F) noexcept  {
    size_t count = 0;
    for (VAR v = 1; v <= pa.n; ++v) {
      const PA p = pa[v];
      assert(p != PA::o);
      if (p == PA::b) ++count;
      else if (p == PA::tp or p == PA::fp) {
        const LIT x = lit(v, -LIT(p));
        const auto& occ = F.occ[x];
        if (std::all_of(occ.cbegin(), occ.cend(),
                        [&pa](const auto& C){return sat(pa,*C);}))
          ++count;
      }
    }
    return count;
  }

  bool maximal_falsified(const Pass& pa, const CLS& F) noexcept {
    Pass min(pa.n);
    for (const CL& C : F) if (falsified(pa, C)) min.set(C, false);
    for (const CL& C : F)
      if (not sat(min, C) and not falsified(min, C)) return false;
    return true;
  }


  typedef std::vector<size_t> cl_index_set;
  std::ostream& operator <<(std::ostream& out, const cl_index_set& s) {
    size_t index = 0;
    for (const size_t x : s) {
      if (index == 0) out << x;
      else out << "," << x;
      ++index;
    }
    return out;
  }
  typedef std::map<cl_index_set, size_t> profile_t;
  std::ostream& operator <<(std::ostream& out, const profile_t& p) {
    for (const auto& e : p)
      out << e.first << " : " << e.second << "\n";
    return out;
  }

  cl_index_set profile(const Pass& pa, const CLS& F) {
    cl_index_set res;
    for (size_t i = 0; i < F.size(); ++i)
      if (falsified(pa, F[i])) res.push_back(i);
    return res;
  }

  // Only for small maxn(F):
  profile_t profile(const CLS& F) {
    profile_t m;
    const VAR n = maxn(F);
    const CLS full = full_cls(n);
    for (const CL& C : full) ++m[profile(Pass(n, C), F)];
    return m;
  }

}

#endif

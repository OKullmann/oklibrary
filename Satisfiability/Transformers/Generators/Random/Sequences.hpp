// Oliver Kullmann, 8.11.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Random sequences



*/

#ifndef SEQUENCES_dZakTZB00i
#define SEQUENCES_dZakTZB00i

#include <ostream>

#include <cassert>
#include <cstdint>
#include <cmath>

#include "Numbers.hpp"

namespace RandGen {

  struct ExpSeq {
    typedef gen_uint_t size_t;
    typedef std::uint8_t small_size_t;
    const size_t E;
    const small_size_t S;
    const size_t s = std::pow(size_t(10), S);
    constexpr static small_size_t max_S = 19;
    static_assert(std::pow(size_t(10), max_S) < randgen_max);
    const size_t N;
    const bool ge1;

    constexpr ExpSeq() = delete;
    constexpr ExpSeq(const size_t E, const size_t S0, const size_t N, const bool ge1) noexcept : E(E), S(S0), N(N), ge1(ge1) {
      assert(E >= 1);
      assert(S0 >= 1);
      assert(S0 <= max_S);
      assert(N >= 1);
    }

    constexpr size_t main_size() const noexcept { return E * (s-1); }
    constexpr size_t size() const noexcept { return main_size() * N; }

    struct iterator_value {
      const size_t i, s, j, val;
      const bool ge1;
    };
    friend constexpr bool operator ==(const iterator_value& l, const iterator_value& r) {
      return l.i==r.i && l.s==r.s && l.j==r.j && l.val==r.val && l.ge1==r.ge1;
    }
    friend std::ostream& operator <<(std::ostream& out, const iterator_value& v) {
      out << v.i << " " << v.s << " " << v.j << " " << v.val << " " << v.ge1;
      return out;
    }
    template <typename FP>
    static FP translate(const iterator_value& v) {
      const FP p = FP(v.val) / FP(randgen_max);
      const FP base = std::pow(FP(10), v.i);
      const FP quot = base / FP(v.s);
      const FP factor = 10 * quot;
      const FP main_point = base + v.j * factor;
      const FP radius = quot / 10;
      const FP point = main_point + radius * (2 * p - 1);
      return v.ge1 ? point : 1 / point;
    }

  private :

    struct iterator_t {

      explicit iterator_t() = default;

      iterator_t(const size_t E, const small_size_t S, const size_t s, const size_t N, const bool ge1) noexcept : E(E), S(S), s(s), N(N), ge1(ge1), g(new RandGen_t(transform({E,S,N,ge1}))), val((*g)()) {}

      iterator_t(const iterator_t& it) noexcept : E(it.E), S(it.S), s(it.s), N(it.N), ge1(it.ge1), i(it.i), j(it.j), g(it.g ? new RandGen_t(*it.g):nullptr), val(it.val) {}
      iterator_t& operator =(const iterator_t& it) noexcept {
        E = it.E; S = it.S; s = it.s; N = it.N; ge1 = it.ge1;
        i = it.i; j = it.j;
        delete g;
        g = it.g ? new RandGen_t(*it.g) : nullptr;
        val = it.val;
        return *this;
      }

      ~iterator_t() noexcept { delete g; }

      iterator_value operator *() const noexcept {
        return {i,s,j,val,ge1};
      }
      iterator_t& operator ++() noexcept {
        assert(g);
        if (++k == N) {
          k = 0;
          if (++j == s) {
            j=1;
            if (++i == E) {
              delete g;
              g = nullptr;
              return *this;
            }
          }
        }
        val = (*g)();
        return *this;
      }

      friend bool operator ==(const iterator_t& lhs, const iterator_t& rhs) noexcept {
        return (lhs.g == nullptr and rhs.g == nullptr) or
          (lhs.E==rhs.E&&lhs.S==rhs.S&&lhs.N==rhs.N&&lhs.ge1==rhs.ge1 and
           lhs.i == rhs.i and lhs.j == rhs.j and lhs.g == rhs.g);
      }
      friend bool operator !=(const iterator_t& lhs, const iterator_t& rhs) noexcept {
        return not (lhs == rhs);
      }

    private :

      size_t E = 0;
      small_size_t S = 0;
      size_t s = 0;
      size_t N = 0;
      bool ge1 = 0;

      size_t i = 0; // i < E
      size_t j = 1; // j < s
      size_t k = 0; // k < N

      RandGen_t* g = nullptr;
      gen_uint_t val = 0;

    };

  public :

    iterator_t begin() const noexcept { return iterator_t(E,S,s,N,ge1); }
    iterator_t end() const noexcept { return iterator_t(); }

  };

}

#endif

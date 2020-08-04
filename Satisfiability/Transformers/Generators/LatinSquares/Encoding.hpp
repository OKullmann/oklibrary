// Oliver Kullmann, 4.8.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef ENCODING_OskaTwamjk
#define ENCODING_OskaTwamjk

#include <ostream>
#include <utility>

#include <cassert>

#include "Statistics.hpp"

namespace Encoding {

  // The indices of Euler-pairs, and their colexicographical ordering:
  struct IndexEuler {
    Statistics::dim_t p, q; // p < q
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

  constexpr Statistics::var_t index(const IndexEuler e) noexcept {
    if (e.q % 2 == 0) return e.p + Statistics::var_t(e.q/2) * (e.q-1);
    else return e.p + Statistics::var_t(e.q) * ((e.q-1) / 2);
  }
  static_assert(index({0,1}) == 0);
  static_assert(index({0,2}) == 1);
  static_assert(index({1,2}) == 2);
  static_assert(index({0,3}) == 3);
  static_assert(index({65534, 65535}) == 2147450879);
  static_assert(index({65533, 65534}) == 2147385344);


  struct ValPair {
    Statistics::dim_t x, y; // x, y < N
  };
  /*std::ostream& operator <<(std::ostream& out, const ValPair p) {
    return out << p.x << "," << p.y;
  }*/

  constexpr Statistics::var_t index(const ValPair eps, const Statistics::dim_t N) noexcept {
    assert(eps.x < N);
    assert(eps.y < N);
    return Statistics::var_t(eps.x) * N + eps.y;
  }
  static_assert(index({0,0}, 10) == 0);
  static_assert(index({0,1}, 10) == 1);
  static_assert(index({1,0}, 10) == 10);
  static_assert(index({9,9}, 10) == 99);


  struct VarEncoding {
    const Statistics::var_t N;
    const Statistics::var_t k;
    const Statistics::NumVarsCls nvc;
    const Options::SymP symopt;
    const Options::EAloP ealoopt;
    const Options::EulP eulopt;
    const Options::PrimeP primopt;

    const Statistics::var_t N2 = N*N;
    const Statistics::var_t N3 = N2 * N;

  private :
    mutable Statistics::var_t next = nvc.n0;
  public :

    constexpr VarEncoding(const Statistics::Param ps, const Options::SymP s, const Options::EAloP e, const Options::EulP eul, const Options::PrimeP prim) noexcept : N(ps.N), k(ps.k), nvc(numvarscls(ps,s,e,eul,prim)), symopt(s), ealoopt(e), eulopt(eul), primopt(prim) {}

    void nls(std::ostream& out) const {
      if (symopt == Options::SymP::full)
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
      if (symopt == Options::SymP::full)
        out << Environment::DWW{"  npes=0.5k(k-1)N4"} << nvc.npes << "\n";
      else
        out << Environment::DWW{"  npes=0.5k(k-1)(N-1)(N-2)*"} << "\n"
            << Environment::DWW{"       (N2-(1+4/k)N-2+10/k)"} << nvc.npes << "\n";
    }
    void naux(std::ostream& out) const {
      out << Environment::DWW{"  naux~0.5npes"} << nvc.naux << "\n";
    }

    void cls(std::ostream& out) const {
      if (symopt == Options::SymP::full) {
        if (primopt == Options::PrimeP::full)
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
        if (primopt == Options::PrimeP::full)
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
        if (eulopt == Options::EulP::full)
          out << Environment::DWW{"  ces~6npes"} << nvc.ces << "\n";
        else
          out << Environment::DWW{"  ces~4npes"} << nvc.ces << "\n";
      }
      else {
        if (eulopt == Options::EulP::full)
          out << Environment::DWW{"  ces~6.5npes"} << nvc.ces << "\n";
        else
          out << Environment::DWW{"  ces~4.5npes"} << nvc.ces << "\n";
      }
    }

    Statistics::var_t operator()() const noexcept {
      assert(next < nvc.n);
      return ++next;
    }

    constexpr Statistics::var_t operator()(const Statistics::dim_t i, const Statistics::dim_t j, const Statistics::dim_t eps, const Statistics::dim_t p) const noexcept {
      assert(i < N);
      assert(j < N);
      assert(eps < N);
      assert(p < k);

      if (symopt == Options::SymP::full) {
        assert(symopt == Options::SymP::full);
        const Statistics::var_t v = 1 + p * nvc.nbls1 + i * N2 + j * N + eps;
        assert(v <= nvc.nls);
        return v;
      }
      else {
        assert(symopt == Options::SymP::reduced);
        if (p == 0) {
          assert(i != 0);
          assert(j != 0);
          assert(eps != i);
          assert(eps != j);
          const Statistics::var_t n_prev_lines = (i-1) * ((N-2)*(N-2) + (N-1));
          const Statistics::var_t n_prev_cells = i>=j ? (j-1)*(N-2) : (j-2)*(N-2) + (N-1);
          const Statistics::var_t v = 1 + n_prev_lines + n_prev_cells + eps_adj(i,j,eps);
          assert(v <= nvc.nls);
          return v;
        }
        else {
          assert(i != 0);
          assert(eps != j);
          assert(j != 0 or eps != i);
          const Statistics::var_t n_prev_ls = nvc.nbls1 + (p-1) * nvc.nbls2;
          const Statistics::var_t n_prev_lines = (i-1) * (N*(N-1) - 1);
          const Statistics::var_t n_prev_cells = j * (N-1) - (j==0 ? 0 : 1);
          const Statistics::var_t v = 1 + n_prev_ls + n_prev_lines + n_prev_cells +
            (j==0 ? eps_adj(i,j,eps) : eps_adj(j,eps));
          assert(v <= nvc.nls);
          return v;
        }
      }
    }

    constexpr Statistics::var_t operator()(const Statistics::dim_t i, const Statistics::dim_t j, const ValPair eps, const IndexEuler pq) const noexcept {
      assert(i < N);
      assert(j < N);
      assert(eps.x < N);
      assert(eps.y < N);
      assert(pq.p < pq.q);
      assert(pq.q < k);

      if (symopt == Options::SymP::full) {
        const Statistics::var_t n_prev_es = index(pq) * nvc.nbes1;
        const Statistics::var_t n_prev_lines = i * N3;
        const Statistics::var_t n_prev_cells = j * N2;
        const Statistics::var_t v = 1+nvc.nls + n_prev_es + n_prev_lines + n_prev_cells + index(eps,N);
        assert(nvc.nls < v and v <= nvc.n0);
        return v;
      }

      else {
        assert(symopt == Options::SymP::reduced);
        assert(i != 0);
        assert(eps.x != j);
        assert(eps.y != j);
        assert(eps.x != eps.y);
        const Statistics::var_t p = pq.p, q = pq.q;
        if (p >= 1) {
          assert(j != 0 or (eps.x != i and eps.y != i));
          const Statistics::var_t n_prev_es = q * nvc.nbes1 + (index(pq)-q) * nvc.nbes2;
          const Statistics::var_t n_prev_lines = (i-1) * N * (N-1) * (N-2) - (i-1) * 2 * (N-2);
          const Statistics::var_t n_prev_cells = j==0 ? 0  : j * (N-1) * (N-2) - 2 * (N-2);
          const Statistics::var_t v = 1+nvc.nls + n_prev_es + n_prev_lines + n_prev_cells
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
          const Statistics::var_t n_prev_es = (q-1) * nvc.nbes1 + (index(pq)-(q-1)) * nvc.nbes2;
          const Statistics::var_t n_prev_lines = (i-1) * ((N-2)*(N-2)*(N-2) + (N-1)*(N-2));
          const Statistics::var_t n_prev_cells = j<=i ? (j-1) * (N-2) * (N-2) :
                                           (N-1)*(N-2) + (j-2) * (N-2) * (N-2);
          const Statistics::var_t v = 1+nvc.nls + n_prev_es + n_prev_lines + n_prev_cells + index_adj(i, j, eps);
          assert(nvc.nls < v and v <= nvc.n0);
          return v;
        }
      }
    }

    // Using var_t for the arguments to avoid implicit conversions:
    static constexpr Statistics::var_t eps_adj(Statistics::var_t i, Statistics::var_t j, const Statistics::var_t eps) noexcept {
      if (i == j) {
        if (eps < i) return eps;
        else return eps-1;
      }
      if (i > j) std::swap(i,j);
      if (eps > j) return eps-2;
      else if (eps > i) return eps-1;
      else return eps;
    }
    static constexpr Statistics::var_t eps_adj(const Statistics::var_t j, const Statistics::var_t eps) noexcept {
      if (eps > j) return eps-1;
      else return eps;
    }

    constexpr Statistics::var_t index_adj(const Statistics::var_t j, const ValPair eps) const noexcept {
      Statistics::var_t res = index(eps, N);
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
    constexpr Statistics::var_t index_adj(const Statistics::var_t i, const Statistics::var_t j, const ValPair eps) const noexcept {
      /* The relevant constraints are: x, y != j, x != i, x != y.
          For i!=j there are (N-2)^2 variables in total, namely N-2 rows
            (minus i,j), each with N-2 elements (minus j, minus diagonal).
          and for i=j there are (N-1)*(N-2) variables in total, namely N-1
            rows (minus i=j), each with N-2 elements (minus j, minus diagonal).
      */
      const Statistics::var_t xadj = eps_adj(i,j, eps.x);
      const Statistics::var_t yadj = eps_adj(eps.x,j, eps.y);
      return xadj * (N-2) + yadj;
    }
    constexpr Statistics::var_t index_adj2(const Statistics::var_t i, const Statistics::var_t j, const ValPair eps) const noexcept {
      assert(i != 0 and j == 0);
      /* The relevant constraints are: x,y >= 1, x,y != i; x != y. */
      return index_adj(i,j,eps) - (eps.x-1) - (eps.y > i ? 1 : 0);
    }


  };
  static_assert(VarEncoding({1,1},Options::SymP::full,Options::EAloP::none,Options::EulP::full,Options::PrimeP::full)(0,0,0,0) == 1);
  static_assert(VarEncoding({2,1},Options::SymP::full,Options::EAloP::none,Options::EulP::full,Options::PrimeP::full)(0,0,0,0) == 1);
  static_assert(VarEncoding({2,1},Options::SymP::full,Options::EAloP::none,Options::EulP::full,Options::PrimeP::full)(0,0,1,0) == 2);
  static_assert(VarEncoding({2,1},Options::SymP::full,Options::EAloP::none,Options::EulP::full,Options::PrimeP::full)(0,1,0,0) == 3);
  static_assert(VarEncoding({2,1},Options::SymP::full,Options::EAloP::none,Options::EulP::full,Options::PrimeP::full)(0,1,1,0) == 4);
  static_assert(VarEncoding({2,1},Options::SymP::full,Options::EAloP::none,Options::EulP::full,Options::PrimeP::full)(1,0,0,0) == 5);
  static_assert(VarEncoding({2,1},Options::SymP::full,Options::EAloP::none,Options::EulP::full,Options::PrimeP::full)(1,0,1,0) == 6);
  static_assert(VarEncoding({2,1},Options::SymP::full,Options::EAloP::none,Options::EulP::full,Options::PrimeP::full)(1,1,0,0) == 7);
  static_assert(VarEncoding({2,1},Options::SymP::full,Options::EAloP::none,Options::EulP::full,Options::PrimeP::full)(1,1,1,0) == 8);
  static_assert(VarEncoding({2,2},Options::SymP::full,Options::EAloP::none,Options::EulP::full,Options::PrimeP::full)(0,0,0,1) == 9);
  static_assert(VarEncoding({2,2},Options::SymP::full,Options::EAloP::none,Options::EulP::full,Options::PrimeP::full)(0,0,{0,0},{0,1}) == 17);
  static_assert(VarEncoding({2,2},Options::SymP::full,Options::EAloP::none,Options::EulP::full,Options::PrimeP::full)(0,0,{0,1},{0,1}) == 18);
  static_assert(VarEncoding({2,3},Options::SymP::full,Options::EAloP::none,Options::EulP::full,Options::PrimeP::full)(1,1,{1,1},{1,2}) == 72);

}

#endif

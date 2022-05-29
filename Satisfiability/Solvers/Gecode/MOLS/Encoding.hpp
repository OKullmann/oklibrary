// Oliver Kullmann, 12.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Encoding the LS-MOLS-conditions, for Gecode

  Namespace Encoding, abbreviated "EC".

  Contains the class EncCond for providing encoding-services.

*/

/*

TODOS:

1. DONE Complete the encoding of the unary conditions:

  The following Gecode constraints are enough for encoding all
  LS-MOLS-conditions parsed in Conditions.hpp:
  - distinct(IntVarArray X) : the all-different constraint for the array X.
  - element(IntVarArray X, IntVar y, IntVar z) : z is the y-th element of the
      array X, i.e. X_y = z.
  - rel(IntVar x, IntRelType irt, int val) : if irt is IRT_EQ, then x=val;
      if irt is IRT_NQ, then x!=val.

  Question OZ: Tip 4.8 in MPG seems not relevant for us, since apparently
  it concerns only the effort in posting the constraints, which is
  negligible?
  Answer: Yes. If this tip is taken into account, the conversion from an
  integer array to a shared integer array is done only once for all
  constraints (which are posted for this integer array). Also, less memory
  is used in this case. This tip makes sense if there is a huge amount of
  such constraints and/or the array is very large. It seems that this is not
  the case for Latin squares.

2. The 6 conjugates

  Consider a Latin square A of order N.

    B = c132 A
  means
    A(i,j) = k <=> B(i,k) = j
    B(i,j) = k <=> A(i,k) = j
  i.e., A(i,B(i,k)) = k, B(i,A(i,k)) = k
  equivalent to
    element(A[i,*], B[i,k], k)
  Alternatively
    element(B[i,*], A[i,k], k).

c132 is row-inversion (obtained from c123 by 2<->3).


    B = c312 A
  means
    A(i,j) = k <=> B(k,i) = j
    B(i,j) = k <=> A(j,k) = i
  i.e.: A(i,B(k,i)) = k, B(A(j,k),j) = k
  equivalent to
    element(A[i,*], B[k,i], k).
  Alternatively
    element(B[*,j], A[j,k], k).


    B = c231 A
  means
    A(i,j) = k <=> B(j,k) = i
    B(i,j) = k <=> A(k,i) = j
  i.e., A(B(j,k),j) = k, B(i,A(k,i)) = k
  equivalent to
    element(A[*,j], B[j,k], k).
  Alternatively
    element(B[i,*], A[k,i], k).


    B = c321 A
  means
    A(i,j) = k <=> B(k,j) = i
    B(i,j) = k <=> A(k,j) = i
  i.e., A(B(k,j),j) = k, B(A(k,j),j) = k
  equivalent to
    element(A[*,j], B[k,j], k).
  Alternatively
    element(B[*,j], A[k,j], k).

c321 is column-inversion (obtained from c123 by 1<->3).


  It is be possible to use only 2 squares instead of 4 (as above),
  by reading off the other 2 by some permutation of the cells.
  Example:

Attention: composition of permutations must here be performed
from left to right, in order to account for the operation on the
array-columns!

    B1 = c132 A, B2 = c312 A => B2 = c312 c132^-1 B1 = c312 c132 B1
    = c213 B1 = B1^t.

    B1 = c132 A, B2 = c231 A => B2 = c231 c132^-1 B1 = c231 c132 B1
    = c321 B1 -- non-elementary.

    B1 = c312 A, B2 = c321 A => B2 = c321 c312^-1 B1 = c321 c231 B1
    = c132 B1 -- non-elementary.

    B1 = c321 A, B2 = c231 A => B2 = c231 c321^-1 B = c231 c321 B1
    = c213 B1 = B1^t.

   So we have three groups: {c123, c213=c123^t},
                            {c231, c321=c231^t},
                            {c312, c132=c312^t}.

Indeed quite obvious that
 (a) c213 is row-inversion, c312 is column-inversion (swapping the
     result-place with the argument-place).
 (b) (c123, c213), (c231, c321), (c312, c132) are transposition-pairs
     (just swapping the first two places).


3. What about the two versions for the constraints on the versions ?

   c132 A with the first condition says that B is *right*-inverse to A,
   while the second condition says that B is *left*-inverse to A.
   Due to finiteness, both conditions are equivalent for latin squares,
   but perhaps they are different "below ls"? And perhaps they differ in
   behaviour?

   Perhaps one has two versions for the two non-trivial main representatives?

   There is also the question of charactersing when the 4 non-trivial
   conjugates (affecting the value) exist.


4. DONE (needs documentation>
   New unary conditions: "box"

   Let k be maximal with k^2 <= N, that is, k = floor(sqrt(N)).
   Beginning bottom-left ((0,0)), without gap partition the whole square into
   kxk boxes, and on each box pose an all-different constraint.
   If N is a square, then these are the (big) Sudoku boxes, and so together
   with the ls-condition we get the (big) Sudoku condition.

   Nor N=8 say we get 4*4=16 2x2 boxes (complete partition of the square),
   for N=7 we have 3*3=9 2x2 boxes, and the upper and right edge-cell are not
   covered.

5. Provide SAT-encoding

    - With a global option for using the linear-size AMO-condition (see
      module LatinSquares, there in Mols.cpp).
    - Compare with NQueens.cpp and LatinSquares/Encoding.hpp.

*/

#ifndef ENCODING_HqEmYk6s0p
#define ENCODING_HqEmYk6s0p

#include <vector>
#include <istream>
#include <utility>
#include <exception>
#include <string>
#include <sstream>

#include <cmath>
#include <cstdint>

#include <gecode/int.hh>

#include "Conditions.hpp"
#include "PartialSquares.hpp"
#include "Verification.hpp"
#include "GcVariables.hpp"

namespace Encoding {

  namespace GC = Gecode;
  namespace CD = Conditions;
  namespace PS = PartialSquares;
  namespace GV = GcVariables;

  typedef CD::size_t size_t;


  struct EncCond {

    const CD::AConditions ac;
    const PS::PSquares ps;
    const size_t N;
    const size_t N2;
    const size_t num_vars;
    const GC::IntPropLevel pl;

    static bool valid(const size_t N) noexcept {
      return N >= 2 and N <= 10000;
    }

    EncCond(const CD::AConditions& ac,
            const PS::PSquares& ps,
            const GC::IntPropLevel pl = {})
      : ac(ac), ps(ps), N(ps.N), N2(N*N), num_vars(ac.num_squares() * N2),
        pl(pl) {
      assert(valid(N));
      if (not ps.consistent()) {
        const CD::Square sq = std::ranges::find_if(ps.psqs, [](const auto& s){
                              return not s.consistent();}) -> s;
        std::ostringstream ss;
        ss << "ERROR[EncCond(ac,ps,pl)]: Inconsistent square-instantiation"
          " for square \"" << sq << "\".";
        throw std::runtime_error(ss.str());
      }
      if (not PS::included(ps, ac)) {
        const CD::Square sq =
          std::ranges::find_if(ps.psqs, [&ac](const auto& p){
                                 return not ac.contains(p.s);}) -> s;
        std::ostringstream ss;
        ss << "ERROR[EncCond(ac,ps,pl)]: square \"" << sq << "\" not"
          " mentioned in conditions.";
        throw std::runtime_error(ss.str());
      }
      assert(PS::valid(ps, ac));
    }

    bool operator ==(const EncCond&) const noexcept = default;

    /* The interface to Gecode-constraints */

    template <class VAR, typename SP>
    void not_equal(const SP s, const VAR& v, const size_t k) const {
      GC::rel(*s, v, GC::IRT_NQ, k, pl);
    }
    // VAR2 can be VAR or size_t:
    template <class VAR, typename SP, class VAR2>
    void equal(const SP s, const VAR& v, const VAR2& w) const {
      GC::rel(*s, v, GC::IRT_EQ, w, pl);
    }
    template <class VAV, typename SP>
    void all_equal(const SP s, const VAV& v) const {
      GC::rel(*s, v, GC::IRT_EQ, pl);
    }

    template <class VAV, class VAR, typename SP>
    void element_val(const SP s, const VAV& vv, const VAR& v, const size_t k) const {
      GC::element(*s, vv, v, k, pl);
    }
    template <class VAV, class VAR, typename SP>
    void element_var(const SP s, const VAV& vv, const VAR& v, const VAR& w) const {
      GC::element(*s, vv, v, w, pl);
    }

    template <class VAV, typename SP>
    void distinct(const SP s, const VAV& v) const {
      GC::distinct(*s, v, pl);
    }

    template <class VAV>
    std::vector<size_t> values(const VAV& v, const size_t i) const {
      std::vector<size_t> res;
      assert(v.size() >= 0 and i < size_t(v.size()));
      for (GC::IntVarValues eps(v[i]); eps(); ++eps) {
        const int val = eps.val();
        assert(val >= 0);
        res.push_back(val);
      }
      return res;
    }


    /* Translating the constraints to Gecode-constraints */

    template <class VAV, typename SP>
    void post_psquares(const VAV& va, const SP s) const {
      assert(s);
      for (const PS::PSquare& ps : ps.psqs) {
        for (size_t i = 0; i < N; ++i) {
          const auto& row = ps.ps[i];
          for (size_t j = 0; j < N; ++j) {
            const size_t ind = index(ps.s, i, j);
            const auto& c = row[j].c;
            for (size_t k = 0; k < N; ++k)
              if (c[k]) not_equal(s, va[ind], k);
          }
        }
      }
    }
    template <class VAR, class VAV, typename SP>
    void post_versions(const VAV& va, const SP s) const {
      assert(s);
      typedef std::vector<VAR> vv_t;
      for (size_t ind = 0; ind < ac.k; ++ind) {
        for (const CD::VS vs : ac.versions()[ind].choices()) {
          assert(CD::is_main_rep(vs));
          if (vs == CD::VS::id) continue;
          const Square sq(ind), csq(ind, vs);

          if (vs == CD::VS::c312) {
            for (size_t j = 0; j < N; ++j) {
              vv_t bcol; bcol.reserve(N);
              for (size_t i = 0; i < N; ++i)
                bcol.push_back(va[index(csq,i,j)]);
              for (size_t k = 0; k < N; ++k)
                element_val(s, bcol, va[index(sq,j,k)], k);
            }
          }
          else {
            assert(vs == CD::VS::c231);
            for (size_t i = 0; i < N; ++i) {
              vv_t brow; brow.reserve(N);
              for (size_t j = 0; j < N; ++j)
                brow.push_back(va[index(csq,i,j)]);
              for (size_t k = 0; k < N; ++k)
                element_val(s, brow, va[index(sq,k,i)], k);
            }
          }
        }
      }
    }

    size_t t(const size_t i) const noexcept {
      assert(i < N);
      return N-i-1;
    }
    typedef std::pair<size_t, size_t> co_t; // coordinates in a square
    // Reflection at diagonal and/or antidiagonal:
    co_t t(const co_t x, const bool diag, const bool antidiag) const noexcept {
      if (diag) {
        if (antidiag) return {t(x.first), t(x.second)};
        else return {x.second, x.first};
      }
      else {
        if (antidiag) return {t(x.second), t(x.first)};
        else return x;
      }
    }

    typedef std::int64_t signed_t;
    typedef CD::Square Square;
    template <class VAR, class VA, typename SP>
    void post_unary(const VA& va, const SP s) const {
      assert(s);
      typedef std::vector<VAR> vv_t;
      using CD::UC;
      for (const auto& [uc, S] : ac.map())
        for (const Square sq : S.sqs()) {
          switch(uc) {
          case UC::rls : {
            for (size_t i = 0; i < N; ++i) { vv_t vv;
              for (size_t j = 0; j < N; ++j) vv.push_back(va[index(sq,i,j)]);
              distinct(s, vv);
            } break; }
          case UC::cls : {
            for (size_t j = 0; j < N; ++j) { vv_t vv;
              for (size_t i = 0; i < N; ++i) vv.push_back(va[index(sq,i,j)]);
              distinct(s, vv);
            } break; }
          case UC::ls : {
            for (size_t i = 0; i < N; ++i) { vv_t vv;
              for (size_t j = 0; j < N; ++j) vv.push_back(va[index(sq,i,j)]);
              distinct(s, vv);
            }
            for (size_t j = 0; j < N; ++j) { vv_t vv;
              for (size_t i = 0; i < N; ++i) vv.push_back(va[index(sq,i,j)]);
              distinct(s, vv);
            } break; }
          case UC::diag : { vv_t vv;
            for (size_t i = 0; i < N; ++i) vv.push_back(va[index(sq,i,i)]);
            distinct(s, vv); break; }
          case UC::antidiag : { vv_t vv;
            for (size_t i = 0; i < N; ++i) vv.push_back(va[index(sq,i,t(i))]);
            distinct(s, vv); break; }
          case UC::uni : { vv_t vv;
            for (size_t i = 0; i < N; ++i) vv.push_back(va[index(sq,i,i)]);
            all_equal(s, vv); break; }
          case UC::antiuni : { vv_t vv;
            for (size_t i = 0; i < N; ++i) vv.push_back(va[index(sq,i,t(i))]);
            all_equal(s, vv); break; }
          case UC::idem : {
            for (size_t i = 0; i < N; ++i) equal(s, va[index(sq,i,i)], i);
            break; }
          case UC::antiidem : {
            for (size_t i = 0; i < N; ++i) equal(s, va[index(sq,i,t(i))], i);
            break; }
          case UC::moddiag : {
            for (size_t diff = 0; diff < N; ++diff) { vv_t vv;
              for (size_t i = 0; i < N; ++i)
                vv.push_back(va[index(sq, i, (diff + i) % N)]);
              distinct(s, vv);
            } break; }
          case UC::modantidiag : {
            for (size_t sum = N; sum < 2*N; ++sum) { vv_t vv;
              for (size_t i = 0; i < N; ++i)
                vv.push_back(va[index(sq, i, (sum - i) % N)]);
              distinct(s, vv);
            } break; }
          case UC::queendiag : {
            const signed_t sN = N;
            for (signed_t diff = -sN + 2; diff <= sN - 2; ++diff) { vv_t vv;
              const signed_t lb = diff <= 0 ? 0 : diff,
                ub = diff >= 0 ? sN : sN + diff;
              for (signed_t i = lb; i < ub; ++i)
                vv.push_back(va[index(sq, i, i-diff)]);
              distinct(s, vv);
            } break; }
          case UC::queenantidiag : {
            for (size_t sum = 1; sum < 2*N - 2; ++sum) { vv_t vv;
              const size_t lb = sum >= N ? (sum+1) - N : 0,
                ub = std::min(N, sum+1);
              for (size_t i = lb; i < ub; ++i)
                vv.push_back(va[index(sq, i, sum-i)]);
              distinct(s, vv);
            } break; }
          case UC::rred : {
            for (size_t i = 0; i < N; ++i) equal(s, va[index(sq,0,i)], i);
            break; }
          case UC::orred : {
            for (size_t i = 0; i < N; ++i) equal(s, va[index(sq,t(0),i)], i);
            break; }
          case UC::cred : {
            for (size_t i = 0; i < N; ++i) equal(s, va[index(sq,i,0)], i);
            break; }
          case UC::ocred : {
            for (size_t i = 0; i < N; ++i) equal(s, va[index(sq,i,t(0))], i);
            break; }
          case UC::red : {
            for (size_t i = 0; i < N; ++i) equal(s, va[index(sq,0,i)], i);
            for (size_t i = 0; i < N; ++i) equal(s, va[index(sq,i,0)], i);
            break; }
          case UC::ored : {
            for (size_t i = 0; i < N; ++i) equal(s, va[index(sq,t(0),i)], i);
            for (size_t i = 0; i < N; ++i) equal(s, va[index(sq,i,t(0))], i);
            break; }
          case UC::wcred : {
            for (size_t i = 0; i < N-2; ++i)
              for (size_t e = i+2; e < N; ++e)
                not_equal(s, va[index(sq,i,0)], e);
            break; }
          case UC::box : {
            if (N <= 3) break;
            const size_t b = std::sqrt(N);
            assert(1 <= b and b*b <= N and (b+1)*(b+1) > N);
            const size_t q = N / b;
            for (size_t i = 0; i < q; ++i) {
              const size_t x = i*b;
              for (size_t j = 0; j < q; ++j) {
                const size_t y = j*b;
                vv_t vv;
                for (size_t i1 = 0; i1 < b; ++i1)
                  for (size_t j1 = 0; j1 < b; ++j1)
                    vv.push_back(va[index(sq,x+i1,y+j1)]);
                assert(vv.size() == b*b);
                distinct(s, vv);
              }
            }
            const size_t r = N % b;
            if (r == 0) break;
            for (size_t i = 0; i < q; ++i) {
              const size_t x = i*b, y = q*b;
              vv_t vv;
              for (size_t i1 = 0; i1 < b; ++i1)
                for (size_t j1 = 0; j1 < r; ++j1)
                  vv.push_back(va[index(sq,x+i1,y+j1)]);
              assert(vv.size() == b*r);
              distinct(s, vv);
            }
            for (size_t j = 0; j < q; ++j) {
              const size_t x = q*b, y = j*b;
              vv_t vv;
              for (size_t i1 = 0; i1 < r; ++i1)
                for (size_t j1 = 0; j1 < b; ++j1)
                  vv.push_back(va[index(sq,x+i1,y+j1)]);
              assert(vv.size() == b*r);
              distinct(s, vv);
            }
            if (r == 1) break;
            {const size_t x = q*b, y = q*b;
             vv_t vv;
             for (size_t i1 = 0; i1 < r; ++i1)
               for (size_t j1 = 0; j1 < r; ++j1)
                 vv.push_back(va[index(sq,x+i1,y+j1)]);
             assert(vv.size() == r*r);
             distinct(s, vv);
            }
            break; }
          case UC::symm : {
            for (size_t i = 0; i < N-1; ++i)
              for (size_t j = i+1; j < N; ++j)
                equal(s, va[index(sq,i,j)], va[index(sq,j,i)]);
            break; }
          case UC::antisymm : {
            for (size_t i = 0; i < N-1; ++i)
              for (size_t j = 0; j < N-i-1; ++j)
                equal(s, va[index(sq,i,j)], va[index(sq,t(j),t(i))]);
            break; }
          case UC::mention : break;
          default : throw std::runtime_error("ERROR[post_unary]: UNKNOWN uc="
                                             +std::to_string(size_t(uc)));}
        }
    }
    template <class VA, typename SP>
    void post_equations(const VA& va, const SP s) const {
      assert(s);
      for (const CD::Equation e : ac.eq()) {
        for (size_t i = 0; i < N; ++i)
          for (size_t j = 0; j < N; ++j)
            equal(s, va[index(e.lhs(),i,j)], va[index(e.rhs(),i,j)]);
      }
    }
    template <class VAR, class VA, typename SP>
    void post_prod_equations(const VA& va, const SP s) const {
      assert(s);
      typedef std::vector<VAR> vv_t;
      for (const CD::ProdEq& p : ac.peq()) {
        const Square A = p.r(), B = p.f2(), C = p.f1(); // A = B C
        if (p.pt() == CD::PT::rprod) {
          for (size_t i = 0; i < N; ++i) {
            vv_t brow; brow.reserve(N);
            for (size_t j = 0; j < N; ++j) brow.push_back(va[index(B,i,j)]);
            for (size_t j = 0; j < N; ++j) {
              element_var(s, brow, va[index(C,i,j)], va[index(A,i,j)]);
            }
          }
        }
        else {
          assert(p.pt() == CD::PT::cprod);
          for (size_t j = 0; j < N; ++j) {
            vv_t bcol; bcol.reserve(N);
            for (size_t i = 0; i < N; ++i) bcol.push_back(va[index(B,i,j)]);
            for (size_t i = 0; i < N; ++i) {
              element_var(s, bcol, va[index(C,i,j)], va[index(A,i,j)]);
            }
          }
        }
      }
    }


    // Return the variable-vector, with constraints imbued, to be used
    // in the node-class (derived frm GC::Space):
    template <class VAV, class VAR, typename SP>
    VAV post(const SP s) const {
      assert(s);
      VAV va(*s, num_vars, 0, N-1);
      post_psquares(va, s);
      post_versions<VAR>(va, s);
      post_unary<VAR>(va, s);
      post_equations(va, s);
      post_prod_equations<VAR>(va, s);
      return va;
    }


    // The encoding of variables */

    size_t index(const Square s) const noexcept {
      assert(ac.valid(s));
      return ac.index(s) * N2;
    }
    size_t index(const Square s, const size_t i, const size_t j) const noexcept {
      assert(i < N and j < N);
      const auto [ti, tj] = t({i,j}, CD::with_t(s.v), CD::with_at(s.v));
      return index(s) + ti * N + tj;
    }


    /* Decoding solutions */

    // The initial domain-restriction:
    PS::PSquares full_tass() const {
      PS::psquares_t P; P.reserve(ac.num_squares());
      const auto f = PS::full_psquare(N);
      for (size_t i = 0; i < ac.versions().size(); ++i)
        for (const CD::VS v : ac.versions()[i].choices())
          P.emplace_back(f, Square{i,v});
      assert(P.size() == ac.num_squares());
      return {N, P};
    }

    template <class VAV>
    PS::PSquares decode(const VAV& va) const {
      assert(size_t(va.size()) == num_vars);
      PS::PSquares res = full_tass();
      size_t v = 0; // index for va
      for (PS::PSquare& s : res.psqs)
        for (size_t i = 0; i < N; ++i) {
          PS::prow_t& row = s.ps[i];
          for (size_t j = 0; j < N; ++j, ++v) {
            PS::Cell& c = row[j];
            for (const size_t val : values(va, v)) c.c[val] = 0;
          }
        }
      assert(v == num_vars);
      assert(Verification::correct(ac, res));
      return res;
    }
    PS::PSquares decode(const GV::solutions_t& va) const {
      assert(size_t(va.size()) == num_vars);
      PS::PSquares res = full_tass();
      size_t v = 0; // index for va
      for (PS::PSquare& s : res.psqs)
        for (size_t i = 0; i < N; ++i) {
          PS::prow_t& row = s.ps[i];
          for (size_t j = 0; j < N; ++j, ++v) {
            PS::Cell& c = row[j];
            const int val = va[v];
            assert(val >= 0);
            c.c[val] = 0;
          }
        }
      assert(v == num_vars);
      assert(Verification::correct(ac, res));
      return res;
    }
    template <class LIST>
    std::vector<PS::PSquares> ldecode(const LIST& lva) const {
      std::vector<PS::PSquares> res;
      for (const auto& va : lva) res.push_back(decode(va));
      return res;
    }

    template <class VAV>
    static bool unit(const VAV& va) noexcept {
      for (size_t i = 0; i < size_t(va.size()); ++i) {
        if (va[i].size() != 1) return false;
      }
      return true;
    }

  };

}

#endif

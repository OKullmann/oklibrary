// Oleg Zaikin, 7.4.2021 (Irkutsk)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  A trivial class Sum is inherited from Gecode::Space.

  For arguments (sz, a, b) an object carrying an integer-array V of size sz and
  with values in {a,...,b} is created.

  If sz>2, then constraints for a linear equation V[0] + ... + V[sz-2] = V[sz-1]
  are added, the problem is to find all integer arrays V subject to the constraints.

  Example: sz=3, a=0, b=1.
  The equation is V[0] + V[1] = V[2].
  The solutions are: (0, 0, 0), (0, 1, 1), (1, 0, 1).


  TODOS:

  0. Documentation is needed (with references to the appropriate parts of
     the documentation.

  1. Make function measure() adaptive:
    - Currently mu0 or mu1 is hardcoded.
    - What really is needed is a "distance".
    - Perhaps a function-object is easiest (as a data-member).
    - Using std::function is likely easiest.

  2. The copy-constructor is faulty (non-const argument) !! OZ
    - We need references from Gecode, where and why such a constructor is
      needed.
    - If this misuse of a copy-constructor is part of the Gecode-library,
      one needs to see how to possibly work around that.
    - Possibly the real copy-constructor provided (creating an independent
      copy) does not work in the context of the Gecode-library.
    - It seems that the issue with copy constructor is not discussed directly
      in the Gecode documentation.
    - In the mailing list
      https://www.gecode.org/users-archive/2006-March/000439.html
      the following recomendation is given when a copy of a space in needed:
      'Never use copy! or the constructor! Use the clone method instead
      (which, in turn uses the copy method).'
    - In Section 9.1.1 of the Gecode documentation (version 6.2.0, May 28, 2019)
      it is stated that: 'Cloning creates a clone of a space. A clone and the
      original space are of course equivalent. Restoration with cloning is
      straightforward: before following a particular alternative during search,
      a clone of the space is made and used later if necessary.'
    - If in a cloned space any Gecode variable (e.g. of type IntVar), which was
      made in the parent space, is used, then any constraints for the variable
      affect the parent space, not the cloned one.
    - It is not clear whether this issue is discussed anywhere.
    - If in a cloned space no constraints are added for Gecode variables from
      the parent space, then the cloned space can be treated as an independent
      one.This is enough for lookahead. This is how it is already implemented
      in branching_lookahead().
    - It seems that the provided real copy-constructor (that deals with const
      Sum& s) should be removed.

  3. Call of status() likely needs a check for early abortion.

  4. Later: we don't want to handle variables, but branchers.
    - We can't restrict to just branching on values.
    - We want access to the given possibilities for branching.

*/

#ifndef TRIVIAL_UeAozKZjBa
#define TRIVIAL_UeAozKZjBa

#include <memory>

#include <gecode/int.hh>

#include "Lookahead.hpp"

namespace Trivial {

  namespace LA = Lookahead;

  bool operator==(const Gecode::IntVarArray& lhs, const Gecode::IntVarArray& rhs) noexcept {
    if (lhs.size() != rhs.size()) return false;
    const auto size = LA::tr(lhs.size());
    for (LA::size_t i = 0; i < size; ++i) {
      if (lhs[i].size() != rhs[i].size()) return false;
      for (Gecode::IntVarValues jl(lhs[i]), jr(rhs[i]); jl(); ++jl, ++jr)
        if (jl.val() != jr.val()) return false;
    }
    return true;
  }

  class Sum : public Gecode::Space {
  protected:
    Gecode::IntVarArray V;
    const LA::size_t sz, a, b;
  public:

    Sum(const LA::size_t sz, const LA::size_t a, const LA::size_t b) noexcept :
      V(*this, sz, a, b), sz(sz), a(a), b(b) {
      assert(sz > 0 and a <= b);
      // Add a linear equation V[0] + ... + V[sz-2] = V[sz-1]:
      Gecode::IntArgs c(sz); Gecode::IntVarArgs x(sz);
      for (LA::size_t i = 0; i < sz-1; ++i) c[i] = 1;
      c[sz-1] = -1;
      for (LA::size_t i = 0; i < sz; ++i) x[i] = V[i];
      Gecode::linear(*this, c, x, Gecode::IRT_EQ, 0);
    }

    Sum(Sum& s) : Gecode::Space(s), sz(s.sz), a(s.a), b(s.b) {
      V.update(*this, s.V);
    }

    virtual Gecode::Space* copy() noexcept { return new Sum(*this); }
    void print() const noexcept { std::cout << V << "\n"; }

    LA::size_t size() const noexcept { return V.size(); }

    bool valid (const LA::size_t i) const noexcept {return i<LA::tr(V.size());}

    friend bool operator ==(const Sum& lhs, const Sum& rhs) noexcept {
      return lhs.V == rhs.V;
    }
    friend bool operator !=(const Sum& lhs, const Sum& rhs) noexcept {
      return not (lhs.V == rhs.V);
    }

    void constr_var_eq(const LA::size_t v, const LA::size_t val) noexcept {
      assert(valid(v));
      Gecode::rel(*this, V[v], Gecode::IRT_EQ, val);
    }

    LA::float_t mu0() const noexcept { return LA::mu0(V); }
    LA::float_t mu1() const noexcept { return LA::mu1(V); }
    LA::float_t measure() const noexcept { return mu0(); }

    LA::float_t la_measure(const LA::size_t v, const LA::size_t val) noexcept {
      // Clone space:
      std::unique_ptr<Sum> c(static_cast<Sum*>(this->clone()));
      assert(c->valid(v));
      // Add an equality constraint for the given variable and its value:
      c->constr_var_eq(v, val);
      // Propagate:
      c->status();
      // Measure the simplified formula
      const float_t f = c->measure();
      return f;
    }

    void branching_min_var_size() noexcept {
      Gecode::branch(*this, V, Gecode::INT_VAR_SIZE_MIN(),
                     Gecode::INT_VAL_MIN());
    }
    void branching_lookahead() noexcept {
      // Call propagation for the current formula:
      this->status();
      // For each variable value clone a space, set value, propagate,
      // and measure:
      const auto size = LA::tr(V.size());
      for (size_t i = 0; i < size; ++i) {
        const auto v = V[i];
        if (v.assigned()) continue;
        assert(v.size() >= 2);
        for (Gecode::IntVarValues j(v); j(); ++j) {
          // Call propagation for the simplified formula:
          const float_t f = la_measure(i, j.val());
          std::cout << f << "\n";
        }
      }
      const float_t f = measure();
      std::cout << f << "\n";
      //Gecode::branch(*this, V, Gecode::INT_VAR_SIZE_MIN(), Gecode::INT_VAL_MIN());
    }

  };

}

#endif

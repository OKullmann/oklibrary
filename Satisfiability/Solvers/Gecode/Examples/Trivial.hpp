// Oleg Zaikin, 7.4.2021 (Irkutsk)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Several trivial classes inherited from Gecode::Space are implemented.
  All these classes are needed for testing.

  1. Sum
    For arguments (sz, a, b) an object carrying an integer-array V of size sz and
    with values in {a,...,b} is created.

    If sz>2, then constraints for a linear equation V[0] + ... + V[sz-2] = V[sz-1]
    are added, the problem is to find all integer arrays V subject to the constraints.

    Example: sz=3, a=0, b=1.
    The equation is V[0] + V[1] = V[2].
    The solutions are: (0, 0, 0), (0, 1, 1), (1, 0, 1).

  2. OneNodeNoSolution
    In the constructor, for only one existing Gecode integer variable v two
    contradictory constraints are posted: v=0 and v!=0. As a result, the search tree
    contains only one node, while the problem has no solutions.

  3. OneNodeOneSolution
    No internal variables are used. Even in this case the corresponding search tree
    is not empty and has one node, while the problem has one solution.


  TODOS:

  0. Documentation is needed (with references to the appropriate parts of
     the documentation.

  1. Make function measure() adaptive:
    - Currently mu0 or mu1 is hardcoded.
    - What really is needed is a "distance".
    - Perhaps a function-object is easiest (as a data-member).
    - Using std::function is likely easiest.

  2. DONE The copy-constructor is faulty (non-const argument) !! OZ
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

  4. Later: we don't want to handle variables, but branchers.
    - We can't restrict to just branching on values.
    - We want access to the given possibilities for branching.

*/

#ifndef TRIVIAL_UeAozKZjBa
#define TRIVIAL_UeAozKZjBa

#include <memory>
#include <iostream>

#include <cassert>

#include <gecode/int.hh>

#include "../Lookahead.hpp"

namespace Trivial {

  namespace GC = Gecode;
  namespace LA = Lookahead;

  typedef GC::IntVarArray IntVarArray;
  typedef LA::BrTypeO BrTypeO;
  typedef LA::BrSourceO BrSourceO;

  class OneNodeOneSolution : public GC::Space {
    const BrTypeO brt;
    const BrSourceO brs;
  public:
    OneNodeOneSolution( const BrTypeO brt, const BrSourceO brs ) noexcept : brt(brt), brs(brs) { }
    OneNodeOneSolution(OneNodeOneSolution& s) : GC::Space(s), brt(s.brt), brs(s.brs) { }
    virtual GC::Space* copy() noexcept { return new OneNodeOneSolution(*this); }
    LA::size_t size() const noexcept { return 0; }
    bool valid() const noexcept { return true; }
    BrTypeO branching_type() const noexcept { assert(valid()); return brt; }
    BrSourceO branching_source() const noexcept { assert(valid()); return brs; }
    void print() const noexcept {}
    void print(std::ostream&) const noexcept {}
  };

  class OneNodeNoSolution : public GC::Space {
    IntVarArray V;
    const BrTypeO brt;
    const BrSourceO brs;
  public:
    OneNodeNoSolution( const BrTypeO brt, const BrSourceO brs) noexcept :
                         V(*this, 1, 0, 0), brt(brt), brs(brs) {
      assert(valid(V));
      GC::rel(*this, V[0], GC::IRT_EQ, 0);
      GC::rel(*this, V[0], GC::IRT_NQ, 0);
    }
    OneNodeNoSolution(OneNodeNoSolution& s) : GC::Space(s), brt(s.brt), brs(s.brs) {
      assert(valid(s.V));
      V.update(*this, s.V);
      assert(valid(V));
    }
    virtual GC::Space* copy() noexcept { return new OneNodeNoSolution(*this); }
    LA::size_t size() const noexcept { return V.size(); }
    bool valid () const noexcept { return valid(V); }
    bool valid (const IntVarArray V) const noexcept { return V.size() == 1; }
    BrTypeO branching_type() const noexcept { assert(valid()); return brt; }
    BrSourceO branching_source() const noexcept { assert(valid()); return brs; }
    void print() const noexcept {
      assert(valid(V)); std::cout << V << std::endl;
    }
    void print(std::ostream& os) const noexcept {
      assert(valid(V)); os << V << std::endl;
    }
  };

  class Sum : public GC::Space {
  protected:
    IntVarArray V;
    const LA::size_t sz, a, b;
    const BrTypeO brt;
    const BrSourceO brs;

  public:

    Sum(const LA::size_t sz, const LA::size_t a, const LA::size_t b,
        const BrTypeO brt, const BrSourceO brs) noexcept :
      V(*this, sz, a, b), sz(sz), a(a), b(b), brt(brt), brs(brs) {
      assert(valid(V));
      assert(sz > 0 and a <= b);

      // Add a linear equation V[0] + ... + V[sz-2] = V[sz-1]:
      GC::IntArgs c(sz); GC::IntVarArgs x(sz);
      assert(c.size() > 0 and x.size() > 0);
      for (LA::size_t i = 0; i < sz-1; ++i) c[i] = 1;
      c[sz-1] = -1;
      for (LA::size_t i = 0; i < sz; ++i) x[i] = V[i];
      GC::linear(*this, c, x, GC::IRT_EQ, 0);

      // Post branching:
      LA::post_branching<Sum>(*this, V, brt, brs);
    }

    bool valid () const noexcept { return valid(V); }
    static bool valid (const IntVarArray& V) noexcept { return V.size() > 0; }
    bool valid (const LA::size_t i) const noexcept { return i<LA::tr(V.size()); }

    inline GC::IntVar at(const LA::size_t i) const noexcept {
      assert(valid()); assert(valid(i));
      return V[i];
    }
    inline GC::IntVarArray at() const noexcept { assert(valid()); return V; }

    Sum(Sum& s) : GC::Space(s), sz(s.sz), a(s.a), b(s.b), brt(s.brt), brs(s.brs) {
      assert(valid(s.V));
      V.update(*this, s.V);
      assert(valid(V));
    }

    virtual GC::Space* copy() noexcept { return new Sum(*this); }
    inline LA::size_t size() const noexcept { return V.size(); }

    BrTypeO branching_type() const noexcept { assert(valid()); return brt; }
    BrSourceO branching_source() const noexcept { assert(valid()); return brs; }

    void print() const noexcept { assert(valid(V)); std::cout << V << std::endl; }
    void print(std::ostream& os) const noexcept {
      assert(valid(V)); os << V << std::endl;
    }

  };

}

#endif

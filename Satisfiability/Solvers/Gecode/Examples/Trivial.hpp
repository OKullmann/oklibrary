// Oleg Zaikin, 7.4.2021 (Irkutsk)
/* Copyright 2021, 2022 Oliver Kullmann
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

*/

#ifndef TRIVIAL_UeAozKZjBa
#define TRIVIAL_UeAozKZjBa

#include <memory>
#include <iostream>

#include <cassert>

#include <gecode/int.hh>

#include "../Lookahead.hpp"
#include "../Statistics.hpp"

namespace Trivial {

  namespace GC = Gecode;
  namespace LA = Lookahead;

  typedef GC::IntVarArray IntVarArray;
  typedef LA::option_t option_t;
  typedef LA::weights_t weights_t;
  typedef LA::statistics_t statistics_t;

  class OneNodeOneSolution : public LA::Node {
    const option_t options;
    statistics_t stat;
  public:
    OneNodeOneSolution( const option_t options, statistics_t stat ) noexcept :
      options(options), stat(stat) { }
    OneNodeOneSolution(OneNodeOneSolution& s) : LA::Node(s), options(s.options),
      stat(s.stat) { }
    virtual GC::Space* copy() noexcept { return new OneNodeOneSolution(*this); }
    LA::size_t size() const noexcept { return 0; }
    bool valid() const noexcept { return true; }
    option_t branching_options() const noexcept { assert(valid()); return options; }
    statistics_t statistics() const noexcept { assert(valid()); return stat; }
    void print() const noexcept {}
    void print(std::ostream&) const noexcept {}
  };

  class OneNodeNoSolution : public LA::Node {
    IntVarArray V;
    const option_t options;
    statistics_t stat;
  public:
    OneNodeNoSolution(const option_t options, statistics_t stat) noexcept :
      V(*this, 1, 0, 0), options(options), stat(stat) {
      assert(valid(V));
      GC::rel(*this, V[0], GC::IRT_EQ, 0);
      GC::rel(*this, V[0], GC::IRT_NQ, 0);
    }
    OneNodeNoSolution(OneNodeNoSolution& s) : LA::Node(s), options(s.options),
     stat(s.stat) {
      assert(valid(s.V));
      V.update(*this, s.V);
      assert(valid(V));
    }
    virtual GC::Space* copy() noexcept { return new OneNodeNoSolution(*this); }
    LA::size_t size() const noexcept { return V.size(); }
    bool valid () const noexcept { return valid(V); }
    bool valid (const IntVarArray V) const noexcept { return V.size() == 1; }
    option_t branching_options() const noexcept { assert(valid()); return options; }
    statistics_t statistics() const noexcept { assert(valid()); return stat; }
    void print() const noexcept {
      assert(valid(V)); std::cout << V << std::endl;
    }
    void print(std::ostream& os) const noexcept {
      assert(valid(V)); os << V << std::endl;
    }
  };

  class Sum : public LA::Node {
  protected:
    IntVarArray V;
    const LA::size_t sz, a, b;
    const option_t options;
    const LA::statistics_t stat;
    const LA::weights_t wghts;

  public:

    Sum(const LA::size_t sz, const LA::size_t a, const LA::size_t b,
        const option_t options, const LA::statistics_t stat = nullptr,
        const LA::weights_t wghts = nullptr) noexcept :
        V(*this, sz, a, b), sz(sz), a(a), b(b), options(options),
          stat(stat), wghts(wghts) {
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
      LA::post_branching<Sum>(*this, V, options);
    }

    bool valid () const noexcept { return valid(V); }
    static bool valid (const IntVarArray& V) noexcept { return V.size() > 0; }
    bool valid (const LA::size_t i) const noexcept { return i<LA::tr(V.size()); }

    inline GC::IntVar at(const LA::size_t i) const noexcept {
      assert(valid()); assert(valid(i));
      return V[i];
    }
    inline GC::IntVarArray at() const noexcept { assert(valid()); return V; }

    Sum(Sum& s) : LA::Node(s), sz(s.sz), a(s.a), b(s.b), options(s.options),
      stat(s.stat), wghts(s.wghts) {
      assert(valid(s.V));
      V.update(*this, s.V);
      assert(valid(V));
    }

    virtual GC::Space* copy() noexcept { return new Sum(*this); }
    inline LA::size_t size() const noexcept { return V.size(); }

    option_t branching_options() const noexcept { assert(valid()); return options; }

    weights_t weights() const noexcept { assert(valid()); return nullptr; }

    statistics_t statistics() const noexcept { assert(valid()); return stat; }

    void print() const noexcept { assert(valid(V)); std::cout << V << std::endl; }
    void print(std::ostream& os) const noexcept {
      assert(valid(V)); os << V << std::endl;
    }

  };

}

#endif

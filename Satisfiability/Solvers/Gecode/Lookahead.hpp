// Oleg Zaikin, 5.4.2021 (Irkutsk)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

    Lookahead for the Gecode library

 TODOS:

1. Is it appropriate to pass Gecode::IntVarArray by copy?

  - Copying would only be appropriate if the internal data stored
    is just a pointer, plus a bit more data.
  - The base class Gecode::VarArray
      https://www.gecode.org/doc-latest/reference/kernel_2data_2array_8hpp_source.html
    seems to contain exactly two data-members:
      int n;
      Var* x;
  - Thus possibly copying is safe and efficient.

*/

#ifndef LOOKAHEAD_lNFKYYpHQ8
#define LOOKAHEAD_lNFKYYpHQ8

#include <iostream>
#include <limits>
#include <vector>

#include <cmath>
#include <cassert>

#include <gecode/int.hh>

#include <Numerics/FloatingPoint.hpp>

namespace Lookahead {

  namespace FP = FloatingPoint;
  namespace GC = Gecode;

  typedef unsigned size_t;
  typedef FP::float80 float_t;
  typedef GC::ViewArray<GC::Int::IntView> IntView;
  typedef std::vector<int> values_t;

  inline bool show_usage(const Environment::ProgramInfo proginfo,
                         const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " [visual]\n\n" <<
    "visual    : \"-gist\" (run Gist to visualise the search tree).\n\n";
    return true;
  }

  inline constexpr size_t tr(const int size, [[maybe_unused]] const size_t bound = 0) noexcept {
    assert(bound <= std::numeric_limits<int>::max());
    assert(size >= int(bound));
    return size;
  }

  inline float_t mu0(const GC::IntVarArray V) noexcept {
    float_t s = 0;
    for (const auto& v : V) {
      const auto is = tr(v.size(), 1);
      s += is - 1;
    }
    return s;
  }

  inline float_t mu1(const GC::IntVarArray V) noexcept {
    float_t s = 0;
    for (const auto& v : V) {
      const auto is = tr(v.size(), 1);
      s += FloatingPoint::log2(is);
    }
    return s;
  }

  class NarySizeMin : public GC::Brancher {
    IntView x;
    mutable int start;

    struct VarVal : public GC::Choice {
      int pos;
      values_t values;

      VarVal(const NarySizeMin& b, const int p, const values_t V)
        : GC::Choice(b, V.size()), pos(p), values(V) {}

      virtual void archive(GC::Archive& e) const {
        GC::Choice::archive(e);
        size_t width = tr(values.size());
        e << width << pos;
        for (auto v : values) e << v;
      }
    };

  public:

    NarySizeMin(const GC::Home home, const IntView& x)
      : GC::Brancher(home), x(x), start(0) {}
    NarySizeMin(GC::Space& home, NarySizeMin& b)
      : GC::Brancher(home,b), start(b.start) {
      x.update(home, b.x);
    }

    static void post(GC::Home home, const IntView& x) {
      new (home) NarySizeMin(home, x);
    }
    virtual std::size_t dispose(GC::Space& home) {
      GC::Brancher::dispose(home);
      return sizeof(*this);
    }
    virtual GC::Brancher* copy(GC::Space& home) {
      return new (home) NarySizeMin(home, *this);
    }
    virtual bool status(const GC::Space&) const {
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; return true; }
      return false;
    }

    virtual GC::Choice* choice(GC::Space&) {
      int pos = start;
      auto width = tr(x[pos].size());
      for (auto i = start + 1; i < x.size(); ++i)
        if (not x[i].assigned() and x[i].size() < width) {
          pos = i; width = tr(x[pos].size());
        }
      values_t values;
      for (GC::Int::ViewValues i(x[pos]); i(); ++i)
        values.push_back(i.val());
      return new VarVal(*this, pos, values);
    }
    virtual GC::Choice* choice(const GC::Space&, GC::Archive& e) {
      size_t width; int pos;
      e >> width >> pos;
      int v; values_t values;
      for (size_t i = 0; i < width; ++i) {
        e >> v; values.push_back(v);
      }
      return new VarVal(*this, pos, values);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) {
      const VarVal& pv = static_cast<const VarVal&>(c);
      assert(branch < pv.values.size());
      return GC::me_failed(x[pv.pos].eq(home, pv.values[branch])) ?
             GC::ES_FAILED : GC::ES_OK;
    }

    virtual void print(const GC::Space&, const GC::Choice& c,
                       const unsigned branch, std::ostream& out) const {
      const VarVal& pv = static_cast<const VarVal&>(c);
      const auto pos = pv.pos;
      const auto values = pv.values;
      const size_t width = tr(values.size());
      assert(width > 0);
      assert(branch < width);
      out << "branch = " << branch << "\n";
      out << "x[" << pos << "] = {";
      for (size_t i = 0; i < width-1; ++i) out << values[i] << ",";
      out << values[width-1] << "}";
    }
  };

  inline void post_narysizemin(GC::Home home, const GC::IntVarArgs& x) {
    if (home.failed()) return;
    const IntView y(home, x);
    NarySizeMin::post(home, y);
  }

}

#endif

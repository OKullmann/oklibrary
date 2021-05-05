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
  typedef std::vector<int> values;

  inline bool show_usage(const Environment::ProgramInfo proginfo,
                         const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " [visial]\n\n" <<
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
    GC::ViewArray<GC::Int::IntView> x;
    mutable int start;

    struct PosVal : public GC::Choice {
      size_t width;
      int pos;
      values val;

      PosVal(const NarySizeMin& b, const unsigned width, const int p, const values v)
        : GC::Choice(b,width), width(width), pos(p), val(v) {}

      virtual void archive(GC::Archive& e) const {
        GC::Choice::archive(e);
        e << width << pos;
        for (auto v : val) e << v;
      }
    };

  public:

    NarySizeMin(const GC::Home home, const GC::ViewArray<GC::Int::IntView>& x)
      : GC::Brancher(home), x(x), start(0) {}
    NarySizeMin(GC::Space& home, NarySizeMin& b)
      : GC::Brancher(home,b), start(b.start) {
      x.update(home, b.x);
    }

    static void post(GC::Home home, const GC::ViewArray<GC::Int::IntView>& x) {
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
      values val;
      for (GC::Int::ViewValues i(x[pos]); i(); ++i)
        val.push_back(i.val());
      return new PosVal(*this, width, pos, val);
    }
    virtual GC::Choice* choice(const GC::Space&, GC::Archive& e) {
      size_t width;
      int pos;
      values val;
      e >> width >> pos;
      int v;
      for (size_t i = 0; i < width; ++i) {
        e >> v;
        val.push_back(v);
      }
      return new PosVal(*this, width, pos, val);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned alt) {
      const PosVal& pv = static_cast<const PosVal&>(c);
      assert(alt < pv.width and alt < pv.val.size());
      return GC::me_failed(x[pv.pos].eq(home, pv.val[alt])) ?
             GC::ES_FAILED : GC::ES_OK;
    }

    virtual void print(const GC::Space&, const GC::Choice& c,
                       const unsigned alt, std::ostream& out) const {
      const PosVal& pv = static_cast<const PosVal&>(c);
      const auto width = pv.width;
      const auto pos = pv.pos;
      const auto val = pv.val;
      const auto size = tr(val.size());
      assert(alt < width);
      assert(size > 0 and size == width);
      out << "alt = " << alt << "\n";
      out << "width = " << width << "\n";
      out << "x[" << pos << "] = {";
      for (size_t i = 0; i < size-1; ++i) out << val[i] << ",";
      assert(size-1 < val.size());
      out << val[size-1] << "}";
    }
  };

  inline void post_narysizemin(GC::Home home, const GC::IntVarArgs& x) {
    if (home.failed()) return;
    const GC::ViewArray<GC::Int::IntView> y(home, x);
    NarySizeMin::post(home, y);
  }

}

#endif

// Oliver Kullmann, 26.3.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  General algorithms (tools)

   - empty_intersection(RAN r1, RAN r2) (r1, r2 must be sorted)

   - append_ranges(RAN1 r1, RAN2 r2) (copies r1, and appends to it)
   - append_ranges(RAN1 r1, RAN2 r2, RAN3 r3)


TODOS:

1. Once we switch to C++23, use std::vector::append_range
   in function append_ranges.

*/


#ifndef ALGORITHMS_iWjSrlmVWS
#define ALGORITHMS_iWjSrlmVWS

namespace Algorithms {

  // For sorted ranges decide whether their intersection is empty:
  template <class RAN>
  inline bool empty_intersection(const RAN& r1, const RAN& r2) noexcept {
    if (r1.empty() or r2.empty()) return true;
    const auto e1 = r1.end(); const auto e2 = r2.end();
    auto i1 = r1.begin(); auto i2 = r2.begin();
    if (r1.size() <= r2.size()) {
      for (;;) {
        while (*i1 < *i2) { ++i1; if (i1 == e1) return true; }
        if (*i1 == *i2) return false;
        do { ++i2; if (i2 == e2) return true; } while  (*i2 < *i1);
        if (*i2 == *i1) return false;
      }
    }
    else {
      for (;;) {
        while (*i2 < *i1) { ++i2; if (i2 == e2) return true; }
        if (*i2 == *i1) return false;
        do { ++i1; if (i1 == e1) return true; } while (*i1 < *i2);
        if (*i1 == *i2) return false;
      }
    }
    return true;
  }


  // Using implicit conversions:
  template <class RANGE1, class RANGE2>
  RANGE1 append_ranges(RANGE1 r1, const RANGE2& r2) {
    const auto size = r1.size() + r2.size();
    r1.reserve(size);
    for (const auto& x : r2) r1.push_back(x);
    assert(r1.size() == size);
    return r1;
  }
  template <class RANGE1, class RANGE2, class RANGE3>
  RANGE1 append_ranges(RANGE1 r1, const RANGE2& r2, const RANGE3& r3) {
    const auto size = r1.size() + r2.size() + r3.size();
    r1.reserve(size);
    for (const auto& x : r2) r1.push_back(x);
    for (const auto& x : r3) r1.push_back(x);
    assert(r1.size() == size);
    return r1;
  }

}

#endif

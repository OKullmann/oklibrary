// Oleg Zaikin, 11.5.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Helper functions for dealing with Gecode variables.

TODOS:

*/

#ifndef GCVARIABLES_FQM6cOCEcZ
#define GCVARIABLES_FQM6cOCEcZ

#include <vector>

#include <gecode/int.hh>
#include <gecode/search.hh>

namespace GcVariables {

  namespace GC = Gecode;

  size_t assignedvars(const GC::IntVarArray& V) noexcept {
    size_t assigned = 0;
    for (int var = 0; var < V.size(); ++var)
      if (V[var].size() == 1) ++assigned;
    return assigned;
  }

  size_t sumdomsizes(const GC::IntVarArray& V) noexcept {
    size_t sum = 0;
    for (int var = 0; var < V.size(); ++var) sum += V[var].size();
    return sum;
  }

  int assignedval(const GC::IntVarArray& V, const int var) noexcept {
    assert(V.size() > 0 and var < V.size());
    GC::Int::IntView view = V[var];
    assert(view.assigned());
    GC::IntVarValues j(view);
    return j.val();
  }

  [[deprecated]] bool valid(const GC::IntVarArray& x) noexcept {
    return x.size() > 0;
  }
  bool valid(const GC::IntVarArray& x, const int i) noexcept {
    assert(i >= 0);
    return i<x.size();
  }

}

#endif

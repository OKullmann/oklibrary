// Oleg Zaikin, 7.4.2020 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* A trivial Gecode class. For a given triple (sz, a, b) an object with an integer
array [a..b] of size sz is created.
*/

#ifndef LOOKAHEAD_UeAozKZjBa
#define LOOKAHEAD_UeAozKZjBa

#include <gecode/int.hh>


using namespace Gecode;

class Trivial : public Space {
protected:
  IntVarArray l;
public:
  Trivial(const std::uint64_t sz, const std::uint64_t min, const std::uint64_t max) :
    l(*this, sz, min, max) { }
  Trivial(Trivial& s) : Space(s) {
    l.update(*this, s.l);
  }
  virtual Space* copy(void) {
    return new Trivial(*this);
  }
  void print(void) const {
    std::cout << l << std::endl;
  }
  std::uint64_t size() {
    return (std::uint64_t)l.size();
  }
};

#endif

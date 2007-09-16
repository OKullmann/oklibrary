// Oliver Kullmann, 8.3.2003 (Swansea)
/* Copyright 2003 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef VALUEWAECHTER

#define VALUEWAECHTER

#include <ostream>

// DEPRICATED
// to be eliminated soon

namespace Values {

  class BooleanValues {
  public :
    enum values {zero = 0, one = 1};
    explicit BooleanValues(values v) : v(v) {}
    // Attention: conversion constructor!
    BooleanValues(const BooleanValues& v) : v(v.v) {}
    BooleanValues& operator =(const BooleanValues& val) {
      v = val.v; return *this;
    }
    BooleanValues& operator ++() {
      v = negate(v);
      return *this;
    }
    BooleanValues operator ++(int) {
      BooleanValues s(*this);
      v = negate(v);
      return s;
    }
    bool operator <(BooleanValues val) const {
      return v < val.v;
    }
    bool operator ==(BooleanValues val) const {
      return v == val.v;
    }
    bool operator !=(BooleanValues val) const {
      return not (*this == val);
    }
    BooleanValues& add(BooleanValues val) {
      if (val.v == one)
	v = negate(v);
      return *this;
    }
    
  private :
    values v;

    friend inline std::ostream& operator <<(std::ostream& os, BooleanValues v) {
      return os << v.v;
    }

    static values negate(values val) {
      switch(val) {
      case zero : return one;
      case one : return zero;
      }
    }
  };

  inline BooleanValues operator +(BooleanValues lhs, BooleanValues rhs) {
    BooleanValues bv(lhs);
    return bv.add(rhs);
  }

  const BooleanValues pos(BooleanValues::zero);
  const BooleanValues neg(BooleanValues::one);

}

#endif

// Oliver Kullmann, 3.8.2003 (Swansea)
/* Copyright 2003 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef ALGEBRAICSTRUCTURES

#define ALGEBRAICSTRUCTURES

#include <iterator>

namespace AlgebraicStructures {

  class SignRing {

    enum Signs {Neg = -1, Nu = 0, Pos = 1};
    
    Signs s;

    SignRing(Signs s) : s(s) {}

  public :

    typedef ConceptDefinitions::SignRing_tag Concept;

    SignRing() : s(Nu) {}
    SignRing(const SignRing& s) : s(s.s) {}
    SignRing& operator =(SignRing rhs) {
      s = rhs.s;
      return *this;
    }
    friend inline bool operator ==(SignRing lhs, SignRing rhs) {
      return lhs.s == rhs.s;
    }
    friend inline bool operator !=(SignRing lhs, SignRing rhs) {
      return not (lhs == rhs);
    }
    friend inline bool operator <(SignRing lhs, SignRing rhs) {
      return lhs.s < rhs.s;
    }
    friend inline bool operator >(SignRing lhs, SignRing rhs) {
      return rhs < lhs;
    }
    friend inline bool operator <=(SignRing lhs, SignRing rhs) {
      return lhs < rhs or lhs == rhs;
    }
    friend inline bool operator >=(SignRing lhs, SignRing rhs) {
      return lhs > rhs or lhs == rhs;
    }

    class iterator {
      Signs s;
    public :
      iterator() : s(Neg) {}
      iterator(SignRing s) : s(s.s) {}
      iterator(const iterator& i) : s(i.s) {}
      iterator& operator =(iterator i) {
	s = i.s;
	return *this;
      }
      friend inline bool operator ==(iterator lhs, iterator rhs) {
	return lhs.s == rhs.s;
      }
      friend inline bool operator !=(iterator lhs, iterator rhs) {
	return not (lhs == rhs);
      }
      
      typedef SignRing value_type;
      typedef SignRing reference;
      typedef SignRing* pointer;
      typedef int difference_type;
      typedef std::bidirectional_iterator_tag iterator_category;

      reference operator *() const {
	return SignRing(s);
      }
      iterator& operator ++() {
        s = static_cast<Signs>(static_cast<int>(s) + 1);
	return *this;
      }
      iterator operator ++(int) {
	iterator v(*this);
	s = static_cast<Signs>(static_cast<int>(s) + 1);
	return v;
      }
      iterator& operator --() {
	s = static_cast<Signs>(static_cast<int>(s) - 1);
	return *this;
      }
      iterator operator --(int) {
	iterator v(*this);
	s = static_cast<Signs>(static_cast<int>(s) - 1);
	return v;
      }
    };

    typedef int size_type;

    static size_type size() { return 3; }

    static iterator begin() {
      return iterator();
    }
    static iterator end() {
      return ++iterator(SignRing(Pos));
    }

    static SignRing identity_plus() {
      return SignRing(Neg);
    }
    static SignRing identity_times() {
      return SignRing(Pos);
    }

    SignRing& plus(SignRing x) {
      s = std::max(s, x.s);
      return *this;
    }
    SignRing& operator +=(const SignRing x) {
      s = std::max(s, x.s);
      return *this;
    }
    SignRing& times(SignRing x) {
      s = std::min(s, x.s);
      return *this;
    }
    SignRing& operator *=(const SignRing x) {
      s = std::min(s, x.s);
      return *this;
    }

    friend inline SignRing operator +(SignRing lhs, SignRing rhs) {
      SignRing res(lhs);
      lhs.plus(rhs);
      return lhs;
    }
    friend inline SignRing operator *(SignRing lhs, SignRing rhs) {
      SignRing res(lhs);
      lhs.times(rhs);
      return lhs;
    }

    friend inline std::ostream& operator <<(std::ostream& o, SignRing e) {
      return o << e.s;
    }

    static SignRing Null() { return SignRing(); }
    static SignRing Negative() { return SignRing(Neg); }
    static SignRing Positive() { return SignRing(Pos); }

  };

}

#endif



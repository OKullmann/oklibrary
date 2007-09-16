// Oliver Kullmann, 27.7.2003 (Swansea)
/* Copyright 2003 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef LITSETWAECHTER
#define LITSETWAECHTER

#include <set>
#include <algorithm>
#include <functional>

namespace LitSets {

  template <class Literal, class Comp = std::less<Literal> >
  class Litset {

    typedef std::set<Literal, Comp> setlit;
    setlit ls;

  public :

    typedef typename setlit::size_type size_type;
    typedef Literal Lit;
    typedef typename setlit::const_iterator const_iterator;

    Litset(): ls() {}
    Litset(const Litset& l) : ls(l) {}
    Litset& operator =(const Litset& l) {
      ls = l.ls;
    }

    size_type size() const { return ls.size(); }
    bool empty() const { return ls.empty(); }

    Litset& add(Lit x) {
      ls.insert(x);
      return *this;
    }

    Litset& erase(Lit x) {
      ls.erase(x);
      return *this;
    }

    Litset& add(const Litset& L) {
      ls.insert(L.ls.begin(), L.ls.end());
      return *this;
    }

    Litset& erase(const Litset& L) {
      for (typename setlit::const_iterator i = L.ls.begin(); i != L.ls.end(); ++i)
	ls.erase(*i);
      return *this;
    }

    const_iterator begin() const { return ls.begin(); }
    const_iterator end() const { return ls.end(); }

    bool contains(Lit x) const { return ! (ls.find(x) == ls.end());}
    
    bool operator == (const Litset& L) const { return ls == L.ls ;}
    bool operator < (const Litset& L) const { return ls < L.ls; }

  };

}

#include "Literals.hpp"

namespace LitSets {

  typedef Litset<Literals::LitIntOccString> LitSet;
}
#endif

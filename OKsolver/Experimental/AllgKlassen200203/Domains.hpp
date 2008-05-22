// Oliver Kullmann, 30.7.2003 (Swansea)
/* Copyright 2003 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef DOMAINSWAECHTER

#define DOMAINSWAECHTER

#include <algorithm>
#include <utility>
#include <set>
#include <vector>
#include <iterator>
#include <ostream>

#include <boost/static_assert.hpp>

#include <Transitional/OKsolver/Experimental/AllgKlassen200203/ConceptDefinitions.hpp>
#include <Transitional/OKsolver/Experimental/AllgKlassen200203/AlgebraicStructures.hpp>

namespace Domains {

  template <typename Int, Int D>
  class FiniteDomain {

    BOOST_STATIC_ASSERT(D >= 1);

    Int index; // 0 <= index <= D
    explicit FiniteDomain(Int i) : index(i) {
      assert(0 <= index and index <= D);
    }
    static const Int max_index = D-1;

  public :

    typedef ConceptDefinitions::FiniteDomain_tag Concept;

    typedef Int size_type;

    FiniteDomain() : index(0) {}
    FiniteDomain(const FiniteDomain& v) : index(v.index) {
      assert(0 <= index and index <= D);
    }
    FiniteDomain& operator =(const FiniteDomain& v) {
      index = v.index;
      assert(0 <= index and index <= D);
      return *this;
    }

    friend inline bool operator ==(FiniteDomain lhs, FiniteDomain rhs) {
      return lhs.index == rhs.index;
    }
    friend inline bool operator !=(FiniteDomain lhs, FiniteDomain rhs) {
      return not(lhs == rhs);
    }
    
    friend inline bool operator <(FiniteDomain lhs, FiniteDomain rhs) {
      return lhs.index < rhs.index;
    }
    friend inline bool operator >(FiniteDomain lhs, FiniteDomain rhs) {
      return rhs < lhs;
    }
    friend inline bool operator <=(FiniteDomain lhs, FiniteDomain rhs) {
      return lhs < rhs or lhs == rhs;
    }
    friend inline bool operator >=(FiniteDomain lhs, FiniteDomain rhs) {
      return lhs > rhs or lhs == rhs;
    }

    static size_type size() {
      return D;
    }
    static FiniteDomain min() {
      return FiniteDomain();
    }
    static FiniteDomain max() {
      return FiniteDomain(max_index);
    }

    class iterator {
      FiniteDomain val;
      // QUESTION: Better "Int index;" ?!
    public :
      iterator() : val(FiniteDomain(D)) {}
      iterator(FiniteDomain val) : val(val) {}
      iterator(const iterator& i) : val(i.val) {}
      iterator& operator =(iterator i) {
	val = i.val;
	return *this;
      }
      friend inline bool operator ==(iterator lhs, iterator rhs) {
	return lhs.val == rhs.val;
      }
      friend inline bool operator !=(iterator lhs, iterator rhs) {
	return not (lhs == rhs);
      }

      typedef FiniteDomain value_type;
      typedef FiniteDomain reference;
      typedef FiniteDomain* pointer;
      typedef Int difference_type;
      typedef std::bidirectional_iterator_tag iterator_category;

      reference operator *() const {
	assert(0 <= val.index and val.index < D);
	return val;
      }
      iterator& operator ++() {
	assert(0 <= val.index and val.index < D);
	++val.index;
	return *this;
      }
      iterator operator ++(int) {
	assert(0 <= val.index and val.index < D);
	iterator v(*this);
	++val.index;
	return v;
      }
      iterator& operator --() {
	assert(0 < val.index and val.index <= D);
	--val.index;
	return *this;
      }
      iterator operator --(int) {
	assert(0 < val.index and val.index <= D);
	iterator v(*this);
	--val.index;
	return v;
      }
    };

    static iterator begin() {
      return iterator(min());
    }
    static iterator end() {
      return iterator(FiniteDomain(D));
    }

  private :
    typedef std::set<FiniteDomain> set_type;
    static const set_type all;

  public :

    enum insertion_status {forced_assignment, contradiction_created, no_critical_change};

    class set {
      set_type s;
      mutable std::vector<FiniteDomain> v;
    public :
      set() : v(1) {}
      insertion_status insert(FiniteDomain e) {
	if (s.insert(e).second) {
	  const size_type s_s = s.size();
	  if (s_s == D-1)
	    return forced_assignment;
	  else if (s_s == D)
	    return contradiction_created;
	}
	return no_critical_change;
      }
      FiniteDomain unit() const {
	std::set_difference(all.begin(), all.end(), s.begin(), s.end(), v.begin());
	return v[0];
      }
    };

    friend inline std::ostream& operator <<(std::ostream& o, FiniteDomain e) {
      return o << e.index;
    }
    
  };
  template <typename Int, Int D>
  const typename FiniteDomain<Int, D>::set_type FiniteDomain<Int, D>::all(FiniteDomain<Int, D>::begin(), FiniteDomain<Int, D>::end());

  typedef FiniteDomain<unsigned char, 2> FD2;
  typedef FiniteDomain<unsigned char, 3> FD3;
  typedef FiniteDomain<unsigned char, 4> FD4;
}

// --------------------------------------------------------------------
// --------------------------------------------------------------------
// --------------------------------------------------------------------


namespace Domains {

  template <class SignRing>
  class Boolean_T {

    bool b;

  public :

    typedef ConceptDefinitions::BooleanDomain_tag Concept;

    BOOST_CLASS_REQUIRE(SignRing, ConceptDefinitions, SignRing_concept);

    typedef int size_type;

    Boolean_T() : b(false) {}
    Boolean_T(const Boolean_T& b) : b(b.b) {}

    explicit Boolean_T(bool b) : b(b) {}

    Boolean_T& operator =(const Boolean_T& bo) {
      b = bo.b;
      return *this;
    }
    friend inline bool operator ==(Boolean_T lhs, Boolean_T rhs) {
      return lhs.b == rhs.b;
    }
    friend inline bool operator !=(Boolean_T lhs, Boolean_T rhs) {
      return not(lhs == rhs);
    }
    
    friend inline bool operator <(Boolean_T lhs, Boolean_T rhs) {
      return lhs.b < rhs.b;
    }
    friend inline bool operator >(Boolean_T lhs, Boolean_T rhs) {
      return rhs < lhs;
    }
    friend inline bool operator <=(Boolean_T lhs, Boolean_T rhs) {
      return lhs < rhs or lhs == rhs;
    }
    friend inline bool operator >=(Boolean_T lhs, Boolean_T rhs) {
      return lhs > rhs or lhs == rhs;
    }

    operator bool() const { return b; }

    static size_type size() {
      return 2;
    }
    static Boolean_T min() {
      return Boolean_T();
    }
    static Boolean_T max() {
      return Boolean_T(true);
    }

    class iterator {
      enum ItBool {i_false, i_true, past_the_end};
      ItBool it;
    public :
      iterator() : it(past_the_end) {}
      iterator(Boolean_T b) {
	it = static_cast<ItBool>(static_cast<int>(b.b));
      }
      iterator& operator =(iterator i) {
	it = i.it;
	return *this;
      }
      friend inline bool operator ==(iterator lhs, iterator rhs) {
	return lhs.it == rhs.it;
      }
      friend inline bool operator !=(iterator lhs, iterator rhs) {
	return not (lhs == rhs);
      }

      typedef Boolean_T value_type;
      typedef Boolean_T reference;
      typedef Boolean_T* pointer;
      typedef int difference_type;
      typedef std::bidirectional_iterator_tag iterator_category;

      reference operator *() const {
	return Boolean_T(static_cast<bool>(it));
      }
      iterator& operator ++() {
	assert(it != past_the_end);
      it : static_cast<ItBool>(static_cast<int>(it) + 1);
	return *this;
      }
      iterator operator ++(int) {
	assert(it != past_the_end);
	iterator v(*this);
	it : static_cast<ItBool>(static_cast<int>(it) + 1);
	return v;
      }
      iterator& operator --() {
	assert(it != i_false);
	it : static_cast<ItBool>(static_cast<int>(it) - 1);
	return *this;
      }
      iterator operator --(int) {
	assert(it != i_false);
	iterator v(*this);
	it : static_cast<ItBool>(static_cast<int>(it) - 1);
	return v;
      }
    };

    static iterator begin() {
      return iterator(Boolean_T(false));
    }
    static iterator end() {
      return ++iterator(Boolean_T(true));
    }

    enum insertion_status {forced_assignment, contradiction_created, no_critical_change};

    class set {
      SignRing s;
    public :
      set() : s(SignRing::Null()) {}
      insertion_status insert(Boolean_T e) {
	if (s == SignRing::Null()) {
	  if (e.b)
	    s = SignRing::Positive();
	  else
	    s = SignRing::Negative();
	  return forced_assignment;
	}
	else if (s == SignRing::Negative())
	  if (e.b)
	    return contradiction_created;
	  else
	    return no_critical_change;
	else
	  if (e.b)
	    return no_critical_change;
	  else
	    return contradiction_created;
      }
      Boolean_T unit() const {
	if (s == SignRing::Positive())
	  return Boolean_T(false);
	else
	  return Boolean_T(true);
      }
    };

    friend inline std::ostream& operator <<(std::ostream& o, Boolean_T b) {
      return o << b.b;
    }

    static Boolean_T False() { return Boolean_T(); }
    static Boolean_T True() { return Boolean_T(true); }

  };

  typedef Boolean_T<AlgebraicStructures::SignRing> Boolean;

}

#endif

// Oliver Kullmann, 10.11.2002 (Swansea)

#ifndef ITERATORHANDLINGWAECHTER

#define ITERATORHANDLINGWAECHTER

#include <algorithm>
#include <iterator>
#include <cstddef>

namespace IteratorHandling {

  // ----------------------------------------
  // Counting
  // ----------------------------------------

  // Output iterator, which only counts how many time it has been advanced
  template <typename T>
  class Advance_Count : public std::iterator<std::output_iterator_tag, T> {
  public :
    Advance_Count() : c(0) {}
    Advance_Count(const Advance_Count& ac) : c(ac.c) {}
    Advance_Count& operator = (const Advance_Count& ac) { c = ac.c; return *this; }
    Advance_Count& operator = (const T&) { return *this; }
    Advance_Count& operator * () { return *this; }
    Advance_Count& operator ++ () { ++c; return *this; }
    Advance_Count operator ++ (int) {
      Advance_Count ac(*this);
      ++c;
      return ac;
    }

    std::ptrdiff_t count() const { return c; }

  private :
    std::ptrdiff_t c;
  };

  // Random access iterator for counting
  template <typename Int = std::ptrdiff_t>
  class Count_iterator : public std::iterator<std::random_access_iterator_tag, Int, Int> {
    Int c;
  public :
    Count_iterator() : c(0) {}
    Count_iterator(Int x) : c(x) {}

    friend inline bool operator ==(const Count_iterator lhs, const Count_iterator rhs) { return lhs.c == rhs.c; }
    friend inline bool operator <(Count_iterator lhs, Count_iterator rhs) { return lhs.c < rhs.c; }

    Int& operator *() { return c; }

    Count_iterator& operator ++() { ++c; return *this; }
    Count_iterator& operator --() { --c; return *this; }
    Count_iterator operator ++(int) { Count_iterator ci(*this); ++c; return ci; }
    Count_iterator operator --(int) { Count_iterator ci(*this); --c; return ci; }
    Count_iterator& operator +=(Int n) { c += n; return *this; }
    Count_iterator& operator -=(Int n) { c -= n; return *this; }
    friend inline Count_iterator operator + (Count_iterator ci, Int n) { ci += n; return ci; }
    friend inline Int operator - (Count_iterator lhs, Count_iterator rhs) { return lhs.c - rhs.c; }
  };
  template <typename Int>
  inline bool operator !=(const Count_iterator<Int> lhs, const Count_iterator<Int> rhs) { return not(lhs == rhs); }
  template <typename Int>
  inline bool operator <=(const Count_iterator<Int> lhs, const Count_iterator<Int> rhs) { return (lhs < rhs) or (lhs == rhs); }
  template <typename Int>
  inline bool operator >(const Count_iterator<Int> lhs, const Count_iterator<Int> rhs) { return rhs < lhs; }
  template <typename Int>
  inline bool operator >=(const Count_iterator<Int> lhs, const Count_iterator<Int> rhs) { return (lhs > rhs) or (lhs == rhs); }

  template <typename Int>
  Count_iterator<Int> count_iterator(Int x) { return Count_iterator<Int>(x); }



  // Arithmetical progressions (as a collection)
  // Collection a, a + d, a + 2d, ..., a + m * d
  // a, d numerical, m integral

  template <typename Num, typename Int>
  class Arithmetical_progression {
    const Num a, d;
    const Int m;

  public :
    Arithmetical_progression(const Num a, const Num d, const Int m) : a(a), d(d), m(m) {}
    Int size() const { return m+1; }
    
    class iterator : public Count_iterator<Int> {
      const Arithmetical_progression* ap;
      iterator(const Arithmetical_progression* ap) : ap(ap) {}
      iterator(const Arithmetical_progression* ap, Int i) : Count_iterator<Int>(i), ap(ap) {}
      friend class Arithmetical_progression;

    public :
      typedef Num value_type;
      typedef const Num* pointer;
      typedef Num reference;
      iterator() : ap(0) {}
      reference operator *() {
	assert(ap);
	return ap -> eval(Count_iterator<Int>::operator *());
      }
      reference operator *() const {
	assert(ap);
	return ap -> eval(Count_iterator<Int>::operator *());
      }

      friend inline bool operator ==(const iterator lhs, const iterator rhs) {
	return static_cast<Count_iterator<Int> >(lhs) == static_cast<Count_iterator<Int> >(rhs) and lhs.ap == rhs.ap;
      }
      friend inline bool operator !=(const iterator lhs, const iterator rhs) {
	return not(lhs == rhs);
      }

      iterator& operator ++() {
	Count_iterator<Int>::operator ++();
	return *this;
      }
      iterator& operator --() {
	Count_iterator<Int>::operator --();
	return *this;
      }
      iterator operator ++(int) {
	iterator i(*this); operator ++();
	return i;
      }
      iterator operator --(int) {
	iterator i(*this); operator --();
	return i;
      }
      iterator& operator +=(Int n) {
	Count_iterator<Int>::operator +=(n);
	return *this;
      }
      iterator& operator -=(Int n) {
	Count_iterator<Int>::operator -=(n);
	return *this;
      }
      friend inline iterator operator + (iterator i, Int n) {
	i += n; return i;
      }
      friend inline Int operator - (iterator lhs, iterator rhs) {
	return static_cast<Count_iterator<Int> >(lhs) - static_cast<Count_iterator<Int> >(rhs);
      }
    };
    
    typedef iterator const_iterator;
    iterator begin() { return iterator(this); }
    iterator end() {
      assert(m >= 0);
      return iterator(this, m + 1);
    }
    
  private :
    Num eval(Int i) const { return a + i * d; }
  };

  template <typename Num, typename Int>
  inline bool operator !=(const typename Arithmetical_progression<Num, Int>::iterator lhs, const typename Arithmetical_progression<Num, Int>::iterator rhs) {
    return not(lhs == rhs);
  }


}

#endif

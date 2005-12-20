// Oliver Kullmann, 3.7.2002 (Swansea)

#ifndef FUNCTIONHANDLINGWAECHTER

#define FUNCTIONHANDLINGWAECHTER

#include <functional>
#include <utility>
#include <iterator>

namespace FunctionHandling {

  // -------------------------------------------------
  // Access
  // -------------------------------------------------

  template <class C>
  struct First : std::unary_function<C, const typename C::first_type&> {
    const typename C::first_type& operator() (const C& c) const { return c.first; }
  };
  template <class C>
  struct FirstMutable : std::unary_function<C, typename C::first_type&> {
    typename C::first_type& operator() (C& c) const { return c.first; }
  };


  template <class C>
  struct Second : std::unary_function<C, const typename C::second_type&> {
    const typename C::second_type& operator() (const C& c) const { return c.second; }
  };
  template <class C>
  struct SecondMutable : std::unary_function<C, typename C::second_type&> {
    typename C::second_type& operator() (C& c) const { return c.second; }
  };


  template <typename Value, typename Pointer = const Value*>
  struct Dereferenciation : std::unary_function<const Pointer&, const Value&> {
    const Value& operator()(const Pointer& p) { return *p; }
  };

  // -------------------------------------------------
  // Deletion
  // -------------------------------------------------

  struct DeleteObject {
    template <typename T> void operator() (const T* ptr) const {
      delete ptr;
    }
  };
  struct DeleteObjectSecond {
    template <typename U, typename T> void operator() (const std::pair<U, T*>& p) const {
      delete p.second;
    }
  };
  struct DeleteObjectSecondNullify {
    template <typename U, typename T> void operator() (std::pair<const U, T*>& p) const {
      delete p.second;
      p.second = 0;
    }
  };


  // -------------------------------------------------
  // Comparison
  // -------------------------------------------------

  template <typename T>
  struct in_right_open_interval : std::unary_function<T, bool> {
    in_right_open_interval(T left, T right) : l(left), r(right) {}
    bool operator() (T x) const {
      return (l <= x and x < r);
    }
    T l, r;
  };
  template <typename T>
  struct in_left_open_interval : std::unary_function<T, bool> {
    in_left_open_interval(T left, T right) : l(left), r(right) {}
    bool operator() (T x) const {
      return (l < x and x <= r);
    }
    T l, r;
  };
  template <typename T>
  struct in_open_interval : std::unary_function<T, bool> {
    in_open_interval(T left, T right) : l(left), r(right) {}
    bool operator() (T x) const {
      return (l < x and x < r);
    }
    T l, r;
  };
  template <typename T>
  struct in_closed_interval : std::unary_function<T, bool> {
    in_closed_interval(T left, T right) : l(left), r(right) {}
    bool operator() (T x) const {
      return (l <= x and x <= r);
    }
    T l, r;
  };


  // -------------------------------------------------
  // Counting
  // -------------------------------------------------


  template <typename Int = int>
  struct Counter {
    // Counter::count() starts with zero
    // counter.c gives the number of times it has been called
    typedef Int IndexType;
    Counter() : c(0) {}
    Counter(IndexType c) : c(c) {}
    IndexType operator() () { return c++; }
    void reset() { c = 0; }
    void reset(IndexType cnew) { c = cnew; }
    IndexType c;
  };

}

#endif

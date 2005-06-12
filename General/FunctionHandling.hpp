// Oliver Kullmann, 3.7.2002 (Swansea)

#ifndef FUNCTIONHANDLINGWAECHTER

#define FUNCTIONHANDLINGWAECHTER

#include <functional>
#include <utility>

namespace FunctionHandling {

  // -------------------------------------------------
  // Access
  // -------------------------------------------------

  template <typename>
  struct First;

  template <typename T1, typename T2>
  struct First<std::pair<T1, T2> > : std::unary_function<std::pair<T1, T2>, const T1&> {
    const T1& operator() (const std::pair<T1, T2>& p) const { return p.first; }
  };

  template <typename>
  struct Second;

  template <typename T1, typename T2>
  struct Second<std::pair<T1, T2> > : std::unary_function<std::pair<T1, T2>, const T2&> {
    const T2& operator() (const std::pair<T1, T2>& p) const { return p.second; }
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

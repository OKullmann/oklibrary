// Oliver Kullmann, 21.4.2003 (Swansea)

#ifndef COMBINATORICSWAECHTER

#define COMBINATORICSWAECHTER

#include <algorithm>
#include <functional>
#include <vector>
#include <numeric>
#include <cassert>
#include <iostream>
#include <iterator>

#include "ErrorHandling.hpp"
#include "FunctionHandling.hpp"

namespace Combinatorics {

  struct Error : ErrorHandling::Error {
    Error(const std::string& what) : ErrorHandling::Error(what) {}
  };

}

namespace Combinatorics {
  
  // -------------------------------------------------------------
  // Elementary counting
  // -------------------------------------------------------------

  template <typename Int>
  struct Factorial {
    Int operator() (const Int n) const {
      const Int zero(0);
      assert(n >= zero);
      Int f(1);
      for (int i = Int(2); i <= n; ++i)
	f *= i;
      return f;
    }
  };
  template <typename Int>
  inline Int factorial(const Int n) {
    return Factorial<Int>()(n);
  }

  template <typename Num, typename Int>
  struct Descending_power {
    Num operator() (Num b, Int k) const {
      const Int zero(0);
      assert(k >= zero);
      Num p(1);
      for (Int i = zero; i != k; --b, ++i)
	p *= b;
      return p;
    }
  };
  template <typename Num, typename Int>
  inline Num descending_power(Num b, Int k) {
    return Descending_power<Num, Int>()(b, k);
  }

  template <typename Int>
  struct Binom_integer_over_integer {
    Int operator() (Int n, Int k) const {
      
      if (k < Int(0)) return Int(0);
      if (k == Int(0)) return Int(1);
      assert(k > Int(0));
      if (k == Int(1)) return n;
      assert(k >= Int(2));
      if (n == Int(0)) return Int(0);
      if (n == k) return Int(1);
      if (n == Int(-1)) return Int(1);
      if (n < 0)
	n = k - n - Int(1);
      else
	if (n < k) return Int(0);
      assert(n > Int(0));
      assert(k < n);
      if (n < Int(2) * k)
	k = n - k;
      assert(Int(2) * k <= n);
      assert(k > Int(0));
      if (k == Int(1)) return n;
      assert(k >= Int(2));
      // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

      typedef typename std::vector<Int> vector_type;
      typedef typename vector_type::iterator iterator;
      vector_type v;
      v.reserve(k+1);
      v.push_back(1); v.push_back(1);
      iterator first = v.begin();
      const Int diff = n - k;
      for (Int i = Int(2); i <= n; ++i) {
	std::adjacent_difference(first, v.end(), first, std::plus<Int>());
	if (i <= k) v.push_back(1);
	if (i > diff) ++first;
      }
      return *first;
    }
  };
  template <typename Int>
  inline Int binom_integer_over_integer(const Int z, const Int k) {
    return Binom_integer_over_integer<Int>()(z, k);
  }
  template <typename Int>
  inline Int binom(Int a, Int b) {
    return Binom_integer_over_integer<Int>()(a, b);
  }

  enum choose_possibilities {no_subsets = -1, no_further_subsets = 0, further_subsets = 1};
  // NOT VERY EFFICIENT
  // SHOULD BE ITERATOR-BASED
  template <typename Int, class ForwardIterator>
  choose_possibilities choose_next(const Int n, const Int k, const ForwardIterator begin, const ForwardIterator end) {
    // Assigns to the range [begin, end) the next k-subset of {0,1, ..., n-1}
    const Int zero(0);

    if (n < zero or k > n or k < zero) return no_subsets;
    if (k == zero or k == n) return no_further_subsets;

    assert(std::distance(begin, end) == k);
    assert(std::adjacent_find(begin, end, std::greater_equal<Int>()) == end);
    assert(std::find_if(begin, end, std::not1(FunctionHandling::in_right_open_interval<Int>(zero, Int(n)))) == end);

    Int v = *begin; ++v;
    ForwardIterator first_candidate = begin;
    {
      ForwardIterator next = begin;
      for (; ++next != end and *next == v; first_candidate = next, ++v);
    }
    if (v == n) return no_further_subsets;
    assert(v < n and *first_candidate == v - 1);
    *first_candidate = v;
    std::generate(begin, first_candidate, FunctionHandling::Counter<Int>());
    return further_subsets;
  }
}

#endif


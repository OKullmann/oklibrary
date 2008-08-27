// Oliver Kullmann, 4.6.2002 (Swansea)
/* Copyright 2002 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file General/Algorithms.hpp
  \brief Collection of various basic generic algorithms
  \deprecated Contents shall move to dedicated places


  \todo What are the right places for the code to move?
  We have
  <ul>
   <li> Algorithms related to statistics :
    <ol>
     <li> Algorithms::Median </li>
     <li> Algorithms::Rand </li>
     <li> Algorithms::RandomGeneratorInterval </li>
    </ol>
    These three algorithms should be superceeded by the upcoming
    Boost statistics library.
   </li>
   <li> Container algorithms :
    <ol>
     <li> Algorithms::exact_matches </li>
     <li> Algorithms::matching_values </li>
    </ol>
    Should go to Programming/Container.
   </li>
   <li> Sequence algorithms :
    <ol>
     <li> Algorithms::copy_n and variations </li>
     <li> Algorithms::pair2_find_if and variations </li>
    </ol>
    Should go to Programming/Sequences.
   </li>
   <li> Algebra :
    <ol>
     <li> Algorithms::Power_natural (fast exponentiation) </li>
    </ol>
    Should go to Structures/Algebra/BasicComputations.
   </li>
  </ul>

*/

#ifndef ALGORITHMSWAECHTER
#define ALGORITHMSWAECHTER

#include <algorithm>
#include <iterator>
#include <utility>
#include <cassert>
#include <functional>
#include <numeric>

#include <cstdlib> // for rand, srand

#include <boost/concept_check.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/linear_congruential.hpp>

#include <Transitional/General/IteratorHandling.hpp>
#include <Transitional/General/Algebra_Traits.hpp>


namespace Algorithms {

  // ---------------------------------
  // Statistics
  // ---------------------------------

  template <typename Out, typename It>
  struct Median {
    Out operator() (It beg, const It end) {
      
      // Can not be used with input iterators.
      // Assumptions:
      //   the range is sorted;
      //   the expressions "Out(0), Out(2)" are defined;
      //   "v + w" is defined for values v, w of It's value type,
      //   yielding a value of some type T;
      //   "v / w" is defined for a value v of type T and a
      //   value w of type Out, yielding a value of some type T2;
      //   "Out(v)" is defined for a value of type T2.
      // Usage:
      //   median(beg, end) is equivalent to median<double>(beg, end);
      //   median<Out>(beg, end) computes the median as an Out-value.
      
      if (beg == end)
	return Out(0);

      typedef typename std::iterator_traits<It>::difference_type difftype;
      const difftype length = std::distance(beg, end);
      const difftype half_length = length / 2;

      if (length % 2 == 1) {
	std::advance(beg, half_length);
	return Out(*beg);
      }
      else {
	std::advance(beg, half_length - 1);
	const typename std::iterator_traits<It>::value_type& v(*beg);
	return Out((v + *(++beg)) / Out(2));
      }
      
    }
  };
  template <typename Out, typename It>
  inline Out median(const It beg, const It end) {
    return Median<Out, It>()(beg, end);
  }
  template <typename It>
  inline double median(const It beg, const It end) {
    return Median<double, It>()(beg, end);
  }


  template <typename Out, typename It>
  struct Median_with_sorting {
    Out operator() (const It beg, const It end) {
      // as median, but mutates the range;
      // thus assumes random access iterators

      if (beg == end)
	return Out(0);

      typedef typename std::iterator_traits<It>::difference_type difftype;
      const difftype length = end - beg;
      const difftype half_length = length / 2;
      const It middle = beg + half_length;

      if (length % 2 == 1) {
	std::nth_element(beg, middle, end);
	return Out(*middle);
      }
      else {
	const It middlem1 = middle - 1;
	std::nth_element(beg, middlem1, end);
	return Out((*middlem1 + *(std::min_element(middle, end))) / Out(2));
      }
    }
  };
  template <typename Out, typename It>
  inline Out median_with_sorting(const It beg, const It end) {
    return Median_with_sorting<Out, It>()(beg, end);
  }
  template <typename It>
  inline double median_with_sorting(const It beg, const It end) {
    return Median_with_sorting<double, It>()(beg, end);
  }

}

namespace Algorithms {

  // ---------------------------------------------------
  // Comparing containers
  // ---------------------------------------------------

  // Counting the number of corresponding positions with equal content

  enum Assumptions_exact_matches { no_check, do_check };

  template <class C> // C = container
  inline typename C::difference_type exact_matches(const C& a, const C& b, Assumptions_exact_matches check = no_check) {
    if (check = do_check)
      return std::inner_product(a.begin(), a.begin() + std::min(a.size(), b.size()), b.begin(), 0, std::plus<typename C::difference_type>(), std::equal_to<typename C::value_type>());
    else
      return std::inner_product(a.begin(), a.end(), b.begin(), 0, std::plus<typename C::difference_type>(), std::equal_to<typename C::value_type>());
  }

  // Counting the maximal number of positions with equal content over all permutations
  
  template <class C>
  inline typename C::difference_type matching_values(const C& pattern, C test) {
    // pattern must be sorted
    // computes the size of the intersection of pattern and test as multi-sets
    std::sort(test.begin(), test.end());
    return (std::set_intersection(pattern.begin(), pattern.end(), test.begin(), test.end(), IteratorHandling::Advance_Count<typename C::value_type>())).count();
  }
  
}

namespace Algorithms {

  // ---------------------------------------------------
  // Random numbers
  // ---------------------------------------------------

  // A pseudo-random-number generator for numbers in [0, RAND_MAX]
  struct Rand : std::unary_function<int, int> {
    Rand() {}
    Rand(int seed) { std::srand(seed); }
    int operator() () const { return std::rand(); }
    static int min() { return 0; }
    static int max() { return RAND_MAX; }
  };
  // g++, Version 3.0.4: RAND_MAX = 2147483647
  // int rand(int);
  // rand returns a number from 0 to RAND_MAX (inclusive).

  // Different random generators may derive from Rand (using overwriting)

  // Two models of a random number generator is the sense of 25.2.11 note (3)
  // from the standard:
  template < typename T, typename R >
  inline T adopt_rand(T max, R& f) {
    // f is a generator like Rand from above;
    // returns a random number from 0 to max - 1;
    // assumes max >= 1.
    return static_cast<T>( static_cast<double>(f()) / (static_cast<double>(f.max()) + 1) * static_cast<double>(max) );
    // the larger f.max() and the smaller max the more uniform the distribution
  }
  template < typename T >
  T adopt_rand(T max) {
    // returns a random number from 0 to max - 1
    // assumes max >= 1
    return static_cast<T>( static_cast<double>(std::rand()) / (static_cast<double>(RAND_MAX) + 1) * static_cast<double>(max) );
  }

  // Generating random numbers from an interval using the boost random number library:

  template <class bgt = boost::minstd_rand, class dt = boost::uniform_int<> >
  class RandomGeneratorInterval {
  public :
    typedef bgt base_generator_type;
    typedef dt distribution_type;
    typedef boost::variate_generator<base_generator_type, distribution_type> generator_type;
    typedef typename generator_type::result_type interval_type;
    typedef typename base_generator_type::result_type seed_type;
    RandomGeneratorInterval(interval_type min, interval_type max) : min(min), max(max) {}
    generator_type operator() (seed_type seed = 1) const {
      assert(seed != 0);
      return generator_type(base_generator_type(seed), distribution_type(min, max));
    }
  private :
    const interval_type min, max;
  };
  // An object rgi defined by "RandomGeneratorInterval rgi(min, max)" can create a random number generator g via g = rgi(seed).
  // Random numbers in the interval from min to max then are created with g().
  // It is g() of type interval_type as well as min and max, while seed is of type seed_type; g itself is of type generator_type.
  // g is a model of a uniform random number generator (even a model of a pseudo-random number generator?!).

}

namespace Algorithms {

  // ---------------------------------------------------
  // Mutating sequences
  // ---------------------------------------------------

  template <typename InputIterator, typename Size, typename OutputIterator>
  OutputIterator copy_n(InputIterator first1, Size count, OutputIterator first2) {
    for (; count > 0; --count)
      *(first2++) = *(first1++);
    return first2;
  }

  template <typename BidirectionalIterator1, typename Size, typename BidirectionalIterator2>
  // Copied sequence is [last1 - count, ..., last1), copied backward
  BidirectionalIterator2 copy_backward_n(BidirectionalIterator1 last1, Size count, BidirectionalIterator2 last2) {
    for (; count > 0; --count)
      *(--last2) = *(--last1);
    return last2;
  }

}

namespace Algorithms {

  // ---------------------------------------------------
  // Variations and generalisations of find
  // ---------------------------------------------------
  
  //Version 1 (THB): Local Assert in Predicate
  template <typename InputIterator, typename MultiPassInputIterator, class Predicate>
  inline void pair2_find_if(InputIterator begin1, const InputIterator end1, const MultiPassInputIterator begin2, const MultiPassInputIterator end2) {
    for (; begin1 != end1; ++begin1)
      for (MultiPassInputIterator i = begin2; i != end2; ++i)
        Predicate(*begin1, *i);
  }
  
  //Version 2 (OK): One assert, calling predicate
  template <typename InputIterator, typename MultiPassInputIterator, class Predicate>
  inline std::pair<InputIterator, MultiPassInputIterator> pair2_find_if(InputIterator begin1, const InputIterator end1, const MultiPassInputIterator begin2, const MultiPassInputIterator end2, Predicate pred) {
    for (; begin1 != end1; ++begin1)
      for (MultiPassInputIterator i = begin2; i != end2; ++i)
        if (pred(*begin1, *i))
          return std::make_pair(begin1, i);
    return std::make_pair(end1, end2);
  }
  
  //Version 1 (THB): Local Assert in Predicate
  template <typename MultiPassInputIterator, class Predicate>
  inline void pair1_find_if(const MultiPassInputIterator begin, const MultiPassInputIterator end) {
    pair2_find_if<MultiPassInputIterator,MultiPassInputIterator,Predicate>(begin, end, begin, end);
  }
  
  //Version 2 (OK): One assert, calling predicate
  template <typename MultiPassInputIterator, class Predicate>
  inline std::pair<MultiPassInputIterator, MultiPassInputIterator> pair1_find_if(const MultiPassInputIterator begin, const MultiPassInputIterator end, Predicate pred) {
    return pair2_find_if(begin, end, begin, end, pred);
  }
  
  //Version 1 (THB): Local Assert in Predicate
  template <typename MultiPassInputIterator, class Predicate>
  inline void pair1_nonself_find_if(const MultiPassInputIterator begin, const MultiPassInputIterator end) {
    for (MultiPassInputIterator i = begin; i != end; ++i)
      for (MultiPassInputIterator j = begin; j != end; ++j)
        if (j != i) Predicate(*i, *j);
  }

  //Version 2 (OK): One assert, calling predicate
  template <typename MultiPassInputIterator, class Predicate>
  inline std::pair<MultiPassInputIterator, MultiPassInputIterator> pair1_nonself_find_if(const MultiPassInputIterator begin, const MultiPassInputIterator end, Predicate pred) {
    for (MultiPassInputIterator i = begin; i != end; ++i)
      for (MultiPassInputIterator j = begin; j != end; ++j)
        if (j != i and pred(*i, *j))
          return std::make_pair(i, j);
    return std::make_pair(end, end);
  }
  
  //Version 1 (THB): Local Assert in Predicate
  template <typename MultiPassInputIterator, class Predicate>
  inline void pair1_symmpred_find_if(const MultiPassInputIterator begin, const MultiPassInputIterator end) {
    for (MultiPassInputIterator i = begin; i != end; ++i)
      for (MultiPassInputIterator j = i; j != end; ++j)
        Predicate(*i, *j);
  }

  //Version 2 (OK): One assert, calling predicate
  template <typename MultiPassInputIterator, class Predicate>
  inline std::pair<MultiPassInputIterator, MultiPassInputIterator> pair1_symmpred_find_if(const MultiPassInputIterator begin, const MultiPassInputIterator end, Predicate pred) {
    for (MultiPassInputIterator i = begin; i != end; ++i)
      for (MultiPassInputIterator j = i; j != end; ++j)
        if (pred(*i, *j))
          return std::make_pair(i, j);
    return std::make_pair(end, end);
  }
  
  //Version 1 (THB): Local Assert in Predicate
  template <typename MultiPassInputIterator, class Predicate>
  inline void pair1_symmpred_nonself_find_if(const MultiPassInputIterator begin, const MultiPassInputIterator end) {
    for (MultiPassInputIterator i = begin; i != end; ++i)
      for (MultiPassInputIterator j = ++MultiPassInputIterator(i); j != end; ++j)
        Predicate(*i, *j);
  }

  //Version 2 (OK): One assert, calling predicate
  template <typename MultiPassInputIterator, class Predicate>
  inline std::pair<MultiPassInputIterator, MultiPassInputIterator> pair1_symmpred_nonself_find_if(const MultiPassInputIterator begin, const MultiPassInputIterator end, Predicate pred) {
    for (MultiPassInputIterator i = begin; i != end; ++i)
      for (MultiPassInputIterator j = ++MultiPassInputIterator(i); j != end; ++j)
        if (pred(*i, *j))
          return std::make_pair(i, j);
    return std::make_pair(end, end);
  }
}

namespace Algorithms {

  // ---------------------------------------------------
  // Arithmetic and algebra in general
  // ---------------------------------------------------

  // OK, 6.11.2004
  template <typename MultiplicativeMonoid, typename Int>
  struct Power_natural : std::binary_function<MultiplicativeMonoid, Int, MultiplicativeMonoid> {
    MultiplicativeMonoid operator() (MultiplicativeMonoid basis, Int exponent) {
      assert(exponent >= 0);
      MultiplicativeMonoid result = Algebra_Traits::MultiplicativeGroupoid_traits<MultiplicativeMonoid>::identity();
      while (exponent != Int(0)) {
	if (exponent % Int(2) == Int(1))
	  result *= basis;
	basis *= basis;
	exponent /= Int(2);
      }
      return result;
    }
  };

  template <typename MultiplicativeMonoid, typename Int>
  MultiplicativeMonoid power_natural(const MultiplicativeMonoid basis, const Int exponent) {
    return Power_natural<MultiplicativeMonoid, Int>()(basis, exponent);
  }
}

#endif

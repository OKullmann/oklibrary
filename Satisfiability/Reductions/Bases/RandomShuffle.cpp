// Oliver Kullmann, 16.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Reductions/Bases/RandomShuffle.cpp
  \brief Temporary file, to experiment with std::random_shuffle

  At most two inputs are required, an integer seed (default 1) and an
  unsigned integer N (default 10; the number of elements to be shuffled).

  Outputs the underlying sequence of N-1 random numbers, and then the sequence
  1, ..., N first shuffled by std::random_shuffle, and then by
  ::random_shuffle. For example (with gcc 4.1.2 as well as 4.5.2):
  \verbatim
> RandomShuffle
Underlying random sequence:
4 8 5 6 0 0 1 2 0 

std::random_shuffle:
7 10 8 3 5 2 6 4 9 1 
::random_shuffle:
5 10 8 2 1 6 3 4 9 7 
  \endverbatim

*/

#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <cassert>

#include <boost/lexical_cast.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/random_number_generator.hpp>

namespace {

  enum { errcode_parameter = 1 };
        
  const std::string program = "RandomShuffle";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.0.3";

  const int default_seed = 1;

  typedef boost::mt19937 base_generator_type;
  base_generator_type base_rand_gen;
  inline void set_random(const int seed) {
    assert(seed >= 1);
    base_rand_gen.seed(seed);
  }

  const unsigned int default_N = 10;

  typedef std::vector<int> vec_t;
  inline void initialise(vec_t& a) {
    for (unsigned int i = 0; i < a.size(); ++i) a[i] = i+1;
  }
  inline void output(const vec_t& a) {
    for (unsigned int i = 0; i < a.size(); ++i) std::cout << a[i] << " ";
    std::cout << "\n";
  }

  template <class RandomAccessIterator, class RandomNumberGenerator>
  inline void random_shuffle(const RandomAccessIterator first, const RandomAccessIterator last,
    RandomNumberGenerator& rand) {
    typedef typename std::iterator_traits<RandomAccessIterator>::difference_type difference_type;
    difference_type n = last - first;
    assert(n >= 0);
    if (n <= 1) return;
    for (RandomAccessIterator i = first; i != last-1; ++i,--n) {
      const difference_type r = rand(n);
      assert(r >= 0);
      assert(r < n);
      std::iter_swap(i, i+r);
    }
    assert(n == 1);
  }

}

int main(const int argc, const char* const argv[]) {
  if (argc > 3) {
    std::cerr << err << "\n At most two arguments are allowed "
      "(the seed for the random-number generator and the number of elements).\n";
    return errcode_parameter;
  }

  const int seed = (argc == 1) ? default_seed : boost::lexical_cast<int>(argv[1]);
  const unsigned int N = (argc < 3) ? default_N :  boost::lexical_cast<unsigned int>(argv[2]);

  vec_t V(N);

  typedef boost::uniform_int<> uniform_distribution_type;
  uniform_distribution_type uniform_distribution(0,std::numeric_limits<int>::max()); // is this correct???
  typedef boost::variate_generator<base_generator_type&, uniform_distribution_type> generator_type;
  generator_type rand_gen(base_rand_gen, uniform_distribution);
  typedef boost::random_number_generator<generator_type> RandomNumberGenerator;
  RandomNumberGenerator rg(rand_gen);

  set_random(seed);
  std::cout << "Underlying random sequence:\n";
  for (long n = V.size(); n > 1; --n) std::cout << rg(n) << " ";
  std::cout << "\n\n";

  initialise(V);
  set_random(seed);
  std::random_shuffle(V.begin(), V.end(), rg);
  std::cout << "std::random_shuffle:\n";
  output(V);

  initialise(V);
  set_random(seed);
  ::random_shuffle(V.begin(), V.end(), rg);
  std::cout << "::random_shuffle:\n";
  output(V);
}

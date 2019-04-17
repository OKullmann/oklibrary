// Oliver Kullmann, 15.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Algorithms based on random numbers

    - Algorithm shuffle for shuffling a sequence.

TODOS:

1. Implement "choose k from n"

  - Just giving n, this means "from 1,...,n".
  - Alternatively a vector v (length at least k) is given, from
    which to choose.

2. Add a further version of shuffle, accepting RandGen_t.

*/

#ifndef ALGORITHMS_PE1w1ejM65
#define ALGORITHMS_PE1w1ejM65

#include <utility>

// Guaranteed to be included:
#include "Numbers.hpp"

#include "Distributions.hpp"

namespace RandGen {

  // As std::shuffle, but now fixing the algorithm and the random-generator:
  template <class RandomAccessIterator>
  inline void shuffle(const RandomAccessIterator begin, const RandomAccessIterator end, randgen_t& g) noexcept {
    for (auto i = (end - begin) - 1; i > 0; --i) {
      using std::swap;
      swap(begin[i], begin[UniformRange(g, gen_uint_t(i+1))()]);
    }
  }
  template <class RAI>
  inline void shuffle(const RAI begin, const RAI end, randgen_t&& g) noexcept {
    RandGen::shuffle(begin, end, g);
  }
  // Remark: If randgen_t would also be a template parameter, then just one
  // version would be sufficient, due to "perfect forwarding". Here however
  // we want to be sure that exactly type randgen_t is used.

}

#endif

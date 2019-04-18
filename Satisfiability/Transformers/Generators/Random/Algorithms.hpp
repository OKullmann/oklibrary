// Oliver Kullmann, 15.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Algorithms based on random numbers

    - Algorithm shuffle for shuffling a sequence.

TODOS:

1. Implement "choose k from n": choose_kn(k, n, g)

  - Just giving n, this means "from 1,...,n". DONE (CHANGED)
  - Alternatively a vector v (length at least k) is given, from
    which to choose. Though this could be handled by just choosing
    the indices. DONE (NO)
  - So perhaps just starting with parameter n, which means {0, ..., n-1}. DONE
  - The output is most naturally a vector; perhaps we just fix gen_uint_t
    as the index-type (of n). The output is sorted.
  - There is a choice of including versus including: choosing 3 from 1000
    the obvious choice is including, while choosing 997 from 1000 the
    obvious choice is excluding. The splitting point in this example is
    500 from 1000, where here the choice doesn't matter.

    The point is here that the number of random choices is to be minimised.
  - It seems natural to provide a RandGen_t via reference, for using the
    service once. DONE
  - But there are also natural applications where k, n are fixed, and
    where a stream of choices is to be done.
  - The main loop is a choice of k numbers s_1,...,s_k obtaind from
    UniformRange, for n, n-1, ..., n-k+1 (either for inclusion or exclusion).
  - Then a helper-function transforms this vector into the result-vector.
  - In the inclusion-case, the first number obtained is s_1, the second is
    s_2 applied to {0,...,n-1} - {s_1}, and so on.

    So for each s_i, one makes a binary search with the s_1, ..., s_{i-1},
    and according to the position found one substracts from s_i.
  - For a given vector from which to chose, one can just doing it as in
    shuffle, just considering k steps. But in general n is much larger
    than k, and so creating the vector would be too costly.

    However one needs actually to store only the changed elements, which
    could be done in a std::map. That is, the algorithm to transform
    s_1, ..., s_k is as follows: the first number is s_1, and implicitly
    s_1 is replaced by n-1. If s_2 != s1, then s_2 is the next number,
    and we implicitly replace s_2 by n-2; otherwise the next number is n-1.
    and s_1 is implicitly replaced by n-2. "Implicitly replaced" can just
    mean using std::map m: Initially m is empty. The algorithm for s_i
    determines whether s_i is in m: if no, then the chosen number is s_i,
    otherwise it is m[s_i]. In both cases, then update m[s_i] = n-i.

    One could use a round-counter and a static array for m, which would
    guarantee constant-time lookup. (This doesn't yield the chosen elements
    in sorted order.) However std::map should be sufficient.
  - In the exclusion-case, one could select n-k elements, and then leave
    them out in the result vector. This can be done by first override the
    values at the chosen index with n, and then remove those elements = n. DONE

2. Add a further version of shuffle, accepting RandGen_t.

*/

#ifndef ALGORITHMS_PE1w1ejM65
#define ALGORITHMS_PE1w1ejM65

#include <utility>
#include <numeric>
#include <algorithm>
#include <map>

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


  /* Helper function, choosing k from {0,...,n-1} by inclusion,
     without sorting (so that for k = n a permutation of 0,...,n-1 is returned:
  */
  inline vec_eseed_t choose_kn_inclusion(const gen_uint_t k, const gen_uint_t n, RandGen_t& g) {
    vec_eseed_t res;
    if (k > n or k == 0) return res;
    res.reserve(k);
    using U = UniformRange<RandGen_t>;
    if (k == 1) { res.push_back(U(g, n)()); return res; }
    std::map<gen_uint_t, gen_uint_t> M;
    {const auto first = U(g, n)();
     res.push_back(first);
     if (first != n-1) M.insert({first, n-1});
    }
    for (gen_uint_t i = 1; i < k; ++i) {
      const auto end = M.end();
      const auto it_current = M.find(n-i-1);
      const auto current = it_current == end ? n-i-1 : it_current->second;
      const auto choice = U(g, n-i)();
      const auto it_choice = M.find(choice);
      if (it_choice == end) {
        res.push_back(choice);
        if (choice != n-i-1) M.insert({choice, current});
      }
      else {
        res.push_back(it_choice -> second);
        if (choice != n-i-1) it_choice -> second = current;
      }
    }
    return res;
  }
  inline vec_eseed_t choose_kn(const gen_uint_t k, const gen_uint_t n, RandGen_t& g) {
    if (k > n or k == 0) return {};
    if (k > n/2) {
      vec_eseed_t res(n); std::iota(res.begin(), res.end(), 0);
      if (k == n) return res;
      for (const auto i : choose_kn_inclusion(n-k, n, g)) res[i] = n;
      res.erase(std::remove(res.begin(), res.end(), n), res.end());
      return res;
    }
    else return choose_kn_inclusion(k, n, g);
  }

}

#endif

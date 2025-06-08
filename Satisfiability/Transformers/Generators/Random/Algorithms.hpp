// Oliver Kullmann, 15.3.2019 (Swansea)
/* Copyright 2019, 2021, 2023, 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Algorithms based on random numbers

    - shuffle for shuffling a sequence:
        - shuffle(RAI begin, RAI end, randgen_t&(&) g)
        - shuffle(RAI begin, RAI end, RandGen_t&(&) g)
        - shuffle_seed(RAI begin, RAI end, const vec_eseed_t& seeds)

    - random_permutation for creating a random permutation of size N:
        - random_permutation(gen_uint_t N, RandGen_t& g) -> VEC
        - random_permutation(gen_uint_t N, const vec_eseed_t& seeds) -> VEC

    - Algorithm choose_kn for choosing k random numbers from 0, ..., n-1:
        - choose_kn(gen_uint_t k, gen_uint_t n, RandGen_t&(&) g, bool sorted)
          -> vec_eseed_t
          (see Bicliques::Algorithms::erase_krandom for removing k
          random elements from a vector)
        - choose_kn_inclusion(k,n,g) : this helper-function always makes k
          calls to g, while the above form makes n-k calls to g if that
          number is smaller.

TODOS:

1. Improve choose_kn(k, n, g):
  - Implement special case k=3; first creating test-cases to make
    sure that the current semantics are maintained.
  - For k very large one could implement M as the index-vector of length n,
    with value 0 meaning "not in domain" (no update will assign this value).
    - One needed computational experiments to check whether this yields
      improvements (and for what k).
  - Possibly more attractive is to use a hash-map for M.

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
  inline void shuffle(const RandomAccessIterator begin,
                      const RandomAccessIterator end, randgen_t& g) noexcept {
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
  template <class RandomAccessIterator>
  inline void shuffle(const RandomAccessIterator begin,
                      const RandomAccessIterator end, RandGen_t& g) noexcept {
    for (auto i = (end - begin) - 1; i > 0; --i) {
      using std::swap;
      swap(begin[i], begin[UniformRange(g, gen_uint_t(i+1))()]);
    }
  }
  template <class RAI>
  inline void shuffle(const RAI begin, const RAI end, RandGen_t&& g) noexcept {
    RandGen::shuffle(begin, end, g);
  }

  template <class RAI>
  inline void shuffle_seeds(const RAI begin, const RAI end,
                            const vec_eseed_t& seeds) noexcept {
    RandGen::shuffle(begin, end, RandGen_t(seeds));
  }


  template <class VEC>
  VEC random_permutation(const gen_uint_t N, RandGen_t& g) {
    VEC res; res.reserve(N);
    for (gen_uint_t i = 0; i < N; ++i) res.push_back(i);
    if (N <= 1) return res;
    shuffle(res.begin(), res.end(), g);
    return res;
  }
  template <class VEC>
  VEC random_permutation(const gen_uint_t N, const vec_eseed_t& seeds) {
    VEC res; res.reserve(N);
    for (gen_uint_t i = 0; i < N; ++i) res.push_back(i);
    if (N <= 1) return res;
    shuffle(res.begin(), res.end(), RandGen_t(seeds));
    return res;
  }


  /* Helper function, choosing k from {0,...,n-1} by inclusion,
     without sorting; for k = n thus a permutation of 0,...,n-1 is returned,
     and in general a uniform random permutation of the uniform random
     subset is returned.

     The algorithms maintains the bijective map P_i with
     domain id_{0,...,n-i-1}, for i = 0, ..., k-1, where range(P_i) is the set
     of the remaining elements from {0,...,n-1} to be chosen. In each round a
     random element choice from the domain is chosen, using P_i(choice) for the
     result, and P_i is updated in case choice is not the last element, mapping
     it now to that last element.
     P_0 is the identity, and the implementation actually only mantains the
     changes from the identity in map M_i (so M_0 is the empty map).
  */
  vec_eseed_t choose_kn_inclusion(const gen_uint_t k, const gen_uint_t n,
                                  RandGen_t& g) {
    if (k > n or k == 0) return {};
    using U = UniformRange<RandGen_t>;
    if (k == 1) return n == 1 ? vec_eseed_t{0} : vec_eseed_t{U(g, n)()};
    else if (k == 2) {
      const gen_uint_t a = U(g, n)(), b0 = U(g, n-1)(), b = b0==a ? n-1 : b0;
      return {a, b};
    }
    else {
      vec_eseed_t res;
      res.reserve(k);
      std::map<gen_uint_t, gen_uint_t> M; // the changes to the identity
      {const auto first_choice = U(g, n)();
        res.push_back(first_choice);
        if (first_choice != n-1) M.insert({first_choice, n-1});
      }
      for (gen_uint_t i = 1; i < k; ++i) {
        const auto end = M.end();
        const auto it_current = M.find(n-i-1);
        // "current" is the value of at the current end of the domain of P_i
        const auto current = it_current == end ? n-i-1 : it_current->second;
        const auto choice = U(g, n-i)();
        const auto it_choice = M.find(choice);
        if (it_choice == end) { // no previous selection of "choice"
          res.push_back(choice);
          if (choice != n-i-1) M.insert({choice, current});
        }
        else { // "choice" previously selected, and the mapping is used, and
          // updated if needed:
          res.push_back(it_choice -> second);
          if (choice != n-i-1) it_choice -> second = current;
        }
      }
      return res;
    }
  }
  // Now choosing inclusion or exclusion, depending on k.
  // If sorted is true, then the result is sorted (in ascending order),
  // otherwise the order is unspecified:
  vec_eseed_t choose_kn(const gen_uint_t k, const gen_uint_t n,
                        RandGen_t& g, const bool sorted = false) {
    if (k > n or k == 0) return {};
    else if (k == 1) return choose_kn_inclusion(1, n, g);
    else if (k > n/2) { // the exclusion-case
      vec_eseed_t res(n); std::iota(res.begin(), res.end(), 0);
      if (k == n) return res;
      for (const auto i : choose_kn_inclusion(n-k, n, g)) res[i] = n;
      res.erase(std::remove(res.begin(), res.end(), n), res.end());
      return res;
    }
    else if (k == 2) {
      using U = UniformRange<RandGen_t>;
      const gen_uint_t a = U(g, n)(), b0 = U(g, n-1)();
      return b0 == a ? vec_eseed_t{a, n-1} :
        (sorted and b0 < a ? vec_eseed_t{b0, a} : vec_eseed_t{a, b0});
    }
    else { // 3 <= k <= n/2 :
      if (not sorted) return choose_kn_inclusion(k, n, g);
      auto res = choose_kn_inclusion(k, n, g);
      std::ranges::sort(res);
      return res;
    }
  }
  vec_eseed_t choose_kn(const gen_uint_t k, const gen_uint_t n,
                        RandGen_t&& g, const bool sorted = false) {
    return RandGen::choose_kn(k, n, g, sorted);
  }

}

#endif

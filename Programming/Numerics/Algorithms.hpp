// Oliver Kullmann, 28.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  General algorithmic tools used in the context of sampling

    - next_combination allows to run through all combinations, via considering
      a range given by a vector of iterators, which are iterated through
      anti-lexicographically
    - get_vit_range(std::vector<ITER> begin, const vector<ITER> end) ->
        std::vector<std::vector<ITER>>
      shows how to use next_combination (by creating explicitly the
      range of iterator-vectors)
    here "iterator" just means that == and ++ are available (so for example numbers
    can be ITER).


TODOS:

1. Proper ranges for anti-lexicographical order:
    - next_combination likely should evolve into a proper
      range-generator.
    - So that the users (get_vit_range for a start) just employ the usual
      range-operations.
    - Same for Lockstep (this performs bundling of iterators).
    - So that the users (get_vcon_range for a start) again just employ the
      usual range-operations.

*/

#ifndef ALGORITHMS_XAVwhAdv5A
#define ALGORITHMS_XAVwhAdv5A

#include <vector>
#include <ranges>

#include <cassert>

namespace Sampling {

  // The range is given by begin, end (assumed to be fixed), as a vector
  // of iterators (just need ++ and end()), and this range is iterated through
  // in antilexicographical order, with current within this range; returns
  // false iff at the end, and always advances current:
  template <class ITER>
  bool next_combination(
      std::vector<ITER>& current,
      const std::vector<ITER>& begin, const std::vector<ITER>& end) noexcept {
    const auto N = current.size();
    assert(begin.size() == N);
    assert(end.size() == N);
    typedef typename std::vector<ITER>::size_type index_t;
    for (index_t i = 0; i < N; ++i) {
      assert(current[i] != end[i]);
      ++current[i];
      if (current[i] != end[i]) {
        for (index_t j = 0; j < i; ++j) current[j] = begin[j];
        return true;
      }
    }
    return false;
  }
  // Remark: Iterator is here taken in a very broad sense, so that also
  // integers themselves can be used.
  // Demonstration of how to run through such a "range", putting all combinations
  // of iterators between "begin" and "end" into the result:
  template <class ITER>
  std::vector<std::vector<ITER>>
  get_vit_range(const std::vector<ITER>& begin,
                const std::vector<ITER>& end) {
    const auto size = begin.size();
    assert(size == end.size());
    std::vector<std::vector<ITER>> res;
    if (begin == end) return res;
    assert(not std::ranges::equal(begin, end));
    std::vector<ITER> current = begin;
    do res.push_back(current);
    while (next_combination(current, begin, end));
    assert(current == end);
    return res;
  }

}

#endif

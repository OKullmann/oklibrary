// Oliver Kullmann, 27.10.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Lookup tables for the 2-sweep algorithm

   If N is even, then a board is divided into two parts of size N/2, otherwise
   the lower part has size N/2 + 1 (N/2 for the upper one).

   The struct SignificantZeroIndices contains two const arrays. The
   first/second array stores indices of the most/least significant zero in a
   bit-representation of 1<<bits_num unsigned values, where bits_num is the
   size of a board part. Two instances of the struct SignificantZeroIndices
   are made: lower_indices stores indices for the lower part (bottom and
   lower), while upper_indices reflects the upper part (upper, top).

   The function bottom takes the value of lower_indices.first, etc.
   An unsigned long representation of the bitset b from DoubleSweep is
   divided into two parts each of which is used as an index for
   arrays lower_indices/upper_indices.

   For the upper part, shift_value is additionally used to take into account
   the lower part while assigning upper and top.

   If the lower part is totally set, then the elements of lower_indices.first
   and lower_indices.second are set to N+1 and N, respectively. If the upper
   part is totally set, then the corresponding values are set to N and N-1.
   As a result, unrequired iterations in DoubleSweep:ucp() are skipped.

   Example 1: N=16, b is 0010001111111111 (binary). The unsigned long
   representation of b is 9215 (decimal), so it follows that bottom(9215) == 17,
   lower(9215) == 16, upper(9215) == 2, and top(9215) == 5.

   Example 2: N=17, b is 01000111111000100 (binary). The unsigned long
   representation of b is 36804 (decimal), so it follows that bottom(36804) == 0,
   lower(36804) == 5, upper(36804) == 10, and top(36804) == 15.

TODOS:

0. DONE Improve comments grammatically (where do they belong to?).

1. DONE Use std::array.

2. DONE Write tests

3. DONE Look into compilation issue for small N:

Tables.hpp: In instantiation of ‘constexpr Tables::SignificantZeroIndices<bits_num, shift_value>::SignificantZeroIndices() [with unsigned int bits_num = 0; unsigned int shift_value = 1]’:
Tables.hpp:92:90:   required from here
Tables.hpp:72:17: warning: comparison of unsigned expression in ‘< 0’ is always false [-Wtype-limits]
   72 |         for (; j<bits_num; ++j)

Comment: The above output constains unknown characters, likely a bad character
for quotation.
This should be avoided.

*/

#ifndef TABLES_csYg41f1faq
#define TABLES_csYg41f1faq

#include <array>

#include "Dimensions.hpp"

namespace Tables {
  using sizet = Dimensions::sizet;

  constexpr sizet lower_bits_num =
    Dimensions::N%2 == 1 ? Dimensions::N/2 + 1 : Dimensions::N/2;
  constexpr sizet upper_bits_num = Dimensions::N/2;
  // Set bits of the lower part (correspond to closed rows):
  constexpr sizet lower_set_bits = (size_t(1) << lower_bits_num) - sizet(1);
  // Set bits of the upper part:
  constexpr sizet upper_set_bits = (size_t(1) << upper_bits_num) - sizet(1);

  template<sizet bits_num, sizet shift_value>
  struct SignificantZeroIndices {
    std::array<sizet, size_t(1) << bits_num> first;
    std::array<sizet, size_t(1) << bits_num> second;

    consteval SignificantZeroIndices() noexcept : first{}, second{} {
      sizet max_index = (size_t(1) << bits_num) - 1;
      // If all bits are set (i.e. all rows of the current part are closed),
      // then the loop from first to second in DoubleSweep:ucp() will not iterate at all:
      first[max_index] = shift_value == 0 ? Dimensions::N+1 : Dimensions::N;
      second[max_index] = shift_value == 0 ? Dimensions::N : Dimensions::N-1;
      if (max_index > 0) {
        // If Dimensions::N > 1:
        const sizet bits_num_nonzero = bits_num;
        for (sizet i = 0; i < (size_t(1)<<bits_num_nonzero)-1; ++i) {
          sizet j=0;
          for (; j < bits_num_nonzero; ++j)
            // Find the most significant zero:
            if (!(i>>j & 1)) {
              // If exactly one zero, then first == second:
              second[i] = first[i] = j + shift_value;
              break;
            }
          // Find the least significant zero:
          for (sizet j2 = bits_num_nonzero-1; j2 > j; --j2)
            if (!(i>>j2 & 1)) {
              second[i] = j2 + shift_value;
              break;
            }
        }
      }
    }
  };

  // Bottom and lower indices: for the lower part of a board:
  constexpr SignificantZeroIndices<lower_bits_num, 0> lower_indices;
  // Upper and top indices: for the upper part of a board:
  constexpr SignificantZeroIndices<upper_bits_num, lower_bits_num> upper_indices;

  inline constexpr sizet bottom(const sizet closed_rows) noexcept {
    return lower_indices.first[closed_rows & lower_set_bits];
  }
  static_assert(Dimensions::N != 16 or Tables::bottom(9215) == 17);
  static_assert(Dimensions::N != 17 or Tables::bottom(36804) == 0);

  inline constexpr sizet lower(const sizet closed_rows) noexcept {
    return lower_indices.second[closed_rows & lower_set_bits];
  }
  static_assert(Dimensions::N != 16 or Tables::lower(9215) == 16);
  static_assert(Dimensions::N != 17 or Tables::lower(36804) == 5);

  inline constexpr sizet upper(const sizet closed_rows) noexcept {
    return upper_indices.first[(closed_rows >> lower_bits_num) & upper_set_bits];
  }
  static_assert(Dimensions::N != 16 or Tables::upper(9215) == 10);
  static_assert(Dimensions::N != 17 or Tables::upper(36804) == 12);

  inline constexpr sizet top(const sizet closed_rows) noexcept {
    return upper_indices.second[(closed_rows >> lower_bits_num) & upper_set_bits];
  }
  static_assert(Dimensions::N != 16 or Tables::top(9215) == 15);
  static_assert(Dimensions::N != 17 or Tables::top(36804) == 16);
}

#endif

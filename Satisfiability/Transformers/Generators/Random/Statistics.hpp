// Oliver Kullmann, 8.11.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Averages, variance etc.

   For a general overview see

   https://en.wikipedia.org/wiki/Consistent_estimator
   https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
   https://en.wikipedia.org/wiki/Standard_deviation#Uncorrected_sample_standard_deviation

*/

#ifndef STATISTICS_n7Ju3x1bcK
#define STATISTICS_n7Ju3x1bcK

#include <cmath>

#include "Numbers.hpp"

namespace RandGen {

  // Averages, variance, standard deviation,
  // simplest functionality, no storing, naive algorithm:
  template <typename IN, typename OUT>
  struct BasicStats {
    typedef IN input_t;
    typedef OUT output_t;
    typedef gen_uint_t count_t;
    count_t N = 0;
    input_t sum = 0;
    input_t sum_sq = 0;

    BasicStats& operator +=(const input_t x) noexcept {
      ++N;
      sum += x;
      sum_sq += x*x;
      return *this;
    }

    output_t amean() const noexcept {
      if (N == 0) return 0;
      return output_t(sum) / output_t(N);
    }
    output_t var_population() const noexcept {
      if (N <= 1) return 0;
      const output_t s = sum;
      const output_t n = N;
      return (output_t(sum_sq) - s*s / n) / n;
    }
    output_t var_unbiased() const noexcept {
      if (N <= 1) return 0;
      const output_t s = sum;
      return (output_t(sum_sq) - s*s / output_t(N)) / output_t(N-1);
    }
    output_t sd_population() const noexcept {
      return std::sqrt(var_population());
    }
    output_t sd_corrected() const noexcept {
      return std::sqrt(var_unbiased());
    }

    friend bool operator ==(const BasicStats& lhs, const BasicStats& rhs) noexcept {
      return lhs.N == rhs.N and lhs.sum == rhs.sum and lhs.sum_sq == rhs.sum_sq;
    }
  };

}

#endif

// Oliver Kullmann, 8.11.2020 (Swansea)
/* Copyright 2020, 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Averages, variance etc.

    - BasicStats<IN, OUT>
    - median<OUT, V>(V v)
    - StatsStors<IN, OUT>

   For a general overview see

   https://en.wikipedia.org/wiki/Consistent_estimator
   https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
   https://en.wikipedia.org/wiki/Standard_deviation#Uncorrected_sample_standard_deviation

*/

#ifndef STATISTICS_n7Ju3x1bcK
#define STATISTICS_n7Ju3x1bcK

#include <ostream>
#include <vector>
#include <algorithm>
#include <ostream>
#include <iomanip>

#include <cassert>
#include <cmath>
#include <cstdint>

#include <Numerics/FloatingPoint.hpp>
#include <Transformers/Generators/Random/Tests.hpp>

namespace GenStats {

  // Averages, variance, standard deviation,
  // simplest functionality, no storing, naive algorithm:
  template <typename IN, typename OUT>
  struct BasicStats {
    typedef IN input_t;
    typedef OUT output_t;
    typedef std::uint64_t count_t;
    count_t N = 0;
    input_t sum = 0;
    input_t sum_sq = 0;
    input_t min_ = std::numeric_limits<input_t>::max();
    input_t max_ = std::numeric_limits<input_t>::lowest();

    BasicStats& operator +=(const input_t x) noexcept {
      ++N;
      sum += x;
      sum_sq += x*x;
      if (x < min_) min_ = x;
      if (x > max_) max_ = x;
      return *this;
    }

    input_t min() const noexcept { return min_; }
    input_t max() const noexcept { return max_; }
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

    void simple_output(std::ostream& out) const {
      out << min() << " " << amean() << " " << max() << "; " <<
        sd_corrected();
    }

    friend bool operator ==(const BasicStats& lhs, const BasicStats& rhs) noexcept {
      return lhs.N == rhs.N and lhs.sum == rhs.sum and
        lhs.sum_sq == rhs.sum_sq and lhs.min_ == rhs.min_ and lhs.max_ == rhs.max_;
    }
  };


  template <typename OUT, class V>
  OUT median(const V& v) noexcept {
    assert(std::is_sorted(v.begin(), v.end()));
    const auto N = v.size();
    if (N == 0) return 0;
    else if (N % 2 == 1) return v[(N-1)/2];
    else return (OUT(v[N/2-1]) + OUT(v[N/2]))/ OUT(2);
  }

  template <typename IN, typename OUT>
  struct StatsStore {
    typedef IN input_t;
    typedef OUT output_t;

    typedef std::uint64_t count_t;
    typedef std::vector<input_t> vec_t;
    vec_t data;
    count_t N = 0;
    input_t sum = 0;
    input_t min_ = std::numeric_limits<input_t>::max();
    input_t max_ = std::numeric_limits<input_t>::lowest();
    bool sorted = false;

    StatsStore& operator +=(const input_t x) noexcept {
      data.push_back(x); sorted=false;
      ++N;
      sum += x;
      if (x < min_) min_ = x;
      if (x > max_) max_ = x;
      return *this;
    }

    input_t min() const noexcept { return min_; }
    input_t max() const noexcept { return max_; }
    output_t amean() const noexcept {
      if (N == 0) return 0;
      return output_t(sum) / output_t(N);
    }

    output_t sum_diffsquares() const noexcept {
      if (N <= 1) return 0;
      const output_t m = amean();
      output_t sum = 0;
      for (const input_t x : data) {
        const output_t diff = output_t(x) - m;
        sum += diff*diff;
      }
      return sum;
    }
    output_t var_population() const noexcept {
      if (N <= 1) return 0;
      return sum_diffsquares() / output_t(N);
    }
    output_t var_unbiased() const noexcept {
      if (N <= 1) return 0;
      return sum_diffsquares() / output_t(N-1);
    }
    output_t sd_population() const noexcept {
      return std::sqrt(var_population());
    }
    output_t sd_corrected() const noexcept {
      return std::sqrt(var_unbiased());
    }

    output_t median() noexcept {
      if (not sorted) {
        std::sort(data.begin(), data.end());
        sorted = true;
      }
      return GenStats::median<output_t, vec_t>(data);
    }

    RandGen::report_ks ks() {
      if (not sorted) {
        std::sort(data.begin(), data.end());
        sorted = true;
      }
      return RandGen::ks_P(data);
    }

    int width = 30;
    void simple_output(std::ostream& out, const bool with_ks = false) {
      const auto sd = sd_corrected();
      const auto med = median();
      const auto w = std::setw(width);
      out << "N=" << N << "\n"
          << w << min() << w << amean() << w << max() << "\n"
          << w << "median" << w << med << "\n"
          << w << "sd" << w << sd << "\n";
      if (with_ks) {
        const auto K = ks();
        out << "ks:" << w << K.d << w << K.ks << "\n";
      }
    }

  };

}

#endif

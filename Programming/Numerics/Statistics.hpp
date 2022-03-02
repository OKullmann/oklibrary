// Oliver Kullmann, 8.11.2020 (Swansea)
/* Copyright 2020, 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Averages, variance etc.

   One-dimensional sequences:

    - class BasicStats<IN, OUT>
    - function median<OUT, V>(V v)
    - class StatsStors<IN, OUT>

   Sequences of points (pairs of x/y-values):

    - class StatsPoints<FLOAT>

   Evaluating functions:

    - RandVal<IN, OUT> (statistically evaluating function-values for random
      arguments)

TODOS:

1. Improve documentation
  - For a general overview on statistics see
   https://en.wikipedia.org/wiki/Consistent_estimator
   https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
   https://en.wikipedia.org/wiki/Standard_deviation#Uncorrected_sample_standard_deviation

2. See todos in DifferentialEquations/Ode1.cpp.

*/

#ifndef STATISTICS_n7Ju3x1bcK
#define STATISTICS_n7Ju3x1bcK

#include <ostream>
#include <vector>
#include <algorithm>
#include <ostream>
#include <iomanip>
#include <array>
#include <functional>
#include <limits>
#include <numeric>
#include <ios>

#include <cassert>
#include <cmath>
#include <cstdint>

#include <Numerics/FloatingPoint.hpp>
#include <Transformers/Generators/Random/Tests.hpp>
#include <Transformers/Generators/Random/FPDistributions.hpp>

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

    friend bool operator ==(const BasicStats&, const BasicStats&) noexcept = default;
    friend std::ostream& operator <<(std::ostream& out, const BasicStats& s) {
      s.simple_output(out); return out;
    }
  };


  template <typename OUT, class V>
  OUT median(const V& v) noexcept {
    assert(std::is_sorted(v.begin(), v.end()));
    const auto N = v.size();
    if (N == 0) return 0;
    else if (N % 2 == 1) return v[(N-1)/2];
    else return std::midpoint<OUT>(v[N/2-1], v[N/2]);
  }

  /* StatsStore<IN, OUT>

     Now storing the data, and using the more precise calculation of
     statistics based on the summation of the square of the differences.

     - The computation of amean() does not trigger an update, but the
       computation of sum_sqd() or any related function does.
     - The median- and the ks-computation sorts the data (in ascending order).
     - If sum_sqd() or any of the functions involving it was called before,
       then that old value is kept (until further data is updated).
     - If in the same expression there are various expressions involving an
       update (so for example output-streaming), then it seems needed to call
       the update-function upfront.

  */

  template <typename IN, typename OUT>
  struct StatsStore {
    typedef IN input_t;
    typedef OUT output_t;

    typedef std::uint64_t count_t;
    typedef std::vector<input_t> vec_t;

    StatsStore() noexcept {}

    StatsStore& operator +=(const input_t x) noexcept {
      data_.push_back(x); sorted=false; utd=false;
      ++N_;
      sum_ += x;
      if (x < min_) min_ = x;
      if (x > max_) max_ = x;
      return *this;
    }

    input_t min() const noexcept { return min_; }
    input_t max() const noexcept { return max_; }
    count_t N() const noexcept { return N_; }
    input_t sum() const noexcept { return sum_; }
    const vec_t& data() const noexcept { return data_; }

    output_t amean() const noexcept {
      if (utd) return am;
      else return output_t(sum_) / output_t(N_);
    }

    void update() const noexcept {
      if (utd) return;
      assert(N_ >= 1);
      camean(); csqd(); utd = true;
    }
    output_t sum_sqd() const noexcept {
      update(); return sqd;
    }

    output_t var_population() const noexcept {
      if (N_ <= 1) return 0;
      return sum_sqd() / output_t(N_);
    }
    output_t var_unbiased() const noexcept {
      if (N_ <= 1) return 0;
      return sum_sqd() / output_t(N_-1);
    }
    output_t sd_population() const noexcept {
      return std::sqrt(var_population());
    }
    output_t sd_corrected() const noexcept {
      return std::sqrt(var_unbiased());
    }

    output_t median() noexcept {
      if (not sorted) {
        std::sort(data_.begin(), data_.end());
        sorted = true;
      }
      return GenStats::median<output_t, vec_t>(data_);
    }

    RandGen::report_ks ks() {
      if (not sorted) {
        std::sort(data_.begin(), data_.end());
        sorted = true;
      }
      return RandGen::ks_P(data_);
    }

    int width = 30;
    void simple_output(std::ostream& out, const bool with_ks = false) {
      const auto sd = sd_corrected();
      const auto med = median();
      const auto w = std::setw(width);
      out << "N=" << N_ << "\n"
          << w << min() << w << amean() << w << max() << "\n"
          << w << "median" << w << med << "\n"
          << w << "sd" << w << sd << "\n";
      if (with_ks) {
        const auto K = ks();
        out << "ks:" << w << K.d << w << K.ks << "\n";
      }
    }

  private :

    void camean() const noexcept {
      am = output_t(sum_) / output_t(N_);
    }
    void csqd() const noexcept {
      sqd = 0;
      for (const input_t x : data_) {
        const output_t diff = output_t(x) - am;
        sqd += diff*diff;
      }
    }

    vec_t data_;
    count_t N_ = 0;
    input_t sum_ = 0;
    input_t min_ = std::numeric_limits<input_t>::max();
    input_t max_ = std::numeric_limits<input_t>::lowest();

    mutable bool sorted = true;
    mutable bool utd = true;
    mutable output_t am = 0;
    mutable output_t sqd = 0;
  };


  /*  RandVal<IN, OUT>:

       - generate with dimension k and seed-sequence s
       - set the boundaries of the k-dimension input-cube
       - set the number N of iterations
       - create BasicStats-object from evaluating N times a function f
         on random points from the input-cube.

  */

  template<typename IN, typename OUT = IN>
  struct RandVal {
    typedef IN input_t;
    typedef OUT output_t;

    typedef std::uint64_t count_t;
    typedef std::vector<input_t> vec_t;
    typedef std::function<output_t(const vec_t&)> function_t;
    typedef typename vec_t::size_type size_t;
    typedef std::array<input_t, 2> interval_t;
    typedef std::vector<interval_t> bounds_t;
    typedef RandGen::vec_eseed_t seed_t;
    typedef BasicStats<input_t, output_t> stats_t;

    static constexpr count_t default_N = 1000;
    const size_t k;
    count_t N = default_N;
    bool sorted = false;

    RandVal(const size_t k, const seed_t& s) noexcept :
    k(k), rg(s), ib(k,{0,1}) {}

    input_t a(const size_t i) const noexcept {
      assert(i < k);
      return ib[i][0];
    }
    input_t b(const size_t i) const noexcept {
      assert(i < k);
      return ib[i][1];
    }
    void a(const size_t i, const input_t x) noexcept {
      assert(i < k);
      ib[i][0] = x;
    }
    void b(const size_t i, const input_t x) noexcept {
      assert(i < k);
      ib[i][1] = x;
    }

    stats_t run(const function_t& f) noexcept {
      vec_t x(k);
      stats_t res;
      typedef RandGen::Uniform80Range<RandGen::RandGen_t> gen_t;
      std::vector<gen_t> gen; gen.reserve(k);
      for (size_t i = 0; i < k; ++i)
        gen.emplace_back(rg, ib[i][0], ib[i][1]);
      for (count_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < k; ++j) x[j] = gen[j]();
        if (sorted) std::sort(x.begin(), x.end());
        res += f(x);
      }
      return res;
    }

  private :

    RandGen::RandGen_t rg;
    bounds_t ib;
  };


  template <typename FLOAT>
  struct StatsPoints {
    typedef FLOAT float_t;
    typedef std::uint64_t count_t;
    typedef std::array<float_t,2> point_t;

    count_t N;
    point_t xmin, xmax, ymin, ymax;
    float_t xmid, ymid, xspan, yspan, spanq, ymean, ysd, ymed;

    template <class VEC>
    StatsPoints(const VEC& v) noexcept {
      const auto size = v.size();
      assert(size != 0);
      assert(std::is_sorted(v.begin(), v.end(),
                            [](auto a, auto b){return a[0]<b[0];}));
      N = size;
      xmin = v.front(); xmax = v.back();
      xmid = std::midpoint<float_t>(xmin[0], xmax[0]);
      xspan = xmax[0] - xmin[0];
      ymin = {0, std::numeric_limits<float_t>::infinity()};
      ymax = {0, -std::numeric_limits<float_t>::infinity()};
      std::vector<float_t> yv; yv.reserve(size);
      {float_t ysum = 0;
       for (const auto [x,y] : v) {
         yv.push_back(y);
         ysum += y;
         if (y < ymin[1]) { ymin = {x,y}; }
         if (y > ymax[1]) { ymax = {x,y}; }
       }
       ymid = std::midpoint<float_t>(ymin[1], ymax[1]);
       yspan = ymax[1] - ymin[1];
       spanq = yspan / xspan;
       ymean = ysum / size;
      }
      {float_t ssum = 0;
       for (const float_t y : yv) {
         const float_t diff = y - ymean;
         ssum += diff*diff;
       }
       ysd = std::sqrt(ssum / size);
      }
      std::sort(yv.begin(), yv.end());
      ymed = median<float_t>(yv);
    }

    static constexpr int min_width = 20;
    static constexpr int max_width = 32;
    static constexpr std::streamsize min_prec = 5;
    static constexpr const char* def_name = "y";

    struct Format {
      int W = min_width;
      std::streamsize prec = min_prec;
      const char* name = def_name;
      bool sn = false;

      using E = FloatingPoint::WrapE<float_t>;

      Format() noexcept { E::deactivated = true; }
      Format(const char* const n) noexcept : name(n) {}
      Format (const std::streamsize p, const char* const n) noexcept :
        prec(p), name(n) {
        if (prec == -1) W = max_width;
      }
      Format(const std::streamsize p, const char* const n, const bool d)
        noexcept : prec(p), name(n), sn(not d) {
        if (prec == -1) W = max_width;
      }
      Format(const char* const n, const bool rp, const bool sn) noexcept :
        name(n), sn(sn) {
        if (not rp) {prec = -1; W = max_width;}
      }
    };

    void out(std::ostream& out, const Format& f = Format()) const {
      const auto old_prec = out.precision();
      using E = Format::E;
      const auto old_sn = E::deactivated;

      const int W = f.W;
      if (f.prec == -1)
        FloatingPoint::fullprec_floatg<float_t>(std::cout);
      else
        out.precision(f.prec);
      const std::string s = f.name;
      E::deactivated = not f.sn;

      using std::setw;
      const auto w = setw(W);
      const auto L = s.size();

      out <<
        "x" <<setw(W-1)<< xmin[0] <<w<< xmid <<w<< xmax[0] << "\n" <<
        (" "+s) <<setw(W-L-1)<< xmin[1] <<w<< " " <<w<< xmax[1] << "\n" <<
        s <<setw(W-L)<< E(ymin[1]) <<w<< E(ymid) <<w<< E(ymax[1]) << "\n"
        " x" <<setw(W-2)<< ymin[0] <<w<< " " <<w<< ymax[0] << "\n"
        " ads" <<setw(W-4)<< E(ymean) <<w<< E(ymed) <<w<< E(ysd) << "\n"
        "span-q" <<setw(2*W-6)<< spanq << "\n"
        ;

      out.precision(old_prec);
      E::deactivated = old_sn;
    }

  };

}

#endif

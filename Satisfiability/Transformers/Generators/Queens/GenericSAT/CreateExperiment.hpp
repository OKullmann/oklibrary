// Oliver Kullmann, 10.8.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef CREATEEXPERIMENT_8f3BWNHbg0
#define CREATEEXPERIMENT_8f3BWNHbg0

#include <vector>
#include <string>
#include <utility>
#include <exception>
#include <type_traits>
#include <limits>
#include <ostream>

#include "Numerics/FloatingPoint.hpp"

namespace CreateExperiment {

  typedef int par_t; // parameters
  static_assert(std::is_integral_v<par_t>);
  static_assert(std::is_signed_v<par_t>);
  static constexpr par_t min_par_t = std::numeric_limits<par_t>::min();
  static constexpr par_t max_par_t = std::numeric_limits<par_t>::max();
  static_assert(-1 - min_par_t == max_par_t);
  typedef std::make_unsigned<par_t>::type unsigned_par_t;
  constexpr unsigned_par_t max_unsigned_par_t = std::numeric_limits<unsigned_par_t>::max();
  static_assert(2*unsigned_par_t(max_par_t) + 1 == max_unsigned_par_t);

  struct ParRange {
    const par_t a, b;
    const unsigned_par_t si;
    ParRange(const par_t a, const par_t b) : a(a), b(b), si(s(a,b)) {}
    unsigned_par_t size() const noexcept { return si; }
    static constexpr unsigned_par_t s(const par_t a, const par_t b) {
      if (a > b)
        throw std::domain_error("ParRange: a=" + std::to_string(a) +
          " > b=" + std::to_string(b) + ".");
      if (a == min_par_t and b == max_par_t)
        throw std::domain_error("ParRange: extreme case of a=min_par_t and "
          "b=max_par_t not allowed.");
      if (a >= 0 or b < 0) return unsigned_par_t(b-a) + 1;
      return unsigned_par_t(b) + unsigned_par_t(-(a+1)) + 2;
    }
  };
  static_assert(ParRange::s(0, max_par_t) == unsigned_par_t(max_par_t)+1);
  static_assert(ParRange::s(min_par_t, -1) == unsigned_par_t(max_par_t)+1);
  static_assert(ParRange::s(min_par_t+1, max_par_t) == max_unsigned_par_t);
  static_assert(ParRange::s(min_par_t, max_par_t-1) == max_unsigned_par_t);

  typedef std::vector<ParRange> parrange_v;
  typedef parrange_v::size_type size_t;

  size_t size(const parrange_v& v) {
    if (v.empty()) return 0;
    FloatingPoint::float80 prod = 1;
    for (const ParRange& r : v) prod *= r.size();
    if (prod > std::numeric_limits<size_t>::max())
        throw std::domain_error("size(parrange_v): "
          "product of possibilities too large.");
    return prod;
  }

  // Pair of arguments-string and target-name:
  typedef std::pair<std::string, std::string> job_description;
  typedef std::vector<job_description> job_description_v;
  constexpr char target_prefix = 't';

  void extend_jobs(const parrange_v& v, const size_t i, const std::vector<par_t>& par, job_description_v& res) {
    assert(i <= v.size());
    assert(i == par.size());
    if (i == v.size()) {
      assert(i != 0);
      std::string arguments(std::to_string(par[0]));
      std::string target(target_prefix + std::to_string(par[0]));
      for (size_t k = 1; k < i; ++k) {
        const std::string p = std::to_string(par[k]);
        arguments += " " + p;
        target += "_" + p;
      }
      res.emplace_back(arguments, target);
      return;
    }
    else {
      par_t k = v[i].a;
      for (; k < v[i].b; ++k) {
        auto parcopy(par);
        parcopy.push_back(k);
        extend_jobs(v, i+1, parcopy, res);
      }
      assert(k == v[i].b);
      auto parcopy(par);
      parcopy.push_back(k);
      extend_jobs(v, i+1, parcopy, res);
    }
  }

  job_description_v make_job_description(const parrange_v& v) {
    const auto s = size(v);
    job_description_v res; res.reserve(s);
    if (s == 0) return res;
    assert(not v.empty());
    extend_jobs(v, 0, {}, res);
    assert(res.size() == s);
    return res;
  }

  void write_makefile(std::ostream& out, const job_description_v& v, const std::string& executable, const std::string& resultfile) {
    out << "SHELL = /bin/bash\n.SUFFIXES :\n.PHONY : all run transfer\n"
      "all : run transfer\n\nrun :";
    for (const auto& j : v) out << " " << j.second;
    out << "\n\n";
    for (const auto& j : v)
      out << j.second << " :\n\t/usr/bin/time -f\" %e %M\" " << executable
          << " " << j.first << " rd &> " << j.second << "\n";
    out << "\n\ntransfer :\n\tcat";
    for (const auto& j : v) out << " " << j.second;
    out << " >> " << resultfile << "\n";
  }

}

#endif

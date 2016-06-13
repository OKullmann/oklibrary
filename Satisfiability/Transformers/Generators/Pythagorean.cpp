// Oliver Kullmann, 12.6.2016 (Swansea)
/* Copyright 2016 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/Pythagorean.cpp
  \brief CNF generator for the boolean Pythagorean tuples problem

  For triples, uses
  > Pythagorean n 3 0
  or
  > Pythagorean n 3 1
  (doesn't matter here).

  For quadruples, use
  > Pythagorean n 4 0
  while the injective form (all components different) is obtained by
  > Pythagorean n 4 1

  For the third parameter d >= 0, also larger values can be used, which enforce
  a respective minimum distance between the (sorted) components of the tuples

    x_1^2 + ... + x_{K-1}^2 = x_K^2,

  that is, x_i + d <= x_{i+1} for 1 <= i <= K-1.

  Requires C++11. Output to standard output. Compile with

  > g++ -Wall --std=c++11 -Ofast -o Pythagorean Pythagorean.cpp

*/

#include <iostream>
#include <string>
#include <limits>
#include <cmath>
#include <vector>
#include <algorithm>

namespace {

  typedef unsigned long int uint_t;
  typedef long int int_t;

  enum {
    errcode_parameter = 1,
    errcode_too_large = 2,
    errcode_too_small = 3,
    errcode_not_yet = 4
  };

  const std::string program = "Pythagorean";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.1.2";

  typedef std::vector<uint_t> tuple_t;
  typedef std::vector<tuple_t> vector_t;

}

int main(const int argc, const char* const argv[]) {
  if (argc != 4) {
    std::cerr << err << "Exactly three arguments are needed:\n"
     " - The number of elements.\n"
     " - The size of the tuple.\n"
     " - The enforced distance between components.\n";
    return errcode_parameter;
  }

  const uint_t n = std::stoul(argv[1]);

  const uint_t K = std::stoul(argv[2]);
  if (K <= 2) {
    std::cerr << err << "Second input " << K << " must be at least 2.\n";
    return errcode_too_small;
  }
  if ( K > 5) {
    std::cerr << err << "Second input " << K << " currently must be at most 5.\n";
    return errcode_not_yet;
  }
  const uint_t abs_max = std::sqrt(std::numeric_limits<uint_t>::max()) / K;
  if (n > abs_max) {
    std::cerr << err << "First input " << n << " larger than maximal allowed value: " << abs_max << ".\n";
    return errcode_too_large;
  }

  const uint_t dist = std::stoul(argv[3]);
  if (dist > abs_max) {
    std::cerr << err << "Third input " << dist << " larger than maximal allowed value: " << abs_max << ".\n";
    return errcode_too_large;
  }

  // Computing the list of Pythagorean tuples:
  vector_t res;
  
  const uint_t n2 = n*n;
  uint_t max = 0;

  if (K == 3) {
    for (uint_t a = 1; a < n; ++a) {
      const uint_t a2 = a*a;
      for (uint_t b = a+dist; b < n; ++b) {
        const uint_t b2 = b*b;
        const uint_t c2 = a2 + b2;
        if (c2 > n2) break;
        const uint_t c = std::sqrt(c2);
        if (c*c != c2) continue;
        if (c < b+dist) continue;
        if (c > max) max = c;
        res.push_back({{a,b,c}});
      }
    }
  }
  else if (K == 4) {
    for (uint_t a = 1; a < n; ++a) {
      const uint_t a2 = a*a;
      for (uint_t b = a+dist; b < n; ++b) {
        const uint_t b2 = b*b;
        for (uint_t c = b+dist; c < n; ++c) {
          const uint_t c2 = c*c;
          const uint_t d2 = a2 + b2 + c2;
          if (d2 > n2) break;
          const uint_t d = std::sqrt(d2);
          if (d*d != d2) continue;
          if (d < c+dist) continue;
          if (d > max) max = d;
          res.push_back({{a,b,c,d}});
        }
      }
    }
  }
  else {
    for (uint_t a = 1; a < n; ++a) {
      const uint_t a2 = a*a;
      for (uint_t b = a+dist; b < n; ++b) {
        const uint_t b2 = b*b;
        for (uint_t c = b+dist; c < n; ++c) {
          const uint_t c2 = c*c;
          for (uint d = c+dist; d < n; ++d) {
            const uint_t d2 = d*d;
            const uint_t e2 = a2 + b2 + c2 + d2;
            if (e2 > n2) break;
            const uint_t e = std::sqrt(e2);
            if (e*e != e2) continue;
            if (e < d+dist) continue;
            if (e > max) max = e;
            res.push_back({{a,b,c,d,e}});
          }
        }
      }
    }
  }

  // removing duplicates:
  for (auto& x : res) x.erase(std::unique(x.begin(), x.end()), x.end());

  // anti-lexicographical sorting:
  std::sort(res.begin(), res.end(),
    [](const tuple_t& x, const tuple_t& y) {
      return std::lexicographical_compare(x.rbegin(), x.rend(), y.rbegin(), y.rend());
    }
  );

  // DIMACS output:
  std::cout << "c Boolean Pythagorean " << K << "-tuples problem, up to n=" << n << ", with minimum-distance between (sorted) components = " << dist << ", yielding " << res.size() << " tuples.\n";
  std::cout << "c OKlibrary, program " << program << " in version " << version << ".\n";
  std::cout << "p cnf " << max << " " << 2*res.size() << "\n";
  for (const auto& x : res) {
    for (const auto i : x) std::cout << i << " "; std::cout << "0 ";
    for (const int_t i : x) std::cout << -i << " "; std::cout << "0\n";
  }
}

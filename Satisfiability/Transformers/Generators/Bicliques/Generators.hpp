// Oliver Kullmann, 26.2.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Graph generators

  - From Graphs:
   - AdjMapStr
   - size_t

  - scoped enum Types:
   - clique      (n)
   - biclique    (n,m)
   - crown       (n)    a biclique(n,n) minus one perfect matching
   - grid        (n,m)

  - For each type T:
   - bcc_T(params) (minimum bc-cover)
     realised by cnf_t(params)
   - bcp_T(params) (minimum bc-partition, if known)
   - numcc_T(params) (number of connected components)
   - T(params) generatoring AdjMapStr

  - Helper functions:
   - acnf(k) for the canonical unsatisfiable cnf with k variables.

  - create(argc, argv) for generating all the T's


TODOS:

1. Providing a creation-mode, which creates only half of the edges

2. Prove that bcp(crown(n)) = n.

3. What is bcp(grid(n,m)) ?

*/

#ifndef GENERATORS_1twuS7HUpM
#define GENERATORS_1twuS7HUpM

#include <string>
#include <vector>
#include <exception>
#include <sstream>
#include <algorithm>

#include <cassert>
#include <cstdlib>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "Graphs.hpp"
#include "DimacsTools.hpp"

namespace Generators {

  namespace DT = DimacsTools;

  typedef Graphs::AdjMapStr AdjMapStr;

  typedef AdjMapStr::size_t size_t;


  enum class Types { clique=0, biclique=1, crown=2, grid=3 };
}
namespace Environment {
  template <>
  struct RegistrationPolicies<Generators::Types> {
    static constexpr int size = int(Generators::Types::grid)+1;
    static constexpr std::array<const char*, size> string
    {"clique", "biclique", "crown", "grid"};
  };
}
namespace Generators {
  std::ostream& operator <<(std::ostream& out, const Types t) {
    switch (t) {
    case Types::clique : return out << "clique";
    case Types::biclique : return out << "biclique";
    case Types::crown : return out << "crown";
    case Types::grid : return out << "grid";
    default : return out << "Types::UNKNOWN";
    }
  }


  size_t bcc_clique(const size_t n) noexcept {
    if (n == 0) return 0;
    else return FloatingPoint::ceil(FloatingPoint::log2(n));
  }
  size_t bcp_clique(const size_t n) noexcept {
    if (n == 0) return 0;
    else return n-1;
  }
  size_t numcc_clique(const size_t n) noexcept {
    return n == 0 ? 0 : 1;
  }
  AdjMapStr clique(const size_t n) {
    AdjMapStr G(Graphs::GT::und);
    G.comments.push_back("clique(" + std::to_string(n) + ")");
    G.comments.push_back("bcc=" + std::to_string(bcc_clique(n)));
    if (n == 0) return G;
    std::vector<std::string> V; V.reserve(n);
    for (size_t i = 0; i < n; ++i) V.push_back(std::to_string(i+1));
    [[maybe_unused]] const auto res = G.add_clique(V);
    assert(res.first == n and res.second == (n*(n-1)) / 2);
    return G;
  }
  DT::DimacsClauseList acnf(const DT::var_t k) {
    assert(k <= 63);
    if (k == 0) return DT::DimacsClauseList{{0,1},{{}}};
    const DT::var_t size = FloatingPoint::exp2(k);
    DT::DimacsClauseList F; F.second.reserve(size);
    F = acnf(k-1);
    assert(F.first.n == k-1); assert(F.first.c == size/2);
    for (DT::var_t i = 0; i < size/2; ++i)
      F.second.push_back(F.second[i]);
    assert(F.second.size() == size);
    for (DT::var_t i = 0; i < size/2; ++i)
      F.second[i].emplace_back(k,1);
    for (DT::var_t i = size/2; i < size; ++i)
      F.second[i].emplace_back(k,-1);
    F.first.n = k; F.first.c = size;
    return F;
  }
  DT::DimacsClauseList cnf_clique(const size_t n) {
    DT::DimacsClauseList F; F.second.reserve(n);
    if (n == 0) return F;
    const DT::var_t k = FloatingPoint::log2(n);
    F = acnf(k); const DT::var_t size = F.first.c;
    if (size == n) return F;
    assert(size < n);
    for (DT::var_t i = 0; i < n - size; ++i)
      F.second.push_back(F.second[i]);
    assert(F.second.size() == n);
    for (DT::var_t i = 0; i < size; ++i)
      F.second[i].emplace_back(k+1,1);
    for (DT::var_t i = size; i < n; ++i)
      F.second[i].emplace_back(k+1,-1);
    F.first = {k+1,n};
    return F;
  }


  size_t bcc_biclique(const size_t n, const size_t m) noexcept {
    if (n == 0 or m == 0) return 0;
    else return 1;
  }
  size_t bcp_biclique(const size_t n, const size_t m) noexcept {
    if (n == 0 or m == 0) return 0;
    else return 1;
  }
  size_t numcc_biclique(const size_t n, const size_t m) noexcept {
    return std::min(n,m) == 0 ? std::max(n,m) : 1;
  }
  AdjMapStr biclique(const size_t n, const size_t m, const bool with_c = true) {
    AdjMapStr G(Graphs::GT::und);
    if (with_c) {
      G.comments.push_back("biclique(" + std::to_string(n) + "," +
                           std::to_string(m) + ")");
      G.comments.push_back("bcc=" + std::to_string(bcc_biclique(n,m)));
    }
    if (n == 0 and m == 0) return G;
    std::vector<std::string> V1; V1.reserve(n);
    for (size_t i = 0; i < n; ++i) V1.push_back("l" + std::to_string(i+1));
    std::vector<std::string> V2; V2.reserve(m);
    for (size_t i = 0; i < m; ++i) V2.push_back("r" + std::to_string(i+1));
    [[maybe_unused]] const auto res = G.add_biclique(V1, V2);
    assert(res.first == n+m and res.second == n*m);
    return G;
  }


  bool special_case_grid(size_t n, size_t m) noexcept {
    assert(n >= 2 and m >= 2);
    if (n > m) std::swap(n,m);
    if (n % 2 == 1) return false;
    const auto [p,q] = std::lldiv(m-1, n-1);
    if (q % 2 == 0) return q/2 < p;
    else return (q + (n - 1))/2 < size_t(p)-1;
  }
  // According to [Guo, Huynh, Macchia 2019]:
  size_t bcc_grid(const size_t n, const size_t m) noexcept {
    if (n == 0 or m == 0) return 0;
    if (n == 1) return m/2;
    if (m == 1) return n/2;
    if (special_case_grid(n,m)) return (n*m) / 2 - 1;
    else return (n*m) / 2;
  }
  size_t numcc_grid(const size_t n, const size_t m) noexcept {
    return n == 0 or m == 0 ? 0 : 1;
  }
  AdjMapStr grid(const size_t n, const size_t m) {
    AdjMapStr G(Graphs::GT::und);
    G.comments.push_back("grid(" + std::to_string(n) + "," +
                         std::to_string(m) + ")");
    G.comments.push_back("bcc=" + std::to_string(bcc_grid(n,m)));
    if (n == 0 or m == 0) return G;
    for (size_t i = 0; i < n; ++i) {
      const std::string is = std::to_string(i+1) + ",";
      std::vector<std::string> rowi; rowi.reserve(m);
      for (size_t j = 0; j < m; ++j)
        rowi.push_back(is + std::to_string(j+1));
      [[maybe_unused]] const auto res = G.add_path(rowi);
      assert(res.first == m); assert(res.second == m-1);
    }
    assert(G.n() == n*m); assert(G.m() == n*(m-1));
    for (size_t j = 0; j < m; ++j) {
      const std::string js = "," + std::to_string(j+1);
      std::vector<std::string> columnj; columnj.reserve(n);
      for (size_t i = 0; i < n; ++i)
        columnj.push_back(std::to_string(i+1) + js);
      [[maybe_unused]] const auto res = G.add_path(columnj);
      assert(res.first == 0); assert(res.second == n-1);
    }
    assert(G.n() == n*m); assert(G.m() == n*(m-1) + m*(n-1));
    return G;
  }


  size_t bcc_crown(const size_t n) noexcept {
    return FloatingPoint::inv_fcbinomial_coeff(n);
  }
  size_t bcp_crown(const size_t n) noexcept {
    return n;
  }
  size_t numcc_crown(const size_t n) noexcept {
    switch (n) {
    case 0 : return 0;
    case 1 : return 2;
    case 2 : return 2;
    default : return 1;}
  }
  AdjMapStr crown(const size_t n) {
    AdjMapStr G = biclique(n,n,false);
    G.comments.push_back("crown(" + std::to_string(n) + ")");
    G.comments.push_back("bcc=" + std::to_string(bcc_crown(n)));
    std::vector<std::pair<std::string,std::string>> matching;
    matching.reserve(n);
    for (size_t i =0; i < n; ++i)
      matching.emplace_back("l" + std::to_string(i+1),
                            "r" + std::to_string(i+1));
    [[maybe_unused]] const size_t res = G.remove_edgesr(matching);
    assert(res == n);
    assert(G.n() == 2*n and G.m() == n * (n-1));
    return G;
  }


  AdjMapStr create(const int argc, const char* const argv[]) {
    if (argc < 2)
      throw std::invalid_argument("Generators::create: no arguments");
    const auto opt = Environment::read<Types>(argv[1]);
    if (not opt)
      throw std::invalid_argument(std::string("Generators::create: option=")
                                  + argv[1]);

    const Types t = opt.value();
    switch (t) {
    case Types::clique : {
      if (argc < 3)
        throw std::invalid_argument("Generators::create:clique: argc=2");
      const size_t N{FloatingPoint::toUInt(argv[2])};
      return clique(N);
    }
    case Types::biclique : {
      if (argc < 4)
        throw std::invalid_argument("Generators::create:biclique: argc=" +
                                    std::to_string(argc));
      const size_t N{FloatingPoint::toUInt(argv[2])};
      const size_t M{FloatingPoint::toUInt(argv[3])};
      return biclique(N,M);
    }
    case Types::crown : {
      if (argc < 3)
        throw std::invalid_argument("Generators::create:crown: argc=2");
      const size_t N{FloatingPoint::toUInt(argv[2])};
      return crown(N);
    }
    case Types::grid : {
      if (argc < 4)
        throw std::invalid_argument("Generators::create:grid: argc=" +
                                    std::to_string(argc));
      const size_t N{FloatingPoint::toUInt(argv[2])};
      const size_t M{FloatingPoint::toUInt(argv[3])};
      return grid(N,M);
    }
    default : assert(0);
      throw std::range_error("Generators::create: UNKNOWN t="
                                  + std::to_string(int(t)));
    }
  }

}

#endif

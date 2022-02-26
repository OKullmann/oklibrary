// Oliver Kullmann, 26.2.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Graph generators

*/

#ifndef GENERATORS_1twuS7HUpM
#define GENERATORS_1twuS7HUpM

#include <string>
#include <vector>
#include <exception>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumInOut.hpp>
#include <Numerics/Conversions.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "Graphs.hpp"

namespace Generators {

  typedef Graphs::AdjMapStr AdjMapStr;

  typedef AdjMapStr::size_t size_t;


  enum class Types { clique=0, biclique=1, crown=2 };
}
namespace Environment {
  template <>
  struct RegistrationPolicies<Generators::Types> {
    static constexpr int size = int(Generators::Types::crown)+1;
    static constexpr std::array<const char*, size> string
    {"clique", "biclique", "crown"};
  };
}
namespace Generators {
  std::ostream& operator <<(std::ostream& out, const Types t) {
    switch (t) {
    case Types::clique : return out << "clique";
    case Types::biclique : return out << "biclique";
    case Types::crown : return out << "crown";
    default : return out << "Types::UNKNOWN";
    }
  }


  size_t bcc_clique(const size_t n) {
    if (n == 0) return 0;
    else return FloatingPoint::ceil(FloatingPoint::log2(n));
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

  size_t bcc_biclique(const size_t n, const size_t m) {
    if (n == 0 or m == 0) return 0;
    else return 1;
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

  size_t bcc_crown(const size_t n) {
    return FloatingPoint::inv_fcbinomial_coeff(n);
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
        throw std::invalid_argument("Generators::create:clique: argc=2");
      const size_t N{FloatingPoint::toUInt(argv[2])};
      return crown(N);
    }
    default : assert(0);
      throw std::range_error("Generators::create: UNKNOWN t="
                                  + std::to_string(int(t)));
    }
  }

}

#endif

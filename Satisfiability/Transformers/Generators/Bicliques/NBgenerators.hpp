// Oliver Kullmann, 4.6.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Generators for nonbolean (multi-)clause-sets

*/

#ifndef NBGENERATORS_AoX6P27dqY
#define NBGENERATORS_AoX6P27dqY

#include <exception>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/Algorithms.hpp>
#include <Numerics/NumBasicFunctions.hpp>
#include <Transformers/Generators/Random/GenClauseSets.hpp>

#include "Algorithms.hpp"
#include "GenConflictGraphs.hpp"

namespace NBgenerators {

  namespace GL = GenLit;
  namespace GC = GenClauses;
  namespace GCS = GenClauseSets;

  typedef GCS::gclauseset_t gclauseset_t;
  typedef GCS::GClauseList GClauseList;

  enum class Types { complete=0 };
  constexpr int size(Types) noexcept { return int(Types::complete) + 1; }
}
namespace Environment {
  template <>
  struct RegistrationPolicies<NBgenerators::Types> {
    static constexpr int size = size(NBgenerators::Types{});
    static constexpr std::array<const char*, size> string
    {"complete"};
  };
}
namespace NBgenerators {
  std::ostream& operator <<(std::ostream& out, const Types t) {
    switch (t) {
    case Types::complete : return out << "complete";}
    return out << "Types::UNKNOWN";
  }


  gclauseset_t full_range(const GC::GClause& C, const GC::GClause& D,
                               const GL::var_t mult = 1) {
    if (not varequal_valleq(C,D) or mult == 0) return {};
    const GL::var_t size = [&C,&D]{GL::var_t res = 1;
                                   for (GL::var_t i = 0; i < C.size(); ++i)
                                     res *= val(D[i])-val(C[i]);
                                   return res;}();
    if (size == 0) return {};
    gclauseset_t res; res.reserve(mult*size);
    GC::gclause_t current(C.C);
    do
      if (mult == 1) res.push_back(current);
      else res.insert(res.end(), mult, current);
    while (Sampling::next_combination(current, C.C, D.C));
    assert(res.size() == mult*size);
    return res;
  }

  constexpr FloatingPoint::float80
  numcl_full_nbcls(const GL::var_t n, const GL::val_t dom,
                   const GL::var_t mult) noexcept {
    return FloatingPoint::pow(dom,n) * mult;
  }
  GClauseList full_nbcls(const GL::var_t n, const GL::val_t dom,
                         const GL::var_t mult = 1) {
    GClauseList res; res.dom.assign(n, dom);
    if ((dom == 0 and n >= 1) or mult == 0) return res;
    GC::GClause C, D; C.C.reserve(n); D.C.reserve(n);
    for (GL::var_t i = 0; i < n; ++i) {
      C.push_back({i,0}); D.push_back({i,dom});
    }
    res.F = full_range(C,D,mult);
    assert(res.c() == numcl_full_nbcls(n,dom,mult));
    assert((mult == 1 and res.is_clauseset()) or res.is_fully_standardised());
    return res;
  }


  GClauseList create(const int argc, const char* const argv[]) {
    const std::string err = "NBgenerators::create: ";
    if (argc < 2)
      throw std::invalid_argument(err + "no arguments");
    const auto opt = Environment::read<Types>(argv[1]);
    if (not opt)
      throw std::invalid_argument(err + "type=" + argv[1]);
    using FloatingPoint::toUInt; using std::to_string;
    const Types t = opt.value();
    switch (t) {
    case Types::complete : {
      if (argc < 5)
        throw std::invalid_argument(err + "complete: num-args="
                                    + to_string(argc-2) + " # 3");
      size_t n, dom, mult;
      try {
        n = toUInt(argv[2]);
        dom = toUInt(argv[3]);
        mult = toUInt(argv[4]);
      }
      catch (std::exception& e) {
        throw std::invalid_argument(err + "reading parameters:\n  " +
                                    e.what());
      }
      return full_nbcls(n,dom,mult);
    }}
    throw std::range_error(err + " UNKNOWN t=" + to_string(int(t)));
  }

}

#endif

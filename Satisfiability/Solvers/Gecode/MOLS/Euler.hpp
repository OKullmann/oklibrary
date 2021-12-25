// Oliver Kullmann, 25.12.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

TODOS:

1. Remove references to iostream
    - Functions should always take a std::istream& or std::ostream& parameter.

*/

#ifndef EULER_g2rGlQAGDl
#define EULER_g2rGlQAGDl

#include <ostream>
#include <tuple>
#include <vector>
#include <string>
#include <iostream>
#include <array>

#include <cstdlib>

#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/search.hh>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/Conversions.hpp>
#include <Transformers/Generators/Random/LSRG.hpp>

namespace Euler {

  namespace GC = Gecode;
  namespace LS = LatinSquares;
  namespace RG = RandGen;

  typedef std::vector<int> gecode_intvec_t;
  typedef std::vector<GC::IntVar> gecode_intvarvec_t;
  typedef std::vector<std::string> partial_ls_t;

  constexpr LS::ls_dim_t N_default = 0;
  constexpr LS::ls_dim_t k_default = 2;


  // Pgopagation level for Gecode constraints.
  // def: default propagation (can be different for different
  //      constraints).
  // val: value propagation (the fastest and the simplest one).
  // bnd: bound propagation (average speed and performance).
  // dom: domain propagation (the slowest and the most effective).
  enum class PropO {def=0, val=1, bnd=2, dom=3};
  constexpr int PropOsize = 4;

  enum class HeO {show=0, noshow=1};
  constexpr int HeOsize = 2;
  enum class StatO {show=0, noshow=1};
  constexpr int StatOsize = 2;
  enum class SolO {show=0, noshow=1};
  constexpr int SolOsize = 2;
}
namespace Environment {
  template <> struct RegistrationPolicies<Euler::PropO> {
    static constexpr int size = Euler::PropOsize;
    static constexpr std::array<const char*, size> string {"def", "val", "bnd", "dom"};
  };
  template <> struct RegistrationPolicies<Euler::HeO> {
    static constexpr int size = Euler::HeOsize;
    static constexpr std::array<const char*, size> string {"+head", "-head"};
  };
  template <> struct RegistrationPolicies<Euler::StatO> {
    static constexpr int size = Euler::StatOsize;
    static constexpr std::array<const char*, size> string {"+stat", "-stat"};
  };
  template <> struct RegistrationPolicies<Euler::SolO> {
    static constexpr int size = Euler::SolOsize;
    static constexpr std::array<const char*, size> string {"+sol", "-sol"};
  };
}
namespace Euler {
  constexpr char sep = ',';

  typedef std::tuple<PropO> gecode_option_t;
  std::ostream& operator <<(std::ostream& out, const PropO m) {
    switch (m) {
    case PropO::val : return out << "values-prop";
    case PropO::bnd : return out << "bounds-prop";
    case PropO::dom : return out << "domain-prop";
    default : return out << "default-prop";}
  }

  typedef std::tuple<HeO, StatO, SolO> output_option_t;
  std::ostream& operator <<(std::ostream& out, const HeO m) {
    switch (m) {
    case HeO::show : return out << "show-header";
    default : return out << "noshow-header";}
  }
  std::ostream& operator <<(std::ostream& out, const StatO m) {
    switch (m) {
    case StatO::show : return out << "show-statistics";
    default : return out << "noshow-statistics";}
  }
  std::ostream& operator <<(std::ostream& out, const SolO m) {
    switch (m) {
    case SolO::show : return out << "show-solutions";
    default : return out << "noshow-solutions";}
  }


  LS::ls_dim_t read_N(const std::string& s, const std::string& error) noexcept {
    if (s.empty()) return N_default;
    const LS::ls_dim_t N = FloatingPoint::touint(s);
    if (not LS::valid(N) and N != 0) {
      std::cerr << error << "N must be a nonnegative integer in [0,"
                << LS::max_dim-1 << "]" << ", but N=" << N << ".\n";
      std::exit(int(RG::Error::domain));
    }
    return N;
  }
  LS::ls_dim_t read_N(const std::string& error) noexcept {
    std::string s;
    std::cin >> s;
    return read_N(s, error);
  }

  LS::ls_dim_t read_k(const std::string& s,
                      const std::string& error) noexcept {
    if (s.empty()) return k_default;
    const LS::ls_dim_t k = FloatingPoint::touint(s);
    if (not LS::valid(k) and k != 0) {
      std::cerr << error << "k must be a nonnegative integer in [0,"
                << LS::max_dim-1 << "]" << ", but k=" << k << ".\n";
      std::exit(int(RG::Error::domain));
    }
    return k;
  }
  LS::ls_dim_t read_k(const std::string& error) noexcept {
    std::string s;
    std::cin >> s;
    return read_k(s, error);
  }

  gecode_intvec_t read_partial_ls(const LS::ls_dim_t N) noexcept {
    assert(N > 0);
    const LS::ls_dim_t size = N*N;
    gecode_intvec_t partial_ls(size);
    std::string s;
    partial_ls_t ls_s;
    do {
      std::cin >> s;
      if (s.empty()) continue;
      if (N > 10) ls_s.push_back(s);
      else for (auto c : s) ls_s.push_back(std::string(1,c));
      assert(ls_s.size() <= size);
    } while (ls_s.size() != size);
    for (LS::ls_dim_t i=0; i < size; ++i) {
      assert(i < partial_ls.size() and i < ls_s.size());
      partial_ls[i] = (ls_s[i] == "*") ? -1 : std::stoi(ls_s[i]);
    }
    return partial_ls;
  }



}

#endif

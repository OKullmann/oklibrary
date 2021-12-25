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
#include <iomanip>

#include <cstdlib>

#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/search.hh>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/Conversions.hpp>
#include <Transformers/Generators/Random/LSRG.hpp>

#include "../Lookahead.hpp"

namespace Euler {

  namespace GC = Gecode;
  namespace LS = LatinSquares;
  namespace RG = RandGen;
  namespace LA = Lookahead;

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


  LS::ls_dim_t given_cells(const gecode_intvec_t ls_partial) {
    LS::ls_dim_t res = 0;
    for (auto x : ls_partial) res += x==-1 ? 0 : 1;
    return res;
  }

  void print_header() {
    std::cout
      << "N k m1 m2 brt brsrc brsol bregr brpr prp t sat nds inds inds2 inds3 lvs "
      << "ulvs sol 1chld chcs taus sbps chct taut sbpt ptime prog vers\n";
  }

  void print_stat(const LS::ls_dim_t N, const LS::ls_dim_t k,
                  const LS::ls_dim_t m1, const LS::ls_dim_t m2,
                  const double reading_time, const double solving_time,
                  const LA::option_t alg_options,
                  const gecode_option_t gc_options,
                  const Statistics::SearchStat stat,
                  const Environment::ProgramInfo& proginfo) {
    const auto sat = stat.solutions==0 ? 0 : 1;
    const auto lvs = stat.unsat_leaves + stat.solutions;
    const LA::BrTypeO brt = std::get<LA::BrTypeO>(alg_options);
    const LA::BrSourceO brsrc = std::get<LA::BrSourceO>(alg_options);
    const LA::BrSolutionO brsol = std::get<LA::BrSolutionO>(alg_options);
    const LA::BrEagernessO bregr = std::get<LA::BrEagernessO>(alg_options);
    const LA::BrPruneO brpr = std::get<LA::BrPruneO>(alg_options);
    Environment::RegistrationPolicies<LA::BrTypeO> rp_brt;
    Environment::RegistrationPolicies<LA::BrSourceO> rp_brsrc;
    Environment::RegistrationPolicies<LA::BrSolutionO> rp_brsol;
    Environment::RegistrationPolicies<LA::BrEagernessO> rp_bregr;
    Environment::RegistrationPolicies<LA::BrPruneO> rp_brpr;
    const std::string sbrt = rp_brt.string[int(brt)];
    const std::string sbrsrc = rp_brsrc.string[int(brsrc)];
    const std::string sbrsol = rp_brsol.string[int(brsol)];
    const std::string sbregr = rp_bregr.string[int(bregr)];
    const std::string sbrpr = rp_brpr.string[int(brpr)];

    const Euler::PropO prop = std::get<Euler::PropO>(gc_options);
    Environment::RegistrationPolicies<Euler::PropO> rp_prop;
    const std::string sprop = rp_prop.string[int(prop)];

    const unsigned prec_time = 4;
    const auto fi = std::fixed;
    std::cout << std::setprecision(prec_time) << fi << N << " " << k
              << " " << m1 << " " << m2 << " " << sbrt << " " << sbrsrc
              << " " << sbrsol << " " << sbregr << " " << sbrpr
              << " " << sprop << " " << solving_time << " " << sat << " "
              << stat.nodes << " " << stat.inner_nodes << " "
              << stat.inner_nodes_2chld << " " << stat.inner_nodes_3chld
              << " " << lvs << " " << stat.unsat_leaves << " "
              << stat.solutions << " " << stat.single_child_brnch << " "
              << stat.choice_time.N() << " " << stat.tau_time.N() << " "
              << stat.subproblem_time.N() << " " << stat.choice_time.sum()
              << " " << stat.tau_time.sum() << " "
              << stat.subproblem_time.sum() << " " << reading_time << " "
              << proginfo.prg << " " << proginfo.vrs << "\n";
  }

}

#endif

// Oliver Kullmann, 26.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Options for Gecode

*/

#ifndef OPTIONS_JwU0BOX6Tw
#define OPTIONS_JwU0BOX6Tw

#include <array>
#include <ostream>
#include <exception>
#include <string>

#include <gecode/int.hh>

#include <ProgramOptions/Environment.hpp>

namespace Options {

  namespace GC = Gecode;

  // Propagation levels for Gecode-constraints:
  enum class PropO {
    dom=0, // domain propagation (strongest)
    def=1, // default propagation (different for different constraints)
    val=2, // values propagation (weakest)
    bnd=3}; // bounds propagation
  constexpr int PropOsize = int(PropO::bnd) + 1;
  GC::IntPropLevel prop_level(const PropO po) noexcept {
    switch (po) {
    case PropO::dom: return GC::IPL_DOM;
    case PropO::def: return GC::IPL_DEF;
    case PropO::val: return GC::IPL_VAL;
    case PropO::bnd: return GC::IPL_BND;
    default : return GC::IPL_DOM;}
  }

  // Variable-selection for Gecode-branching ("branching-heuristic variables"):
  enum class BHV {
    first = 1, // first open
    mindeg = 2, // smallest degree
    maxdeg = 3, // largest degree
    mindom = 4, // smallest domain
    maxdom = 5, // largest domain
    mindegdom = 6, // smallest degree/domain
    maxdegdom = 7, // largest degree/domain
    minminreg = 8, // smallest minimum-regret
    maxminreg = 9, // largest minimum-regret
    minmaxreg = 10, // smallest maximum-regret
    maxmaxreg = 11 // largest maximum-regret
  };
  constexpr int BHVsize = int(BHV::maxmaxreg) + 1;
  GC::IntVarBranch var_branch(const BHV bvar) {
    switch (bvar) {
    case BHV::first: return GC::INT_VAR_NONE();
    case BHV::mindeg: return GC::INT_VAR_DEGREE_MIN();
    case BHV::maxdeg: return GC::INT_VAR_DEGREE_MAX();
    case BHV::mindom: return GC::INT_VAR_SIZE_MIN();
    case BHV::maxdom: return GC::INT_VAR_SIZE_MAX();
    case BHV::mindegdom: return GC::INT_VAR_DEGREE_SIZE_MIN();
    case BHV::maxdegdom: return GC::INT_VAR_DEGREE_SIZE_MAX();
    case BHV::minminreg: return GC::INT_VAR_REGRET_MIN_MIN();
    case BHV::maxminreg: return GC::INT_VAR_REGRET_MIN_MAX();
    case BHV::minmaxreg: return GC::INT_VAR_REGRET_MAX_MIN();
    case BHV::maxmaxreg: return GC::INT_VAR_REGRET_MAX_MAX();
    default : throw std::runtime_error("Options::var_branch: UNKNOWN bvar="
                                       + std::to_string(int(bvar)));
    }
  }

  // Values-section for Gecode-branching (also determining the branching-type
  // "binary" vs "enumerativ"); for us "branching-heuristic order":
  enum class BHO {
    binvalmin = 1, // INT_VAL_MIN()
    binvalmax = 2, // INT_VAL_MAX()
    enumvalmin = 3, // INT_VALUES_MIN()
    enumvalmax = 4 // INT_VALUES_MAX()
  };
  constexpr int BHOsize = int(BHO::enumvalmax) + 1;
  GC::IntValBranch val_branch(const BHO bord) {
    switch (bord) {
    case BHO::binvalmin: return GC::INT_VAL_MIN();
    case BHO::binvalmax: return GC::INT_VAL_MAX();
    case BHO::enumvalmin: return GC::INT_VALUES_MIN();
    case BHO::enumvalmax: return GC::INT_VALUES_MAX();
    default : throw std::runtime_error("Options::val_branch: UNKNOWN bord="
                                       + std::to_string(int(bord)));
    }
  }

}
namespace Environment {
  template <> struct RegistrationPolicies<Options::PropO> {
    static constexpr int size = Options::PropOsize;
    static constexpr std::array<const char*, size>
      string {"dom", "def", "val", "bnd"};
  };
  template <> struct RegistrationPolicies<Options::BHV> {
    static constexpr int size = Options::BHVsize;
    static constexpr std::array<const char*, size>
    string {"first", "mindeg", "maxdeg", "mindom", "maxdom", "mindegdom",
        "maxdegdom", "minminreg", "maxminreg", "minmaxreg", "maxmaxreg"};
  };
  template <> struct RegistrationPolicies<Options::BHO> {
    static constexpr int size = Options::BHOsize;
    static constexpr std::array<const char*, size>
      string {"bvalmin", "bvalmax", "evalmin", "evalmax"};
  };
}
namespace Options {

  std::ostream& operator <<(std::ostream& out, const PropO m) {
    switch (m) {
    case PropO::dom : return out << "domain-prop";
    case PropO::def : return out << "default-prop";
    case PropO::val : return out << "values-prop";
    case PropO::bnd : return out << "bounds-prop";
    default : return out << "Options::PropO: UNKNOWN=" << int(m);}
  }
  std::ostream& operator <<(std::ostream& out, const BHV bvar) {
    switch (bvar) {
    case BHV::first: return out << "first-var";
    case BHV::mindeg: return out << "min-deg-var";
    case BHV::maxdeg: return out << "max-deg-var";
    case BHV::mindom: return out << "min-dom-var";
    case BHV::maxdom: return out << "max-dom-var";
    case BHV::mindegdom: return out << "min-deg/dom-var";
    case BHV::maxdegdom: return out << "max-deg/dom-var";
    case BHV::minminreg: return out << "min-min-reg-var";
    case BHV::maxminreg: return out << "max-min-reg-var";
    case BHV::minmaxreg: return out << "min-max-reg-var";
    case BHV::maxmaxreg: return out << "max-max-reg-var";
    default : return out << "Options::BHV: UNKNOWN=" << int(bvar);}
  }
  std::ostream& operator <<(std::ostream& out, const BHO bord) {
    switch (bord) {
    case BHO::binvalmin: return out << "bin-branch-min";
    case BHO::binvalmax: return out << "bin-branch-max";
    case BHO::enumvalmin: return out << "enum-branch-min";
    case BHO::enumvalmax: return out << "enum-branch-max";
    default : return out << "Options::BHO: UNKNOWN=" << int(bord);}
  }

}

#endif

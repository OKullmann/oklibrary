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

#include <gecode/int.hh>

#include <ProgramOptions/Environment.hpp>

namespace Options {

  namespace GC = Gecode;

  // Propagation levels for Gecode-constraints:
  // dom: domain propagation (the strongest).
  // def: default propagation (can be different for different
  //      constraints).
  // val: values propagation (the weakest).
  // bnd: bounds propagation.
  enum class PropO {dom=0, def=1, val=2, bnd=3};
  constexpr int PropOsize = int(PropO::bnd)+1;
}
namespace Environment {
  template <> struct RegistrationPolicies<Options::PropO> {
    static constexpr int size = Options::PropOsize;
    static constexpr std::array<const char*, size>
      string {"dom", "def", "val", "bnd"};
  };
}
namespace Options {

  std::ostream& operator <<(std::ostream& out, const PropO m) {
    switch (m) {
    case PropO::dom : return out << "domain-prop";
    case PropO::def : return out << "default-prop";
    case PropO::val : return out << "values-prop";
    case PropO::bnd : return out << "bounds-prop";
    default : return out << "Options::PropP: UNKNOWN=" << int(m);}
  }

  GC::IntPropLevel prop_level(const PropO po) noexcept {
    switch (po) {
    case PropO::dom: return GC::IPL_DOM;
    case PropO::def: return GC::IPL_DEF;
    case PropO::val: return GC::IPL_VAL;
    case PropO::bnd: return GC::IPL_BND;
    default : return GC::IPL_DOM;}
  }

}

#endif

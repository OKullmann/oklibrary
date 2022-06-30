// Oliver Kullmann, 26.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Options for Gecode

  Namespace Options, abbreviated "OP".

  Providing option-types (as supported by Environment.hpp):

    General parameters:
     - RT
     - PropO
     - STO, LRST

    Gecode-based branching for gcMols and rlaMols:
     - BHV
     - BRT
     - GBO
    Support-class for the interface to Gecode (just plain scoped enum):
     - BHO

    Lookahead-reduction for rlaMols and laMols:
     - LAR

    Lookahead-branching for laMols:
     - LBRT branching-type (lookahead-brt) bin, enu; plus later binbal
     - LBRO branching-order asc, desc, ascd, descd
     - DIS distance deltaL, wdeltaL, newvars
     - SPW special-weights other, zero, one, ap, ld
     - EXW weights for experiments rand, asc, desc

    Output-options:
     - INFO on, off
     - WGHTS on, off
     - HDS (headers) on, off
     - COMP on, off
     - WrapP<POL> to control default-values for batchmode
       resulting in Info, Weights, Headers, Computations
     - SIVA the output-variables; output-modifiers:
       - NEG off, on
       - STOP on, off
       - STAT ave, min, max, stddev
       - NOTY inode, leaf

     - output_options_t: tuple of the above
     - function adapt(output_options_t) -> output_options_t

     - struct OutputOptions: wrapper around output_options_t.

TODOS

0. Write documentation.

1. Provide the choice of a random branching-variable:
    - This needs a bit of design how to use our random-number-generator.
    - We could just use our 64-bit generator, and throw away half of
      the bits (or xor the first 32 with the second 32 bits).

2. Provide for Gecode-variable-choices some of the active forms:
    - Thus adding more possibilities to BHV.
    - We need to see that we can also simulate this.

3. Real choice of branch-order for binary branching (simulated Gc-branching):
    - The value-choice for branching (min- or max-value) needs to be
      separated from the order of the branches (which of =, != first).
    - Easiest perhaps to have this as "branching types" bmin, bmax (for
      the choice of the value).
    - Then the "branching order", asc or desc, could be used for the
      order of the branches (asc means first =).
    - One would need thus a special type for branching-order for sgc-branching,
      which would break the current simple implementation of sgcMols, just
      employing a macro at one place.
    - While still using the same basis would mean that for gcMols there are
      non-functional combination, namely for the choice of bmin/bmax then
      asc, desc becomes irrelevant.

4. Extended possibilities for simulated Gecode-branching (sgcMols):
    - When sharing facilities with gcMols, one could throw an exception
      for non-provided possibilities --- but that would break the looping
      over all possible options.
    - Or the macro SIMBRANCH needs to be used at more places; especially
      proving different definitions of BRT and BHV.
    - Random order of branches.
    - Last variable.

*/

#ifndef OPTIONS_JwU0BOX6Tw
#define OPTIONS_JwU0BOX6Tw

#include <vector>
#include <array>
#include <ostream>
#include <exception>
#include <string>
#include <algorithm>
#include <tuple>

#include <gecode/int.hh>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumTypes.hpp>

namespace Options {

  namespace GC = Gecode;

  typedef std::vector<FloatingPoint::float80> weights_t;


  // Run-Type:
  enum class RT {
    sat_solving = 0,
    sat_decision = 1,
    unique_solving = 2,
    unique_s_with_log = 3,
    unique_decision = 4,
    unique_d_with_log = 5,
    count_solutions = 6,
    count_with_log = 7,
    enumerate_solutions = 8,
    enumerate_with_log = 9
  };
  constexpr int RTsize = int(RT::enumerate_with_log) + 1;
  constexpr bool valid(const RT rt) noexcept {
    return int(rt) < RTsize;
  }
  constexpr bool count_only(const RT rt) noexcept {
    return rt == RT::count_solutions or rt == RT::count_with_log
      or rt == RT::enumerate_with_log;
  }
  constexpr bool with_file_output(const RT rt) noexcept {
    return rt == RT::sat_solving or rt == RT::unique_solving
      or rt == RT::enumerate_solutions;
  }
  constexpr bool with_solutions(const RT rt) noexcept {
    return rt == RT::sat_solving or
      rt == RT::unique_solving or rt == RT::unique_s_with_log or
      rt == RT::enumerate_solutions or rt == RT::enumerate_with_log;
  }
  constexpr bool with_log(const RT rt) noexcept {
    return rt == RT::unique_s_with_log or rt == RT::unique_d_with_log or
      rt == RT::count_with_log or rt == RT::enumerate_with_log;
  }
  constexpr bool test_sat(const RT rt) noexcept {
    return rt == RT::sat_solving or rt == RT::sat_decision;
  }
  constexpr bool test_unique(const RT rt) noexcept {
    return rt == RT::unique_solving or rt == RT::unique_s_with_log or
      rt == RT::unique_decision or rt == RT::unique_d_with_log;
  }
  constexpr size_t with_stop(const RT rt) noexcept {
    if (test_sat(rt)) return 1;
    else if (test_unique(rt)) return 2;
    else return 0;
  }


  // Propagation levels for Gecode-constraints:
  enum class PropO {
    dom=0, // domain propagation (strongest)
    def=1, // default propagation (different for different constraints)
    val=2, // values propagation (weakest)
    bnd=3 // bounds propagation
  };
  constexpr int PropOsize = int(PropO::bnd) + 1;
  GC::IntPropLevel prop_level(const PropO po) noexcept {
    switch (po) {
    case PropO::dom: return GC::IPL_DOM;
    case PropO::def: return GC::IPL_DEF;
    case PropO::val: return GC::IPL_VAL;
    case PropO::bnd: return GC::IPL_BND;
    default : return GC::IPL_DOM;}
  }


  // Stopping the search engine, for gcMols:
  enum class STO {
    none=0,
    by_gnds=1,
    by_flvs=2,
    by_time=3
  };
  constexpr int STOsize = int(STO::by_time) + 1;
  // Stopping the search after la-reduction:
  enum class LRST {
    none=0,
    nds=1,
    lvs=2,
    inds=3,
    satc=4,
  };
  constexpr int LRSTsize = int(LRST::satc) + 1;


  // Variable-selection for Gecode-branching ("branching-heuristic variables").
  // According to Section 8.5.1 of 'Modeling and Programming with Gecode.
  // 6.2.0.', the degree of a variable is the number of propagators depending
  // on it.
  enum class BHV {
    mindom = 0, // smallest domain
    maxdeg = 1, // largest degree
    maxdegdom = 2, // largest degree/domain
    first = 3, // first open
    mindeg = 4, // smallest degree
    maxdom = 5, // largest domain
    mindegdom = 6, // smallest degree/domain
  };
  constexpr int BHVsize = int(BHV::mindegdom) + 1;
  GC::IntVarBranch var_branch(const BHV bvar) {
    switch (bvar) {
    case BHV::mindom: return GC::INT_VAR_SIZE_MIN();
    case BHV::maxdeg: return GC::INT_VAR_DEGREE_MAX();
    case BHV::maxdegdom: return GC::INT_VAR_DEGREE_SIZE_MAX();
    case BHV::first: return GC::INT_VAR_NONE();
    case BHV::mindeg: return GC::INT_VAR_DEGREE_MIN();
    case BHV::maxdom: return GC::INT_VAR_SIZE_MAX();
    case BHV::mindegdom: return GC::INT_VAR_DEGREE_SIZE_MIN();
    default : throw std::runtime_error("Options::var_branch: UNKNOWN bvar="
                                       + std::to_string(int(bvar)));
    }
  }


  // The branching-type:
  enum class BRT {
    bin = 0, // binary branching
    enu = 1 // enumerative branching
  };
  constexpr int BRTsize = int(BRT::enu) + 1;

  enum class LBRT {
    bin = 0,
    enu = 1
  };
  constexpr int LBRTsize = int(LBRT::enu) + 1;


  // The Gecode-branching-orders:
  enum class GBO {
    asc = 0, // ascending
    desc = 1 // descending
  };
  constexpr int GBOsize = int(GBO::desc) + 1;

  // Values-section for Gecode-branching (also determining the branching-type
  // "binary" vs "enumerativ"); for us "branching-heuristic order":
  enum class BHO {
    binvalmin = 0, // INT_VAL_MIN()
    binvalmax = 1, // INT_VAL_MAX()
    enumvalmin = 2, // INT_VALUES_MIN()
    enumvalmax = 3 // INT_VALUES_MAX()
  };
  constexpr BHO translate(const BRT bt, const GBO bo) noexcept {
    if (bt == BRT::bin)
      if (bo == GBO::asc) return BHO::binvalmin;
      else return BHO::binvalmax;
    else
      if (bo == GBO::asc) return BHO::enumvalmin;
      else return BHO::enumvalmax;
  }
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

  enum class LBRO {
    asc = 0,
    desc = 1,
    ascd = 2,
    descd = 3
  };
  constexpr int LBROsize = int(LBRO::descd) + 1;


  // Algorithmic options for lookahead-reduction, in two dimensions:
  // relaxed vs eager and pruning vs nonpruning
  enum class LAR {
    rel_pr = 0,
    rel_npr = 1,
    eag_pr = 2,
    eag_npr = 3
  };
  constexpr int LARsize = int(LAR::eag_npr) + 1;
  constexpr bool eager(const LAR lar) noexcept { return int(lar) >= 2; }
  constexpr bool pruning(const LAR lar) noexcept { return int(lar) % 2 == 0; }


  enum class DIS {
    wdeltaL = 0,
    newvars = 1
  };
  constexpr int DISsize = int(DIS::newvars) + 1;

  enum class SPW { // special patterns for weights
    other=0,
    zero=1,
    one=2,
    ap=3,
    ld=4
  };
  constexpr int SPWsize = int(SPW::ld) + 1;

  enum class EXW { // experimental weights
    rand=0,
    asc=1,
    desc=2
  };
  constexpr int EXWsize = int(EXW::desc) + 1;


  enum class INFO { on=0, off=1 };
  enum class WGHTS { on=0, off=1 };
  enum class HDS { on=0, off=1 };
  enum class COMP { on=0, off=1 };

  // Wrapper for different default in batchmode:
  template <typename P, bool bothon = false>
  struct WrapP {
    typedef P policy_t;
    static constexpr bool both_on = bothon;
    inline static P def = P::on;
    static void set_def(const bool batch_mode) noexcept {
      if constexpr (not bothon)
        def = batch_mode ? P::off : P::on;
    }
    static constexpr int size = int(P::off) + 1;
    P o;
    WrapP() noexcept : o(def) {}
    WrapP(const P p) noexcept : o(p) {}
    WrapP(const int code) noexcept : o(P(code)) {}

    operator P() const noexcept { return o; }
    int code() const noexcept { return int(o); }
  };

  typedef WrapP<INFO> Info;
  typedef WrapP<WGHTS> Weights;
  typedef WrapP<HDS> Headers;
  typedef WrapP<COMP, true> Computations;

  // Single values:
  enum class SIVA {
    all=0,satc=1,t=2,ppc=3,nds=4,inds=5,lvs=6,
    mu0=7,qfppc=8,pprunes=9,pmprune=10,pprobes=11,rounds=12,solc=13,
      tr=14,pelvals=15,dp=16,
    mu1=17,w=18,ltau=19,minp=20,meanp=21,maxp=22,sdd=23,tb=24,
    estlvs=25
  };
  constexpr int SIVAsize = int(SIVA::estlvs) + 1;

  // Additional specifications, ignored if not relevant:
  enum class NEG { off=0, on=1 };
  constexpr int NEGsize = int(NEG::on) + 1;
  enum class STOP { on=0, off=1 };
  constexpr int STOPsize = int(STOP::off) + 1;
  enum class STAT { ave=0, min=1, max=2, stddev=3 };
  constexpr int STATsize = int(STAT::stddev) + 1;
  enum class NOTY { inode=0, leaf=1 };
  constexpr int NOTYsize = int(NOTY::leaf) + 1;

  typedef std::tuple<Info, Weights, Headers, Computations,
                     SIVA, NEG, STOP, STAT, NOTY>
    output_options_t;

  // Adapt to output of single value:
  output_options_t adapt(output_options_t in) noexcept {
    if (std::get<SIVA>(in) == SIVA::all) return in;
    std::get<Headers>(in) = HDS::off;
    return in;
  }

  struct OutputOptions {
    const output_options_t options;
    static void set_def(const bool bm) noexcept {
      Info::set_def(bm); Weights::set_def(bm); Headers::set_def(bm);
      Computations::set_def(bm);
    }
    OutputOptions(output_options_t o) noexcept : options(adapt(o)) {}

    bool with_info() const noexcept {
      return INFO(std::get<Info>(options)) == INFO::on;
    }
    bool with_weights() const noexcept {
      return WGHTS(std::get<Weights>(options)) == WGHTS::on;
    }
    bool with_headers() const noexcept {
      return HDS(std::get<Headers>(options)) == HDS::on;
    }
    bool with_computations() const noexcept {
      return COMP(std::get<Computations>(options)) == COMP::on;
    }
    SIVA values() const noexcept { return std::get<SIVA>(options); }
    bool negated() const noexcept {
      return std::get<NEG>(options) == NEG::on;
    }
    bool single_valued() const noexcept { return values() != SIVA::all; }
    bool with_stop() const noexcept {
      return std::get<STOP>(options) == STOP::on;
    }
    STAT stat() const noexcept { return std::get<STAT>(options); }
    NOTY node_type() const noexcept { return std::get<NOTY>(options); }
  };

}
namespace Environment {
  template <> struct RegistrationPolicies<Options::RT> {
    static constexpr const char* sname = "rt";
    static constexpr int size = Options::RTsize;
    static constexpr std::array<const char*, size>
      string {"sats", "satd",
        "uniques", "+uniques", "uniqued", "+uniqued",
        "count", "+count",
        "enum", "+enum"};
    static constexpr std::array<const char*, size>
      estring {"sat-solving", "sat-decision",
        "unique-solving", "unique-sol-with-log",
        "unique-decision", "unique-dec-with-log",
        "count-solutions", "count-sols-with-log",
        "enumerate-solutions", "enum-sols-with-log"};
  };
  template <> struct RegistrationPolicies<Options::PropO> {
    static constexpr const char* name = "propagation-level";
    static constexpr const char* sname = "pl";
    static constexpr int size = Options::PropOsize;
    static constexpr std::array<const char*, size>
      string {"dom", "def", "val", "bnd"};
    static constexpr std::array<const char*, size>
      estring {"domain-prop", "default-prop", "values-prop", "bounds-prop"};
  };
  template <> struct RegistrationPolicies<Options::STO> {
    static constexpr const char* name = "gecode-stopping";
    static constexpr int size = Options::STOsize;
    static constexpr std::array<const char*, size>
      string {"none", "gnds", "flvs", "t"};
    static constexpr std::array<const char*, size>
      estring {"none", "by-gc-node-count", "by-failed-leaf-count", "by-time"};
  };
  template <> struct RegistrationPolicies<Options::LRST> {
    static constexpr const char* name = "la-reduction-stopping";
    static constexpr int size = Options::LRSTsize;
    static constexpr std::array<const char*, size>
    string {"none", "nds", "lvs", "inds", "satc"};
    static constexpr std::array<const char*, size>
      estring {"none", "by-node-count", "by-leaf-count",
        "by-inner-node-count", "by-solution-count"};
  };
  template <> struct RegistrationPolicies<Options::BHV> {
    static constexpr const char* name = "variable-heuristic";
    static constexpr const char* sname = "bv";
    static constexpr int size = Options::BHVsize;
    static constexpr std::array<const char*, size>
    string {"mindom", "maxdeg", "maxdegdom",
        "first", "mindeg", "maxdom", "mindegdom"};
    static constexpr std::array<const char*, size>
      estring {"min-dom-var", "max-deg-var", "max-deg/dom-var",
        "first-var", "min-deg-var", "max-dom-var", "min-deg/dom-var"};
  };
  template <> struct RegistrationPolicies<Options::BRT> {
    static constexpr const char* name = "branching-type";
    static constexpr const char* sname = "bt";
    static constexpr int size = Options::BRTsize;
    static constexpr std::array<const char*, size>
      string {"bin", "enu"};
    static constexpr std::array<const char*, size>
      estring {"binary-branching", "enumerative-branching"};
  };
  template <> struct RegistrationPolicies<Options::LBRT> {
    static constexpr const char* name = "la-branching-type";
    static constexpr const char* sname = "lbt";
    static constexpr int size = Options::LBRTsize;
    static constexpr std::array<const char*, size>
      string {"bin", "enu"};
    static constexpr std::array<const char*, size>
      estring {"binary-branching", "enumerative-branching"};
  };
  template <> struct RegistrationPolicies<Options::GBO> {
    static constexpr const char* name = "order-heuristic";
    static constexpr const char* sname = "bo";
    static constexpr int size = Options::GBOsize;
    static constexpr std::array<const char*, size>
      string {"asc", "desc"};
    static constexpr std::array<const char*, size>
      estring {"ascending-order", "descending-order"};
  };
  template <> struct RegistrationPolicies<Options::LBRO> {
    static constexpr const char* name = "la-order-heuristic";
    static constexpr const char* sname = "lbo";
    static constexpr int size = Options::LBROsize;
    static constexpr std::array<const char*, size>
      string {"asc", "desc", "dasc", "ddesc"};
    static constexpr std::array<const char*, size>
    estring {"ascending-order", "descending-order",
        "distances-ascending", "distances-descending"};
  };
  template <> struct RegistrationPolicies<Options::LAR> {
    static constexpr const char* name = "la-reduction-type";
    static constexpr const char* sname = "lar";
    static constexpr int size = Options::LARsize;
    static constexpr std::array<const char*, size>
      string {"relpr", "relnpr", "eagpr", "eagnpr"};
    static constexpr std::array<const char*, size>
      estring {"relaxed-pruning", "relaxed-nonpruning",
        "eager-pruning", "eager-nonpruning"};
  };
  template <> struct RegistrationPolicies<Options::DIS> {
    static constexpr const char* name = "distance-type";
    static constexpr const char* sname = "dis";
    static constexpr int size = Options::DISsize;
    static constexpr std::array<const char*, size>
      string {"wdL", "newv"};
    static constexpr std::array<const char*, size>
      estring {"weighted-delta-literals", "new-variables"};
  };
  template <> struct RegistrationPolicies<Options::SPW> {
    static constexpr const char* name = "special-weights";
    static constexpr int size = Options::SPWsize;
    static constexpr std::array<const char*, size>
    string {"", "Z", "O", "A", "L"};
    static constexpr std::array<const char*, size>
      estring {"disabled", "zero-weights", "one-weights", "ap-weights",
        "log2-weights"};
  };
  template <> struct RegistrationPolicies<Options::EXW> {
    static constexpr const char* name = "experimental-weights";
    static constexpr int size = Options::EXWsize;
    static constexpr std::array<const char*, size>
      string {"r", "a", "d"};
    static constexpr std::array<const char*, size>
      estring {"random", "ascending", "descending"};
  };
  template <> struct RegistrationPolicies<Options::Info> {
    static constexpr const char* name = "show-info";
    static constexpr int size = Options::Info::size;
    static constexpr std::array<const char*, size>
      string {"+info", "-info"};
    static constexpr std::array<const char*, size>
      estring {"show-info", "not-show-info"};
  };
  template <> struct RegistrationPolicies<Options::Weights> {
    static constexpr const char* name = "show-weights";
    static constexpr int size = Options::Weights::size;
    static constexpr std::array<const char*, size>
      string {"+w", "-w"};
    static constexpr std::array<const char*, size>
      estring {"show-weights", "not-show-weights"};
  };
  template <> struct RegistrationPolicies<Options::Headers> {
    static constexpr const char* name = "show-headers";
    static constexpr int size = Options::Headers::size;
    static constexpr std::array<const char*, size>
      string {"+headers", "-headers"};
    static constexpr std::array<const char*, size>
      estring {"show-headers", "not-show-headers"};
  };
  template <> struct RegistrationPolicies<Options::Computations> {
    static constexpr const char* name = "perform-computations";
    static constexpr int size = Options::Computations::size;
    static constexpr std::array<const char*, size>
      string {"+computations", "-computations"};
    static constexpr std::array<const char*, size>
      estring {"perform_computations", "not-perform_computations"};
  };
  template <> struct RegistrationPolicies<Options::SIVA> {
    static constexpr const char* name = "selected-values";
    static constexpr int size = Options::SIVAsize;
    static constexpr std::array<const char*, size>
      string {"all", "satc", "t", "ppc", "nds", "inds", "lvs",
        "mu0", "qfppc", "pprunes", "pmprune", "pprobes", "rounds",
        "solc", "tr", "pelvals", "dp",
        "mu1", "w", "ltau", "minp", "meanp", "maxp", "sdd", "tb",
        "estlvs"};
  };
  template <> struct RegistrationPolicies<Options::NEG> {
    static constexpr const char* name = "negation-sign";
    static constexpr int size = Options::NEGsize;
    static constexpr std::array<const char*, size>
      string {"+sign", "-sign"};
    static constexpr std::array<const char*, size>
      estring {"no-negation-sign", "with-negation-sign"};
  };
  template <> struct RegistrationPolicies<Options::STOP> {
    static constexpr const char* name = "show-stopped";
    static constexpr int size = Options::STOPsize;
    static constexpr std::array<const char*, size>
      string {"+stop", "-stop"};
    static constexpr std::array<const char*, size>
      estring {"show-stopped", "not-show-stopped"};
  };
  template <> struct RegistrationPolicies<Options::STAT> {
    static constexpr const char* name = "statistics-type";
    static constexpr int size = Options::STATsize;
    static constexpr std::array<const char*, size>
      string {"ave", "min", "max", "stddev"};
    static constexpr std::array<const char*, size>
      estring {"arithmetic-mean", "minimum", "maximum", "standard-deviation"};
  };
  template <> struct RegistrationPolicies<Options::NOTY> {
    static constexpr const char* name = "node-type";
    static constexpr int size = Options::NOTYsize;
    static constexpr std::array<const char*, size>
      string {"inode", "leaf"};
    static constexpr std::array<const char*, size>
      estring {"inner-node", "leaf-node"};
  };
}
namespace Options {
  std::ostream& operator <<(std::ostream& out, const RT rt) {
    return out << Environment::W2(rt);
  }
  std::ostream& operator <<(std::ostream& out, const PropO po) {
    return out << Environment::W2(po);
  }
  std::ostream& operator <<(std::ostream& out, const STO st) {
    return out << Environment::W2(st);
  }
  std::ostream& operator <<(std::ostream& out, const LRST st) {
    return out << Environment::W2(st);
  }
  std::ostream& operator <<(std::ostream& out, const BHV bvar) {
    return out << Environment::W2(bvar);
  }
  std::ostream& operator <<(std::ostream& out, const BRT bt) {
    return out << Environment::W2(bt);
  }
  std::ostream& operator <<(std::ostream& out, const LBRT bt) {
    return out << Environment::W2(bt);
  }
  std::ostream& operator <<(std::ostream& out, const GBO bo) {
    return out << Environment::W2(bo);
  }
  std::ostream& operator <<(std::ostream& out, const LBRO bo) {
    return out << Environment::W2(bo);
  }
  std::ostream& operator <<(std::ostream& out, const LAR lar) {
    return out << Environment::W2(lar);
  }
  std::ostream& operator <<(std::ostream& out, const DIS dis) {
    return out << Environment::W2(dis);
  }
  std::ostream& operator <<(std::ostream& out, const SPW sp) {
    return out << Environment::W2(sp);
  }
  std::ostream& operator <<(std::ostream& out, const EXW ew) {
    return out << Environment::W2(ew);
  }
  std::ostream& operator <<(std::ostream& out, const Info info) {
    return out << Environment::W2(info);
  }
  std::ostream& operator <<(std::ostream& out, const Weights w) {
    return out << Environment::W2(w);
  }
  std::ostream& operator <<(std::ostream& out, const Headers h) {
    return out << Environment::W2(h);
  }
  std::ostream& operator <<(std::ostream& out, const Computations c) {
    return out << Environment::W2(c);
  }
  std::ostream& operator <<(std::ostream& out, const SIVA sv) {
    return out << Environment::W0(sv);
  }
  std::ostream& operator <<(std::ostream& out, const NEG n) {
    return out << Environment::W2(n);
  }
  std::ostream& operator <<(std::ostream& out, const STOP s) {
    return out << Environment::W2(s);
  }
  std::ostream& operator <<(std::ostream& out, const STAT s) {
    return out << Environment::W2(s);
  }
  std::ostream& operator <<(std::ostream& out, const NOTY n) {
    return out << Environment::W2(n);
  }

  auto read(EXW, const std::string& s) noexcept {
    return Environment::read<EXW>(s);
  }
}

#endif

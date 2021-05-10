// Oliver Kullmann, 27.12.2020 (Swansea)
/* Copyright 2020, 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Organisation of the seed space

   The general principles for the seed-sequences (all seeds 64 bits):

   (A) The initial part, always of length 6:

   1. The seed-sequence starts with the organisation-timestamp (all timestamps
   in nanoseconds).
   2. The area-identifier (logic=0, combinatorics=1).
   3. For each area the type.
   4. The timestamp of the program (also fixed forever).
   5. The variant-number (starting with 0).
   6. The number of parameters in the second part (the generic parameters;
      they are again concluded with the size of the subsequent block), which
      is also counted here. This number is determined (currently) by area and
      type (alone).

   (B) The generic parameters of the instance (e.g., generic options).
       This part is again concluded with the (total) size of the next block.

   (C) The special parameters (like the number of variables).
       Not concluded with the (arbitrary) size of the final block.

   (D) The user specified seeds (possibly none).

   These 64-bit seeds yield the vec_eseed_t-vector, which is the argument
   for RandGen::RandGen_t


   The components delivered here:

   The initial part:

    - 1. OKlibrary_timestamp
    - 2. The main classifier: enum class Area
    - 3. The second-level classifier (the "type):
        - enum class Logic
        - enum class Combinatorics
    - 4.+5. brg/qbrg/dqbrg/lsrg_timestamp/variant
    - 6. NumGenParams<Area>()(Area type), with specialisations for Logic and
         Combinatorics.

    - size_first_part = 6
    - Function-template
        initial_seeding<Area, Type>(org, area, type, program, variant)
      delivers the initial seed-vector.

    - default_filestem/suffix are helper functions for default-filenames.

   The second part, the generic parameters of the instance:

    - Function
        add_generic_parameters(vec_eseed_t& given, vec_eseed_t& add, size).

   The third part, the specific parameters (not concluded with the size of
   the fourth, final block):

    - Function
        add_specific_parameters(given, add)

   The fourth part, the user-provided seeds:

    - add_user_seeds(given, string_view)
    - add_user_seeds(given, vec_eseed_t)


   Further helper-functions:

    - default_seeds condenses all seeds into one number (for the
      default filename).

*/

#ifndef SEEDORGANISATION_esc1hEcQw5
#define SEEDORGANISATION_esc1hEcQw5

#include <string>
#include <algorithm>

// Guaranteed to be included:
#include "Numbers.hpp"

namespace SeedOrganisation {

  using eseed_t = RandGen::gen_uint_t;
  using RandGen::seed_t;


  /* The generic initial part of the seed sequence */

  constexpr eseed_t OKlibrary_timestamp = 1609092523835210350L;

  enum class Area : eseed_t {
    logic=0,
    combinatorics=1,
  };
  // Possible extensions: graph_theory=2, mathematics=3, physics=4.

  enum class Logic : eseed_t {
    block_uniform_cnf = 0,
    block_uniform_qcnf = 1,
    block_uniform_dqcnf = 2,
    block_uniform_dqcnf_planteda1 = 3,
    block_uniform_dqcnf_plantede1 = 4,
  };
  std::string default_filestem(const Logic t) {
    // bug gcc 10.1.0 with "using enum"
    switch (t) {
    case Logic::block_uniform_cnf : return "BlRaGe";
    case Logic::block_uniform_qcnf : return "QuBlRaGe";
    case Logic::block_uniform_dqcnf : return "DeQuBlRaGe";
    default : return "NOT_IMPLEMENTED";
    }
  }
  std::string default_filesuffix(const Logic t) {
    // bug gcc 10.1.0 with "using enum"
    switch (t) {
    case Logic::block_uniform_cnf : return ".dimacs";
    case Logic::block_uniform_qcnf : return ".qdimacs";
    case Logic::block_uniform_dqcnf : return ".dqdimacs";
    default : return "NOT_IMPLEMENTED";
    }
  }

  enum class Combinatorics : eseed_t {
    latin_squares=0,
  };
  std::string default_filestem(const Combinatorics t) {
    // bug gcc 10.1.0 with "using enum"
    switch (t) {
    case Combinatorics::latin_squares : return "LSRG";
    default : return "NOT_IMPLEMENTED";
    }
  }

  // The number of generic parameters (in the second block, without the final
  // size of the third block):
  template<typename Area> struct NumGenParams;

  template <> struct NumGenParams<Logic> {
    constexpr eseed_t operator()(const Logic l) noexcept {
      switch(l) {
      case Logic::block_uniform_cnf : return 3; break;
      default : return 3;}
    }
  };
  template <> struct NumGenParams<Combinatorics> {
    constexpr eseed_t operator()(const Combinatorics c) noexcept {
      switch(c) {
      case Combinatorics::latin_squares : return 2; break;
      default : return 2;}
    }
  };


  constexpr eseed_t brg_timestamp = 1609092700427021645L;
  constexpr eseed_t brg_variant = 0;

  constexpr eseed_t qbrg_timestamp = 1609092727890643693L;
  constexpr eseed_t qbrg_variant = 0;

  constexpr eseed_t dqbrg_timestamp = 1609092751610097777L;
  constexpr eseed_t dqbrg_variant = 0;

  constexpr eseed_t lsrg_timestamp = 1609092786237186306L;

  constexpr eseed_t size_first_part = 5+1;


  template <typename Area, typename Type>
  RandGen::vec_eseed_t initial_seeding(const eseed_t org, const Area area, const Type type, const eseed_t program, const eseed_t variant) noexcept {
    return {org, eseed_t(area), eseed_t(type), program, variant, NumGenParams<Type>()(type)+1};
  }


  /* The second part, for the generic parameters */

  // Add to v the generic parameters in add, plus the size of the third block:
  void add_generic_parameters(RandGen::vec_eseed_t& v, const RandGen::vec_eseed_t& add, const eseed_t size_next) {
    assert(v.size() == size_first_part);
    assert(add.size()+1 == v.back());
    v.reserve(size_first_part + add.size());
    for (eseed_t x : add) v.push_back(x);
    v.push_back(size_next);
  }


  /* The third part, for the specific parameters */

  void add_specific_parameters(RandGen::vec_eseed_t& v, const RandGen::vec_eseed_t add) {
    assert(not v.empty());
    assert(add.size() == v.back());
    v.reserve(v.size() + add.size());
    for (eseed_t x : add) v.push_back(x);
  }


  /* The fourth, final part, for the user-provided seeds */

  void add_user_seeds(RandGen::vec_eseed_t& v, const std::string_view s) {
    assert(not v.empty());
    RandGen::add_seeds(s, v);
  }
  void add_user_seeds(RandGen::vec_eseed_t& v, const RandGen::vec_eseed_t& s) {
    assert(not v.empty());
    v.reserve(v.size() + s.size());
    for (eseed_t x : s) v.push_back(x);
  }


  // Accumulating all seeds into one number (for a filename):
  std::string default_seeds(const RandGen::vec_eseed_t& s) {
    return std::to_string(std::accumulate(
      s.begin(), s.end(), RandGen::gen_uint_t(0)));
  }


}

#endif

// Oliver Kullmann, 27.12.2020 (Swansea)
/* Copyright 2020, 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Organisation of the seed space

*/

#ifndef SEEDORGANISATION_esc1hEcQw5
#define SEEDORGANISATION_esc1hEcQw5

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

  enum class Logic : eseed_t {
    block_uniform_cnf = 0,
    block_uniform_qcnf = 1,
    block_uniform_dqcnf = 2,
    block_uniform_dqcnf_planteda1 = 3,
    block_uniform_dqcnf_plantede1 = 4,
  };
  constexpr eseed_t num_generic_params(const Logic l) noexcept {
    switch(l) {
    case Logic::block_uniform_cnf : return 4; break;
    default : return 4;
    }
  }

  enum class Combinatorics : eseed_t {
    latin_squares=0,
  };
  constexpr eseed_t num_generic_params(const Combinatorics c) noexcept {
    switch(c) {
    case Combinatorics::latin_squares : return 2; break;
    default : return 2;
    }
  }

  // The number of real specific parameters:
  template<typename Type> struct NumGenParams;
  template <> struct NumGenParams<Logic> {
    constexpr eseed_t operator()(const Logic l) noexcept {
      switch(l) {
      case Logic::block_uniform_cnf : return 4; break;
      default : return 4;
      }
    }
  };
  template <> struct NumGenParams<Combinatorics> {
    constexpr eseed_t operator()(const Combinatorics c) noexcept {
      switch(c) {
      case Combinatorics::latin_squares : return 2; break;
      default : return 2;
      }
    }
  };


  constexpr eseed_t brg_timestamp = 1609092700427021645L;
  constexpr eseed_t brg_variant = 0;

  constexpr eseed_t qbrg_timestamp = 1609092727890643693L;
  constexpr eseed_t qbrg_variant = 0;

  constexpr eseed_t Dqbrg_timestamp = 1609092751610097777L;
  constexpr eseed_t dqbrg_variant = 0;

  constexpr eseed_t lsrg_timestamp = 1609092786237186306L;
  constexpr eseed_t lsrg_variant = 0;

  constexpr eseed_t size_first_part = 5+1;


  template <typename Area, typename Type>
  RandGen::vec_eseed_t initial_seeding(const eseed_t org, const Area area, const Type type, const eseed_t program, const eseed_t variant) noexcept {
    return {org, eseed_t(area), eseed_t(type), program, variant, NumGenParams<Type>()(type)+1};
  }


  /* The second part, for the generic parameters */

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


}

#endif

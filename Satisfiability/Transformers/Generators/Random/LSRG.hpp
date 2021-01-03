// Oliver Kullmann, 3.11.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*



*/

#ifndef LSRG_r3HkyAjxRY
#define LSRG_r3HkyAjxRY

#include "SeedOrganisation.hpp"

//Guaranteed to be included:
#include "LatinSquares.hpp"

namespace LSRG {

  namespace LS = LatinSquares;
  namespace RG = RandGen;
  namespace SO = SeedOrganisation;

  enum class GenO : SO::eseed_t {majm=0, jm=1, ma=2};

  RG::vec_eseed_t basic_seeds(const LS::ls_dim_t N, const LS::Selection& sel, const GenO go, const LS::StRLS so) {
    RG::vec_eseed_t res = SO::initial_seeding(
        SO::OKlibrary_timestamp,
        SO::Area::combinatorics,
        SO::Combinatorics::latin_squares,
        SO::lsrg_timestamp,
        SO::lsrg_variant);
    using SO::eseed_t;
    const eseed_t size_spec_params = 1 + 3;
    SO::add_generic_parameters(res,
                               {eseed_t(go), eseed_t(so)}, size_spec_params);
    SO::add_specific_parameters(res, {N, sel.r,sel.c,sel.s});
    return res;
  }

  LS::ls_t random_ls(const LS::ls_dim_t N, const LS::Selection& sel, const GenO go, const LS::StRLS so, RG::RandGen_t& g) {
    switch (go) {
    case GenO::majm :
      return LS::select(LS::standardise(
        LS::JacobsMatthews(
          LS::random_ma_ls(N, LS::CrRLS::with_initial_phase, g),
        g).ls(), so), sel, g);
    case GenO::jm :
      return LS::select(LS::standardise(
        LS::JacobsMatthews(N, g).ls(), so), sel, g);
    case GenO::ma :
      return LS::select(LS::standardise(
        LS::random_ma_ls(N, LS::CrRLS::with_initial_phase, g), so), sel, g);
    default : return LS::empty_ls(N);
    }
  }

  LS::ls_t random_ls(const LS::ls_dim_t N, const LS::Selection& sel,
                     const GenO go, const LS::StRLS so,
                     const RG::vec_eseed_t seeds) {
    RG::RandGen_t g(seeds);
    return random_ls(N, sel, go, so, g);
  }

  std::pair<LS::ls_t, RG::vec_eseed_t>
    random_ls(const LS::ls_dim_t N, std::string_view seeds,
              const LS::Selection& sel, const GenO go = GenO{},
              const LS::StRLS so = LS::StRLS{}) {
    RG::vec_eseed_t s = basic_seeds(N, sel, go , so);
    SO::add_user_seeds(s, seeds);
    return {random_ls(N, sel, go, so, s), s};
  }
  std::pair<LS::ls_t, RG::vec_eseed_t>
  random_ls(const LS::ls_dim_t N, std::string_view seeds,
            const GenO go = GenO{}, const LS::StRLS so = LS::StRLS{}) {
    return random_ls(N, seeds, LS::Selection(N), go, so);
  }

}

#endif

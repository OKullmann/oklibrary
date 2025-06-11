// Oliver Kullmann, 10.6.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  General definitions regarding the transformer NBsel for random sub-cnfs

*/

#ifndef NBSEL_eKIUHrK7Ct
#define NBSEL_eKIUHrK7Ct

#include <Transformers/Generators/Random/SeedOrganisation.hpp>

#include "GRRG.hpp"

namespace NBsel {

  namespace SO = SeedOrganisation;

  using UInt_t = RandGen::gen_uint_t;

  // k resp. (nom,den) and is_uniform, is_inclusion:
  typedef std::tuple<UInt_t, RandGen::Prob64, bool, bool>
          selection_info_t;
  constexpr selection_info_t default_selection = {0,{1,2},false,true};

  selection_info_t read_sel_arg(std::string arg,
                                const std::string& error) noexcept {
    if (arg.empty()) return default_selection;
    const bool inclusion = [&arg]{bool res = true;
      if (arg[0] == '+') arg = arg.substr(1);
      else if (arg[0] == '-') {res = false; arg = arg.substr(1);}
      return res;
    }();
    return std::tuple_cat(GRRG::read_mp_arg(arg, error),
                          std::make_tuple(inclusion));
  }

  RandGen::vec_eseed_t
  basic_seeds(const UInt_t k, const RandGen::Prob64 p,
              const bool uniform, const bool inclusion) noexcept {
    // The variant of the initial seed is the boolean "uniform":
    namespace SO = SeedOrganisation;
    using SO::eseed_t;
    RandGen::vec_eseed_t res = SO::initial_seeding(
        SO::OKlibrary_timestamp,
        SO::Area::logic,
        SO::Logic::select_noboconf,
        SO::nbsel_timestamp,
        eseed_t(uniform));
    const eseed_t size_spec_params = 2;
    SO::add_generic_parameters(res,
                               {eseed_t(inclusion)},
                               size_spec_params);
    if (uniform)
      SO::add_specific_parameters(res, {k, 0});
    else
      SO::add_specific_parameters(res, {p.nom(), p.den()});
    return res;
  }

  typedef std::pair<RandGen::vec_eseed_t, UInt_t> seed_info_t;
  seed_info_t all_seeds(
      const UInt_t k, const RandGen::Prob64 p,
      const bool uniform, const bool inclusion,
      const std::string& us) noexcept {
    seed_info_t res;
    res.first = basic_seeds(k, p, uniform, inclusion);
    res.second = res.first.size();
    SO::add_user_seeds(res.first, us);
    return res;
  }

}

#endif

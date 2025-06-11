// Oliver Kullmann, 15.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  General definitions regarding the generator GRRG for random graphs

*/

#ifndef GRRG_jJ3soB9pCb
#define GRRG_jJ3soB9pCb

#include <utility>
#include <tuple>
#include <ostream>

#include <cstdlib>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumInOut.hpp>
#include <Transformers/Generators/Random/Numbers.hpp>
#include <Transformers/Generators/Random/SeedOrganisation.hpp>

//Guaranteed to be included:
#include "Graphs.hpp"
#include "RandomGraphs.hpp"

namespace GRRG {

  namespace SO = SeedOrganisation;
  namespace GR = Graphs;
  namespace RG = RandomGraphs;

  using UInt_t = RG::UInt_t;

  // Type and with-loops:
  std::pair<GR::GT, bool> read_type_arg(std::string arg,
                                        const std::string& error) noexcept {
    bool with_loops = false;
    if (arg.starts_with("+")) {
      with_loops = true; arg = arg.substr(1);
    }
    const auto opt_type = Environment::read<GR::GT>(arg);
    if (not opt_type) {
      std::cerr << error << "Faulty type-argument \"" << arg << "\".\n";
      std::exit(1);
    }
    return {opt_type.value(), with_loops};
  }

  // m resp. (nom,den) and is_uniform:
  typedef std::tuple<UInt_t, RandGen::Prob64, bool> edges_info_t;

  edges_info_t read_mp_arg(const std::string& arg,
                           const std::string& error) noexcept {
    edges_info_t res{0,{0,1},true};
    if (arg.contains('/')) {
      const auto opt_p = RandGen::toProb64(arg);
      if (not opt_p) {
        std::cerr << error << "Faulty p-argument \"" << arg << "\".\n";
        std::exit(1);
      }
      std::get<1>(res) = opt_p.value();
      std::get<2>(res) = false;
    }
    else std::get<0>(res) = FloatingPoint::toUInt(arg);
    return res;
  }

  RandGen::vec_eseed_t
  basic_seeds(const GR::GT gt, const bool with_loops,
              const UInt_t n,
              const UInt_t m, const RandGen::Prob64 p, const bool uniform) noexcept {
    // The variant of the initial seed is the boolean "uniform":
    namespace SO = SeedOrganisation;
    using SO::eseed_t;
    RandGen::vec_eseed_t res = SO::initial_seeding(
        SO::OKlibrary_timestamp,
        SO::Area::graphtheory,
        SO::GraphTheory::uniformbinom,
        SO::grrg_timestamp,
        eseed_t(uniform));
    const eseed_t size_spec_params = 3;
    SO::add_generic_parameters(res,
                               {eseed_t(int(gt)), eseed_t(with_loops)},
                               size_spec_params);
    if (uniform)
      SO::add_specific_parameters(res, {n, m, 0});
    else
      SO::add_specific_parameters(res, {n, p.nom(), p.den()});
    return res;
  }

  typedef std::pair<RandGen::vec_eseed_t, UInt_t> seed_info_t;
  seed_info_t all_seeds(
      const GR::GT gt, const bool with_loops,
      const UInt_t n,
      const UInt_t m, const RandGen::Prob64 p, const bool uniform,
      const std::string& us) noexcept {
    seed_info_t res;
    res.first = basic_seeds(gt, with_loops, n, m, p, uniform);
    res.second = res.first.size();
    SO::add_user_seeds(res.first, us);
    return res;
  }

  GR::GrFo read_format_arg(const std::string& arg,
                           const std::string& error) noexcept {
    const auto opt_format = Environment::read<GR::GrFo>(arg);
    if (not opt_format) {
      std::cerr << error << "Faulty format-argument \"" << arg << "\".\n";
      std::exit(1);
    }
    return opt_format.value();
  }

  void output_seeds(std::ostream& out, const GR::GrFo format,
                    const RandGen::vec_eseed_t& seeds) {
    out << GR::comment_symbol(format) << " ";
    Environment::out_line(out, seeds);
    out << std::endl;
  }

}

#endif

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

#include <cstdlib>

#include <ProgramOptions/Environment.hpp>
#include <Transformers/Generators/Random/SeedOrganisation.hpp>

//Guaranteed to be included:
#include "Graphs.hpp"
#include "RandomGraphs.hpp"

namespace GRRG {

  namespace SO = SeedOrganisation;
  namespace GR = Graphs;
  namespace RG = RandomGraphs;

  using size_t = GR::AdjVecUInt::id_t;

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

  // m resp. (nom,den):

}

#endif

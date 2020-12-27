// Oliver Kullmann, 27.12.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Organisation of the seed space

*/

#ifndef SEEDORGANISATION_esc1hEcQw5
#define SEEDORGANISATION_esc1hEcQw5

#include "Numbers.hpp"

namespace SeedOrganisation {

  enum class MainType : RandGen::gen_uint_t {
    block_uniform_cnf = 0,
    block_uniform_qcnf = 1,
    block_uniform_dqcnf = 2,
    block_uniform_dqcnf_planteda1 = 3,
    block_uniform_dqcnf_plantede1 = 4,
  };

}

#endif

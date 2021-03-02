// Oliver Kullmann, 2.3.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  -

*/

#ifndef TIMING_tq2ePIcTdj
#define TIMING_tq2ePIcTdj

#include <ProgramOptions/Environment.hpp>

namespace Timing {

  enum class Error {
    pnumber = 1,
    empty_command = 2,
    invalid_code = 3,
    invalid_N = 4,
    remove_out = 5,
    remove_err = 6,
    os_error = 7,
    wrong_code = 8,
    system_call = 9,
  };

}

#endif

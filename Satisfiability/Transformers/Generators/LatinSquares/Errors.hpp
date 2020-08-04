// Oliver Kullmann, 3.8.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef ERRORS_30olsDFJVR
#define ERRORS_30olsDFJVR

#include <string>

#include "ProgramInfo.hpp"

namespace Errors {

  const std::string error = "ERROR[" + ProgramInfo::proginfo.prg + "]: ";

  enum class Error {
    conversion = 11,
    too_big = 12,
    too_small = 13,
    file_open = 14,
    sym_par = 15,
    ealo_par = 16,
    eul_par = 17,
    p_par = 18,
  };

}

#endif


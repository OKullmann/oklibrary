// Oliver Kullmann, 5.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Strings describing the environment */

#ifndef ENVIRONMENT_6Kk9MX4Wbw
#define ENVIRONMENT_6Kk9MX4Wbw

#include <string>

namespace Environment {

  const std::string compilation_date = __DATE__ " " __TIME__;
  const std::string compiler_version =
#ifdef __GNUC__
   "g++ " __VERSION__
#else
   "Compiler not gcc"
#endif
;
  const std::string git_id = GITID;
  const std::string optimisation = OPTIMISATION;

  void output_environment(std::ostream& out, const std::string program, const std::string version, const std::string date) {
    out << program << " " << version << " " << date << " " << git_id << "\n";
    out << compiler_version << " " << compilation_date << "\n";
    out << optimisation << "\n";

  }

}

#endif

// Oliver Kullmann, 5.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Strings describing the environment

   Class ProgramInfo
   is constructed with program-name, program-version and program-date,
   and has additionally stored information on the machine, the compilation,
   and the git-id.

Assumes macros
 - MACHINE (the machine-name)
 - GITID (the git-ID as string)
 - OPTIMISATION (the list of compilation-options as string)
are defined.

In order to define them on the command-line, assuming that "$(CXXFLAGS)" is
used late in the command for compilation, use e.g

> CXXFLAGS="-DGITID=\"\\\"ABC\\\"\"" make

to define GITIT as the string "ABC" (the will yield a redefinition-warning,
if the makefile already gives its own definition, got GITID, and this
warning seems appropriate).

If your makefile doesn't define GITIT in the first place, then just use

> -DGITID="ABC" make

For our makefile, one can use

> make git_id="ABC"


*/

#ifndef ENVIRONMENT_6Kk9MX4Wbw
#define ENVIRONMENT_6Kk9MX4Wbw

#include <string>
#include <algorithm>
#include <ostream>

namespace Environment {

// Turning the value of a macro into a string:
#define S(x) #x
#define STR(x) S(x)

  std::string replace(const std::string& orig, const char x, const char y) {
    std::string res(orig);
    std::replace(res.begin(), res.end(), x, y);
    return res;
  }
  const std::string compilation_orig_date = __DATE__;
  const std::string compilation_tr_date = replace(compilation_orig_date, ' ', '_');
  const std::string compilation_full_date = compilation_tr_date + " " __TIME__;

  const std::string compiler_version =
#ifdef __GNUC__
   "g++ " __VERSION__
#elif _MSC_VER
   STR(_MSC_VER)
#else
   ""
#endif
;
  const std::string optimisation =
#ifdef OPTIMISATION
   OPTIMISATION
#else
   ""
#endif
;
  const std::string git_id =
#ifdef GITID
   GITID
#else
   ""
#endif
;
  const std::string machine_name =
#ifdef MACHINE
    MACHINE
#else
    ""
#endif
;
 const double bogomips_value =
#ifdef BOGOMIPS
    BOGOMIPS
#else
    0
#endif
;

  std::string basename(const std::string& name) {
    return name.substr(0, name.find('.'));
  }

  std::string auto_prg(const std::string& name) {
#ifdef NDEBUG
    return basename(name);
#else
    return basename(name) + "_debug";
#endif
  }

  // Naming policies:
  enum class NP { given, extracted };

  struct ProgramInfo {
    const std::string& machine = machine_name;
    const double bogomips = bogomips_value;
    const std::string& comp_date = compilation_full_date;
    const std::string& comp_version = compiler_version;
    const std::string& comp_opt = optimisation;
    const std::string& git = git_id;
    const std::string vrs;
    const std::string date;
    const std::string prg;

    ProgramInfo(const std::string& vrs, const std::string& date, const std::string& prg, const NP p = NP::extracted) noexcept : vrs(vrs), date(date), prg((p==NP::given) ? prg : auto_prg(prg)) {}

  };

  std::ostream& operator <<(std::ostream& out, const ProgramInfo& pi) {
    out << pi.prg << " " << pi.vrs << " " << pi.date << " " << pi.git << "\n";
    out << pi.machine << " " << pi.bogomips << "\n";
    out << pi.comp_version << " " << pi.comp_date << "\n";
    return out << pi.comp_opt << "\n";
  }

  // ProgramInfo output-policy:
  enum class PIp { simple=0, explained=1, dimacs=2 };
  struct Wrap {
    const ProgramInfo& pi;
    const PIp p;
    Wrap(const ProgramInfo& pi, const PIp p=PIp::simple) noexcept :
      pi(pi), p(p) {}
  };
  std::ostream& operator <<(std::ostream& out, const Wrap& w) {
    const ProgramInfo& i{w.pi};
    switch (w.p) {
    case PIp::explained : return
    out << "program name:       " << i.prg << "\n"
        << " version:           " << i.vrs << "\n"
        << " last change:       " << i.date << "\n"
        << " git-id:            " << i.git << "\n"
        << "machine name:       " << i.machine << "\n"
        << " bogomips:          " << i.bogomips << "\n"
        << "compiler version:   " << i.comp_version << "\n"
        << " date:              " << i.comp_date << "\n"
        << " options:           " << i.comp_opt << "\n"
    ;
    default : return out << i;
    }
  }

}

#endif

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
#include <chrono>
#include <iomanip>
#include <optional>
#include <array>

#include <cassert>

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
    using string = std::string;
    const std::string& machine = machine_name;
    const double bogomips = bogomips_value;
    const string& comp_date = compilation_full_date;
    const string& comp_version = compiler_version;
    const string& comp_opt = optimisation;
    const string& git = git_id;
    const string vrs;
    const string date;
    const string prg;
    const string aut;
    const string url;
    const string lic;

    ProgramInfo(const string vrs, const string date, const string prg, const string aut="", const string url="", const string lic="", const NP p = NP::extracted) noexcept : vrs(vrs), date(date), prg((p==NP::given) ? prg : auto_prg(prg)), aut(aut), url(url), lic(lic) {}

  };

  std::ostream& operator <<(std::ostream& out, const ProgramInfo& pi) {
    out << pi.prg << " " << pi.vrs << " " << pi.date << " " << pi.git << "\n";
    out << pi.machine << " " << pi.bogomips << "\n";
    out << pi.comp_version << " " << pi.comp_date << "\n";
    return out << pi.comp_opt << "\n";
  }

  // ProgramInfo output-policy:
  enum class OP { simple=0, explained=1, dimacs=2,
                   rh=3, rd=4, rf=5 };
                   // r : R
                   // h : header
                   // d : data
                   // f = hd
  constexpr std::array<const char*, int(OP::rf)+1> OP2str
    {"s", "e", "d", "rh", "rd", "rf"};
  std::optional<OP> rOP(const std::string& s) noexcept {
    const auto i = std::find(OP2str.begin(), OP2str.end(), s);
    if (i == OP2str.end()) return {};
    else return static_cast<OP>(i - OP2str.begin());
  }
  std::ostream& operator <<(std::ostream& out, const OP o) {
    return out << OP2str[int(o)];
  }


  const std::string r_header = "machine bogomips compdate compversion compoptions gitid version date program author url license";

  struct Wrap {
    const ProgramInfo& pi;
    const OP p;
    Wrap(const ProgramInfo& pi, const OP p=OP::simple) noexcept :
      pi(pi), p(p) {}
  };

  void current_time(std::ostream& out) {
    const auto now = std::chrono::system_clock::now();
    const auto now_t = std::chrono::system_clock::to_time_t(now);
    out << std::put_time(std::localtime(&now_t), "%d.%m.%Y %T_%z");
    out << " " << now.time_since_epoch().count();
  }

  std::ostream& operator <<(std::ostream& out, const Wrap& w) {
    const ProgramInfo& i{w.pi};
    switch (w.p) {

    case OP::explained :
    if (not i.aut.empty())
    out << "author:             " << "\"" << i.aut << "\"\n";
    if (not i.url.empty())
    out << " url:               " << "\"" << i.url << "\"\n";
    if (not i.lic.empty())
    out << " license:           " << "\"" << i.lic << "\"\n";
    out << "program name:       " << i.prg << "\n"
        << " version:           " << i.vrs << "\n"
        << " last change:       " << i.date << "\n"
        << " git-id:            " << i.git << "\n";
    out << "machine name:       " << i.machine << "\n"
        << " bogomips:          " << i.bogomips << "\n"
        << "compiler version:   " << i.comp_version << "\n"
        << " date:              " << i.comp_date << "\n"
        << " options:           " << i.comp_opt << "\n"
    ;
    return out;

    case OP::rd : return out;

    case OP::rf :
    [[fallthrough]];

    case OP::rh :
    out << "# Timestamp: "; current_time(out); out << "\n";
    if (not i.url.empty())
      out << "# Producing program: " << i.url << "\n";
    out << "# program name:       " << i.prg << "\n"
        << "#  version:           " << i.vrs << "\n"
        << "#  last change:       " << i.date << "\n"
        << "#  git-id:            " << i.git << "\n";
    out << "# machine name:       " << i.machine << "\n"
        << "#  bogomips:          " << i.bogomips << "\n"
        << "# compiler version:   " << i.comp_version << "\n"
        << "#  compilation date:  " << i.comp_date << "\n"
        << "#  used options:      " << i.comp_opt << "\n"
    ;
    return out;

    default : return out << i;
    }
  }

  bool version_output(std::ostream& out, const ProgramInfo& pi, const int argc, const char* const argv[]) {
    if (argc == 2 and std::string(argv[1]) == "-v") {
      out << Wrap(pi, OP::explained);
      return true;
    }
    else return false;
  }

}

#endif

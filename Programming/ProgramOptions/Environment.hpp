// Oliver Kullmann, 5.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Strings describing the environment

   String helper functions:
    - replace(string, char, char)
    - basename(string)
    - auto_prg(filename) ("automatic" program-name from file-name)
    - split(string, char)
    - transform_spaces(string)

   General machinery for handling policy enumerations:
    - class RegistrationPolicies (registration of size and strings)
    - function-template read(string) (converting strings in enum-values)

   Global variables:
    - compilation_orig_date
    - compilation_tr_date (spaces replaced by "_")
    - compilation_full_date (date and time)
    - compiler_version
    - optimisation (indeed all the essential options for the compiler)
    - git_id
    - machine_name
    - bogomips_value

   Class ProgramInfo
    - is constructed from program-version, -date, and -name,
    - optionally author, url, license, naming-policy;
    - has additionally stored information on the machine, the compilation,
      and the git-id.
    - Operator << outputs basic info,
    - for more refined output use the Wrap(pi, OP).

   Here OP is the enum-class for output-policies.

   Then there are further tools for handling of the command-line:
    - version_output(ostream, ProgramInfo, int, char*[])
    - profiling(int, char*[])
   both return booleans (whether the command-line is just asking to output
   the version or to run the profiling-version).

   For time-handling there is
    - current_time(ostream).


Assumes the macros
 - MACHINE (the machine-name)
 - BOGOMIPS (as floating-point number)
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

For our makefiles, recommend is to use

> make git_id="ABC"


*/

#ifndef ENVIRONMENT_6Kk9MX4Wbw
#define ENVIRONMENT_6Kk9MX4Wbw

#include <string>
#include <algorithm>
#include <ostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <optional>
#include <array>
#include <locale>
#include <ios>

#include <cassert>

namespace Environment {

  // General tools for string handling

// Turning the value of a macro into a string:
#define S(x) #x
#define STR(x) S(x)

  // Replace character x by y in string orig (returning a copy):
  std::string replace(const std::string& orig, const char x, const char y) {
    std::string res(orig);
    std::replace(res.begin(), res.end(), x, y);
    return res;
  }

  // The initial part of the string before the first '.':
  std::string basename(const std::string& name) {
    return name.substr(0, name.find('.'));
  }

  // The "automatic" program name, derived from the filename:
  std::string auto_prg(const std::string& filename) {
#ifdef NDEBUG
    return basename(filename);
#else
    return basename(filename) + "_debug";
#endif
  }

  // Split string s into a vector of tokens, using separator sep:
  typedef std::vector<std::string> tokens_t;
  tokens_t split(const std::string& s, const char sep) {
    std::stringstream ss(s);
    tokens_t res;
    std::string item;
    while (std::getline(ss, item, sep)) res.push_back(item);
    return res;
  }

  // Transforms whitespace into char alt, contracting adjacent whitespace,
  // and eliminating leading and trailing whitespace:
  std::string transform_spaces(const std::string& s, const char alt = ' ') {
    std::string s2(s);
    const std::locale loc;
    const auto sp = [&loc](const char c){return std::isspace(c,loc);};
    s2.erase(s2.begin(), std::find_if_not(s2.begin(), s2.end(), sp));
    s2.erase(std::find_if_not(s2.rbegin(), s2.rend(), sp).base(), s2.end());
    s2.erase(std::unique(s2.begin(), s2.end(), [&](const char c1, const char c2){return sp(c1) and sp(c2);}), s2.end());
    std::replace_if(s2.begin(), s2.end(), sp, alt);
    return s2;
  }


  /* General machinery for handling "policy enums" P, which can be read
     from a string s via read<P>(s), and written to a stream s via output-
     streaming:
  */
  // The "registration of policies", to be specialised:
  template <typename Policy> struct RegistrationPolicies;
  // Reading a policy from a string:
  template <typename Policy>
  std::optional<Policy> read(const std::string& s) noexcept {
    typedef RegistrationPolicies<Policy> reg;
    const auto begin = reg::string.begin(), end = reg::string.end();
    const auto i = std::find(begin, end, s);
    if (i == end) return {};
    else return static_cast<Policy>(i - begin);
  }


  // Basic variables from macros

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
   transform_spaces(OPTIMISATION)
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


  // Class ProgramInfo

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
  enum class OP { simple=0, explained=1, dimacs=2, rh=3, rd=4, rf=5 };
  // r : R, h : header, d : data, f = hd.
  template <>
  struct RegistrationPolicies<OP> {
    static constexpr int size = int(OP::rf) + 1;
    static constexpr std::array<const char*, size> string
      {"s", "e", "d", "rh", "rd", "rf"};
  };
  std::ostream& operator <<(std::ostream& out, const OP o) {
    return out << RegistrationPolicies<OP>::string[int(o)];
  }


  // Output of ProgramInfo

  // Currently information-only:
  const std::string r_header = "machine bogomips compdate compversion compoptions gitid version date program author url license";

  // Output-wrapper for ProgramInfo:
  struct Wrap {
    const ProgramInfo& pi;
    const OP p;
    Wrap(const ProgramInfo& pi, const OP p=OP::simple) noexcept :
      pi(pi), p(p) {}
  };

  // Current date, time, timestamp:
  void current_time(std::ostream& out) {
    const auto now = std::chrono::system_clock::now();
    const auto now_t = std::chrono::system_clock::to_time_t(now);
    out << std::put_time(std::localtime(&now_t), "%d.%m.%Y %T_%z");
    out << " " << now.time_since_epoch().count();
  }

  constexpr std::streamsize default_dimacs_width = 40;
  std::streamsize dimacs_width = default_dimacs_width;
  // "Dimacs Width-Wrapper":
  struct DWW { std::string s; };
  std::ostream& operator <<(std::ostream& out, const DWW& s) {
    const auto ow = out.width();
    out.width(dimacs_width);
    const auto of = out.setf(std::ios_base::left, std::ios_base::adjustfield);
    out << "c " + s.s;
    out.setf(of, std::ios_base::adjustfield);
    out.width(ow);
    return out;
  }
  std::string qu(const std::string& s) {
    return "\"" + s + "\"";
  }


  std::ostream& operator <<(std::ostream& out, const Wrap& w) {
    const ProgramInfo& i{w.pi};
    switch (w.p) {

    case OP::explained :
    if (not i.aut.empty())
    out << "author:             " << qu(i.aut) << "\n";
    if (not i.url.empty())
    out << " url:               " << qu(i.url) << "\n";
    if (not i.lic.empty())
    out << " license:           " << qu(i.lic) << "\n";
    out << "program name:       " << i.prg << "\n"
        << " version:           " << i.vrs << "\n"
        << " last change:       " << i.date << "\n"
        << " git-id:            " << i.git << "\n";
    out << "machine name:       " << i.machine << "\n"
        << " bogomips:          " << i.bogomips << "\n"
        << "compiler version:   " << i.comp_version << "\n"
        << " date:              " << i.comp_date << "\n"
        << " options:           " << qu(i.comp_opt) << "\n"
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

    case OP::dimacs :
    out << "c Output_time "; current_time(out); out << "\n"
        << "c ** Program information **\n"
        << DWW{"program_name"} << qu(i.prg) << "\n"
        << DWW{"version"} << qu(i.vrs) << "\n"
        << DWW{"date"} << qu(i.date) << "\n"
        << DWW{"gid_id"} << qu(i.git) << "\n"
        << DWW{"author"} << qu(i.aut) << "\n"
        << DWW{"url"} << qu(i.url) << "\n"
        << "c ** Machine information **\n"
        << DWW{"machine_name"} << qu(i.machine) << "\n"
        << DWW{"bogomips"} << i.bogomips << "\n"
        << "c ** Compilation information **\n"
        << DWW{"compiler_version"} << qu(i.comp_version) << "\n"
        << DWW{"compilation_date"} << qu(i.comp_date) << "\n"
        << DWW{"compilation_options"} << qu(i.comp_opt) << "\n"
    ;

    return out;

    default : return out << i;
    }
  }


  // Tools for special command-line arguments

  bool version_output(std::ostream& out, const ProgramInfo& pi, const int argc, const char* const argv[]) {
    if (argc == 2 and std::string(argv[1]) == "-v") {
      out << Wrap(pi, OP::explained);
      return true;
    }
    else return false;
  }

  bool profiling(const int argc, const char* const argv[]) noexcept {
    return argc == 2 and std::string(argv[1]) == "-p";
  }

}

#endif

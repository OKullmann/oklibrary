// Oliver Kullmann, 5.3.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Strings describing the environment

   String helper functions:
    - STR(x) is a macro, putting quotation marks around x
    - qu(string) adds quotes around a string
    - replace(string, char, char)
    - basename(string) extracts the part of the string before "."
    - auto_prg(filename) ("automatic" program-name from file-name)
    - split(string, char)
    - transform_spaces(string, char) replaces whitespace-characters,
      contracting adjacent ones and eliminating leading and trailing ones.

   General machinery for handling policy enumerations:
    - class RegistrationPolicies (registration of size and strings)
    - function-template read(string) (converting strings in enum-values)
    - function-template translate(string, char), converting strings to
      tuples of policy-values.

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

   Here OP is the enum-class for output-policies, allowing "simple",
   "explained", "dimacs", and three R-format policies: "rh, rd, rf"
   (header-only, data-only, and both).
   The corresponding strings for input and output are "s, e, d, rh, rd, rf".

   Further tools for handling of the command-line:
    - version_output(ostream, ProgramInfo, int, char*[])
    - profiling(int, char*[])
     both return booleans (whether the command-line is just asking to output
     the version or to run the profiling-version)
    - class Index for running through the indices of the command-line
      arguments.

   For time-handling there is
    - get_date(time_t*, string), get_time(time_t*, string)
      to extract from a time_t-object date and time, using the given formats
    - class CurrentTime (contains the now-timepoint in various formats)
    - output-streaming for CurrentTime.

   Tools for Dimacs-output:
    - constant default_dimacs_width
    - variable dimacs_width
    - wrapper DWW(string) for outputting the initial part of a Dimacs-line
    - wrapper DHW(string) for outputting a dimacs-header-line.


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
#include <string_view>
#include <algorithm>
#include <ostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <optional>
#include <array>
#include <locale>
#include <ios>
#include <tuple>

#include <cassert>
#include <ctime>
#include <cstdlib>

#include <Numerics/FloatingPoint.hpp>

namespace Environment {

  // General tools for string handling

// Turning the value of a macro into a string:
#define SVBR333ZxeL(x) #x
#define STR(x) SVBR333ZxeL(x)

  inline std::string qu(std::string s) {
    return "\"" + s + "\"";
  }

  // Replace character x by y in string orig (returning a copy):
  inline std::string replace(std::string s, const char x, const char y) {
    std::replace(s.begin(), s.end(), x, y);
    return s;
  }

  // The initial part of the string before the first '.':
  inline std::string basename(const std::string_view name) {
    return std::string(name.substr(0, name.find('.')));
  }

  // The "automatic" program name, derived from the filename:
  inline std::string auto_prg(const std::string_view filename) {
#ifdef NDEBUG
    return basename(filename);
#else
    return basename(filename) + "_debug";
#endif
  }

  // Split string s into a vector of tokens, using separator sep:
  typedef std::vector<std::string> tokens_t;
  inline tokens_t split(const std::string_view s, const char sep) {
    std::stringstream ss(s.data());
    tokens_t res;
    std::string item;
    while (std::getline(ss, item, sep)) res.push_back(item);
    return res;
  }

  // Transforms whitespace into char alt, contracting adjacent whitespace,
  // and eliminating leading and trailing whitespace:
  inline std::string transform_spaces(std::string s, const char alt = ' ') {
    const std::locale loc;
    const auto sp = [&loc](const char c){return std::isspace(c,loc);};
    s.erase(s.begin(), std::find_if_not(s.begin(), s.end(), sp));
    s.erase(std::find_if_not(s.rbegin(), s.rend(), sp).base(), s.end());
    s.erase(std::unique(s.begin(), s.end(), [&](const char c1, const char c2){return sp(c1) and sp(c2);}), s.end());
    std::replace_if(s.begin(), s.end(), sp, alt);
    return s;
  }


  /* General machinery for handling "policy enums" P, which can be read
     from a string s via read<P>(s), and written to a stream s via output-
     streaming:
  */
  // The "registration of policies", to be specialised:
  template <typename Policy> struct RegistrationPolicies;
  // Reading a policy from a string:
  template <typename Policy>
  inline std::optional<Policy> read(const std::string_view s) noexcept {
    typedef RegistrationPolicies<Policy> reg;
    const auto begin = reg::string.begin(), end = reg::string.end();
    const auto i = std::find(begin, end, s);
    if (i == end) return {};
    else return static_cast<Policy>(i - begin);
  }

  namespace detail {
    template <class Res>
    void process_item(const std::string_view, Res&) noexcept {}
    template <typename P1, typename ... Policies, class Res>
    inline void process_item(const std::string_view item, Res& res) noexcept {
      const auto ri = read<P1>(item);
      if (ri) std::get<P1>(res) = *ri;
      else process_item<Policies...>(item, res);
    }
  }
  template <typename... Policies>
  struct translate {
    typedef std::tuple<Policies...> tuple_t;
    tuple_t operator()(const std::string_view arg, const char sep) noexcept {
      tuple_t res;
      for (const std::string& item : split(arg,sep)) {
        if (item.empty()) continue;
        detail::process_item<Policies...>(item, res);
      }
      return res;
    }
  };
  template <typename... Policies>
  struct translate<std::tuple<Policies...>> : translate<Policies...> {};


  // Basic variables from macros

  const std::string compilation_orig_date = __DATE__;
  const std::string compilation_tr_date = transform_spaces(compilation_orig_date, '_');
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
  inline std::string get_date(const time_t* const t, const std::string_view format = "%d.%m.%Y") {
    std::stringstream s;
    s << std::put_time(std::localtime(t), format.data());
    return s.str();
  }
  inline std::string get_time(const time_t* const t, const std::string_view format = "%T_%z") {
    std::stringstream s;
    s << std::put_time(std::localtime(t), format.data());
    return s.str();
  }
  struct CurrentTime {
    typedef std::chrono::high_resolution_clock clock;
    typedef clock::time_point time_point;
    typedef time_point::rep ticks_t;
    typedef std::time_t time_t;

    const time_point now = clock::now();
    const ticks_t ticks = now.time_since_epoch().count();

    const time_t now_t = std::chrono::system_clock::to_time_t(now);
    const std::string date = get_date(&now_t);
    const std::string time = get_time(&now_t);

    // Only timestamp:
    static ticks_t timestamp() noexcept {
      return clock::now().time_since_epoch().count();
    }
    // The number of nanoseconds per tick of timestamp:
    static FloatingPoint::float80 ns_per_tick() noexcept {
      typedef std::chrono::duration<FloatingPoint::float80, std::nano> NS;
      const NS res = clock::duration(1);
      return res.count();
    }
    static constexpr bool ticks_t_is_integer = std::numeric_limits<ticks_t>::is_integer;
    static constexpr bool ticks_t_is_signed = std::numeric_limits<ticks_t>::is_signed;
    static constexpr int ticks_t_digits = std::numeric_limits<ticks_t>::digits;

  };
  std::ostream& operator <<(std::ostream& out, const CurrentTime& t) {
    return out << t.date << " " << t.time << " " << t.ticks;
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
  // "Dimacs Header-Wrapper":
  struct DHW { std::string s; };
  std::ostream& operator <<(std::ostream& out, const DHW& s) {
    return out << "c ** " << s.s << " **\n";
  }


  std::ostream& operator <<(std::ostream& out, const Wrap& w) {
    const ProgramInfo& i{w.pi};
    switch (w.p) {

    case OP::explained :
    out << "** Information on the program:\n\n";
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
        << "\n** Current date, time, and ticks since the Unix epoch (1.1.1970):\n  "
        << CurrentTime{}
        << "\n  The number of ticks per nanosecond is "
        << CurrentTime::ns_per_tick() << "."
        << "\n  The underlying arithmetic type of the ticks-count is ";
    if (CurrentTime::ticks_t_is_signed) out << "signed ";
    else out << "unsigned ";
    if (CurrentTime::ticks_t_is_integer) out << "integral";
    else out << "floating-point";
    out << " with " << CurrentTime::ticks_t_digits << " digits.\n";

    return out;

    case OP::rd : return out;

    case OP::rf :
    [[fallthrough]];

    case OP::rh :
    out << "# Timestamp: " << CurrentTime{} << "\n";
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
    out << "c Output_time " << CurrentTime{} << "\n"
        << DHW{"Program information"}
        << DWW{"program_name"} << qu(i.prg) << "\n"
        << DWW{"version"} << qu(i.vrs) << "\n"
        << DWW{"date"} << qu(i.date) << "\n"
        << DWW{"gid_id"} << qu(i.git) << "\n"
        << DWW{"author"} << qu(i.aut) << "\n"
        << DWW{"url"} << qu(i.url) << "\n"
        << DHW{"Machine information"}
        << DWW{"machine_name"} << qu(i.machine) << "\n"
        << DWW{"bogomips"} << i.bogomips << "\n"
        << DHW{"Compilation information"}
        << DWW{"compiler_version"} << qu(i.comp_version) << "\n"
        << DWW{"compilation_date"} << qu(i.comp_date) << "\n"
        << DWW{"compilation_options"} << qu(i.comp_opt) << "\n"
    ;

    return out;

    default : return out << i;
    }
  }


  // Tools for special command-line arguments

  inline bool version_output(std::ostream& out, const ProgramInfo& pi, const int argc, const char* const argv[]) {
    if (argc == 2 and std::string_view(argv[1]) == "-v") {
      out << Wrap(pi, OP::explained);
      return true;
    }
    else return false;
  }

  inline bool profiling(const int argc, const char* const argv[]) noexcept {
    return argc == 2 and std::string_view(argv[1]) == "-p";
  }

  class Index {
    bool active = true;
    int i=1;
    void error() const noexcept {
      if (not active) {
        std::cerr << "Environment::Index: Inactive at i=" << i << ".\n";
        std::exit(error_code);
      }
    }
  public :
    static constexpr int error_code = 1;
    operator int() const noexcept { error(); return i; }
    Index operator ++(int) noexcept {
      error();
      const Index old = *this; ++i; return old;
    }
    void deactivate() noexcept { error(); active = false; }
  };

}

#endif

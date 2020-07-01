// Oliver Kullmann, 5.3.2019 (Swansea)
/* Copyright 2019, 2020 Oliver Kullmann
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
    - function-template code(Policy) to return the underlying integral-value
    - Wrapper WRP to output the strings registered (for example for
      help-output), with variations WRPI and wrpi(Policy)
    - function-template read(string) (converting strings to Policy-values)
    - function-template translate(string, char), converting strings to
      tuples of policy-values.

   Global variables:
    - compilation_orig_date
    - compilation_tr_date (spaces replaced by "_")
    - compilation_full_date (date and time)
    - compiler_version
    - ndebug
    - optimize
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
   Function isR(OP) return true iff r is one of rh, rd, rf.

   Further tools for handling of the command-line:
    - bool is_version_string(s), bool is_help_string(s)
      determine whether s is a string for version- resp. help-output.
    - bool help_header(ostream, int, char*[], ProgramInfo)
      checks for help-output, and outputs the header in the positive case
    - bool version_output(ostream, ProgramInfo, int, char*[],
        const addvo_fot& ao)
      checks for version-output, and outputs it in the positive case,
      where ao ("additional output") is a function-object
    - is_rheader(int, char*[]), is_rinfo(int, char*[], profiling(int, char*[])
      return booleans
    - args_output(ostream, int, char*[]) outputs the command-line
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
#include <sstream>
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
#include <exception>
#include <limits>
#include <random>
#include <iostream>
#include <type_traits>
#include <functional>

namespace Environment {
  constexpr bool ndebug =
#ifdef NDEBUG
    true;
#else
    false;
#endif
}
// Guaranteed to be included:
#include <cassert>

#include <ctime>
#include <cstdlib>
#include <cmath>
#include <cfenv>
#include <cstdint>

#include <Numerics/FloatingPoint.hpp>

namespace Environment {

  // General tools for string handling

// Turning the value of a macro into a string:
#define SVBR333ZxeL(x) #x
#define STR(x) SVBR333ZxeL(x)

  inline std::string qu(std::string s) {
    return "\"" + s + "\"";
  }

  // Replace character x by y in string s (returning a copy):
  inline std::string replace(std::string s, const char x, const char y) {
    std::replace(s.begin(), s.end(), x, y);
    return s;
  }
  // Remove character x in s:
  inline std::string remove(std::string s, const char x) {
    s.erase(std::remove(s.begin(), s.end(), x), s.end());
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

  // Remove all whitespace:
  inline void remove_spaces(std::string& s) noexcept {
    const std::locale loc;
    const auto sp = [&loc](const char c){return std::isspace(c,loc);};
    s.erase(std::remove_if(s.begin(), s.end(), sp), s.end());
  }
  // Transforms whitespace into char alt, contracting adjacent whitespace,
  // and eliminating leading and trailing whitespace:
  inline std::string transform_spaces(std::string s, const char alt = ' ') noexcept {
    const std::locale loc;
    const auto sp = [&loc](const char c){return std::isspace(c,loc);};
    s.erase(s.begin(), std::find_if_not(s.begin(), s.end(), sp));
    s.erase(std::find_if_not(s.rbegin(), s.rend(), sp).base(), s.end());
    s.erase(std::unique(s.begin(), s.end(), [&](const char c1, const char c2){return sp(c1) and sp(c2);}), s.end());
    std::replace_if(s.begin(), s.end(), sp, alt);
    return s;
  }


  /* General machinery for handling "policy enums" P, which can be read
     from a string s via read<P>(s), yielding an object of type
     std::optional<P>, with failure iff s wasn't equal any given string
     (except for the empty string, which is interpreted as the default-value
     of P); requirements on P:

      - P can be default-constructed, and converted from a pointer-difference.
      - The specialisation RegistrationPolicies<P> (in namespace Environment)
        constains the static member "string", a std::array of C-strings, which
        are used in the translation (for a matching string, the corresponding
        index is used to construct an element of P).
  */
  // The "registration of policies", to be specialised:
  template <typename Policy> struct RegistrationPolicies;
  template<typename P>
  constexpr auto code(P p) {
    return static_cast<typename std::underlying_type<P>::type>(p);
  }

  // Wrapper for output-streaming:
  template <typename Policy>
  struct WRP {
    typedef Policy P;
    typedef RegistrationPolicies<P> R;
  };
  // Output of the strings (as used for parsing), in the form
  // "input_string:policy_string":
  template <typename Policy>
  std::ostream& operator <<(std::ostream& out, const WRP<Policy>&) {
    using W = WRP<Policy>;
    if (W::R::size == 0) return out;
    out << W::R::string[0] << ":" << Policy(0);
    for (std::size_t i = 1; i < W::R::size; ++i)
      out << "," << W::R::string[i] << ":" << Policy(i);
    return out;
  }
  // Now only the input_strings:
  template <typename Policy>
  struct WRPI {
    typedef Policy P; typedef RegistrationPolicies<P> R;
  };
  template <typename Policy>
  std::ostream& operator <<(std::ostream& out, const WRPI<Policy>&) {
    using W = WRPI<Policy>;
    if (W::R::size == 0) return out;
    out << W::R::string[0];
    for (std::size_t i = 1; i < W::R::size; ++i) out << "," << W::R::string[i];
    return out;
  }
  // Variation of WPRI, with p first:
  template <typename Policy>
  std::string wrpi(const Policy p) {
    using W = WRPI<Policy>;
    std::string out;
    if (W::R::size == 0) return out;
    out += W::R::string[code(p)];
    for (std::size_t i = 0; i < W::R::size; ++i) {
      const Policy pi = Policy(i);
      if (pi == p) continue;
      out += ","; out += W::R::string[i];
    }
    return out;
  }

  // Reading a policy from a string:
  template <typename Policy>
  inline std::optional<Policy> read(const std::string_view s) noexcept {
    if (s.empty()) return Policy{};
    typedef RegistrationPolicies<Policy> reg;
    const auto begin = reg::string.begin(), end = reg::string.end();
    const auto i = std::find(begin, end, s);
    if (i == end) return {};
    else return static_cast<Policy>(i - begin);
  }

  /* Now the generalisation to a tuple of policies P1, ..., Pm:
      - Registration as for read (above).
      - The Pi must be pairwise different types.
      - The instantiated struct translate<P1,...,Pm> has members
        tuple_t = std::tuple<P1,...,Pm> and operator
        ()(std::string_view arg, char sep), which returns tuple_t,
        by separating arg according to sep, and for each string s obtained,
        ignoring empty strings, if there is a Pi which has a matching string,
        then the first i and the first string (via read, as above) is taken,
        yielding the value of Pi in the tuple.
      - If Pi is assigned several times, the last value is taken, if none,
        it is default-initialised.
      - Strings which don't mach anything are ignored.
  */

  namespace detail {
    template <typename T, typename... Elements>
    void process_element_wrp(std::ostream& out) {
      out << WRP<T>{};
      if constexpr (sizeof...(Elements) == 0) out << ")";
      else {
        out << " , ";
        process_element_wrp<Elements...>(out);
      }
    }
  }
  template <typename T, typename... Elements>
  std::ostream& operator <<(std::ostream& out, const WRP<std::tuple<T, Elements...>>&) {
    out << "(";
    detail::process_element_wrp<T, Elements...>(out);
    return out;
  }

  namespace detail {
    template <class Res>
    void process_item_reg(const std::string_view, Res&) noexcept {}
    template <typename P1, typename ... Policies, class Res>
    inline void process_item_reg(const std::string_view item, Res& res) noexcept {
      const auto ri = read<P1>(item);
      if (ri) std::get<P1>(res) = *ri;
      else process_item_reg<Policies...>(item, res);
    }
  }

  template <typename... Policies>
  struct translate {
    typedef std::tuple<Policies...> tuple_t;
    tuple_t operator()(const std::string_view arg, const char sep) noexcept {
      tuple_t res;
      for (const std::string& item : split(arg,sep)) {
        if (item.empty()) continue;
        detail::process_item_reg<Policies...>(item, res);
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
  constexpr bool optimize =
#ifdef __OPTIMIZE__
    true;
#else
    false;
#endif
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
    const string vrs;
    const string date;
    const string prg;
    const string aut;
    const string url;
    const string lic;
    const std::string& machine = machine_name;
    const double bogomips = bogomips_value;
    const string& comp_date = compilation_full_date;
    const string& comp_version = compiler_version;
    const bool ndebug = Environment::ndebug;
    const bool optimize = Environment::optimize;
    const string& comp_opt = optimisation;
    const string& git = git_id;

    ProgramInfo(const string vrs, const string date, const string prg, const string aut="", const string url="", const string lic="", const NP p = NP::extracted) noexcept : vrs(vrs), date(date), prg((p==NP::given) ? prg : auto_prg(prg)), aut(aut), url(url), lic(lic) {}

  };

  std::ostream& operator <<(std::ostream& out, const ProgramInfo& pi) {
    out << pi.prg << " " << pi.vrs << " " << pi.date << " " << pi.git << "\n";
    out << pi.machine << " " << pi.bogomips << "\n";
    out << pi.comp_version << " " << pi.comp_date << "\n";
    out << "NDEBUG=" << pi.ndebug << " OPTIMIZE=" << pi.optimize << " " << pi.comp_opt << "\n";
    return out;
  }

  // ProgramInfo output-policy:
  enum class OP { simple=0, explained=1, dimacs=2, rh=3, rd=4, rf=5 };
  // r : R, h : header, d : data, f = hd.
  inline constexpr bool isR(const OP o) noexcept {
    return o == OP::rh or o == OP::rd or o == OP::rf;
  }
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

    static constexpr bool ticks_t_is_integer = std::numeric_limits<ticks_t>::is_integer;
    static constexpr bool ticks_t_is_signed = std::numeric_limits<ticks_t>::is_signed;
    static constexpr int ticks_t_digits = std::numeric_limits<ticks_t>::digits;

    const time_point now = clock::now();
    const ticks_t ticks = now.time_since_epoch().count();

    typedef std::time_t time_t;
    const time_t now_t = std::chrono::system_clock::to_time_t(now);
    const std::string date = get_date(&now_t);
    const std::string time = get_time(&now_t);

    // Only timestamp:
    static ticks_t timestamp() noexcept {
      return clock::now().time_since_epoch().count();
    }

    // The number of nanoseconds per tick of timestamp:
    static constexpr FloatingPoint::float80 ns_per_tick =
      std::chrono::duration<FloatingPoint::float80, std::nano>(clock::duration(1)).count();
    typedef clock::period period;
    static_assert(ns_per_tick == 1e9L * FloatingPoint::float80(period::num) / FloatingPoint::float80(period::den));

    typedef std::uintmax_t uint_ticks_t;
    static constexpr uint_ticks_t ticks_as_uints(const ticks_t ts) {
      if constexpr (ticks_t_is_signed) assert(ts >= 0);
      if constexpr (ticks_t_is_integer) return uint_ticks_t(ts);
      else {
//#pragma STDC FENV_ACCESS ON // unknown gcc 8.3
        std::feclearexcept(FE_INVALID);
        const auto tsll{std::llround(ts)};
        if (std::fetestexcept(FE_INVALID))
          throw std::domain_error("Environment::CurrentTime::ticks_as_uints: FE_INVALID");
        assert(tsll >= 0);
        return tsll;
      }
    }

    // The timestamp as unsigned integer:
    static uint_ticks_t timestamp_uint() {
      return ticks_as_uints(timestamp());
    }

  };
  static_assert(CurrentTime::ticks_as_uints(0) == 0);
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
        << " NDEBUG:            " << (i.ndebug ? "defined" : "undefined") << "\n"
        << " OPTIMIZE:          " << (i.optimize ? "on" : "off") << "\n"
        << "\n** Digits of arithmetic types:\n"
        << " unsigned:          " << std::numeric_limits<unsigned>::digits << "\n"
        << " unsigned long:     " << std::numeric_limits<unsigned long>::digits << "\n"
        << " unsigned long long:" << std::numeric_limits<unsigned long long>::digits << "\n"
        << " double:            " << std::numeric_limits<double>::digits << "\n"
        << " long double:       " << std::numeric_limits<long double>::digits << "\n"
        << " radix long double: " << std::numeric_limits<long double>::radix << "\n"
        << "The random device uses unsigned, and reports entropy " << std::random_device().entropy() << ".\n"
        << "\n** Current date, time, and ticks since the Unix epoch (1.1.1970):\n  "
        << CurrentTime{}
        << "\n  Number of ticks per nanosecond = "
        << CurrentTime::ns_per_tick << "."
        << "\n  Underlying arithmetic type of ticks-count: ";
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
        << "#  NDEBUG:            " << i.ndebug << "\n"
        << "#  OPTIMIZE:          " << i.optimize << "\n"
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
        << DWW{"NDEBUG"} << i.ndebug << "\n"
        << DWW{"OPTIMIZE"} << i.optimize << "\n"
    ;

    return out;

    default : return out << i;
    }
  }


  // Tools for special command-line arguments

  bool is_version_string(const std::string_view s) noexcept {
    return s == "-v" or s == "--version";
  }
  typedef std::function<void(std::ostream&)> addvo_fot;
  bool version_output(std::ostream& out, const ProgramInfo& pi, const int argc, const char* const argv[], const addvo_fot& ao = addvo_fot()) {
    if (argc == 2 and is_version_string(argv[1])) {
      out << Wrap(pi, OP::explained);
      if (ao) ao(out);
      return true;
    }
    else return false;
  }

  bool is_help_string(const std::string_view s) noexcept {
    return s == "-h" or s == "--help";
  }
  bool help_header(std::ostream& out, const int argc, const char* const argv[], const ProgramInfo& pi) {
    assert(argc >= 1);
    if (argc != 2 or not is_help_string(argv[1])) return false;
    const std::string& program = pi.prg;
    out << "USAGE for \"" << program << "\" in version " << pi.vrs << ":\n\n"
    "> " << program << " [-v | --version]\n"
    " shows version information and exits.\n"
    "> " << program << " [-h | --help]\n"
    " shows help information and exits.\n\n"
;
    return true;
  }

  bool is_rheader(const int argc, const char* const argv[]) noexcept {
    return argc == 2 and std::string_view(argv[1]) == "-rh";
  }
  bool is_rinfo(const int argc, const char* const argv[]) noexcept {
    return argc == 2 and std::string_view(argv[1]) == "-ri";
  }

  bool profiling(const int argc, const char* const argv[]) noexcept {
    return argc == 2 and std::string_view(argv[1]) == "-p";
  }

  void args_output(std::ostream& out, const int argc, const char* const argv[]) {
    out << qu(argv[0]);
    for (int i = 1; i < argc; ++i) out << " " << qu(argv[i]);
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

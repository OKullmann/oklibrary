// Oliver Kullmann, 6.3.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  General definitions regarding the SAT-translation of the bcc-problem

   - Types imported from Bicliques2SAT:
    - var_t

   - Constants:
    - default_B
    - default_sec

   - Scoped enum Error

   - Functions:

    - read_var_t(string, var_t) -> var_t
    - read_uint_t(string, uint_t) -> uint_t
    -   read_uint_with_plus(string) -> <uint_t, bool> (helper function)

    - read_current(string) -> Bounds
    - read_bounds(string) -> optional<Bounds>

    - class Log for a pointer to an ofstream
    - read_log(string name, string error) -> Log
    - read_stats(name, infix, error) -> <Log,string>

TODOS:

1. Complete read_bounds
   - enable the direction as an argument

*/

#ifndef BCC2SAT_LsdBtFBZpI
#define BCC2SAT_LsdBtFBZpI

#include <utility>
#include <optional>
#include <algorithm>
#include <fstream>
#include <iostream>

#include <cstdlib>
#include <cassert>

#include <Numerics/NumTypes.hpp>
#include <Numerics/NumInOut.hpp>
#include <ProgramOptions/Strings.hpp>

#include "Bicliques2SAT.hpp"

namespace BCC2SAT {

  typedef Bicliques2SAT::var_t var_t;
  constexpr FloatingPoint::int_t default_sec = FloatingPoint::P231m1;

  enum class Error {
    missing_parameters = 1,
    faulty_parameters = 2,
    bad_sb = 3,
    bad_log = 4,
    bad_stats = 5,
    found_unsat = 20,
  };


  var_t read_var_t(const std::string& s, const var_t def) {
    if (s.empty()) return def;
    return FloatingPoint::toUInt(s);
  }

  FloatingPoint::uint_t read_uint_t(const std::string& s,
                                    const FloatingPoint::uint_t def) {
    if (s.empty()) return def;
    return FloatingPoint::touint(s);
  }


  typedef Bicliques2SAT::value_or_increment_t valoror_t;
  valoror_t read_valorinc(const std::string& s) {
    return s.starts_with('+') ?
      valoror_t{FloatingPoint::touint(s.substr(1)), true} :
      valoror_t{FloatingPoint::touint(s), false};
  }
  typedef std::vector<valoror_t> vec_valoror_t;
  vec_valoror_t read_vecvalorinc(const std::string& s) {
    const auto split = Environment::split(s, ',');
    const auto size = split.size();
    vec_valoror_t res; res.reserve(size);
    for (const auto& s : split) res.push_back(read_valorinc(s));
    return res;
  }

  typedef Bicliques2SAT::Bounds Bounds;
  Bounds read_current(const std::string& s) {
    using namespace Bicliques2SAT;
    const DI d = DI::none; const Bounds::choose_l cl;
    if (s.empty()) return Bounds(d, cl, {0,true});
    else return Bounds(d, cl, read_valorinc(s));
  }

  typedef Bicliques2SAT::DI DI;
  Bounds extract_bounds(const DI d, const vec_valoror_t& L) noexcept {
    const auto size = L.size();
    assert(size <= 2);
    if (size <= 1) {
      const valoror_t& val = size == 0 ? valoror_t{0,true} : L[0];
      return d == DI::upwards or d == DI::none ?
        Bounds(d, Bounds::choose_l{}, val) :
        Bounds(d, Bounds::choose_u{}, val);
    }
    else
      return Bounds(d, L[0], L[1]);
  }


  class Log {
    mutable std::ofstream* p;
  public :
    const bool is_cout;
    Log(std::ofstream* const p, const bool ic) noexcept : p(p), is_cout(ic) {
      assert(not is_cout or not null());
    }
    std::ofstream* pointer() const noexcept { return p; }
    bool null() const noexcept { return p == nullptr; }
    void close() const {
      if (is_cout) {assert(p and *p); *p << std::endl;}
      else if (p) {p->close(), p = nullptr;}
    }
  };

  typedef std::pair<Log, std::string> log_t;
  void print(std::ostream& out, const log_t& L) {
    if (L.first.is_cout) out << "/dev/stdout\n";
    else if (L.first.null()) out << "null\n";
    else out << L.second << "\n";
  }

  Log read_log(const std::string& s, const std::string& error) {
    if (s.empty()) return {nullptr, false};
    if (s == "/dev/stdout") return {new std::ofstream(s), true};
    std::ofstream* const res = new std::ofstream(s);
    if (res and not *res) {
      std::cerr << error <<
        "Log-output-file \"" << s << "\" can not be opened.\n";
      std::exit(int(Error::bad_log));
    }
    return {res, false};
  }

  log_t read_stats(std::string s,
                   const std::string& infix,
                   const std::string& error) {
    if (s.empty()) return {{nullptr, false},""};
    if (s == "/dev/stdout") return {{new std::ofstream(s), true}, s};
    if (s == "t")
      s = "Stats_" + infix + "_" + Environment::CurrentTime::timestamp_str();
    std::ofstream* const res = new std::ofstream(s);
    if (res and not *res) {
      std::cerr << error <<
        "Stats-output-file \"" << s << "\" can not be opened.\n";
      std::exit(int(Error::bad_stats));
    }
    return {{res, false}, s};
  }


  template <class FORMOPT>
  void commandline_output(const FORMOPT& formopt, const std::string& com,
                          std::ostream& out, const int argc,
                          const char* const argv[]) {
    using DC = Bicliques2SAT::DC;
    if (std::get<DC>(formopt) != DC::with) return;
    out << com;
    Environment::args_output(out, argc, argv);
    out << "\n";
  }


}

#endif

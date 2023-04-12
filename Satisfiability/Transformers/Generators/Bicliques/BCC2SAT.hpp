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


  std::pair<FloatingPoint::uint_t, bool>
  read_uint_with_plus(const std::string& s) {
    if (s.starts_with('+'))
      return {FloatingPoint::touint(s.substr(1)), true};
    else return {FloatingPoint::touint(s), false};
  }

  Bicliques2SAT::Bounds read_current(const std::string& s) {
    using namespace Bicliques2SAT;
    if (s.empty()) return Bounds(0, true, 0);
    const auto [B, with_plus] = read_uint_with_plus(s);
    if (with_plus) return Bounds(0, true, B);
    else return Bounds(B, false, 0);
  }

  std::optional<Bicliques2SAT::Bounds> read_bounds(const std::string& s) {
    // assuming for now DI::downwards XXX
    using namespace Bicliques2SAT;
    if (s.empty()) return Bounds{DI::downwards, true, 0, 0, 0};
    const auto split = Environment::split(s, ',');
    const auto size = split.size();
    assert(size != 0);
    if (size >= 3) return {};
    else if (size == 1) {
      const auto [B, with_plus] = read_uint_with_plus(s);
      if (with_plus) return Bounds{DI::downwards, true, B, 0, 0};
      else return Bounds{DI::downwards, false, 0, 0, B};
    }
    else {
      const auto l = read_uint_t(split[0], 0);
      const auto [u,with_plus] = read_uint_with_plus(split[1]);
      if (with_plus) return Bounds{DI::downwards, true, u, l, 0};
      else return Bounds{DI::downwards, false, 0, l, u};
    }
  }

  std::optional<Bicliques2SAT::Bounds> read_bounds(const std::string& s,
      const var_t n, const var_t numver, const var_t numedg) {
    // assuming for now DI::downwards XXX
    using namespace Bicliques2SAT;
    if (s.empty()) {
      const var_t B = std::min({n, numver-1, numedg});
      return Bounds{DI::downwards, false, 0, 0, B};
    }
    else return read_bounds(s);
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

// Oliver Kullmann, 15.2.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Components related to general input and output */

#ifndef INOUT_lrB3HGlVcP
#define INOUT_lrB3HGlVcP

#include <string>
#include <iostream>
#include <fstream>

#include <cstdlib>

#include "Generics.hpp"

namespace InOut {

  enum class ConformityLevel {normal=0, strict, verystrict, general};
  /* "Strict" yields error on pseudoempty clauses (without existential
      variables); having such clauses can make sense for autarky search.
  */
  std::ostream& operator <<(std::ostream& out, const ConformityLevel cl) noexcept {
    switch (cl) {
    case ConformityLevel::general : return out << "general";
    case ConformityLevel::normal : return out << "normal";
    case ConformityLevel::strict : return out << "strict";
    case ConformityLevel::verystrict : return out << "verystrict";
    default : return out << "normal";
    }
  }
  // String to ConformityLevel:
  ConformityLevel s2conlev(const std::string& s) noexcept {
    if (s == "g") return ConformityLevel::general;
    else if (s == "s") return ConformityLevel::strict;
    else if (s == "vs") return ConformityLevel::verystrict;
    else return ConformityLevel::normal;
  }

  enum class Error {
    file_reading=1,
    file_writing=2,
    file_pline=3,
    num_vars=4,
    allocation=5,
    literal_read=6,
    variable_value=7,
    number_clauses=8,
    empty_clause=9,
    a_rep_line=11,
    e_rep_line=12,
    a_read=13,
    e_read=14,
    a_rep=15,
    e_rep=16,
    a_line_read=17,
    e_line_read=18,
    a_empty=19,
    e_empty=21,
    d_empty=22,
    d_bada=23,
    pseudoempty_clause=24,
    empty_line=25,
    bad_comment=26,
    num_cls=27,
    illegal_comment=28,
    e_read_dline=29,
    e_rep_dline=30,
    a_read_dline=31,
    a_rep_dline=32,
    a_line_trail=33,
    e_line_trail=34,
    d_line_trail=35,
  };

  // Error output with ERROR-prefix, and each on a new line:
  struct Outputerr {
    const std::string e;
    Outputerr(const std::string& program) noexcept :
      e("ERROR[" + program + "]: ") {
      std::cerr.exceptions(std::ios::iostate(0));
    }
    template <typename T>
    const Outputerr& operator <<(const T& x) const noexcept {
      std::cerr << e << x << "\n";
      return *this;
    }
  };

  /* Class for output-objects solout and logout, which are initialised
     by function set_output from the command-line parameters.
     In general this class wrappes output-streams. The two
     public members (besides the constructor) are
       out << x;
       out.endl();
    which send the output to the internally stored ostream *p, if set.
    Via out.nil() it can be checked, whether p has been set.
    The destructor deletes *p iff member del = true.
    Furthermore Output-objects can be compared for equality (which refers to
    the underlying pointers).
  */
  class Output {
    std::ostream* p = nullptr;
    bool del = false;
    friend void set_output(const int, const char* const*, Output&, Output&, Outputerr&) noexcept;
  public :
    ~Output() noexcept { if (del) delete p; }
    template <typename T>
    const Output& operator <<(const T& x) const noexcept {
      if (p) *p << x; return *this;
    }
    void endl() const noexcept { if (p) {*p << "\n"; p->flush();} }
    bool nil() const noexcept { return p == nullptr; }
    friend bool operator ==(const Output& lhs, const Output& rhs) noexcept {
      return lhs.p == rhs.p;
    }
    friend bool operator !=(const Output& lhs, const Output& rhs) noexcept {
      return lhs.p != rhs.p;
    }
  };

  /* Assuming argv starts with
       input [output] [log]
     and setting solout, logout accordingly, with special cases for "-nil"
     (discarding output) and "-cout", "-cerr" and "-clog", while otherwise
     files are created (for output=log only one file):
  */
  void set_output(const int argc, const char* const argv[], Output& solout, Output& logout, Outputerr& errout) noexcept {
    std::ios_base::sync_with_stdio(false);
    struct NoExceptions {
      Output& solout, logout;
      NoExceptions(Output& s, Output& l) noexcept : solout(s), logout(l) {}
      ~NoExceptions() noexcept {
        if (solout.p != nullptr)
          solout.p->exceptions(std::ios::iostate(0));
        if (logout.p != nullptr)
          logout.p->exceptions(std::ios::iostate(0));
      }
    } noexceptions(solout, logout);
    logout.p = &std::cout;
    if (argc == 2) { solout.p = &std::cout; return; }
    const std::string outname(argv[2]);
    if (outname == "-cout") solout.p = &std::cout;
    else if (outname == "-cerr") solout.p = &std::cerr;
    else if (outname == "-clog") solout.p = &std::clog;
    else if (outname != "-nil") {
      solout.p = new std::ofstream(outname, std::ios::app);
      if (not *solout.p) {
        errout << ("Invalid output file: \"" + outname + "\".");
        std::exit(Generics::code(Error::file_writing));
      }
      solout.del = true;
    }
    if (argc == 3) return;
    const std::string logname = (std::string(argv[3])=="=") ? outname : argv[3];
    if (logname == "-cerr") logout.p = &std::cerr;
    else if (logname == "-clog") logout.p = &std::clog;
    else if (logname == "-nil") logout.p = nullptr;
    else if (logname != "-cout") {
      if (logname == outname) logout.p = solout.p;
      else {
        logout.p = new std::ofstream(logname, std::ios::app);
        if (not *logout.p) {
          errout << ("Invalid output file: \"" + logname + "\".");
          std::exit(Generics::code(Error::file_writing));
        }
        logout.del = true;
      }
    }
  }

  /* Input object, initialised with a "filename", which can be "-cin", in which
     case the input comes from standard input; for objects in of type Input,
     the instream is accessed via *in:
  */
  class InputStream {
    std::istream* const p;
    const bool del;
    Outputerr& errout;
    InputStream(const InputStream&) = delete;
    InputStream(InputStream&&) = delete;
  public :
    InputStream(const std::string& f, Outputerr& errout) :
    p(f == "-cin" ? &std::cin : new std::ifstream(f)), del(f != "-cin"),
    errout(errout) {
      if (not *p) {
        errout << "Invalid input filename.";
        std::exit(Generics::code(Error::file_reading));
      }
    }
    ~InputStream() { if (del) delete p; }
    std::istream& operator *() const { return *p; }
  };


}

#endif

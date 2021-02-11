// Oliver Kullmann, 11.2.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/InputOutput/Matching.cpp
  \brief Application for checking whether one file matches a pattern given by regular expressions

  \todo Which regular expressions?
   - Is awk-style really best?
   - It seems the default ECMAScript is better; see
     https://en.cppreference.com/w/cpp/regex/ecmascript
     for documentation.
   - It seems more powerful, and the depth-first search is likely
     more intuitive (though we might not need this here).
   - However, MatchFiles is used with the old testsystem in many places.
   - So we need either to construct another program, or use ECMAScript only
     with an option.
   - Perhaps best to create another program, called "Matching.cpp".

  \todo More styles for matching
   - The pattern-file P should also allow for line-wise matching.
   - Then it is an error if the numbers of lines are different.
   - In case of error, output the line-number and the two differing lines.
   - This is regulated by an optional third parameter (the option for the
     matching-style).
   - Always going for a full match (line per line, or for the full file),
     via std::regex_match (returning a boolean).
   - The only flat for regex_match possibly of interest might be
     std::regex_constants::match_any (but not really clear what that means).
     It seems that without that flag, the match must be unique -- perhaps
     this is actually what we need?
   - For the construction of the regular expression, std::regex::ECMAScript
     together with std::regex_constants::multiline seems appropriate
     (only the latter needs to be given for the constructor).
     Currently with gcc "multiline" is not available, but since we are
     splitting lines anyway (one regular expression for a line), that doesn't
     matter. And it seems also when using one regular expression for the
     whole file, then we won't use the beginning or end of the line, since
     we (likely) match the whole line.

  \todo More/better output
   - The regular expressions themselves could be faulty, and the
     corresponding error should be output.
   - In case of a matching-error, a precise output is needed (including
     spaces). Obtaining more information on the error is likely not possible.

*/

#include <iostream>
#include <string>
#include <fstream>
#include <regex>

#include <cstdlib>

#include <ProgramOptions/Environment.hpp>

namespace Matching {
  enum class MatO {lines=0, full=1};
  constexpr char sep = ',';
  typedef std::tuple<MatO> option_t;
}
namespace Environment {
  template <>
  struct RegistrationPolicies<Matching::MatO> {
    static constexpr int size = int(Matching::MatO::full)+1;
    static constexpr std::array<const char*, size> string
    {"lm", "fm"};
  };
}
namespace Matching {
  std::ostream& operator <<(std::ostream& out, const MatO m) {
    switch (m) {
    case MatO::lines : return out << "matching-lines";
    default : return out << "matching-file";}
  }

}

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "11.2.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/InputOutput/Matching.cpp",
        "GPL v3"};

  using namespace Matching;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " PatternFile InputFile options\n\n"
    " options : " << Environment::WRP<MatO>{} << "\n\n"
    " compares the patterns to the input, with output only in case of\n"
    " error or non-matching.\n\n"
;
    return true;
  }

  enum class Error {
    pnumber = 1,
    file_open = 2,
    file_read = 3,
    eof = 4,
    empty_pattern_file = 5,
    regular_expression = 6,
    number_lines = 7,
    mismatch = 8,
  };

  Environment::tokens_t split(const std::string& name) {
    std::ifstream in(name);
    if (not in) {
      std::cerr << error << "File \"" << name << "\" not readable.\n";
      std::exit(int(Error::file_open));
    }
    char c;
    auto res = Environment::split(in, '\n', c);
    if (in.bad()) {
      std::cerr << error << "Reading error with file \"" << name << "\".\n";
      std::exit(int(Error::file_read));
    }
    if (c != '\n') {
      std::cerr << error << "File \"" << name << "\" does not finish with"
        "\n end-of-line symbol, but with character-code " << int(c) << ".\n";
      std::exit(int(Error::eof));
    }
    return res;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 4) {
    std::cerr << error << "Exactly three input parameters are required:\n"
      " - the file-name for the pattern,\n"
      " - the file-name of the file to check,\n"
      " - and the matching-opting.\n";
    return int(Error::pnumber);
  }

  const std::string Pfile = argv[1];
  const auto P_lines = split(Pfile);
  typedef std::size_t size_t;
  const size_t N = P_lines.size();
  std::vector<std::regex> regv; regv.reserve(N);
  for (size_t i = 0; i < N; ++i) {
    try {
      regv.emplace_back(P_lines[i]);
    }
    catch (const std::regex_error& e) {
      std::cerr << error << "Regular expression error in file \"" <<
        Pfile << "\", line " << i+1 << ":\n"
        "expression: \"" << P_lines[i] << "\"\n"
        "what: " << e.what() << "\n"
        "code: " << e.code() << "\n";
      return int(Error::regular_expression);
    }
  }

  const std::string Cfile = argv[2];
  const auto C_lines = split(Cfile);
  if (C_lines.size() != N) {
    std::cerr << error << "File \"" << Cfile << "\" has " << C_lines.size() <<
      " lines, but the pattern-file has " << N << " lines.\n";
    return int(Error::number_lines);
  }
  for (size_t i = 0; i < N; ++i) {
    if (not std::regex_match(C_lines[i], regv[i])) {
      std::cerr << error << "Mismatch in line " << i+1 << ":\n"
        "Pattern: \"" << P_lines[i] << "\"\n"
        "Given  : \"" << C_lines[i] << "\"\n";
      return int(Error::mismatch);
    }
  }
}

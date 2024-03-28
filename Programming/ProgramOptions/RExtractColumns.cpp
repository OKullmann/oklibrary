// Oliver Kullmann, 22.3.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Extract named columns from an R-style file

  It is currently assumed:
   - There is a header.
   - Every data-line starts with a running number (these items will
     always be printed).
   - If the named column occurs several times in the header of the input,
     then the first occurrence is taken.
   - All comments are removed.
   - Spacing is standardised.

EXAMPLES:

ProgramOptions> cat Example.R
# Comment
 # also comment

# now comes the header:
  A  B  CD E FG   H  A
#

3  77  88 	99 # also allowed

#
88 a b c d e f g h i

ProgramOptions> cat Example.R | ./RExtractColumns_debug
 3
88
ProgramOptions> cat Example.R | ./RExtractColumns_debug A CD
    A CD
 3 77 99
88  a  c
ProgramOptions> cat Example.R | ./RExtractColumns_debug B A
    B  A
 3 88 77
88  b  a
ProgramOptions> cat Example.R | ./RExtractColumns_debug E
ERROR[RExtractColumns_debug]: Column 4 not found in line 1 = "3 77 88 99".


TODOS:

1. It should be possible to keep the comments.
    - No formatting should be applied to the comments.

*/

#include <iostream>
#include <vector>
#include <ranges>

#include <cstdlib>

#include "Environment.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.2",
        "28.3.2024",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/ProgramOptions/RExtractColumns.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg <<
      " [column-name]*\n\n"
      "  - column-names are strings selected from the header\n"
      "  - if there is exactly one column-name \"*\", then use"
      " complete header\n\n"
      "reads from standard input, and outputs to standard output.\n\n"
;
    return true;
  }

  using Environment::tokens_t;
  typedef std::vector<std::size_t> indices_t;

  indices_t
  determine_positions(const tokens_t& names, const tokens_t& header) {
    indices_t res; res.reserve(names.size());
    using index_map_t = Environment::index_map_t;
    const index_map_t map = [&header]{index_map_t res;
      for (std::size_t i = 0; i < header.size(); ++i)
        res.insert({header[i],i});
      return res;}();
    for (const auto& name : names) {
      const auto find = map.find(name);
      if (find == map.end()) {
        std::cerr << error << "Column \"" << name
                  << "\" not found in header: \"";
        Environment::out_line(std::cerr, header);
        std::cerr << "\".\n";
        std::exit(1);
      }
      res.push_back(find->second + 1);
    }
    return res;
  }


}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc == 2 and std::string(argv[1]) == "*") {
    const auto datafile = Environment::split2_cutoff(std::cin, '\n', '#');
    if (not datafile.empty()) {
      tokens_t print_header;
      print_header.reserve(datafile[0].size() + 1);
      print_header.push_back(" ");
      for (const auto& name : datafile[0]) print_header.push_back(name);
      Environment::print2dformat(std::cout,
        std::ranges::drop_view{datafile,1}, 1, print_header);
    }
    return 0;
  }

  const std::vector<std::string> names(argv+1, argv+argc);

  const auto datafile = Environment::split2_cutoff(std::cin, '\n', '#');
  if (datafile.empty()) return 0;
  const indices_t positions = determine_positions(names, datafile[0]);

  std::vector<tokens_t> extraction;
  for (std::size_t L = 1; L < datafile.size(); ++L) {
    const auto& line = datafile[L];
    assert(not line.empty());
    tokens_t extract; extract.reserve(positions.size() + 1);
    extract.push_back(line[0]);
    for (const auto& i : positions) {
      if (i >= line.size()) {
        std::cerr << error << "Column " << i << " not found in line "
                  << L << " = \"";
        Environment::out_line(std::cerr, line);
        std::cerr << "\".\n";
        return 1;
      }
      extract.push_back(line[i]);
    }
    extraction.push_back(std::move(extract));
  }

  {tokens_t print_header;
   if (not names.empty()) {
     print_header.reserve(names.size() + 1);
     print_header.push_back(" ");
     for (const auto& name : names) print_header.push_back(name);
   }
   Environment::print2dformat(std::cout, extraction, 1, print_header);
  }

}

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
   - Every data-line starts with a running number.

*/

#include <iostream>
#include <vector>

#include <cstdlib>

#include "Environment.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "12.3.2024",
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

  {tokens_t print_header; print_header.reserve(names.size() + 1);
   print_header.push_back(" ");
   for (const auto& name : names) print_header.push_back(name);
   Environment::print2dformat(std::cout, extraction, 1, print_header);
  }

}

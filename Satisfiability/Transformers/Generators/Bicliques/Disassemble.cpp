// Oliver Kullmann, 19.3.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  For input-QCNF F, disassemble F into the global-part and the other-part


EXAMPLES:

*/

#include <iostream>
#include <filesystem>
#include <fstream>

#include <ProgramOptions/Environment.hpp>

#include "DimacsTools.hpp"
#include "Bicliques2SAT.hpp"

#include "Disassemble.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.6",
        "20.3.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/Disassemble.cpp",
        "GPL v3"};

  using namespace Bicliques2SAT;
  using namespace Disassemble;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << " filename dirname\n\n"
    " filename       : " << "the input-QCNF\n"
    " dirname        : " << "the output-directory; default is the stripped input-name\n\n"
    " reads a qcnf from filename, and computes its parts:\n\n"
    "  - Arguments \"\" (the empty string) yield the default-values.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 3) {
    std::cerr << error <<
      "Exactly two arguments (filename, dirname)"
      " needed, but " << argc-1 << " provided.\n";
    return int(Error::missing_parameters);
  }

  const std::string filename = argv[1], dirname = argv[2];
  std::ifstream input(filename);
  if (not input) {
    std::cerr << error << "Can not open input-file \"" << filename
              << "\" for reading.\n";
    return int(Error::input_file_error);
  }
  const std::filesystem::path dir(extract_dir_path(filename, dirname));
  if (not std::filesystem::create_directory(dir)) {
    std::cerr << error << "Can not create output-directory " << dir
              << ".\n";
    return int(Error::output_directory_error);
  }

  const auto F = DimacsTools::read_strict_GslicedCNF(input);
  const GlobRepl GR(F);

  {std::filesystem::path E0path(E0(dir));
   std::ofstream E0file(E0path);
   if (not E0file) {
     std::cerr << error << "Can not create output-file " << E0path
               << ".\n";
     return int(Error::output_E0_error);
   }
   GR.E0(E0file);
  }

}

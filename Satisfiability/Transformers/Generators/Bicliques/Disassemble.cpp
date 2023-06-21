// Oliver Kullmann, 19.3.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  For input-QCNF F, disassemble F into the global-part and the other-part

  The order of the clauses is maintained, and the other-variables are also
  maintained.

EXAMPLES:

Bicliques> cat data/Example_00.qcnf
# added information on connected components
p cnf 7 6
a 3 5 7 0
e 1 2 4 6 0
3 2 -4 0        # C1
-3 5 1 6 0      # C1
-2 -4 0         # trivial (E0)
-5 -6 0         # C1
7 2 4 6 0       # C2
-7 -2 -4 -6 0   # C2
Bicliques> ./Disassemble_debug data/Example_00.qcnf ""
Bicliques> ls -l Example_00/
19 A_1_2_1
26 A_2_3_1
18 E0
29 E_1_2_1
28 E_2_3_1

The trivial components:
Bicliques> cat Example_00/E0
p cnf 6 1
-2 -4 0

C2:
Bicliques> cat Example_00/A_1_2_1
p cnf 1 2
1 0
-1 0
Bicliques> cat Example_00/E_1_2_1
p cnf 6 2
2 4 6 0
-2 -4 -6 0

C1:
Bicliques> cat Example_00/A_2_3_1
p cnf 2 3
1 0
-1 2 0
-2 0
Bicliques> cat Example_00/E_2_3_1
p cnf 6 3
2 -4 0
1 6 0
-6 0

The meaning of the parameters X_Y_Z in A_-files:

 - X is the number of (occurring) variables (and also the maximum index)
 - Y is the number of clauses
 - Z is the running number (for given X, Y).

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
        "0.1.0",
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

  {const std::filesystem::path E0path(E0(dir));
   std::ofstream E0file(E0path);
   if (not E0file) {
     std::cerr << error << "Can not create output-file " << E0path
               << ".\n";
     return int(Error::output_E0_error);
   }
   GR.E0(E0file);
  }

  {const auto begin = GR.ntcc_map.begin(), end = GR.ntcc_map.end();
   for (auto it = begin; it != end; ++it) {
     const auto& [dp, list] = *it;
     for (size_t i = 0; i < list.size(); ++i) {
       {const std::filesystem::path Epath(E(dir, dp, i));
        std::ofstream Efile(Epath);
        if (not Efile) {
          std::cerr << error << "Can not create output-file " << Epath
                    << ".\n";
          return int(Error::output_E_error);
        }
        GR.E(Efile, it, i);
       }
       {const std::filesystem::path Apath(A(dir, dp, i));
        std::ofstream Afile(Apath);
        if (not Afile) {
          std::cerr << error << "Can not create output-file " << Apath
                    << ".\n";
          return int(Error::output_A_error);
        }
        GR.A(Afile, it, i);
       }
     }
   }
  }

}

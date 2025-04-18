// Oliver Kullmann, 19.3.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  For input-QCNF F, disassemble F into the global-part and the other-part

   - The order of the clauses is maintained, and the other-variables are also
     maintained (no renaming).
   - The clauses are maintained (so that the original QCNF can be
     reconstructed) except of the following:
     - The trivial components, that is, of size 1 (having exactly one clause),
       are collected in E0, and here (pure) global literals are removed.
     - The order of literals is the standard-order (-1,1,-2,2,...).
     - For the global parts of a connected components, the global variables
       are renamed to 1, 2, ..., according to the order they occur in the
       a-line.
   - Components are collected into A_n_c_i and corresponding E_n_c_i files,
     where n is the number of global variables in the component, c the
     number of clauses, and i the running index.
   - For fixed n_c, lexicographical order is used (using the given order of
     clauses). So i=1 contains the component with the very first relevant
     clause and so on (recall that components are clause-disjoint).

BUGS

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
Showing the automatically created directory:
Bicliques> ./Disassemble_debug data/Example_00.qcnf "|"
data/Example_00
BBicliques> ls -la data/Example_00
19 A_1_2_1
26 A_2_3_1
18 E0
29 E_1_2_1
28 E_2_3_1
4096 .stats

We have 2 nontrivial components (and thus 2*2+1=5 cnf-files):
> cat data/Example_00/.stats/ntcc
2
Only one of them has more than one variable:
> cat data/Example_00/.stats/nbcc
1

Remark: the files ntcc and nbcc have no final eol, to simplify parsing.

Summary information on the number of components with a given number
of variables:
Bicliques> cat data/Example_00/.stats/nm
1 1
2 1
And with a given number of clauses (i.e., the size/vertex-number):
Bicliques> cat data/Example_00/.stats/cm
2 1
3 1

The trivial components:
Bicliques> cat data/Example_00/E0
p cnf 6 1
-2 -4 0

C2:
Bicliques> cat data/Example_00/A_1_2_1
p cnf 1 2
1 0
-1 0
Bicliques> cat data/Example_00/E_1_2_1
p cnf 6 2
2 4 6 0
-2 -4 -6 0

C1:
Bicliques> cat data/Example_00/A_2_3_1
p cnf 2 3
1 0
-1 2 0
-2 0
Bicliques> cat data/Example_00/E_2_3_1
p cnf 6 3
2 -4 0
1 6 0
-6 0

Remarks: With second argument "|" the created directory is shown,
and with "||" its name is sanitised (then only containing alpha-numerical
characters, underscores and hyphens).
A suffix-string is appended to the name.
For example from filename="T&.A.qdimacs" we get via "||.XYZ":
  T262EA.XYZ
(26 is hex for "&", 2E is hex for ".").


The meaning of the parameters X_Y_Z in A_-files:

 - X is the number of (occurring) variables (and also the maximum index)
 - Y is the number of clauses
 - Z is the running number (for given X, Y).


Another example:
Bicliques> cat data/Example_03.qcnf
p cnf 20 8
a 4 2 17 13 9 15 0
e 1 3 5 20 19 0
a 6 7 8 0
e 11 10 14 12 16 18 0
4 0                        # E0
-3 -1 -20 19 4 6 0         # E0
9 17 -11 10 0              # C1
2 12 9 0                   # C2
-2 9 20 0                  # C2
2 9 15 0                   # C2
-17 13 7 0                 # C1
-13 -11 5 3 1 6 7 0        # C1
Bicliques> ./Disassemble_debug data/Example_03.qcnf DIR
Bicliques> ls -la DIR/
28 A_3_3_1
31 A_3_3_2
30 E0
40 E_3_3_1
23 E_3_3_2
4096 .stats
> cat DIR/.stats/ntcc
2
> cat DIR/.stats/nbcc
2

E0:
p cnf 20 2
0
-1 -3 6 19 -20 0

C1:
> cat A_3_3_1
p cnf 3 3
1 3 0
2 -3 0
-2 0
> cat E_3_3_1
p cnf 20 3
10 -11 0
7 0
1 3 5 6 7 -11 0

C2:
> cat A_3_3_2
p cnf 3 3
1 2 0
-1 2 0
1 2 3 0
> cat E_3_3_2
p cnf 20 3
12 0
20 0
0


An example without global variables:
Bicliques> cat data/Example_04.qcnf
p cnf 4 2
e 1 3 0
a 2 4 0
-1 -2 0
3 4 0
> ./Disassemble_debug data/Example_04.qcnf DIR
> cat DIR/.stats/ntcc
0


*/

#include <iostream>
#include <filesystem>
#include <fstream>
#include <map>

#include <ProgramOptions/Environment.hpp>

#include "DimacsTools.hpp"
#include "Bicliques2SAT.hpp"

#include "Disassemble.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.3.2",
        "23.7.2023",
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
         << " filename dirname [lowerbound=0]\n\n"
    " filename       : " << "the input-QCNF\n"
    " dirname        : " << "the output-directory:\n"
    "                  - \"\" uses the stem of the input-path\n"
    "                  - \"|SUFF\" additionally outputs the new name\n"
    "                  - \"||SUFF\" also sanitises the name\n"
    "                  - in both cases the string SUFF is appended to the name\n"
    " [lowerbound=0] : if used:\n"
    "                  - only A-files are output\n"
    "                  - and only those A_n-files with n > lowerbound\n\n"
    " reads a qcnf from filename, computes its parts, and stores them under dirname:\n\n"
    "  - in case of \"|[|]\" and given lowerbound, if there are no A-files, then\n"
    "    dirname is not created, and the empty string is output.\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 3 and argc != 4) {
    std::cerr << error <<
      "Two or three arguments (filename, dirname, [lower-bound])"
      " needed, but " << argc-1 << " provided.\n";
    return int(Error::missing_parameters);
  }

  const std::string filename = argv[1], dirname = argv[2];
  const std::string show_params = "filename=\"" + filename + "\", dirname=\""
    + dirname + "\"\n  ";
  const std::string ferror = error + show_params;
  std::ifstream input(filename);
  if (not input) {
    std::cerr << ferror << "Can not open input-file \"" << filename
              << "\" for reading.\n";
    return int(Error::input_file_error);
  }
  const auto [dir, with_output] = extract_dir_path(filename, dirname);
  if (std::filesystem::exists(dir)) {
    std::cerr << ferror << "The directory-name " << dir
              << " already exists.\n";
    return int(Error::output_directory_exists);
  }
  const auto [A_only, lower_bound] = extract_restriction(argc, argv);


  const auto F = read_GslicedCNF(input, ferror);
  const GlobRepl GR(F);
  if (A_only and with_output) {
    if (GR.ntcc_map.empty()) return 0;
    const auto& last = *--GR.ntcc_map.end();
    if (last.first.n <= lower_bound) return 0;
  }

  if (not std::filesystem::create_directory(dir)) {
    std::cerr << ferror << "Can not create output-directory " << dir
              << ".\n";
    return int(Error::output_directory_creation);
  }
  if (with_output) std::cout << dir.string() << std::endl;
  const std::filesystem::path statsdir = statsdir_path(dir);
  if (not std::filesystem::create_directory(statsdir)) {
    std::cerr << ferror << "Can not create statistics-directory " << statsdir
              << ".\n";
    return int(Error::output_statsdir_creation);
  }

  if (not A_only) {
    const std::filesystem::path E0path(E0(dir));
    std::ofstream E0file(E0path);
    if (not E0file) {
      std::cerr << ferror << "Can not create output-file " << E0path
                << ".\n";
      return int(Error::output_E0_error);
    }
    GR.E0(E0file);
  }

  size_t ntcc = 0, nbcc = 0;
  std::map<size_t, size_t> n_map, c_map;
  {const auto begin = GR.ntcc_map.begin(), end = GR.ntcc_map.end();
   for (auto it = begin; it != end; ++it) {
     const auto& [dp, list] = *it;
     for (size_t i = 0; i < list.size(); ++i) {
       if (not A_only) {
         const std::filesystem::path Epath(E(dir, dp, i));
         std::ofstream Efile(Epath);
         if (not Efile) {
           std::cerr << ferror << "Can not create output-file " << Epath
                     << ".\n";
           return int(Error::output_E_error);
         }
         GR.E(Efile, it, i);
       }
       const auto [n,c] = dp;
       if (n > lower_bound) {
         ++ntcc; if (n >= 2) ++nbcc;
         ++n_map[n]; ++c_map[c];
         const std::filesystem::path Apath(A(dir, dp, i));
         std::ofstream Afile(Apath);
         if (not Afile) {
           std::cerr << ferror << "Can not create output-file " << Apath
                     << ".\n";
           return int(Error::output_A_error);
         }
         GR.A(Afile, it, i);
       }
     }
   }
  }
  write_item(ntcc, ntccfile, statsdir, ferror);
  write_item(nbcc, nbccfile, statsdir, ferror);
  write_map(n_map, nmapfile, statsdir, ferror);
  write_map(c_map, cmapfile, statsdir, ferror);

}

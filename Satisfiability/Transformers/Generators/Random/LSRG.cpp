// Oliver Kullmann, 24.12.2020 (Swansea)
/* Copyright 2020, 2021, 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Random Latin squares generator

USAGE:

> ./LSRG -v | --version

for information on the program, the version, and the environment.

> ./LSRG -h | --help

for basic help-information.

> ./LSRG [N=10] [options] [seeds] [selection] [output]

for creation one random Latin square of order N.

For the complete documentation, see
  docus/LSRG.txt


BUGS:


TODOS:

-3. docus/LSRG.txt needs to be updated. OZ

  - This must be done always together with every "breaking" change.
  - Some discussion on the number of rounds is needed.
  - We can discuss related implementations

   https://brainwagon.org/2016/05/17/code-for-generating-a-random-latin-square/
     s[i][j][k] are basically the boolean variables as in the direct
     encoding, when using only 0,1-values (can be intepreted as boolean
     values or as integers); now the trick is to allow also some values
     -1 (for the "improper ls's").

     The main loop (variable i) always changes a proper ls into another
     proper ls: Thus the number of iterations N^3 is for PROPER ls's -- not
     for all moves. For the small N considered, that
     seems roughly to make a factor of 10.

-2. Update of help-text (should always be done at the time of
    changing functionality) OZ
     - A balance needs to achieved between giving an overview and giving
       enough details.

-1. Write tests. OZ
     - Application tests in the new system:
      - All options need to be tested.
     - Unit tests:
      - At least the main functionality.

0. Provide a form of the generator for partial-ls-completion, with output as
   N^3 Dimacs-variables, where the N^2 variables set to 1 are discarded, and
   from the remaining N^3-N^2 variables a given fraction is kept.
    - To the option "+enc" the variation "+encneg" (with "neg" for "negative")
      is added.
    - Then the selection-information is a fraction.
    - See the documentation in LSRG.hpp.

1. Extend the dimacs-partial-ls generator to k >= 2.
    - DONE The function LSRG::enc(i,j,k) should be renamed to enc(i,j,eps).
    - And generalised to enc(i,k,eps,p) (with p < k).
    - Symmetry-breaking of the SAT-encoding needs also to be taken into
      account.
    - One could just use Encoding::VarEncoding (in LatinSquares/Encoding.hpp);
      that should be best.
    - The constructor of VarEncoding takes arguments which in our context
      are not needed --- perhaps one could supply them with invalid values,
      to check whethe they are being used?
    - Or VarEncoding could provide a different constructor.

2. Provide the dimacs-output either as assignment (as now) or as
   a cnf consisting of unit-clauses (with or without p-line).
    - Can one integrate the two generators, LSRC and Mols?
    - Without integration, the Dimacs-parameters will be wrong for the
      unit-clauses.
    - Easier with the application of the partial assignment, but nevertheless
      it would be better if LSRG could produce the complete output.
    - Or perhaps Mols should produce it?
    - On the other hand, a script for passing the parameters to the involved
      programs would be quite easy.
    - Perhaps "Mols" should be renamed "Mols2SAT".
    - And the script could be called "MolsRG2SAT".
    - It takes first the parameters for Mols2SAT, and then, separated by "-",
      the parameters for LSRG.
    - In principle better to let MolsRG2SAT be a C++ program, which doesn't
      write the intermediate results to file, but handles them in memory.

3. Test randomness OK
   - At least check all single cells for randomness.
   - And compute for small N all L(N) latin squares, and check whether the
     sequence produced represents a random number from 1,...,L(N).
   - For applying the basic tests for RandomGen::UniformRange, see
     TimingUniformRange.cpp, with results reported in
     data/stat_tests/UniformRange.
   - The current leading software for providing canned tests seems

      http://www.iro.umontreal.ca/~lecuyer/myftp/papers/testu01.pdf
      TestU01: A C Library for Empirical Testing of Random Number Generators
      Pierre L'Ecuyer and Richard Simard

      Available at
      http://simul.iro.umontreal.ca/testu01/tu01.html
      version TestU01-1.2.3
      OK had no problems with installing using the source files.

      Some additional information available at
      https://en.wikipedia.org/wiki/TestU01
   - To use TestU01, one needs to write a file of "random bits", or
     write a generator for unsigned 32-bit numbers.
   - To test a uniform number generator with the range {1,...,R},
     one first determines R' = largest power of 2 less-or-equal R,
     drops random numbers x > R', and converts otherwise x into binary.
     One needs two versions, from both ends.
     In this way one naturally obtains two streams of bits (one in case
     R = R').
   - Random latin squares (in their various forms) are considered as uniform
     number generators for R = total number of possibilies.
   - For the special cases of
       - all ls,   N=2 : R = 2 (1 bit)
       - all rls,  N=4 : R = 4 (2 bits)
     we can indeed actually put this easily into a 32-bit word, and so should
     use the more extensive tests for that.

4. Proper random latin and euler squares from precomputed main classes
   - http://users.cecs.anu.edu.au/~bdm/data/latin.html

*/

#include <iostream>
#include <fstream>
#include <string>

#include <ProgramOptions/Environment.hpp>

#include "LSRG.hpp"
#include "Numbers.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.13.1",
        "15.5.2025",
        __FILE__,
        "Oliver Kullmann and Oleg Zaikin",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/LSRG.cpp",
        "GPL v3"};

  using namespace LSRG;

  namespace RG = RandGen;
  namespace LS = LatinSquares;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " [N[,k]] [options] [selections] [seeds] [output]\n\n"
    " N         : default = " << N_default << "\n"
    " k         : default = " << 1 << "\n"
    " options   : " << Environment::WRP<GenO>{} << "\n"
    "           : " << Environment::WRP<LS::StRLS>{} << "\n"
    "           : " << Environment::WRP<EncO>{} << "\n"
    "           : " << Environment::WRP<ForO>{} << "\n"
    " selection : k=1:\n"
    "               r,c,s with r,c in [0,N], s in [0,N^2], r*c+s <= N^2,\n"
    "               default = N,N,0 = 0,0,N*N\n"
    "           : k>1:\n"
    "               count*selection as above, separated by \";\", sum of counts = k\n"
    " seeds     : ";
    RG::explanation_seeds(std::cout, 11);
    std::cout <<
    " output    : \"-cout\" (standard output) or \"\"[-]\"\" (default filename) or \"FILENAME\"\n\n"
    " generates k random Latin squares of order N:\n\n"
    "  - Trailing arguments can be left out, then using their default-values.\n"
    "  - Arguments \"\" (the empty string) yield also the default-values,\n"
    "    except for the output, where it yields the default output-filename.\n"
    "  - The optional \"-\" for the default-filename means \"don't print filename\" (done otherwise).\n\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  Environment::Index index;

  using LS::ls_dim_t;
  const Dim D = argc <= index ?
    Dim{} : read_N(argv[index++], error);

  const option_t options = argc <= index ? option_t{} :
    Environment::translate<option_t>()(argv[index++], sep);
  const GenO geo = std::get<GenO>(options);
  const LS::StRLS sto = std::get<LS::StRLS>(options);
  const EncO eo = std::get<EncO>(options);
  const ForO fo = std::get<ForO>(options);

  const selection_vt sel = argc <= index ?
    toSelection(D, "", error) : toSelection(D, argv[index++], error);
  assert(sel.size() == D.k);

  // The seed-string:
  const std::string ss = argc <= index ? "" : argv[index++];
  auto seeds = basic_seeds(D, sel, geo, sto);
  const RG::gen_uint_t basic_size = seeds.size();
  SO::add_user_seeds(seeds, ss);

  std::ofstream out;
  std::string filename;
  if (index == argc or std::string_view(argv[index]) == "-cout") {
    out.basic_ios<char>::rdbuf(std::cout.rdbuf());
    filename = "-cout";
  }
  else {
    filename = argv[index];
    if (filename.empty()) filename = default_filename(D, eo, seeds);
    assert(not filename.empty());
    const bool output_message = filename[0] != '-';
    if (not output_message) filename.erase(0,1);
    if (filename.empty()) filename = default_filename(D, eo, seeds);
    assert(not filename.empty());
    out.open(filename);
    if (not out) {
      std::cerr << error << "Can't open file \"" << filename << "\"\n";
      return int(RG::Error::file_open);
    }
    if (output_message)
      std::cout << "Output to file \"" << filename << "\".\n";
  }
  index++;

  index.deactivate();


  if (fo == ForO::wc) {
    out << Environment::Wrap(proginfo, Environment::OP::dimacs);
    using Environment::DHW;
    using Environment::DWW;
    using Environment::qu;
    const auto N = D.N;
    out << DHW{"Parameters"}
        << DWW{"command-line"};
    Environment::args_output(out, argc, argv);
    out << "\n"
        << DWW{"N"} << N << "\n";
    if (eo == EncO::dim)
      out << DWW{" num_vars=N^3"} << std::uint64_t(N)*N*N << "\n";
    out << DWW{"variant"} << D.v << "\n";
    if (D.v == lsrg_variant::with_k)
      out <<DWW{" k"} << D.k << "\n";
    out << DWW{"gen-option"} << geo << "\n"
        << DWW{"std-option"} << sto << "\n"
        << DWW{"encoding-option"} << eo << "\n"
        << DWW{"format-option"} << fo << "\n"
        << DWW{"selection"} << sel[0];
    for (RG::gen_uint_t i = 1; i < sel.size(); ++i)
      out << ";" << sel[i];
    out << "\n"
        << DWW{" num_cells"} << sel.size() << "\n"
        << DWW{"output"} << qu(filename) << "\n"
        << DWW{"num_e-seeds"} << basic_size << "+" << seeds.size() - basic_size << "=" << seeds.size() << "\n"
        << DWW{" e-seeds"} << RG::ESW{seeds} << "\n";
    if (eo == EncO::ls)
      out << "\n";
  }
  else if (fo == ForO::os)
    out << "c " <<  RG::ESW{seeds} << "\n";

  if (D.k == 1) {
    const auto L = random_ls(D.N, sel[0], geo, sto, seeds);
    if (eo == EncO::ls) out << LS::LS_t{L};
    else dimacs_output(out, L);
  }
  else {
    if (eo != EncO::ls) {
      std::cerr << "NOT IMPLEMENTED YET.\n";
      return 1;
    }
    out << D.N << " " << D.k << "\n\n";
    RG::RandGen_t g(seeds);
    out << LS::LS_t{random_ls(D.N, sel[0], geo, sto, g)};
    for (RG::gen_uint_t i = 1; i < D.k; ++i)
      out << "\n" << LS::LS_t{random_ls(D.N, sel[i], geo, sto, g)};
  }
}

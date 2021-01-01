// Oliver Kullmann, 24.12.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
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

> ./LSRG [N=10] [m=1] [seeds] [output]

for creation of m random Latin squares of order N.

For the complete documentation, see
  docus/LSRG.txt


BUGS:

1. docus/LSRG.txt needs to be updated. OZ
  - This must be done always together with every "breaking" change.

TODOS:

-3. Update of help-text (should always be done at the time of
    changing functionality) OZ

-2. Write tests. OZ

-1. Correct construction of LSRandGen_t OZ,OK
     - The order of data-members seems random.
     - public vs private looks strange.
     - In general public vs private needs re-organisation.

0. Complete handling of seeding OK
   (a) There needs to be a fixed initial part of the seed-sequence,
       specifying the organisation and the software.
     - The role-model is ClauseSets::seeds(par), and is described in
       docus/BRG.txt, Part III.
     - (1) In order to allow other groups to create such designs, without
       clash, there is a highest organisation-timestamp (creation time).
     - (2) Then we have a area-identifier, like logic=0, graph_theory=1,
       combinatorics=2, mathematics=3, physics=4.
     - (3) Then the main type (ClauseSets::MainType), which describes which
       type of formula (etc.) is created.
     - (4) Then the identifier of the software (timestamp),
     - (5) Followed by a version number (just a natural number).
     - (6) Followed by the length of the following section (these lengths
       always include the length-component of the following section).
     - Always 64-bit seeds. Should the length be for the final 32-bit
       version or for the input-view, the 64-bit version? Since the user-view
       is shown, likely best the count for the 64-bit form.

   (b) The second part of the initial seed-sequence encodes general parameters,
       relating to generic properties.
     - Finally in ClauseSets we have the thread-index (a hypothetical parameter
       for hypothetical parallel computation); perhaps this should go first.
     - In ClauseSets, afther the main type follows the sub-type, which  encodes
       the main structural parameters (renaming and ordering).
     - The third component of the initial part in ClauseSets is the number
       of clause-blocks.
     - As final part of the second part the length of the following section.
   (c) The third part of the initial seed-sequence encodes the concrete
       parameters. This is at least N here.
       And we have the length of the following, final part.
   (d) The fourth, final part of the seed-sequence is the user-specified seeds.
   (e) The splitting into 32-bit numbers needs also to be fixed.
       General tools are needed.

   (f) Concerning parameter m: it seems best to discard it -- the
       initialisation of the random engine is likely negligible (timewise)
       compared to the other computations.
       On the other hand, it could be made part of the parameter-section, after
       N. Then the output is not a single latin square, but m latin squares,
       used for examples for substitution into MOLS. Then the anticipated
       use-case would not be for experimenting with long sequences, but
       the normal case would be small m, like 1,2,3.
       However then it needed to be guaranteed that exactly m latin squares
       are produced, not more, not less.
       But since we enhance the creation process by starting with a
       pseud-random ls, we should not have different types of "random ls's",
       and thus m should be discarded. IF we want to create say two
       ls's at a time, then the seed-sequence is good enough for that.

1. Additional structural parameters OK,OZ
   (a) Form of generation: "jm", "ma" (matching alone),
       "gma" (greedy+matching alone), "majm", "gmajm" (default).
   (b) Standardisation: "rs", "cs", "rcs", "ns" (default: no standardisation).
   (c) Satisfiable partial latin squares: Three parameters
     - R <= N: number of rows
     - C <= N: number of columns
     - S <= n^2 - R*C: number of single cells
       First a random ls is created, then randomly R rows, C columns, S
       cells are chosen.
       The default value is (N,N,0) (equivalently e.g. (0,0,N^2)).
       If some form of standardisation is chosen, then the standardised
       rows/columns are not touched (and always selected).
   (d) These parameters are given to the overal generator-classes, which
       handles the full generation (inclusive seeding).

       The current class RandGenLS_t just becomes a free-standing function, with
       the initial ls L as parameter (similar to random_ls(n,ocr,ost,g); also
       a better name for that function then is needed).

   (e) For randomly choosing S cells, one after another, and for the cell a
       random available value, another generator is needed (this is a completely
       different design).

2. Reflect on usage of special 16/32-bit types OZ,OK
   - One also has to be careful about "pow" (which means many things,
     and likely one doesn't know what it means).

3. Simplify names OZ
   - Names are often too long, without actually telling "a story".
   - DONE Likely "improper" isn't a good name; perhaps "special".
     So using "SpecialCell".
   - DONE The suffix "i" in names is likely often superfluous.

4. Better semantics and syntax for "improper cells" OZ
   - Likely "positv" is better replaced with a struct (perhaps members
     "i" and "j"). Likely "posit" (what does that mean?) is misleading,
     since it sounds like "positive".
     What really is in "ImproperCell" are the two indices of the cell,
     and its triple. So one could just have "x, y, i,j,k" in the struct.
     It is important to develop shorthands (with documentation) for the
     most important operations -- they must be "mathematised".
   - There likely should be operations with "ImproperCell".
   - How is it indicated that the special cell is inactive?
     The bool "proper" in LSRandGen_t likely should be part of the
     special cell itself.

5. DONE Complete documentation OZ
   - DONE Describe all steps of the algorithm in docus/LSRG.txt.
   - DONE Describe the used data structure.

6. Improve function perturbate_square OZ
   - It is too long -- the should be abstract operations used.

7. Test randomness OZ,OK
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

8. Improve interface OZ,OK
   - DONE The seeding should happen with the construction of the
     ls-generator-object (not independently of it).
   - Different from clause-set-generation, here the generator likely is most
     often used internally, not via file-output.

9. Check improper rows and columns OZ
  - After each perturbation check whether values' sum of both improper row and
    column is N(N-1)/2.

10. Do not search for duplicates OZ
  - For an improper square use saved previous state instead of searching for
    duplicates.

*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>

#include <ProgramOptions/Environment.hpp>

#include "LatinSquares.hpp"
#include "Numbers.hpp"
#include "Distributions.hpp"
#include "ClauseSets.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.11",
        "30.12.2020",
        __FILE__,
        "Oliver Kullmann and Oleg Zaikin",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/LSRG.cpp",
        "GPL v3"};

  using namespace RandGen; // this should be removed XXX
  using namespace LatinSquares;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";
  constexpr ls_dim_t N_default = 10;
  constexpr std::uint64_t m_default = 1;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " [N=10] [m=1] [seeds] [output]\n\n"
    "   N       : order\n"
    "   m       : desired number of random Latin squares\n"
    "   seeds   : \"s1, ..., sp\", with p >= 0 seed-values si, which are\n"
    "             unsigned 64-bit integers, \"r\" (for \"random\"), or \"t\" (for \"timestamp\")\n"
    "   output  : \"-cout\" (standard output) or \"FILENAME\"\n\n"
    " generates m random Latin squares of order N\n\n"
    "  - Trailing arguments can be left out, then using their default-values.\n"
;
    return true;
  }

  // The special cell is used if a current square is improper;
  // x and y are indices of the cell, while triple (i,j,k) is cell's value.
  struct SpecialCell {
    ls_dim_t x, y;
    ls_dim_t i, j, k;
    bool active;
  };
  constexpr bool valid(const SpecialCell& s, const ls_dim_t N) noexcept {
    if (not(s.x<N and s.y<N and s.i<N and s.j<N and s.k<N)) return false;
    if (not s.active) return true;
    return s.i!=s.j and s.i!=s.k and s.j!=s.k;
  }

  class LSRandGen_t {
    SpecialCell scell;
    RandGen_t g;
    const ls_dim_t N;
    ls_t L;
    // Semantics needed: XXX
    std::uint64_t pertrnum;
    std::uint64_t additpertrnum;
    std::uint64_t properlsnum;

  public:

    LSRandGen_t(const ls_dim_t& N, const vec_eseed_t& s) :
      scell{0,0,0,0,0,false}, g(transform(s, SP::split)), N(N), L(cyclic_ls(N)) {}

    // Find a random LS of order N:
    void find_random_ls() noexcept {
      pertrnum=0; additpertrnum=0; properlsnum=0;
      const auto bound = gen_uint_t(N) * N * N;
      for (std::uint32_t i = 0; i < bound; ++i) perturbate_square();
      while (not LatinSquares::valid(L)) {perturbate_square(); ++additpertrnum;}
    }

  private:

    // Perturbate current square:
    void perturbate_square() noexcept {
      ls_dim_t modrow, modcol, modcellnewv, modcelloldv,
        opposrow, opposcol, opposcellv;

      if (not scell.active) {
        // Randomly choose cell and its new value:
        {UniformRange U(g, N);
         modrow = U(); modcol = U();
         modcelloldv = L[modrow][modcol];
         modcellnewv = UniformRange(g,N-1)();
         if (modcellnewv >= modcelloldv) ++modcellnewv;
        }
        // Find a 2*2 subsquare for modification:
        const ls_row_t& row = L[modrow];
        {const auto it = std::find(row.begin(), row.end(), modcellnewv);
         assert(it != row.end());
         opposcol = std::distance(row.begin(), it);
        }
        ls_row_t col(N);
        for (unsigned i = 0; i < N; ++i) col[i] = L[i][modcol];
        {const auto it = std::find(col.begin(), col.end(), modcellnewv);
         assert(it != col.end());
         opposrow = std::distance(col.begin(), it);
        }
        opposcellv = L[opposrow][opposcol];
        // Update the found 2*2 subsquare:
        L[modrow][modcol] = modcellnewv;
        L[modrow][opposcol] = modcelloldv;
        L[opposrow][modcol] = modcelloldv;
        L[opposrow][opposcol] = modcellnewv;
      }
      else {
        // Randomly choose a positive value (one of two) in the special cell:
        const ls_dim_t firstimpposv = bernoulli(g) ? scell.i : scell.j;
        modcelloldv = (firstimpposv == scell.i) ? scell.j : scell.i;
        // Randomly choose one of two duplicate indices in the improper row:
        const ls_row_t& row = L[scell.x];
        {const std::array<ls_dim_t,2> duplvinds = find_first_duplication(row);
         assert(duplvinds.size() == 2);
         opposcol = duplvinds[bernoulli(g)];
        }
        // Randomly choose one of two duplicate indices in the improper column:
        {ls_row_t col(N);
         for (unsigned i = 0; i < N; ++i) col[i] = L[i][scell.y];
         {const std::array<ls_dim_t,2> duplvinds = find_first_duplication(col);
          assert(duplvinds.size() == 2);
          opposrow = duplvinds[bernoulli(g)];
         }
        }
        // Modify values of the formed subsquare:
        assert(L[scell.x][opposcol] == L[opposrow][scell.y]);
        modcellnewv = L[scell.x][opposcol];
        opposcellv = L[opposrow][opposcol];
        L[scell.x][scell.y] = firstimpposv;
        L[scell.x][opposcol] = modcelloldv;
        L[opposrow][scell.y] = modcelloldv;
        L[opposrow][opposcol] = modcellnewv;
      }
      assert(valid_basic(L));

      if (LatinSquares::valid(L))
        {scell.active = false; ++properlsnum;}
      else
        scell = {opposrow,opposcol, opposcellv,modcellnewv,modcelloldv, true};

      ++pertrnum;
    }

    friend std::ostream& operator <<(std::ostream& out, const LSRandGen_t& lsg) {
      out << "c RESULT: \n";
      out << "c " << lsg.pertrnum << " total moves\n";
      out << "c " << lsg.pertrnum - lsg.additpertrnum << " main moves\n";
      out << "c " << lsg.additpertrnum << " additional moves\n";
      out << "c " << lsg.properlsnum << " proper Latin squares\n";
      out << "c " << lsg.pertrnum - lsg.properlsnum << " improper Latin squares\n";
      out << "c final proper random Latin square:\n";
      return out << lsg.L;
    }
  };
}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  Environment::Index index;

  std::size_t converted;
  const std::uint16_t N = argc <= index ? N_default : std::stoul(argv[index++], &converted);
  const std::uint64_t m = argc <= index ? m_default : std::stoul(argv[index++], &converted);

  if (index == argc) {
    std::cerr << error << "Seeds must be specified" << "\n";
    return int(Error::domain);
  }

  vec_eseed_t s;
  add_seeds(argv[index++], s);

  std::ofstream out;
  std::string filename;
  if (index == argc) filename = "cout";
  else filename = argv[index];
  if (filename.empty()) filename = "cout";
  assert(not filename.empty());
  const bool output_message = filename[0] != '-';
  if (not output_message) filename.erase(0,1);
  if (filename.empty()) filename = "cout";
  assert(not filename.empty());
  if (filename == "cout") out.basic_ios<char>::rdbuf(std::cout.rdbuf());
  else {
    out.open(filename);
    if (output_message)
      std::cout << "Output to file \"" << filename << "\".\n";
  }
  if (not out) {
    std::cerr << error << "Can't open file \"" << filename << "\"\n";
    return int(Error::file_open);
  }
  index++;

  index.deactivate();

  out << Environment::Wrap(proginfo, Environment::OP::dimacs);
  using Environment::DHW;
  using Environment::DWW;
  using Environment::qu;
  out << DHW{"Parameters"}
            << DWW{"command-line"};
  Environment::args_output(out, argc, argv);
  out << "\n"
            << DWW{"N"} << N << "\n"
            << DWW{"m"} << m << "\n"
            << DWW{"output"} << qu(filename) << "\n"
            << DWW{"num_e-seeds"} << s.size() << "\n"
            << DWW{" e-seeds"};
  assert(not s.empty());
  out << s[0];
  for (vec_eseed_t::size_type i = 1; i < s.size(); ++i)
    out << " " << s[i];
  out << "\n";

  LSRandGen_t lsg(N,s);
  for (std::uint64_t i=0; i<m; ++i) {
    lsg.find_random_ls();
    out << lsg;
    if (i+1 != m) out << "\n";
  }
}

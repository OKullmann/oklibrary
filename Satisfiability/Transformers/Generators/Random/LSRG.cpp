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


TODOS:

-2. Write tests.

-1. Correct construction of LSRandGen_t
     - The order of data-members seems random.
     - public vs private looks strange.
     - In general public vs private needs re-organisation.

0. Complete handling of seeding
   (a) There needs to be a fixed initial part of the seed-sequence,
       specifying the organisation and the software.
     - The role-model is ClauseSets::seeds(par), and is described in
       docus/BRG.txt, Part III.
     - First the main type (ClauseSets::MainType), which describes which
       type of formula is created.
     - Perhaps we have a higher "species type", like formula=0, hypergraph=1,
       graph=2, combinatorial_design=3.
     - In order to allow other groups to create such designs, without clash,
       there could be a highest organisation-id.
     - Afther the main type follows the sub-type, which in ClauseSets encodes
       the main structural parameters (renaming and ordering).
     - It seems that between main- and sub-type there should be a
       "middle-type", for the general parameter-handling (so that it becomes
       easier to add parameters).
     - The third component of the initial part in ClauseSets is the number
       of clause-blocks; this indeed should be counted as part of the
       specific parameter-encoding.
     - Finally in ClauseSets we have the thread-index (a hypothetical parameter
       for hypothetical parallel computation); this likely should also be
       counted as par to the parameter-encoding. Perhaps as first part of the
       parameter-encoding.
   (b) Followed by the parameters of the creation; this is at least N.
   (c) Followed by the user-specified seeds.
   (d) Concerning parameter m: it seems best to discard it -- the
       initialisation of the random engine is likely negligible (timewise)
       compared to the other computations.
       On the other hand, it could be made part of the parameter-section, after
       N. Then the output is not a single latin square, but m latin squares,
       used for examples for substitution into MOLS. Then the anticipated
       use-case would not be for experimenting with long sequences, but
       the normal case would be small m, like 1,2,3.
       However then it needed to be guaranteed that exactly m latin squares
       are produced, not more, not less.

1. Reflect on usage of special 16/32-bit types
   - One also has to be careful about "pow" (which means many things,
     and likely one doesn't know what it means).

2. Simplify names
   - Names are often too long, without actually telling "a story".
   - Likely "improper" isn't a good name; perhaps "special".
     So using "SpecialCell".
   - The suffix "i" in names is likely often superfluous.

3. Better semantics and syntax for "improper cells"
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

4. Complete documentation
   - Describe all steps of the algorithm in docus/LSRG.txt.
   - Describe the used data structure.

5. Improve function perturbate_square
   - It is too long -- the should be abstract operations used.

6. Test randomness
   - At least check all single cells for randomness.
   - And compute for small N all L(N) latin squares, and check whether the
     sequence produced represents a random number from 1,...,L(N).

7. Improve interface
   - DONE The seeding should happen with the construction of the
     ls-generator-object (not independently of it).
   - Different from clause-set-generation, here the generator likely is most
     often used internally, not via file-output.

8. Check improper rows and columns
  - After each perturbation check whether values' sum of both improper row and
    column is (N+1)/2.

9. Do not search for duplicates
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
        "0.3.0",
        "26.12.2020",
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

  // Semantics needed: XXX
  struct ImproperCell {
    ls_dim_t rowi;
    ls_dim_t coli;
    std::array<ls_dim_t, 2> positv;
    ls_dim_t negatv;
  };

  class LSRandGen_t {
  private:
    bool proper;
    ImproperCell impcell;
    RandGen_t g;
  public:
    const ls_dim_t N;
    ls_t L;
    // Semantics needed: XXX
    std::uint64_t pertrnum;
    std::uint64_t additpertrnum;
    std::uint64_t properlsnum;

    LSRandGen_t(const ls_dim_t& N, const vec_eseed_t& s) :
      proper(true), impcell{0,0,0,{}}, g(transform(s, SP::split)),
      N(N), L(cyclic_ls(N)) {}

    ls_dim_t imp_val_sum() {
      return impcell.positv[0] + impcell.positv[1] - impcell.negatv;
    };

    // Find a random LS of order N:
    void find_random_ls() noexcept {
      pertrnum=0; additpertrnum=0; properlsnum=0;
      const auto bound = gen_uint_t(N) * N * N;
      for (std::uint32_t i = 0; i < bound; ++i) perturbate_square();
      while (not valid(L)) {perturbate_square(); ++additpertrnum;}
      assert(valid(L));
    }

    // Perturbate current square:
    void perturbate_square() noexcept {
      ls_dim_t modrowi, modcoli, modcellnewv, modcelloldv,
        opposrowi, opposcoli, opposcellv;

      if (proper) {
        // Randomly choose cell and its new value:
        {UniformRange U(g, N);
         modrowi = U();
         modcoli = U();
         modcelloldv = L[modrowi][modcoli];
         modcellnewv = UniformRange(g,N-1)();
         if (modcellnewv >= modcelloldv) ++modcellnewv;
        }
        // Find a 2 times 2 subsquare for modification:
        ls_row_t row = L[modrowi];
        {const auto it = std::find(row.begin(), row.end(), modcellnewv);
         assert(it != row.end());
         opposcoli = std::distance(row.begin(), it);
        }
        ls_row_t col(N);
        for (unsigned i = 0; i < N; ++i) col[i] = L[i][modcoli];
        {const auto it = std::find(col.begin(), col.end(), modcellnewv);
         assert(it != col.end());
         opposrowi = std::distance(col.begin(), it);
        }
        opposcellv = L[opposrowi][opposcoli];
        // Update the found 2 times 2 subsquare:
        L[modrowi][modcoli] = modcellnewv;
        L[modrowi][opposcoli] = modcelloldv;
        L[opposrowi][modcoli] = modcelloldv;
        L[opposrowi][opposcoli] = modcellnewv;
      }
      else {
        // Modify the improper cell:
        modrowi = impcell.rowi;
        modcoli = impcell.coli;
        // Randomly choose a positive value (one of two) in the improper cell:
        const ls_dim_t posvi = (bernoulli(g)) ? 0 : 1;
        assert(posvi < impcell.positv.size());
        const ls_dim_t firstimpposv = impcell.positv[posvi];
        modcelloldv = (posvi == 0) ? impcell.positv[1] : impcell.positv[0];
        // Randomly choose one of two duplicate indexes in the improper row:
        const ls_row_t row = L[modrowi];
        std::array<ls_dim_t, 2> duplvinds = find_first_duplication(row);
        assert(duplvinds.size() == 2);
        ls_dim_t duplvi = (bernoulli(g)) ? 0 : 1;
        opposcoli = duplvinds[duplvi];
        // Randomly choose one of two duplicate indexes in the improper column:
        ls_row_t col(N);
        for (unsigned i = 0; i < N; ++i) col[i] = L[i][modcoli];
        duplvinds = find_first_duplication(col);
        assert(duplvinds.size() == 2);
        duplvi = bernoulli(g) ? 0 : 1;
        opposrowi = duplvinds[duplvi];
        // Modify values of the formed subsquare:
        assert(L[modrowi][opposcoli] == L[opposrowi][modcoli]);
        modcellnewv = L[modrowi][opposcoli];
        opposcellv = L[opposrowi][opposcoli];
        L[modrowi][modcoli] = firstimpposv;
        L[modrowi][opposcoli] = modcelloldv;
        L[opposrowi][modcoli] = modcelloldv;
        L[opposrowi][opposcoli] = modcellnewv;
      }
      assert(valid_basic(L));

      proper = valid(L) ? true : false;
      if (proper) ++properlsnum;
      else {
        // Update the improper cell with 3 values:
        impcell.rowi = opposrowi;
        impcell.coli = opposcoli;
        impcell.negatv = modcelloldv;
        impcell.positv = { opposcellv, modcellnewv };
      }

      ++pertrnum;
    }

    friend std::ostream& operator <<(std::ostream& out, const LSRandGen_t& lsg) {
      out << "c RESULT: " << lsg.pertrnum << " iterations\n";
      out << "c " << lsg.additpertrnum << " of them additional iterations\n";
      out << "c " << lsg.properlsnum << " of them produced proper Latin squares\n";
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
  if (index == argc or std::string_view(argv[index]) == "-cout") {
    out.basic_ios<char>::rdbuf(std::cout.rdbuf());
    filename = "-cout";
  }
  else {
    filename = argv[index];
    if (filename.empty()) filename = "-cout";
    out.open(filename);
    if (not out) {
      std::cerr << error << "Can't open file \"" << filename << "\"\n";
      return int(Error::file_open);
    }
    std::cout << "Output to file \"" << filename << "\".\n";
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

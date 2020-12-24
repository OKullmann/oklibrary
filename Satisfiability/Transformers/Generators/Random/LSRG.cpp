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

1. Complete documentation
   - Describe all steps of the algorithm in docus/LSRG.txt.
   - Describe the used data structure.
2. Check improper rows and columns
  - After each perturbation check whether values' sum of both improper row and
    column is (N+1)/2.
3. Do not search for duplicates
  - For an improper square use saved previous state instaed of searching for
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
        "0.2.0",
        "24.12.2020",
        __FILE__,
        "Oliver Kullmann and Oleg Zaikin",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/LSRG.cpp",
        "GPL v3"};

  using namespace RandGen;
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
    ls_dim_t N;
    ls_t L;
    std::uint64_t pertrnum;
    std::uint64_t additpertrnum;
    std::uint64_t properlsnum;

    LSRandGen_t(const ls_dim_t& N_) :
      proper(true),
      pertrnum(0),
      additpertrnum(0),
      properlsnum(0) {
      N = N_;
      // Construct an initial Latin square:
      L = triv_mult_table();
      impcell.rowi = 0;
      impcell.coli = 0;
      impcell.negatv = 0;
      impcell.positv = {};
    }

    // Summation of three values in the improper cell:
    ls_dim_t imp_val_sum() {
      ls_dim_t res = 0;
      for (auto v : impcell.positv)
        res += v;
      assert(res >= impcell.negatv);
      res -= impcell.negatv;
      return res;
    };

    // Form an initial Latin square as a multiplication table of a quasigroup:
    ls_t triv_mult_table() {
      ls_t L = ls_t(N, ls_row_t(N));
      for (ls_dim_t rowi = 0; rowi < N; ++rowi)
        for (ls_dim_t coli = 0; coli < N; ++coli)
          L[rowi][coli] = (rowi + coli) % N;
      assert(valid(L));
      return L;
    }

    // Find a random LS of order N:
    void find_random_ls(RandGen_t &g) {
      for (std::uint32_t iter = 0; iter < pow(N, 3); ++iter)
        perturbate_square(g);

      while (not valid(L)) {
        perturbate_square(g);
        ++additpertrnum;
      }

      assert(valid(L));
    }

    // Perturbate current square:
    void perturbate_square(RandGen_t &g) {
      UniformRange U(g, N, 0);

      ls_dim_t modrowi;
      ls_dim_t modcoli;
      ls_dim_t modcellnewv;
      ls_dim_t modcelloldv;
      ls_dim_t opposrowi;
      ls_dim_t opposcoli;
      ls_dim_t opposcellv;

      if (proper) {
        // Randomly choose cell and its new value:
        modrowi = U();
        modcoli = U();
        modcelloldv = L[modrowi][modcoli];
        do {
          modcellnewv = U();
        } while (modcellnewv == modcelloldv);
        // Find a 2 times 2 subsquare for modification:
        ls_row_t row = L[modrowi];
        ls_row_t::iterator it = std::find(row.begin(), row.end(), modcellnewv);
        assert(it != row.end());
        opposcoli = std::distance(row.begin(), it);
        ls_row_t col(N);
        for (unsigned i = 0; i < N; ++i)
          col[i] = L[i][modcoli];
        it = std::find(col.begin(), col.end(), modcellnewv);
        assert(it != col.end());
        opposrowi = std::distance(col.begin(), it);
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
        ls_dim_t posvi = (bernoulli(g)) ? 0 : 1;
        assert(posvi < impcell.positv.size());
        ls_dim_t firstimpposv = impcell.positv[posvi];
        modcelloldv = (posvi == 0) ? impcell.positv[1] : impcell.positv[0];
        // Randomly choose one of two duplicate indexes in the improper row:
        ls_row_t row = L[modrowi];
        std::array<ls_dim_t, 2> duplvinds = get_duplicates_indexes(row);
        assert(duplvinds.size() == 2);
        ls_dim_t duplvi = (bernoulli(g)) ? 0 : 1;
        opposcoli = duplvinds[duplvi];
        // Randomly choose one of two duplicate indexes in the improper column:
        ls_row_t col(N);
        for (unsigned i = 0; i < N; ++i)
          col[i] = L[i][modcoli];
        duplvinds = get_duplicates_indexes(col);
        assert(duplvinds.size() == 2);
        duplvi = (bernoulli(g)) ? 0 : 1;
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
      if (proper) {
        ++properlsnum;
      }
      else {
        // Update the improper cell with 3 values:
        impcell.rowi = opposrowi;
        impcell.coli = opposcoli;
        impcell.negatv = modcelloldv;
        impcell.positv = { opposcellv, modcellnewv };
      }

      ++pertrnum;
    }

    std::array<ls_dim_t, 2> get_duplicates_indexes(const ls_row_t& l) {
      ls_row_t sortedl = l;
      std::sort(sortedl.begin(), sortedl.end());
      ls_row_t::iterator it = std::adjacent_find(sortedl.begin(), sortedl.end());
      assert(it != sortedl.end());
      ls_dim_t duplv = *it;
      std::array<unsigned, 2> duplvinds = {};
      unsigned duplarri = 0;
      for (unsigned i = 0; i < N; ++i) {
        if (l[i] == duplv) {
          assert(duplarri < 2);
          duplvinds[duplarri++] = i;
        }
      }
      assert(duplvinds.size() == 2);
      return duplvinds;
    }

    friend std::ostream& operator <<(std::ostream& out, const LSRandGen_t& lsg) {
      for (ls_dim_t i = 0; i < lsg.N; ++i) {
        for (ls_dim_t j = 0; j < lsg.N; ++j)
          out << lsg.L[i][j] << " ";
        out << "\n";
      }
      out << lsg.pertrnum << " iterations\n";
      out << lsg.additpertrnum << " of them are additional iterations after main loop\n";
      out << lsg.properlsnum << " iterations produced proper Latin squares\n";

      return out;
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

  RandGen_t g(transform(s, SP::split));
  for (std::uint64_t i=0; i<m; ++i) {
    LSRandGen_t lsg(N);
    lsg.find_random_ls(g);
    out << lsg;
  }
}

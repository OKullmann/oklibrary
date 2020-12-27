// Oliver Kullmann, 30.6.2019 (Swansea)
/* Copyright 2019, 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Random clause-sets

USAGE:

> ./QBRG -v | --version

for information on the program, the version, and the environment.

> ./QBRG -h | --help

for basic help-information.

> ./QBRG [quantifiers] [clauses] [options] [seeds] [output]

for creation of random QCNFS.

For the complete documentation, see
  docus/QBRG.txt


BUGS:

TODOS:

0. Implement the new order of the literals of clauses, and of the clauses
   of clause-sets, using joint components with Quantification/DQCNF.

1 For building the program one has to allow also building outside of
the context of the OKlibrary. Then the Git-id is just hardcoded.

2 Somehow "CCI" (controlled CI) should be implemented.

   The general form is that it yields a clause-block: one quantifier-block i
   is specified, say it contains m_i variables, and then the clause-block
   has 2m_i clauses, with exactly one of the 2m_i literals in each clause
   (in the canonical literal-order).

   Further clause-parts are possible, as usual.

   This could be done by just generalising "m * ..." (m clauses) to
     (i) * ...

   More generally, one could allow "(i,j,...)", a sequence of different
   quantifier-block indices, and then we have 2m_i * 2m_j * ... many
   clauses, all combinations of a single literal from blocks i, j, ... .

*/

#include <iostream>
#include <fstream>
#include <string>

#include <ProgramOptions/Environment.hpp>

#include "Numbers.hpp"
#include "QClauseSets.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.2",
        "27.12.2020",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/QBRG.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  using namespace RandGen;
  using namespace SeedOrganisation;

  std::string default_filename(const Param& par, const vec_eseed_t& s) {
    return default_filename(Logic::block_uniform_qcnf, extract_parameters(par.vp), s);
  }

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " [quantifiers] [clauses] [options] [seeds] [output]\n\n"
    "   quantifiers: \"b1 ... bt\", with t >= 1 quantifier-blocks bi\n"
    "     bi         \"eK\" or \"aK\" for K >= 1, except for bt, which is just K\n"
    "                (automatically existential, i.e., \"e\")\n"
    "   clauses : \"B1; ...; Bs\", with s >= 0 clause-blocks Bi\n"
    "     Bi    : \"C * P1 | ... | Pm\", with m >= 1 clause-parts Pi and C=#clauses\n"
    "     Pi    : \"N, W [, P]\", where\n"
    "              N=quantifier-block-range, W=#literals, P=#sign-probability\n"
    "   options : \"c1, ..., cl\", with l >= 0 option-choices ci from\n"
    "               " << Environment::WRP<option_t>{} << ";\n"
    "             defaults are the first values for both options\n"
    "   seeds   : \"s1, ..., sp\", with p >= 0 seed-values si, which are\n"
    "             unsigned 64-bit integers, \"r\" (for \"random\"), or \"t\" (for \"timestamp\")\n"
    "   output  : \"-cout\" or \"[-]\" (default-filename) or [-]FILENAME\n\n"
    " computes the random QCNF:\n\n"
    "  - The arguments are positional, not named (the names are used here only"
    " for communication).\n"
    "  - Trailing arguments can be left out, then using their default-values.\n"
    "  - Spaces are optional except for quantifiers (if present otherwise, quotation is needed for arguments).\n"
    "  - Arguments \"\" (the empty string) yield also the default-values,\n"
    "    except for the output, where it yields the default output-filename.\n"
    "  - The optional \"-\" for the output means \"don't print filename\" (which otherwise is done).\n"
;
    return true;
  }

  constexpr int index_quantifier = 1;
  constexpr int index_clauses = 2;
  constexpr int index_options = 3;
  constexpr int index_seeds = 4;
  constexpr int index_output = 5;

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

try {

  Environment::Index index;

  const block_v vblock = argc <= index ? read_block_v("1") : read_block_v(argv[index++]);
  assert(vblock.size() >= 2);
  const gen_uint_t num_blocks = vblock.size() - 1;

  const rparam_v vpar = (argc <= index) ? rparam_v{} : read_rparam_v(argv[index++]);
  {const auto dimacs_pars_0 = extract_parameters(vpar);
   if (dimacs_pars_0.n > num_blocks) {
     std::cerr << error << "A quantifier-block-index greater than " << num_blocks << " was used.\n";
     return int(QError::qblock_index);
   }
  }
  auto v_interpreted = interprete(vpar, vblock);
  if (not valid(v_interpreted)) {
    assert(index_clauses < argc);
    std::cerr << error << "Logically invalid clauses-parameter \"" << argv[index_clauses] << "\"\n";
    return int(Error::invalid_clauses);
  }

  const GParam gpar = (argc <= index) ? GParam{} : GParam{Environment::translate<option_t>()(argv[index++], sep)};
  if (gpar == GParam{}) {
    for (const auto& b : vpar) {
      if (b.c == 0) {
        std::cerr << error << "For the default-options an empty clause-block "
          "is not allowed, but clause-block \"" << b << "\" is empty.\n";
        return int(QError::empty_clause_block);
      }
      gen_uint_t k = 0;
      for (const auto& p : b.cps) k += p.k;
      if (k == 0) {
        std::cerr << error << "For the default-options empty clauses are not "
          "allowed, but clause-block \"" << b << "\" yields it.\n";
        return int(QError::empty_clause);
      }
    }
  }
  const Param par(gpar, std::move(v_interpreted));
  v_interpreted.clear(); // now in par.vp

  vec_eseed_t s = seeds({gpar,vpar}, vblock);
  typedef vec_eseed_t::size_type evec_size_t;
  const evec_size_t esize_system = s.size();
  const evec_size_t esize_add = argc > 4 ? add_seeds(argv[index++], s) : 0;

  std::ofstream out;
  std::string filename;
  if (index == argc or std::string_view(argv[index]) == "-cout") {
    out.basic_ios<char>::rdbuf(std::cout.rdbuf());
    filename = "-cout";
  }
  else {
    filename = argv[index];
    if (filename.empty()) filename = default_filename(par, s);
    assert(not filename.empty());
    const bool output_message = filename[0] != '-';
    if (not output_message) filename.erase(0,1);
    if (filename.empty()) filename = default_filename(par, s);
    assert(not filename.empty());
    out.open(filename);
    if (not out) {
      std::cerr << error << "Can't open file \"" << filename << "\"\n";
      return int(Error::file_open);
    }
    if (output_message)
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
      << DWW{"output"} << qu(filename) << "\n"
      << DWW{"options"} << gpar << "\n"
      << DWW{"num_quantifier_blocks"} << vblock.size() - 1 << "\n"
      << DWW{" quantifier_blocks"};
  output_qblocks(out, vblock);
  out << "\n"
      << DWW{"num_clause-blocks"} << par.vp.size() << "\n"
      << DWW{" clause-blocks"} << par.vp << "\n"
      << DWW{"num_e-seeds"} << esize_system << "+" << esize_add << "=" << s.size() << "\n"
      << DWW{" e-seeds"};
  assert(not s.empty());
  out << s[0];
  for (vec_eseed_t::size_type i = 1; i < s.size(); ++i)
    out << " " << s[i];
  out << "\n";

  RandGen_t g(transform(s, SP::split));

  if (gpar == GParam(-1)) rand_clauselist(out, g, par.vp, vblock);
  else {
    const auto R = gpar == GParam{} ?
      rand_qclauseset(g, par.vp, vblock) :
      random(g,par); // note that random(g,par) does not know about quantifiers
    if (gpar.r() != RenameO::original)
      out << R.first.first;
    else
      out << dimacs_pars{vblock[0].v.b(), R.first.first.c};
    if (R.first.first.n != 0)
      output_qblocks(out, vblock, R.second);
    out << R.first.second;
  }

}
catch(const std::domain_error& e) {
    std::cerr << error << "Parameters\n";
    std::cerr << "  " << e.what() << "\n";
    return int(Error::domain);
  }
catch(const std::invalid_argument& e) {
    std::cerr << error << "Parameters: could not perform numeric conversion\n";
    std::cerr << "  " << e.what() << "\n";
    return int(Error::invalid);
  }
catch(const std::out_of_range& e) {
    std::cerr << error << "Parameters: outside the range of 64-bit unsigned integer\n";
    std::cerr << "  " << e.what() << "\n";
    return int(Error::out_of_range);
  }
catch(const std::bad_alloc& e) {
    std::cerr << error << "Bad allocation\n";
    std::cerr << "  " << e.what() << "\n";
    return int(Error::alloc);
  }
catch(const std::exception& e) {
    std::cerr << error << "Unexpected exception\n";
    std::cerr << "  " << e.what() << "\n";
    return int(Error::except);
  }

}

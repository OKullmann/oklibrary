// Oliver Kullmann, 30.6.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
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


TODOS:

0 Use proper error-codes for the return-values.

1 Catch all errors, and give proper error-messages.

2 For building the program one has to allow also building outside of
the context of the OKlibrary. Then the Git-id is just hardcoded.

*/

#include <iostream>
#include <fstream>
#include <string>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "QClauseSets.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.1",
        "13.7.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/QBRG.cpp",
        "GPL v3"};

  using namespace RandGen;

  std::string default_filename(const Param& par, const vec_eseed_t& s) {
    return default_filename(MainType::block_uniform_qcnf, extract_parameters(par.vp), s);
  }

  bool show_usage(const int argc, const char* const argv[]) {
    assert(argc >= 1);
    if (argc != 2 or not Environment::is_help_string(argv[1])) return false;
    const std::string& program = proginfo.prg;
    std::cout << "USAGE:\n"
    "> " << program << " [-v | --version]\n"
    " shows version information and exits.\n"
    "> " << program << " [-h | --help]\n"
    " shows help information and exits.\n"
    "> " << program << " [quantifier] [clauses] [options] [seeds] [output]\n"
    " computes the random QCNF.\n"
    " Trailing arguments can be left out, using their default-values.\n"
    " The default-values are also activated by using \"\" for the argument,\n"
    "  except in case of output, where the default-value is activated by \"-cout\",\n"
    "  while \"\" means here the default output-filename.\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;
  if (argc == 1) return 1;

  Environment::Index index;

  const block_v vblock = read_block_v(argv[index++]);
  assert(vblock.size() >= 2);
  const gen_uint_t num_blocks = vblock.size() - 1;

  const rparam_v vpar = (argc <= index) ? rparam_v{} : read_rparam_v(argv[index++]);
  const auto dimacs_pars_0 = extract_parameters(vpar);
  if (dimacs_pars_0.first > num_blocks) return 1;

  rparam_v tvpar = interprete(vpar, vblock); // not to be used later
  if (not valid(tvpar)) {
    std::cerr << "ERROR: logically invalid clause-parameter \"" << argv[index-1] << "\"\n";
    return 1;
  }
  const GParam gpar = (argc <= index) ? GParam{} : GParam{Environment::translate<option_t>()(argv[index++], sep)};
  if (gpar == GParam{}) {
    for (const auto& b : vpar) {
      if (b.c == 0) {
        std::cerr << "ERROR: for the default-options an empty clause-block "
          "is not allowed, but clause-block \"" << b << "\" is empty.\n";
        return 1;
      }
      gen_uint_t k = 0;
      for (const auto& p : b.cps) k += p.k;
      if (k == 0) {
        std::cerr << "ERROR: for the default-options empty clauses are not "
          "allowed, but clause-block \"" << b << "\" yields it.\n";
        return 1;
      }
    }
  }
  const Param par{gpar, std::move(tvpar)};

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
    out.open(filename);
    if (not out) {
      std::cerr << "ERROR: can't open file \"" << filename << "\"\n";
      return 1;
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
  out << qu(argv[0]);
  for (int i = 1; i < argc; ++i) out << " " << qu(argv[i]);
  out << "\n"
            << DWW{"output"} << qu(filename) << "\n"
            << DWW{"options"} << gpar << "\n"
            << DWW{"num_quantifier_blocks"} << vblock.size() - 1 << "\n"
            << DWW{" quantifier_blocks"} << "1:" << vblock[1];
  for (block_v::size_type i = 2; i < vblock.size(); ++i)
    out << " " << i << ":" << vblock[i];
  out       << "\n"
            << DWW{"num_clause_blocks"} << par.vp.size() << "\n"
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
  else if (gpar != GParam{}) {
    const auto R = random(g,par);
    output_core(out, vblock, R.second);
    out << R.first;
  }
  else {
    const auto R = rand_qclauseset(g,par.vp, vblock);
    output_core(out, vblock, R.second);
    out << R.first;
  }
}

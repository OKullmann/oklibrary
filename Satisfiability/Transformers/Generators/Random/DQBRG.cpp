// Oliver Kullmann, 17.7.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Random clause-sets

USAGE:

> ./DQBRG -v | --version

for information on the program, the version, and the environment.

> ./DQBRG -h | --help

for basic help-information.

> ./DQBRG [quantifiers] [dependencies] [clauses] [options] [seeds] [output]

for creation of random DQCNFS.

For the complete documentation, see
  docus/DQBRG.txt

*/

#include <iostream>
#include <fstream>
#include <string>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "DQClauseSets.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.4",
        "26.8.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/DQBRG.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  using namespace RandGen;

  bool show_usage(const int argc, const char* const argv[]) {
    assert(argc >= 1);
    if (argc != 2 or not Environment::is_help_string(argv[1])) return false;
    const std::string& program = proginfo.prg;
    std::cout << "USAGE:\n"
    "> " << program << " [-v | --version]\n"
    " shows version information and exits.\n"
    "> " << program << " [-h | --help]\n"
    " shows help information and exits.\n"
    "> " << program << " [quantifiers] [dependencies] [clauses] [options] [seeds] [output]\n"
    " computes the random DQCNF.\n"
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

try {

  Environment::Index index;

  const block_v vblock = argc <= index ? read_block_v("1") : read_block_v(argv[index++]);
  assert(vblock.size() >= 2);
  const gen_uint_t num_blocks = vblock.size() - 1;
  const auto [na, ne] = extract_numvars(vblock);
  if (not valid(ae_numvars{na,ne})) {
    std::cerr << error << "The product of na=" << na << " and ne=" << ne
      << " is too large for (unsigned) 64-bit.\n";
    return int(DQError::nane_prod);
  }
  const gen_uint_t total_deps = na * ne;
  const gen_uint_t deps = num_dependencies(vblock);

  const dep_par_t deppar = argc <= index ? dep_par_t{} : read_dep_par(argv[index++]);
  switch (deppar.second) {
  case DepOp::from_scratch :
    if (deppar.first > total_deps) {
      std::cerr << error << "Dependencies required exceed possible dependencies:\n"
        "  " << deppar.first << " > na*ne = " << na << "*" << ne << " = "
        << total_deps << "\n";
      return int(DQError::too_many_deps);
    } else break;
  case DepOp::subtract :
    if (deppar.first > deps) {
      std::cerr << error << "Not enough dependencies to subtract from:\n"
        "  " << deppar.first << " > " << deps << "\n";
      return int(DQError::too_few_deps);
    } else break;
  default :
    const gen_uint_t sum = deps + deppar.first;
    if (sum < deps or sum < deppar.first) {
      std::cerr << error << "Addition overflow with dependency-addition:\n"
        " " << deppar.first << " + " << deps << " > " << randgen_max << "\n";
      return int(DQError::overflow);
    }
    if (sum > total_deps) {
      std::cerr << error << "Additional dependencies exceed possible dependencies:\n"
        << deppar.first << " + " << deps << " = " << sum << " > na*ne = "
        << na << "*" << ne << " = " << total_deps << "\n";
      return int(DQError::too_much_added);
    }
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

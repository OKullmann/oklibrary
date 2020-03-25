// Oliver Kullmann, 22.6.2019 (Swansea)
/* Copyright 2019, 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Random clause-sets

USAGE:

> ./BRG -v | --version

for information on the program, the version, and the environment.

> ./BRG -h | --help

for basic help-information.

> ./BRG [clauses] [options] [seeds] [output]

for creation of random CNFS.

For the complete documentation, see
  docus/BRG.txt


TODOS:

1 For building the program one has to allow also building outside of
the context of the OKlibrary. Then the Git-id is just hardcoded.

*/

#include <iostream>
#include <fstream>
#include <string>

#include <ProgramOptions/Environment.hpp>

#include "ClauseSets.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.2",
        "25.3.2020",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/BRG.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  using namespace RandGen;

  std::string default_filename(const Param& par, const vec_eseed_t& s) {
    return default_filename(MainType::block_uniform_cnf, extract_parameters(par.vp), s);
  }

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg << " [clauses] [options] [seeds] [output]\n\n"
    "   clauses : \"B1; ...; Bs\", with s >= 0 clause-blocks Bi\n"
    "     Bi    : \"C * P1 | ... | Pm\", with m >= 1 clause-parts Pi and C=#clauses\n"
    "     Pi    : \"N, W [, P]\", where\n"
    "              N=variable-range, W=#literals, P=#sign-probability\n"
    "   options : \"c1, ..., cl\", with l >= 0 option-choices ci from\n"
    "               " << Environment::WRP<option_t>{} << ";\n"
    "             defaults are the first values for both options\n"
    "   seeds   : \"s1, ..., sp\", with p >= 0 seed-values si, which are\n"
    "             unsigned 64-bit integers, \"r\" (for \"random\"), or \"t\" (for \"timestamp\")\n"
    "   output  : \"-cout\" or \"\" (default filename) or FILENAME\n\n"
    " computes the random CNF:\n\n"
    "  - The arguments are positional, not named (the names are used here only"
    " for communication).\n"
    "  - Trailing arguments can be left out, then using their default-values.\n"
    "  - Spaces are optional (if present, quotation is needed for arguments).\n"
    "  - Arguments \"\" (the empty string) yield also the default-values,\n"
    "    except for the output, where it yields the default output-filename.\n"
;
    return true;
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

try {

  Environment::Index index;

  rparam_v vpar = (argc <= index) ? rparam_v{} : read_rparam_v(argv[index++]);
  if (not valid(vpar)) {
    std::cerr << error << "Logically invalid clauses-parameter \"" << argv[index-1] << "\"\n";
    return int(Error::invalid_clauses);
  }

  const GParam gpar = (argc <= index) ? GParam{} : GParam{Environment::translate<option_t>()(argv[index++], sep)};
  const Param par{gpar, std::move(vpar)};

  vec_eseed_t s = seeds(par);
  typedef vec_eseed_t::size_type evec_size_t;
  const evec_size_t esize_system = s.size();
  const evec_size_t esize_add = argc > 3 ? add_seeds(argv[index++], s) : 0;

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
            << DWW{"output"} << qu(filename) << "\n"
            << DWW{"options"} << gpar << "\n"
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

  if (gpar == GParam(-1)) rand_clauselist(out, g, par.vp);
  else out << random(g,par).first;

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

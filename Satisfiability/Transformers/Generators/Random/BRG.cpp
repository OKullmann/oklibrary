// Oliver Kullmann, 22.6.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <fstream>
#include <string>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "ClauseSets.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.1",
        "24.6.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/BRG.cpp",
        "GPL v3"};

  using namespace RandGen;

  const std::string filestem = "BlRaGe_";
  const std::string filesuffix = ".dimacs";
  std::string default_filename(const Param& par, const vec_eseed_t& s) {
    const auto dpars = extract_parameters(par.vp);
    gen_uint_t sum = 0;
    for (const gen_uint_t x : s) sum += x;
    using std::to_string;
    return filestem + to_string(dpars.first) + "_" +
      to_string(dpars.second) + "_" + to_string(sum) + filesuffix;
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
    "> " << program << " [clauses] [options] [seeds] [output]\n"
    " computes the random CNF.\n"
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

  Environment::Index index;

  rparam_v vpar = (argc <= index) ? rparam_v{} : read_rparam_v(argv[index++]);
  if (not valid(vpar)) return 1;

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
    if (not out) return 1;
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

  if (gpar == GParam(-1)) rand_clauselist(out, g, par.vp);
  else out << random(g,par);
}

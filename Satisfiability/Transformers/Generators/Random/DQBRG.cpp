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

#include "Algorithms.hpp"

#include "DQClauseSets.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.9",
        "27.10.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/DQBRG.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  using namespace RandGen;

  std::string default_filename(const Param& par, const vec_eseed_t& s) {
    return default_filename(MainType::block_uniform_dqcnf, extract_parameters(par.vp), s);
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
    "> " << program << " [quantifiers] [dependencies] [clauses] [options] [seeds] [output]\n"
    " computes the random DQCNF.\n"
    " Trailing arguments can be left out, using their default-values.\n"
    " The default-values are also activated by using \"\" for the argument,\n"
    "  except in case of output, where the default-value is activated by \"-cout\",\n"
    "  while \"\" means here the default output-filename.\n"
    "  If the leading character of the filename (possibly empty) is \"-\",\n"
    "  then no message about the filename is shown.\n"
;
    return true;
  }

  constexpr int index_quantifier = 1;
  constexpr int index_dependencies = 2;
  constexpr int index_clauses = 3;
  constexpr int index_options = 4;
  constexpr int index_seeds = 5;
  constexpr int index_output = 6;

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
  case DepOp::add : {
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
    else break;
  }
  default :
    std::cerr << error << "Error with form of dependency-generation.\n";
    return int(DQError::bad_deps);
  }
  const gen_uint_t act_deps =
    deppar.second==DepOp::from_scratch ? deppar.first :
      deppar.second==DepOp::subtract ? deps-deppar.first : deps+deppar.first;

  const rparam_v vpar = (argc <= index) ? rparam_v{} : read_rparam_v(argv[index++]);
  {const auto dimacs_pars_0 = extract_parameters(vpar);
   if (dimacs_pars_0.first > num_blocks) {
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
  v_interpreted.clear(); // now in var.vp

  vec_eseed_t s = seeds({gpar,vpar}, vblock, deppar);
  typedef vec_eseed_t::size_type evec_size_t;
  const evec_size_t esize_system = s.size();
  const evec_size_t esize_add = argc > 5 ? add_seeds(argv[index++], s) : 0;

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
      << DWW{" total_na"} << na << "\n"
      << DWW{" total_ne"} << ne << "\n"
      << DWW{"dependency_option"} << deppar.second << "\n"
      << DWW{"  total_possible_dependencies"} << total_deps << "\n"
      << DWW{"  given_prefix_dependencies"} << deps << "\n"
      << DWW{"  change_value"} << deppar.first << "\n"
      << DWW{"  actual_dependencies"} << act_deps << "\n"

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

  if (gpar == GParam(-1)) rand_clauselist(out, g, par.vp, vblock, na, ne, deppar);
  else {
    const auto [dep_sets, dep_vector] = create_dependencies(g, vblock, na, ne, deppar);
    assert(not dep_sets.empty());
    assert(dep_vector.size() == na+ne+1);
    assert(num_dependencies(dep_vector) == act_deps);

    // XXX Preliminary:
    const auto R = gpar == GParam{} ?
      rand_qclauseset(g, par.vp, vblock) : // rand_dqclauseset(g,par.vp,dep_vector)
      random(g,par);
    if (gpar.r() != RenameO::original)
      out << R.first.first;
    else
      out << dimacs_pars(vblock[0].v.b(), R.first.first.second);
    if (R.first.first.first != 0)
      output_dqblocks(out, dep_vector, R.second, deppar.second);
    out << R.first.second;
    out << "NOT IMPLEMENTED YET.\n";
    return -1;
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

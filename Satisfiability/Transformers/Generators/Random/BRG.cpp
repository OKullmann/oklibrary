// Oliver Kullmann, 22.6.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "ClauseSets.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.2",
        "23.6.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/BRG.cpp",
        "GPL v3"};

  using namespace RandGen;

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;

  Environment::Index index;
  const GParam gpar = (argc <= index) ? GParam{} : std::get<GParam>(Environment::translate<GParam>()(argv[index++], ','));
  const Param par{gpar, (argc <= index) ? rparam_v{} : read_rparam_v(argv[index++])};
  if (not valid(par.vp)) return 1;
  vec_eseed_t s = seeds(par);
  typedef vec_eseed_t::size_type evec_size_t;
  const evec_size_t esize_system = s.size();
  const evec_size_t esize_add = add_seeds(argv, index, argc, s);
  index.deactivate();

std::cerr << Environment::Wrap(proginfo, Environment::OP::dimacs);
using Environment::DHW;
using Environment::DWW;
std::cerr << DHW{"Parameters"}
          << DWW{"global"} << gpar << "\n"
          << DWW{"num_clause_blocks"} << par.vp.size() << "\n"
          << DWW{" clause-blocks"} << par.vp << "\n"
          << DWW{"num_e-seeds"} << esize_system << "+" << esize_add << "=" << s.size() << "\n"
          << DWW{" e-seeds"};
assert(not s.empty());
std::cerr << s[0];
for (vec_eseed_t::size_type i = 1; i < s.size(); ++i)
  std::cerr << " " << s[i];
std::cerr << "\n";

  RandGen_t g(transform(s, SP::split));

  if (gpar == GParam(0)) {
    rand_clauselist(std::cout, g, par.vp);
    return 0;
  }
  else {
    std::cout << random(g,par);
    return 0;
  }
}

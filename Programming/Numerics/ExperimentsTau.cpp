// Oliver Kullmann, 8.11.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>
#include <Transformers/Generators/Random/Sequences.hpp>
#include <Transformers/Generators/Random/Statistics.hpp>

#include "Tau.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "8.11.2020",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Numerics/ExperimentsTau.cpp",
        "GPL v3"};

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  if (argc != 5) return 1;
  using namespace RandGen;
  const gen_uint_t E = FloatingPoint::toUInt(argv[1]);
  const gen_uint_t S = FloatingPoint::toUInt(argv[2]);
  const gen_uint_t N = FloatingPoint::toUInt(argv[3]);
  const bool ge1 = FloatingPoint::touint(argv[4]);
  std::cout << "# " << E << " " << S << " " << N << " " << ge1 << "\n";
  std::cout << "x it sdit\n";

  ExpSeq seq(E,S,N,ge1);
  using size_t = ExpSeq::size_t;
  using float80 = FloatingPoint::float80;
  {auto it = seq.begin();
   for (size_t i = 0; i < seq.main_size(); ++i) {
     BasicStats<float80, float80> stats_args;
     BasicStats<gen_uint_t, float80> stats_counts;
     for (size_t j = 0; j < seq.N; ++j, ++it) {
       const float80 x = seq.translate<float80>(*it);
       stats_args += x;
       if (ge1) stats_counts += Tau::wtau_ge1_c(x).c;
       else stats_counts += Tau::wtau_le1_c(x).c;
     }
     using FloatingPoint::Wrap;
     std::cout << Wrap(stats_args.amean()) << " "
               << Wrap(stats_counts.amean()) << " "
               << Wrap(stats_counts.sd_corrected()) << "\n";
   }
  }
}

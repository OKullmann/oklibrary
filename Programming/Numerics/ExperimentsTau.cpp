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
#include "Tau_mpfr.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.8.2",
        "22.12.2020",
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
  const unsigned version = FloatingPoint::touint(argv[4]);
  std::cout << "# " << E << " " << S << " " << N << " " << version << "\n";
  std::cout << "x min max mean sd\n";

  ExpSeq seq(E,S,N,true);
  using size_t = ExpSeq::size_t;
  using float64 = FloatingPoint::float64;
  Tau_mpfr::set_defprec();
  {auto it = seq.begin();
    mpfr_t tau; mpfr_init(tau);
   for (size_t i = 0; i < seq.main_size(); ++i) {
     BasicStats<float64, float64> stats_args;
     BasicStats<gen_uint_t, float64> stats_accur;
     for (size_t j = 0; j < seq.N; ++j, ++it) {
       const float64 x = seq.translate<float64>(*it);
       stats_args += x;
       mpfr_set_ld(tau, x, Tau_mpfr::defrnd);
       Tau_mpfr::wtau(tau);
       const float64 prec_res = Tau_mpfr::to_float64(tau);
       switch (version) {
       case 0 :
         stats_accur += FloatingPoint::accuracy_64(prec_res, Tau::wtau_ge1_64(x));
         break;
       case 1 :
         stats_accur += FloatingPoint::accuracy_64(prec_res, Tau::wtau_ge1_ub_64(x));
         break;
       default :
         stats_accur += FloatingPoint::accuracy_64(prec_res, Tau::wtau_64(x));
       }
     }
     using FloatingPoint::Wrap64;
     std::cout << Wrap64(stats_args.amean()) << " "
               << Wrap64(stats_accur.min()) << " "
               << Wrap64(stats_accur.max()) << " "
               << Wrap64(stats_accur.amean()) << " "
               << Wrap64(stats_accur.sd_corrected()) << "\n";
   }
  }
}

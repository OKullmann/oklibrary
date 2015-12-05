// Oliver Kullmann, 5.12.2015 (Swansea)
/* Copyright 2015 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Solvers/TawSolver/Tau.cpp
  \brief Testing and using tau-components

*/

#include <string>
#include <iostream>
#include <iomanip>

#include <cassert>

#include "Tau.hpp"
// Remark: Not as otherwise in the OKlibrary, since not using standard-build yet.

namespace {

const std::string program = "Tau";
const std::string version = "0.0.8";

using namespace OKlib::Satisfiability::Heuristics;

}


int main(const int argc, const char* const argv[]) {
  {std::cout << std::setprecision(20);
  std::cout << Projections::tau<6,double>(1,2) << "\n";
  std::cout << Projections::tau(1.0,2.0) << "\n";
  std::cout << Projections::tau(double(1),double(2)) << "\n";
  std::cout << Projections::tau<2>(1.0,2.0) << "\n";
  Projections::Taud<1> tau1;
  std::cout << tau1(1,2) << "\n";
  Projections::Taud<0> tau0;
  std::cout << tau0(1,2) << "\n";
  Projections::Tau<> tau;
  std::cout << tau(1,2) << "\n";
  Projections::Tau5<float> tauf;
  std::cout << tauf(1,2) << "\n";}

  Projections::Min_tau<> mtau;
  assert(mtau.index() == 0);
  mtau(1,2);
  assert(mtau.index() == 1);
  mtau(1,3);
  assert(mtau.index() == 2);
  mtau(1,1);
  assert(mtau.index() == 2);
  assert(mtau.count() == 3);
  std::cout << mtau.t1() << " " << mtau.t2() << " " << mtau.min() << " " << mtau.max() << "\n";
}

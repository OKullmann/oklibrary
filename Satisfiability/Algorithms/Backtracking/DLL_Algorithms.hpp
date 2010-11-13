// Oliver Kullmann, 5.3.2002 (Swansea)
/* Copyright 2002 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Algorithms/Backtracking/DLL_Algorithms.hpp
  \brief Old, very simple interface for (boolean) DLL-SAT-algorithms
  \deprecated
*/

#ifndef WAECHTERDLLALGORITHMS_76TTr4
#define WAECHTERDLLALGORITHMS_76TTr4

#include <string>

#include <OKlib/Satisfiability/ProblemInstances/ClauseSets/ClauseSet.hpp>
#include <OKlib/Satisfiability/Assignments/PartialAssignments/PartAssign.hpp>

namespace DLL_Algorithms {

  struct Result {
    bool sat;
    PartAssignments::Pass phi;
    std::string info;
    Result(bool s, const PartAssignments::Pass& p = PartAssignments::Pass(), std::string i = "") {sat = s; phi = p; info = i;}
  };

  typedef Result SAT_Algorithms(const Clausesets::Cls&);

  SAT_Algorithms DLL_1;

}

#endif

// Oliver Kullmann, 5.3.2002 (Swansea)
/* Copyright 2002 - 2007 Oliver Kullmann
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

#include <OKlib/Satisfiability/ProblemInstances/Variables/VarSet.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/Literal.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Clauses/Clause.hpp>
#include <OKlib/Satisfiability/ProblemInstances/ClauseSets/ClauseSet.hpp>
#include <OKlib/Satisfiability/Assignments/PartAssign.hpp>

namespace DLL_Algorithms {

  using namespace Variables;
  using namespace Literals;
  using namespace Clauses;
  using namespace Clausesets;
  using namespace PartAssignments;

  struct result {
    bool sat;
    Pass phi;
    std::string info;
    result(bool s, const Pass& p = Pass(), std::string i = "") {sat = s; phi = p; info = i;}
  };

  typedef result SAT_Algorithms(const Cls&);

  SAT_Algorithms DLL_1;

}

#endif

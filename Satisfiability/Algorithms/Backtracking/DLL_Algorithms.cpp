// Oliver Kullmann, 5.3.2002 (Swansea)
/* Copyright 2002 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Algorithms/Backtracking/DLL_algorithms.cpp
  \brief Old, very simple implementation of a general (boolean) DLL-SAT-algorithms
  \deprecated
*/

#include <algorithm>

#include <string>

#include <Transitional/Satisfiability/ProblemInstances/Variables/VarSet.hpp>
#include <Transitional/Satisfiability/ProblemInstances/Literals/Literal.hpp>
#include <Transitional/Satisfiability/ProblemInstances/Clauses/Clause.hpp>
#include <Transitional/Satisfiability/ProblemInstances/ClauseSets/ClauseSet.hpp>
#include <Transitional/Satisfiability/Assignments/PartAssign.hpp>
#include <Transitional/Satisfiability/Algorithms/Backtracking/DLL_algorithms.hpp>

namespace DLL_Algorithms {

  using namespace Variables;
  using namespace Literals;
  using namespace Clauses;
  using namespace Clausesets;
  using namespace PartAssignments;
  using namespace DLL_Algorithms;

  result DLL_1(const Cls& F){
    if (F.is_empty())
      return true;
    else if (F.empty_clause())
      return false;
    else {
      Var v = *F.var().begin();
      return result(max(DLL_1(Pass(Lit(v, false)) * F).sat, DLL_1(Pass(Lit(v, true)) * F).sat));
    }
  }

}

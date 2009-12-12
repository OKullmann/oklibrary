// Oliver Kullmann, 5.3.2002 (Swansea)
/* Copyright 2002 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Algorithms/Backtracking/DLL_Algorithms.cpp
  \brief Old, very simple implementation of a general (boolean) DLL-SAT-algorithms
  \deprecated
*/

#include <algorithm>

#include <string>

#include <OKlib/Satisfiability/ProblemInstances/Variables/VarSet.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/Literal.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Clauses/Clause.hpp>
#include <OKlib/Satisfiability/ProblemInstances/ClauseSets/ClauseSet.hpp>
#include <OKlib/Satisfiability/Assignments/PartialAssignments/PartAssign.hpp>
#include <OKlib/Satisfiability/Algorithms/Backtracking/DLL_Algorithms.hpp>

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
      return result(std::max(DLL_1(Pass(Lit(v, false)) * F).sat, DLL_1(Pass(Lit(v, true)) * F).sat));
    }
  }

}

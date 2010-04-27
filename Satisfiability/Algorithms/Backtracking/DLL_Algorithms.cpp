// Oliver Kullmann, 5.3.2002 (Swansea)
/* Copyright 2002 - 2007, 2009, 2010 Oliver Kullmann
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

  using namespace Literals;
  using namespace PartAssignments;

  result DLL_1(const Clausesets::Cls& F){
    if (F.is_empty())
      return true;
    else if (F.empty_clause())
      return false;
    else {
      Variables::Var v = *F.var().begin();
      return DLL_Algorithms::result(std::max(DLL_1(Pass(Lit(v, false)) * F).sat, DLL_1(Pass(Lit(v, true)) * F).sat));
    }
  }

}

int main() {
  Clausesets::Cls F;
  Variables::Var_Set V;
  F.read(V);
  std::cout << DLL_Algorithms::DLL_1(F).sat << "\n";
}

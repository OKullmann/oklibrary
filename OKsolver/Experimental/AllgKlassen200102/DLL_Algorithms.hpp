// Oliver Kullmann, 5.3.2002 (Swansea)
/* Copyright 2002 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

// DLL_algorithms.hpp

#ifndef WAECHTERDLLALGORITHMS

#include <string>

#include "VarSet.hpp"
#include "Literal.hpp"
#include "Clause.hpp"
#include "ClauseSet.hpp"
#include "PartAssign.hpp"


namespace DLL_Algorithms {

  using namespace Variables;
  using namespace Literals;
  using namespace Clauses;
  using namespace Clausesets;
  using namespace PartAssignments;

  struct result {
    bool sat;
    Pass phi;
    string info;
    result(bool s, const Pass& p = Pass(), string i = "") {sat = s; phi = p; info = i;}
  };

  typedef result SAT_Algorithms(const Cls&);

  SAT_Algorithms DLL_1;

}

#endif

#define WAECHTERDLLALGORITHMS


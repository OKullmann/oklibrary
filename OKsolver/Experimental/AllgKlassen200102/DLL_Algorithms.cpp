// Oliver Kullmann, 5.3.2002 (Swansea)

// DLL_Algorithms.cpp

#include <algorithm>

#include "VarSet.hpp"
#include "Literal.hpp"
#include "Clause.hpp"
#include "ClauseSet.hpp"
#include "PartAssign.hpp"
#include "DLL_Algorithms.hpp"

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

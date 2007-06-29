// Oliver Kullmann, 24.5.2005 (Swansea)

/*!
  \file Transitional/Satisfiability/ProofSystems/DPv/Input_output_Testobjects.hpp
  \deprecated
*/

#ifndef INPUTOUTPUTTESTOBJECTS_7ytaBb4dsfuw
#define INPUTOUTPUTTESTOBJECTS_7ytaBb4dsfuw

#include <Transitional/Satisfiability/ProofSystems/DPv/Input_output.hpp>
#include <Transitional/Satisfiability/ProofSystems/DPv/Input_output_Tests.hpp>

namespace OKlib {
  namespace DPv {
    namespace Testobjects {

      Test_ParserLiteral<ParserLiteral> test_ParserLiteral;
      Test_DimacsParser_ClauseSet<DimacsParser> test_DimacsParser_ClauseSet;
    }
  }
}

#endif

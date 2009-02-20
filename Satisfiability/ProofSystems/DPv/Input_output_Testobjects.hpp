// Oliver Kullmann, 24.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/ProofSystems/DPv/Input_output_Testobjects.hpp
  \deprecated
*/

#ifndef INPUTOUTPUTTESTOBJECTS_7ytaBb4dsfuw
#define INPUTOUTPUTTESTOBJECTS_7ytaBb4dsfuw

#include <OKlib/Satisfiability/ProofSystems/DPv/Input_output.hpp>
#include <OKlib/Satisfiability/ProofSystems/DPv/Input_output_Tests.hpp>

namespace OKlib {
  namespace DPv {
    namespace Testobjects {

      Test_ParserLiteral<ParserLiteral> test_ParserLiteral;
      Test_DimacsParser_ClauseSet<DimacsParser> test_DimacsParser_ClauseSet;
    }
  }
}

#endif

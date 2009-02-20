// Oliver Kullmann, 24.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/ProofSystems/DPv/Input_output_Tests.hpp
  \deprecated
*/

#ifndef INPUTOUTPUTTESTS_oPLmm8uuY
#define INPUTOUTPUTTESTS_oPLmm8uuY

#include <vector>
#include <string>

#include <boost/lexical_cast.hpp>

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>
#include <OKlib/Programming/Parsing/ParserBase_Tests.hpp>

#include <OKlib/Satisfiability/ProofSystems/DPv/BasicDataStructure.hpp>

namespace OKlib {
  namespace DPv {

    template <class ParserLiteral>
    class Test_ParserLiteral : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserLiteral test_type;
      Test_ParserLiteral() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        Literal l;
        ParserLiteral p(l);
        {
          typedef std::vector<std::string> Vector;
          Vector test_vector;
          test_vector.push_back("123");
          test_vector.push_back("-456");
          for (Vector::const_iterator i = test_vector.begin(); i != test_vector.end(); ++i) {
            OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<ParserLiteral>(p, *i, ::OKlib::Parser::match_full));
            if (l.l != boost::lexical_cast<int_type>(*i))
              OKLIB_THROW("Index is " + boost::lexical_cast<std::string>(l.l) + ", and not " + *i);
          }
        }
        {
          typedef std::vector<std::string> Vector;
          Vector test_vector;
          test_vector.push_back("0123");
          test_vector.push_back("+456");
          for (Vector::const_iterator i = test_vector.begin(); i != test_vector.end(); ++i) {
            OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<ParserLiteral>(p, *i, ::OKlib::Parser::match_not_full));
          }
        }
      }
    };

    template <template <class ClauseSet> class DimacsParser>
    class Test_DimacsParser_ClauseSet : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_DimacsParser_ClauseSet test_type;
      Test_DimacsParser_ClauseSet() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        // ########################################################
      }
    };
  }

  
}

#endif

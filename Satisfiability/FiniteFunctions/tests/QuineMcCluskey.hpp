// Matthew Gwynne, 4.10.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/FiniteFunctions/tests/QuineMcCluskey
  \brief Tests for the components from QuineMcCluskey.hpp

*/

#ifndef TESTQUINEMCCLUSKEY_kkNWdcBq5
#define TESTQUINEMCCLUSKEY_kkNWdcBq5

#include <sstream>
#include <set>
#include <list>

#include <boost/assign/list_of.hpp>

#include <OKlib/Programming/Messages/Utilities/TrivialMessage.hpp>
#include <OKlib/Programming/Messages/messages/Levels.hpp>

#include <OKlib/TestSystem/TestBaseClass_DesignStudy.hpp>
#include <OKlib/TestSystem/TestExceptions_DesignStudy.hpp>
#include <OKlib/Satisfiability/FiniteFunctions/QuineMcCluskey.hpp>

namespace OKlib {
  namespace Satisfiability {
    namespace FiniteFunctions {
      namespace tests {

# define OKLIB_FILE_ID new ::OKlib::Messages::Utilities::FileIdentification \
      (__FILE__, __DATE__, __TIME__, "$Date: 2010/10/4 14:40:08 $", "$Revision: 0.8 $")

        /*!
          \class QuineMcCluskey
          \brief Testing the QuineMcCluskey prime generation
        */
        
        template
        <template<int num_vars = 4,
                  typename ClauseContainer = std::vector<std::vector<int> > >
         class QuineMcCluskey>
        OKLIB_TEST_CLASS(Test_QuineMcCluskey) {
          OKLIB_TEST_CLASS_C(Test_QuineMcCluskey) {}
        private :
          void test(::OKlib::TestSystem::Basic) {
            const int num_vars = 4;
            typedef std::vector<std::vector<int> > clause_set_type;
            typedef boost::range_value<clause_set_type>::type clause_type;
            typedef QuineMcCluskey<num_vars, clause_set_type> qmc_type;
            qmc_type qmc;
            
            { // empty sequence
              clause_set_type empty_F;
              clause_set_type empty_output = qmc(empty_F);
              OKLIB_TEST_EQUAL_RANGES(empty_output, clause_set_type());
            }
            { // example sequence
              clause_type cls_one = boost::assign::list_of(1)(2)(3);
              clause_type cls_two = boost::assign::list_of(1)(2)(-3);
              clause_type cls_three = boost::assign::list_of(1)(-2)(3);
              clause_set_type cls_F = boost::assign::list_of(cls_one)(cls_two)(cls_three);
              clause_type cls_intended_one = boost::assign::list_of(1)(2);
              clause_type cls_intended_two = boost::assign::list_of(1)(3);
              clause_set_type cls_intended_output = boost::assign::list_of
                (cls_intended_one)
                (cls_intended_two);
              clause_set_type cls_output = qmc(cls_F);
              OKLIB_TEST_EQUAL_W2(cls_intended_output, cls_output);
            }
          }
        };
      }
    }
  }
}

# undef OKLIB_FILE_ID
#endif

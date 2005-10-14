// Oliver Kullmann, 24.7.2005 (Swansea)

/*!
  \file Dimacs_Tests.hpp
  \brief Tests for tools concerning DIMACS input and output
*/

#ifndef DIMACSTESTS_0jbmnr

#define DIMACSTESTS_0jbmnr

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

namespace OKlib {
  
  namespace InputOutput {

    /*!
      \class Test_StandardDIMACSInput
      \brief Test the standard DIMACS cnf reader.
      \todo Write basic tests (which execute all exception classes).
    */

    template <template <class CLSAdaptor> class StandardDIMACSInput>
    class Test_StandardDIMACSInput : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_StandardDIMACSInput test_type;
      Test_StandardDIMACSInput() {
        insert(this);
      }
    private :
      void perform_test_trivial() {

      }
    };


  }

}

#endif

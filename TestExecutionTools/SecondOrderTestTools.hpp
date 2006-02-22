// Oliver Kullmann, 26.12.2005 (Swansea)

/*!
  \file SecondOrderTestTools.hpp
  \brief Tools for testing the test system
  \todo With C++09 stdint.h likely is to be replaced by cstdint.
*/

#ifndef SECONDORDERTESTTOOLS_7yyhHbg

#define SECONDORDERTESTTOOLS_7yyhHbg

#include <string>

#include <stdint.h>

#include <boost/lexical_cast.hpp>

#include <Transitional/Testsystem/TestBaseClass.hpp>
#include <Transitional/Testsystem/TestExceptions.hpp>

namespace OKlib {

  namespace TestExecutionTools {

    /*!
      \class TestCounter
      \brief Test function which increments a global counter each time it is called, and throws
      a test exception of the value of that counter after increment equals a throw_value.

      TestCounter<Dummy> uses Dummy only to ensure that the static data members
      are unique.
    */

    template <class Dummy, typename IntType = uint_fast64_t>
    struct TestCounter : OKlib::TestSystem::Test {
      typedef TestCounter test_type;
      typedef IntType int_type;
      static int_type counter;
      static int_type throw_value;
      TestCounter(...) {}
    private :
      void perform_test_trivial() {
        if (++counter == throw_value)
          OKLIB_THROW(boost::lexical_cast<std::string>(counter));
      }
    };
    template <class Dummy, typename IntType>
    typename TestCounter<Dummy, IntType>::int_type TestCounter<Dummy, IntType>::counter = 0;
    template <class Dummy, typename IntType>
    typename TestCounter<Dummy, IntType>::int_type TestCounter<Dummy, IntType>::throw_value = 0;


  }

}

#endif

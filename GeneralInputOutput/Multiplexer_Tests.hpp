// Oliver Kullmann, 19.1.2006 (Swansea)

/*!
  \file Multiplexer_Tests.hpp
  \brief Testing of components for gathering and distributing input and output from and to different streams
*/

#ifndef MULTIPLEXERTESTS_oKnCd2

#define MULTIPLEXERTESTS_oKnCd2

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

namespace OKlib {

  namespace GeneralInputOutput {

    template <class OStreamDescriptor>
    class Test_OStreamDescriptor : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_OStreamDescriptor test_type;
      Test_OStreamDescriptor() {
        insert(this);
      }
    private :
      void perform_test_trivial() {

        typedef typename OStreamDescriptor::label_type label_type;
        OKLIB_TEST_EQUAL(OStreamDescriptor::nullstream, 0);
        OKLIB_TEST_EQUAL(OStreamDescriptor::stdcerr, 1);
        OKLIB_TEST_EQUAL(OStreamDescriptor::stdcout, 2);
        OKLIB_TEST_EQUAL(OStreamDescriptor::stdofstreamappend, 3);
        OKLIB_TEST_EQUAL(OStreamDescriptor::stdofstreamoverwrite, 4);
        OKLIB_TEST_EQUAL(OStreamDescriptor::stdostringstream, 5);

        typedef typename OStreamDescriptor::string_type string_type;
        {
          const OStreamDescriptor od("NULL");
          OKLIB_TEST_EQUAL(od.label, OStreamDescriptor::nullstream);
          OKLIB_TEST_EQUAL(od.name, string_type());
        }
        {
          const OStreamDescriptor od("cerr");
          OKLIB_TEST_EQUAL(od.label, OStreamDescriptor::stdcerr);
          OKLIB_TEST_EQUAL(od.name, string_type());
        }
        {
          const OStreamDescriptor od("cout");
          OKLIB_TEST_EQUAL(od.label, OStreamDescriptor::stdcout);
          OKLIB_TEST_EQUAL(od.name, string_type());
        }
        {
          const OStreamDescriptor od("ofstreamappend=x");
          OKLIB_TEST_EQUAL(od.label, OStreamDescriptor::stdofstreamappend);
          OKLIB_TEST_EQUAL(od.name, string_type("x"));
        }
        {
          const OStreamDescriptor od("ofstreamoverwrite=y x");
          OKLIB_TEST_EQUAL(od.label, OStreamDescriptor::stdofstreamoverwrite);
          OKLIB_TEST_EQUAL(od.name, string_type("y x"));
        }
        {
          const OStreamDescriptor od("ostringstream=9");
          OKLIB_TEST_EQUAL(od.label, OStreamDescriptor::stdostringstream);
          OKLIB_TEST_EQUAL(od.name, string_type("9"));
        }
        {
          const OStreamDescriptor od("abcd 9o =xcd ");
          OKLIB_TEST_EQUAL(od.label, OStreamDescriptor::stdostringstream);
          OKLIB_TEST_EQUAL(od.name, string_type("xcd "));
        }
      }
    };

    // ##################################################



  }

}

#endif

// Oliver Kullmann, 19.1.2006 (Swansea)

/*!
  \file Multiplexer_Tests.hpp
  \brief Testing of components for gathering and distributing input and output from and to different streams
*/

#ifndef MULTIPLEXERTESTS_oKnCd2

#define MULTIPLEXERTESTS_oKnCd2

#include <set>
#include <vector>

#include <boost/assign/std/vector.hpp>
#include <boost/assign/list_of.hpp>

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

    template <class OStreamMultiplexer>
    class Test_OStreamMultiplexer : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_OStreamMultiplexer test_type;
      Test_OStreamMultiplexer() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        typedef typename OStreamMultiplexer::ostream_descriptor_type ostream_descriptor_type;
        typedef typename OStreamMultiplexer::label_type label_type;
        typedef typename OStreamMultiplexer::string_type string_type;
        typedef typename OStreamMultiplexer::stringstream_map_type stringstream_map_type;
        typedef typename OStreamMultiplexer::ofstream_map_type ofstream_map_type;
        typedef typename OStreamMultiplexer::fostream_vector_type fostream_vector_type;

        typedef std::set<ostream_descriptor_type> descriptor_set_type;
        typedef std::vector<descriptor_set_type> test_vector_type;

        typedef ostream_descriptor_type odt;
        typedef descriptor_set_type dst;

        {
          test_vector_type test_vector;
          OStreamMultiplexer multiplexer(test_vector);
          OKLIB_TEST_EQUAL(multiplexer.stringstream_map.empty(), true);
          OKLIB_TEST_EQUAL(multiplexer.ofstream_map.empty(), true);
          OKLIB_TEST_EQUAL(multiplexer.fostream_vector.empty(), true);
        }
        {
          test_vector_type test_vector;
          using namespace boost::assign;
          test_vector +=
            list_of(odt(odt::nullstream, ""));
          OStreamMultiplexer multiplexer(test_vector);
          OKLIB_TEST_EQUAL(multiplexer.stringstream_map.empty(), true);
          OKLIB_TEST_EQUAL(multiplexer.ofstream_map.empty(), true);
          OKLIB_TEST_EQUAL(multiplexer.fostream_vector.size(), 1U);
        }
      }
    };


  }

}

#endif

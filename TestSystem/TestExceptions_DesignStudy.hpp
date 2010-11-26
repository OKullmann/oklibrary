// Oliver Kullmann, 8.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TestSystem/TestExceptions_DesignStudy.hpp
  \brief Module providing test macros

  Main components are:
   - OKLIB_THROW, OKLIB_THROW_M
   - OKLIB_TEST_EQUAL and similar tests
   - OKLIB_TEST_RETHROW.
*/

#ifndef TESTEXCEPTIONS_kjhytRe4
#define TESTEXCEPTIONS_kjhytRe4

#include <string>
#include <ostream>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <cassert>

#include <boost/range/functions.hpp>

#include <OKlib/TestSystem/BasicDeclarations.hpp>

#include <OKlib/TestSystem/messages/TestExceptions.hpp>

namespace OKlib {
  namespace TestSystem {

    /*!
      \def OKLIB_TESTDESCRIPTION
      \brief Basic internal macro to create a description of the circumstances of test failures.

      \todo Explain its usage.
    */

#define OKLIB_TESTDESCRIPTION (::OKlib::TestSystem::messages::ErrorDescription(description(), OKLIB_FILE_ID, OKLIB_LINE))

    /*!
      \def OKLIB_THROW
      \brief Basic macro for throwing an exception in case of test failure.

      Usage:
        OKLIB_THROW(test_description)
      where test_description is a string.
    */

#define OKLIB_THROW(message) throw ::OKlib::TestSystem::messages::TestException(std::string("OKLIB TEST FAILURE: ") + message).add(OKLIB_TESTDESCRIPTION);

    /*!
      \def OKLIB_THROW_M
      \brief Basic macro for throwing an exception in case of test failure, with additional information

      Usage:
        OKLIB_THROW_M(short_test_description, message)
      where short_test_description is a string and message a pointer to a message (if message is a null pointer, then nothing happens).
    */

#define OKLIB_THROW_M(short_test_description, message) { ::OKlib::Messages::MessagesPrePost* const m(message); if (m) throw ::OKlib::TestSystem::messages::TestException(std::string("OKLIB TEST FAILURE: ") + short_test_description, m).add(OKLIB_TESTDESCRIPTION); }

    /*!
      \def OKLIB_TEST_EQUAL
      \brief Use OKLIB_TEST_EQUAL(a,b) for asserting a == b in case a and b are output-streamable.

      In case of a failure a, b are output, where a is the value found, while b is the expected value. The expressions a, b are evaluated exactly once.
    */

#define OKLIB_TEST_EQUAL(v1, v2) \
    OKLIB_THROW_M("OKLIB_TEST_EQUAL", OKlib::TestSystem::messages::test_not_equal((v1), (v2)));


    /*!
      \def OKLIB_TEST_NOTEQUAL
      \brief Use OKLIB_TEST_EQUAL(a,b) for asserting not (a == b); no output of a, b.
    */

#define OKLIB_TEST_NOTEQUAL(v1, v2) \
    if ( (v1) == (v2)) {            \
      OKLIB_THROW("OKLIB_TEST_NOTEQUAL"); \
    }

    /*!
      \def OKLIB_TEST_EQUAL_RANGES
      \brief Use OKLIB_TEST_EQUAL_RANGES(r1,r2) for asserting ranges (in the boost::range
      sense) are equal (no output of the elements of r1 or r2 in case of test failure).

      \todo Upgrade to OKLIB_THROW_M.
    */

#define OKLIB_TEST_EQUAL_RANGES(c1, c2) { \
      using boost::begin; \
      using boost::end; \
      using boost::distance; \
      if (distance(c1) != distance(c2)) { \
        std::stringstream out; \
        out << "Size of first range is " << distance(c1) << ", size of second range is " << distance(c2); \
        OKLIB_THROW(out.str()); \
      } \
      else if (not std::equal(begin(c1), end(c1), begin(c2))) \
        OKLIB_THROW("Containers have different content"); \
    }

    /*!
      \class OutputWrapper
      \brief Wrapper type to mark objects for list-output

      Class template used in conjunction with OKLIB_TEST_EQUAL_W when testing ranges
      for equality, where a list of the elements shall be output in case of test failure.

      \todo Should this class go to a general output facilities module?
    */

    template <typename T>
    struct OutputWrapper {
      const T& t;
      OutputWrapper(const T& t) : t(t) {}
    };
    template <typename T>
    inline OutputWrapper<T> output_wrapper(const T& t) {
      return OutputWrapper<T>(t);
    }

    template <typename T>
    std::ostream& operator <<(std::ostream& out, const OutputWrapper<T>& w) {
      std::copy(w.t.begin(), w.t.end(), std::ostream_iterator<typename T::value_type>(out, ","));
      return out;
    }

   /*!
      \class OutputWrapper2
      \brief Wrapper type to mark objects for nested list-output

      Class template used in conjunction with OKLIB_TEST_EQUAL_W when testing ranges
      for equality, where a list of the list-elements shall be output in case of test failure.

      \todo Should this class go to a general output facilities module?
    */

    template <typename T>
    struct OutputWrapper2 {
      const T& t;
      OutputWrapper2(const T& t) : t(t) {}
    };
    template <typename T>
    inline OutputWrapper2<T> output_wrapper2(const T& t) {
      return OutputWrapper2<T>(t);
    }

    template <typename T>
    std::ostream& operator <<(std::ostream& out, const OutputWrapper2<T>& w) {
      std::copy(w.t.begin(), w.t.end(), std::ostream_iterator<OutputWrapper<typename T::value_type> >(out, "\n"));
      return out;
    }

   /*!
      \class OutputWrapper3
      \brief Wrapper type to mark objects for doubly nested list-output

      Class template used in conjunction with OKLIB_TEST_EQUAL_W when testing ranges
      for equality, where a list of list of the list-elements shall be output in case of test failure.

      \todo Should this class go to a general output facilities module?
    */
    template <typename T>
    struct OutputWrapper3 {
      const T& t;
      OutputWrapper3(const T& t) : t(t) {}
    };
    template <typename T>
    inline OutputWrapper3<T> output_wrapper3(const T& t) {
      return OutputWrapper3<T>(t);
    }
    template <typename T>
    std::ostream& operator <<(std::ostream& out, const OutputWrapper3<T>& w) {
      std::copy(w.t.begin(), w.t.end(), std::ostream_iterator<OutputWrapper2<typename T::value_type> >(out, "\n"));
      return out;
    }

    /*!
      \def OKLIB_TEST_EQUAL_W
      \brief Test macro for asserting equality in case the objects compared
      are not output-streamable, but output in case of test failure is needed.

      Is used by default for containers with output-streamable elements; adaption is possible
      for the general case by defining an output wrapper.

      Without user-defined definitions, OKLIB_TEST_EQUAL_W(a, b) can be used
      for container-types (having member functions begin and end).
      For other classes X, before using of the macro the std::ostream inserter
      must be overloaded for inserting objects of type OKlib::TestSystem::OutputWrapper<X>.

      \todo Likely the same treatment as for OKLIB_TEST_EQUAL is needed
      (to avoid double evaluation).
    */

#define OKLIB_TEST_EQUAL_W(v1, v2) \
    if ( not((v1) == (v2))) {      \
      std::stringstream out; \
      out << "Value is " << ::OKlib::TestSystem::output_wrapper(v1); \
      out << ", and not " << ::OKlib::TestSystem::output_wrapper(v2); \
      OKLIB_THROW(out.str()); \
    }

   /*!
      \def OKLIB_TEST_EQUAL_W2
      \brief Refining OKLIB_TEST_EQUAL_W for the case of a container of containers.
   */

#define OKLIB_TEST_EQUAL_W2(v1, v2) \
    if ( not((v1) == (v2))) {       \
      std::stringstream out; \
      out << "Value is " << ::OKlib::TestSystem::output_wrapper2(v1); \
      out << ", and not " << ::OKlib::TestSystem::output_wrapper2(v2); \
      OKLIB_THROW(out.str()); \
    }

   /*!
      \def OKLIB_TEST_EQUAL_W3
      \brief Refining OKLIB_TEST_EQUAL_W for the case of a container of containers of containers.
   */

#define OKLIB_TEST_EQUAL_W3(v1, v2) \
    if ( not((v1) == (v2))) {       \
      std::stringstream out; \
      out << "Value is " << ::OKlib::TestSystem::output_wrapper3(v1); \
      out << ", and not " << ::OKlib::TestSystem::output_wrapper3(v2); \
      OKLIB_THROW(out.str()); \
    }

    // ######################################################

    /*!
      \def OKLIB_TEST_RETHROW
      \brief Macro for calling sub-tests (rethrowing their exceptions).

      OKLIB_TEST_RETHROW(testobject) invokes the test of testobject at the given level
      and rethrows exceptions derived from OKlib::TestSystem::TestException, adding
      the description of the circumstances under which the test of testobject has been
      involved.
    */

#define OKLIB_TEST_RETHROW(Testobject) \
    try { \
      assert(base_type::level_p); \
      ((Testobject).set_depth(this -> depth() + 1).perform(*base_type::level_p, log_stream())); \
    } \
    catch(::OKlib::TestSystem::messages::TestException& e) {    \
      e.add(OKLIB_TESTDESCRIPTION); \
      throw e; \
    }

  }

}

#endif

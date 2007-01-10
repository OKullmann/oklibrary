// Oliver Kullmann, 8.5.2005 (Swansea)

/*!
  \file TestSystem/TestExceptions_DesignStudy.hpp
  \brief Module providing test exceptions and test macros

  Main components are:
   - TestException
   - OKLIB_LINE
   - OKLIB_THROW
   - OKLIB_TEST_EQUAL and similar tests
   - OKLIB_TEST_RETHROW.
*/

#ifndef TESTEXCEPTIONS_kjhytRe4
#define TESTEXCEPTIONS_kjhytRe4

#include <stdexcept>
#include <string>
#include <ostream>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include <ostream>
#include <cassert>

#include <boost/range/functions.hpp>

#include <Transitional/TestSystem/BasicDeclarations.hpp>
#include <Transitional/TestSystem/messages/TestLevel.hpp>

#include <Transitional/TestSystem/messages/TestExceptions.hpp>

namespace OKlib {
  namespace TestSystem {

    /*!
      \typedef ErrorContainer
      \brief Container of error descriptions

      Vector of error descriptions. Assumes, that the destructor of std::vector does not throw.
    */

    typedef std::vector< ::OKlib::TestSystem::messages::ErrorDescription> ErrorContainer; 

    // ######################################################

    /*!
      \class TestException
      \brief The root of the exception class hierarchy, to be thrown in case of test failure.

      Derived from std::runtime_error.
      Contains a container of error descriptions, the first one being the actual error description,
      followed by descriptions for unwinding the call-stack.
      Should normally not be used directly, but should be invoked by one of the macros
      like OKLIB_TEST_EQUAL.

      \todo Use Messages.
      \todo One could make class TestException also a message-class ?!
      \todo Shouldn't we allow also at the other places in the call-stack to add a string
      describing the special circumstances (not just at the place where the error occurred)?!
      But this might be hard to achieve, and might be better realised by using the log-facilities?!
    */

    class TestException : public std::runtime_error {
      
      ErrorContainer errors;

    public :

      explicit TestException(
                             const std::string& special_circumstances
                             ) :
        std::runtime_error(special_circumstances)
      {}
      ~TestException() throw() {}

      TestException& add(const ::OKlib::TestSystem::messages::ErrorDescription e) {
        errors.push_back(e);
        return *this;
      }

      friend std::ostream& operator <<(std::ostream& out, const TestException& E) {
        typedef ::OKlib::TestSystem::ErrorContainer::const_iterator iterator;
        typedef ::OKlib::TestSystem::ErrorContainer::size_type size_type;

        out << "\nEXCEPTION THROWN:\n Compile time = " << __DATE__ ", " __TIME__ "\n message = " << E.what() << "\n stack trace (";
        {
          const size_type size(E.errors.size());
          out << size << " element";
          if (size != 1) out << "s";
        }
        out << "):\n";
        const iterator& end(E.errors.end());
        size_type counter = 0;
        for (iterator i(E.errors.begin()); i != end; ++i, ++counter)
          out << "Error description " << counter << ":\n" << *i;
        return out;
      }
    };

    // ######################################################

    /*!
      \def OKLIB_LINE
      \brief Macro to create a message object with the current line number

      \todo Explain its usage.
    */

# define OKLIB_NUMBER(N) # N
    //! putting quotes around the line number
# define OKLIB_INTERMEDIATE_LINE(X) OKLIB_NUMBER(X)
# define OKLIB_LINE new ::OKlib::Messages::Utilities::LineIdentification(OKLIB_INTERMEDIATE_LINE(__LINE__))

    /*!
      \def OKLIB_TESTDESCRIPTION
      \brief Basic internal macro to create a description of the circumstances of test failures.

      \todo Explain its usage.
    */

#define OKLIB_TESTDESCRIPTION (::OKlib::TestSystem::messages::ErrorDescription(OKLIB_FILE_ID, OKLIB_LINE, test_function_type_name, new ::OKlib::TestSystem::messages::TestLevelDescriptions(::OKlib::TestSystem::test_level(level_type())), depth()))

    /*!
      \def OKLIB_THROW
      \brief Basic macro for throwing an exception in case of test failure.

      Usage:
        OKLIB_THROW(test_description)
      where test_description is a string.
    */

#define OKLIB_THROW(message) throw ::OKlib::TestSystem::TestException(std::string("OKLIB TEST FAILURE: ") + message).add(OKLIB_TESTDESCRIPTION);

    /*!
      \def OKLIB_TEST_EQUAL
      \brief Use OKLIB_TEST_EQUAL(a,b) for asserting a == b in case a and b are output-streamable.

      In case of a failure a, b are output. where a is the value found, while b is the expected value.
    */

#define OKLIB_TEST_EQUAL(v1, v2) \
    if ( not((v1) == (v2))) {    \
      std::stringstream out; \
      out << "Value is\n" << (v1) << ",\n and not\n" << (v2) << ".";    \
      OKLIB_THROW(out.str()); \
    }

    /*!
      \def OKLIB_TEST_NOTEQUAL
      \brief Use OKLIB_TEST_EQUAL(a,b) for asserting not (a == b); no output of a, b.
    */

#define OKLIB_TEST_NOTEQUAL(v1, v2) \
    if ( (v1) == (v2)) {            \
      OKLIB_THROW("Equal values"); \
    }

    /*!
      \def OKLIB_TEST_EQUAL_RANGES
      \brief Use OKLIB_TEST_EQUAL_RANGES(r1,r2) for asserting ranges (in the boost::range
      sense) are equal (no output of the elements of r1 or r2 in case of test failure).
    */

#define OKLIB_TEST_EQUAL_RANGES(c1, c2) { \
      using boost::begin; \
      using boost::end; \
      using boost::size; \
      if (size(c1) != size(c2)) { \
        std::stringstream out; \
        out << "Size of first range is " << size(c1) << ", size of second range is " << size(c2); \
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
      \def OKLIB_TEST_EQUAL_W
      \brief Test macro for asserting equality in case the objects compared
      are not output-streamable, but output in case of test failure is needed.

      Is used by default for containers with output-streamable elements; adaption is possible
      for the general case by defining an output wrapper.

      Without user-defined definitions, OKLIB_TEST_EQUAL_W(a, b) can be used
      for container-types (having member functions begin and end).
      For other classes X, before using of the macro the std::ostream inserter
      must be overloaded for inserting objects of type OKlib::TestSystem::OutputWrapper<X>.
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
    catch(::OKlib::TestSystem::TestException& e) { \
      e.add(OKLIB_TESTDESCRIPTION); \
      throw e; \
    }

  }

}

#endif

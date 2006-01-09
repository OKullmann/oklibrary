// Oliver Kullmann, 8.5.2005 (Swansea)

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

#include <boost/range/functions.hpp>

namespace OKlib {

  namespace TestSystem {

    typedef unsigned int LineNumber;

    /*!
      \class ErrorDescription
      \brief The unit to describe one point in call history leading to a test failure.

      A concrete class containing C strings for the file name, the line number and the
      name of the type of the test class.
    */

    class ErrorDescription {
      const char* file;
      const char* line;
      const char* type_test_class;
      const char* level_description;
    public :
      ErrorDescription() : file(0), line(0), type_test_class(0), level_description(0) {}
      ErrorDescription(const char* const file, const char* const line, const char* const type_test_class, const char* const level_description) : file(file), line(line), type_test_class(type_test_class), level_description(level_description) {}
      friend std::ostream& operator <<(std::ostream& out, const ErrorDescription& D) {
        return out << "Error context:\n" << " file = " << D.file << "\n line number = " << D.line << "\n test type = " << D.type_test_class << "\n test level = " << D.level_description << "\n";
        // ToDo: Adding messages (using module Messages)
      }
    };

    typedef std::vector<ErrorDescription> ErrorContainer;
    // Assumes, that the destructor of std::vector does not throw exceptions.

    // ######################################################

    /*!
      \class TestException
      \brief The root of the exception class hierarchy to be thrown in case of test failure.

      Should normally not be used directly, but should be invoked by one of the macros
      like OKLIB_TEST_EQUAL. Is derived from std::runtime_error.
    */

    class TestException : public std::runtime_error {
      
      ErrorContainer errors;

    public :

      explicit TestException(const std::string& special_circumstances) :  std::runtime_error(special_circumstances) {}
      ~TestException() throw() {}

      TestException& add(const ErrorDescription e) {
        errors.push_back(e);
        return *this;
      }

      friend std::ostream& operator <<(std::ostream& out, const TestException& E) {
        out << "Exception thrown:\n Compile time = " << __DATE__ ", " __TIME__ "\n message = " << E.what() << "\n stack trace:\n";
        std::copy(E.errors.begin(), E.errors.end(), std::ostream_iterator<ErrorDescription>(out, ""));
        return out;
      }
    };

    // ######################################################

    /*!
      \def OKLIB_TESTDESCRIPTION
      \brief Basic internal macro to create a description of the circumstances of test failures.
    */

# define OKLIB_LINENUMBER(L) # L
# define OKLIB_INTERMEDIATE_TEST(X) OKLIB_LINENUMBER(X)
#define OKLIB_TESTDESCRIPTION (::OKlib::TestSystem::ErrorDescription(__FILE__, OKLIB_INTERMEDIATE_TEST(__LINE__), typeid(test_type).name(), level_type().description()))

    /*!
      \def OKLIB_THROW
      \brief Basic macro for throwing an exception in case of test failure.
      Usage OKLIB_THROW(test_description).
    */

#define OKLIB_THROW(message) throw ::OKlib::TestSystem::TestException(std::string("OKLIB TEST FAILURE: ") + message).add(OKLIB_TESTDESCRIPTION);

    /*!
      \def OKLIB_TEST_EQUAL
      \brief Use OKLIB_TEST_EQUAL(a,b) for asserting a == b in case a and b are
      output-streamable (in case of a failure a, b are output); a here is the value found, while b is the expected value.
    */

#define OKLIB_TEST_EQUAL(v1, v2) \
    if ( not((v1) == (v2))) {    \
      std::stringstream out; \
      out << "Value is\n" << (v1) << ",\n and not\n" << (v2) << ".";    \
      OKLIB_THROW(out.str()); \
    }

    /*!
      \def OKLIB_TEST_NOTEQUAL
      \brief Use OKLIB_TEST_EQUAL(a,b) for asserting not (a == b); no output
     of a, b.
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
      \brief Class template used in conjunction with OKLIB_TEST_EQUAL_W
      when testing ranges for equality, where a list of the elements shall be output
      in case of test failure.
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
      \brief Test macro to be used for asserting equality in case the objects compared
      are not output-streamable, but output in case of test failure is needed. Default
      usage for containers with output-streamable elements; adaption is possible
      for the general case by defining an output wrapper.

      Without user-defined definitions, OKLIB_TEST_EQUAL_W(a, b) can be used
      for container-types (having member functions begin and end).
      For other classes X, before the use of the macro the std::ostream inserter
      must be overloaded for inserting objects of type OKlib::Testsystem::OutputWrapper<X>.
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
      ((Testobject).perform(level_type(), log));        \
    } \
    catch(::OKlib::TestSystem::TestException& e) { \
      e.add(OKLIB_TESTDESCRIPTION); \
      throw e; \
    }

    // this needs a global null-stream object:
// #define OKLIB_TEST_RETHROW_NO_LOG(Testobject) \
//     try { \
//       ((Testobject).perform(level_type(), OKlib::GeneralInputOutput::null_stream));        \
//     } \
//     catch(::OKlib::TestSystem::TestException& e) { \
//       e.add(OKLIB_TESTDESCRIPTION); \
//       throw e; \
//     }


  }

}

#endif

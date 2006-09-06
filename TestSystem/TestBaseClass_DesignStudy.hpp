// Oliver Kullmann, 5.12.2005

/*!
  \file TestSystem/TestBaseClass_DesignStudy.hpp
  \brief The (user) base class TestSystem::TestBase of the "test-function" hierarchy.

  "Test-functions" test (higher) type parameters; they are used either by other test-functions,
  or they are used to create test objects, to be executed by TestSystem::RunTest.
*/

#ifndef TESTBASECLASSTEMPORARY_8uXXzs
#define TESTBASECLASSTEMPORARY_8uXXzs

#include <ostream>
#include <exception>

#include <boost/iostreams/filtering_stream.hpp>

#include <Transitional/GeneralInputOutput/IOStreamFilters.hpp>
#include <Transitional/Messages/MessagesBase.hpp>

#include <Transitional/TestSystem/BasicDeclarations.hpp>
#include <Transitional/TestSystem/TestFondement.hpp>
#include <Transitional/TestSystem/TestExceptions_DesignStudy.hpp>
#include <Transitional/TestSystem/messages/TestBaseClass.hpp>

namespace OKlib {

  namespace TestSystem {

    /*!
      \class TestBase
      \brief Derived from class Test, to be used as base class for test-meta-functions (by the user
      of the test system).

      Test-meta-functions F are derived from TestBase; the construction of F passes information
      about the identity of F to the base class, and private member functions F::test, overloaded on
      the (static) test level, are responsible for performing the test (F::test receives the log-stream
      as second parameter, passed to the base class, which is responsible for managing the log-stream).
    */

    class TestBase : public ::OKlib::TestSystem::Test {

      //! No copying of objects of this class
      TestBase(const TestBase&);
      //! No assignment for objects of this class
      TestBase& operator=(const TestBase&);

      //! Name of the source-code file containing the test-meta-function (fixed at construction time).
      const char* const file_name;
      //! Line number where the test-meta-function declaration starts (w.r.t. file_name; fixed at construction time).
      const ::OKlib::TestSystem::line_number_type line_number;

      //! Nesting depth for the test
      /*!  Initialised at construction to 0. Can be updated after construction via set_depth. */
      ::OKlib::TestSystem::depth_number_type depth_;

      //! Ostream-type (used for the log-stream), automatically handling indentation
      typedef ::OKlib::GeneralInputOutput::IndentLines indent_lines_type;
      //! Current indentation (in number of spaces)
      indent_lines_type::size_type indentation;

      //! Pointer to the log-stream (see TestBase::perform_and_catch below)
      /*! Needs to be a pointer, since the log-stream is not known at construction time of a test-object. */
      std::ostream* log_p;

     protected :

      //! Pointer to the test-level object
      /*!
        Via this pointer we have access to the *original* test-level, so that nested test-meta-functions can perform
        tests at the original test-level (note that in the TestBase::test functions (see below) only the decayed
        (static) test-level is available).
      */
      const ::OKlib::TestSystem::TestLevel* level_p;

      //! Name of the test-meta-function (which is simply the (complete) type).
      const char* const test_function_type_name;

      //! The only constructor
      TestBase(
               const char* const file_name,
               const ::OKlib::TestSystem::line_number_type line_number,
               const char* const test_function) :
        file_name(file_name),
        line_number(line_number),
        depth_(0),
        indentation(1),
        log_p(0),
        level_p(0),
        test_function_type_name(test_function)
      {}

      //! Accessor function for current nesting-depth
      /*!
        Used when constructing a nested test, which must be informed about its new nesting-depth
        (which is depth()+1).
      */
      ::OKlib::TestSystem::depth_number_type depth() const { return depth_; }

    public :
      //! Changing the test-function-nesting-level.
      /*!
        In principle the nesting level could be set (once and for all) during construction, however in this way
        it is easier to have parameterised tests, where the parameters are passed to the constructor of the
        test-object. So for example we can use
          OKLIB_TEST_RETHROW(::OKlib::Module::tests::Test(x));
        where x is a parameter: OKLIB_TEST_RETHROW doesn't need to assume anything on the construction
        of the test-object, but can assume it has been constructed, and then changes the nesting-level *afterwards*.
        It would need more complex macro-programming to enable syntax like
          OKLIB_TEST_RETHROW(::OKlib::Module::tests::Test);
          OKLIB_TEST_RETHROW(::OKlib::Module::tests::Test, x);
          OKLIB_TEST_RETHROW(::OKlib::Module::tests::Test, x, y);
        set_depth is used by OKLIB_TEST_RETHROW to (re)set the new testobject, and thus must be public (although it does not really belong
        to the interface of TestBase).
        \todo Is it possible to hide set_depth better?
        (Since it belongs to the test-system itself, and the user doesn't need to know about it.)
      */
      TestBase& set_depth(const ::OKlib::TestSystem::depth_number_type d) { depth_ = d; return *this; }

    private :

      /*
        The following block of overloaded functions implements the pure virtual member functions from the
        base class Test, delegating the task in each case to the member function template perform_and_catch.
      */

      void perform_(Basic, std::ostream& log) {
        perform_and_catch(Basic(), log);
      }
      void perform_(Full, std::ostream& log) {
        perform_and_catch(Full(), log);
      }
      void perform_(Extensive, std::ostream& log) {
        perform_and_catch(Extensive(), log);
      }

      //! Auxiliary function for performing the test, managing the log-stream and exceptions.
      /*!
        Responsible for setting up the log-stream (with the right indentation level) and the level-object-pointer,
        and for catching exceptions thrown when executing the test; delegating the task of executing the test to
        an overloaded version of the virtual member function TestBase::test according to the test level.

        The log-stream is created in this member function, and destroyed upon exit; via log_p the log-stream
        is made available to the member function TestBase::log during test-execution, so that messages can be
        put on the log stream by tests.
      */

      template <class TestLevel>
      void perform_and_catch(TestLevel, std::ostream& log) {
        typedef TestLevel level_type;
        level_p = &::OKlib::TestSystem::test_level(TestLevel());
        assert(level_p);
        ::OKlib::GeneralInputOutput::IndentLines indent_lines;
        indent_lines.indentation() = indentation;
        boost::iostreams::filtering_ostream log_indent;
        log_p = &log_indent;
        assert(log_p);
        log_indent.push(indent_lines);
        log_indent.push(log);
        ::OKlib::Messages::MessagesBase::set(log_indent, ::OKlib::Messages::MessagesBase::language(log));
        ::OKlib::Messages::MessagesBase::set(log_indent, ::OKlib::Messages::MessagesBase::level(log));

        log_indent << ::OKlib::TestSystem::messages::BasicTestDescription(test_function_type_name, file_name, line_number, depth_, ::OKlib::TestSystem::test_level(TestLevel()));
        try {
          test(TestLevel(), log_indent);
        }
        catch(const TestException&) {
          throw;
        }
        catch(const std::exception& e) {
          TestException e_new(std::string("std::exception\n   what = \"") + e.what() + "\"\n   type = " + typeid(e).name());
          e_new.add(OKLIB_TESTDESCRIPTION);
          throw e_new;
        }
        catch(...) {
          TestException e("unknown exception");
          e.add(OKLIB_TESTDESCRIPTION);
          throw e;
        }
      }

      /*
        The following virtual member functions actually perform the test, and are to be implemented by the
        test-meta-functions (that is, only the Basic-versions needs implementation, while the rest has default
        implementations).
      */

      virtual void test(Basic, std::ostream& log) = 0;
      virtual void test(Full, std::ostream& log) {
        log << "Warning: test level \"Full\" not available, retrograding to test level \"Basic\"" << std::endl; // Messages
        test(Basic(), log);
      }
      virtual void test(Extensive, std::ostream& log) {
        log << "Warning: test level \"Extensive\" not available, retrograding to test level \"Full\"" << std::endl; // Messages
        test(Full(), log);
      }

    protected :

      //! Test-meta-functions create log-messages via this member function.

      void log(const ::OKlib::Messages::MessagesBase* const m, ::OKlib::TestSystem::line_number_type const line, const char* const file) {
        assert(log_p);
        assert(level_p);
        *log_p << ::OKlib::TestSystem::messages::LogDescription(file, line, depth_, level_p) << *m;
      }

      //! DEPRECATED (this was the first approach for creating log-messages)
      /*! \todo All applications are to be replaced by TestBase::log, and then TestBase::log_message is to be eliminated. */
      std::ostream& log_message(std::ostream& log, ::OKlib::TestSystem::line_number_type line) {
        return log << "log at line " << line << ": ";
      }
    };

  }
  
}

#endif

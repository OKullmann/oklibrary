// Oliver Kullmann, 5.12.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TestSystem/TestBaseClass_DesignStudy.hpp
  \brief The (user) base class TestSystem::TestBase of the "test-function" hierarchy.

  "Test-functions" test (higher) type parameters; they are used either by other test-functions,
  or they are used to create test objects, to be executed by TestSystem::RunTest.

  Besides the base class TestBase of the test-class-hierarchy, macros
   - OKLIB_TEST_CLASS
   - OKLIB_TEST_CLASS_C
   - OKLIB_TEST_CLASS_C1

  are defined to ease the definition of test classes.
*/

#ifndef TESTBASECLASSTEMPORARY_8uXXzs
#define TESTBASECLASSTEMPORARY_8uXXzs

#include <ostream>
#include <exception>

#include <boost/iostreams/filtering_stream.hpp>

#include <Transitional/Programming/InputOutput/IOStreamFilters.hpp>
#include <Transitional/Programming/Messages/MessagesBase.hpp>
#include <Transitional/Programming/Messages/Utilities/TrivialMessage.hpp>
#include <Transitional/Programming/Messages/Utilities/FileIdentification.hpp> // guaranteed to be included

#include <Transitional/TestSystem/BasicDeclarations.hpp>
#include <Transitional/TestSystem/TestFondement.hpp>
#include <Transitional/TestSystem/TestExceptions_DesignStudy.hpp>
#include <Transitional/TestSystem/messages/TestBaseClass.hpp>

namespace OKlib {
  namespace TestSystem {

# define OKLIB_FILE_ID new ::OKlib::Messages::Utilities::FileIdentification \
    (__FILE__, __DATE__, __TIME__, "$Date: 2007/02/06 18:38:17 $", "$Revision: 1.9 $")

    /*!
      \class TestBase
      \brief Derived from class Test, to be used as base class for test-meta-functions (by the user
      of the test system).

      Test-meta-functions F are derived from TestBase; the construction of F passes information
      about the identity of F to the base class, and private member functions F::test, overloaded on
      the (static) test level, are responsible for performing the test (F::test receives the log-stream
      as second parameter, passed to the base class, which is responsible for managing the log-stream).

      The public members are:
      <ul>
       <li> the (virtual) destructor </li>
       <li> member function set_depth. </li>
      </ul>

       In principle the nesting level could be set (once and for all) during construction, however in our way
       it is easier to have parameterised tests, where the parameters are passed to the constructor of the
       test-object. So for example we can use
       OKLIB_TEST_RETHROW(::OKlib::Module::tests::Test(x));
       where x is a parameter: OKLIB_TEST_RETHROW doesn't need to assume anything on the construction
       of the test-object, but can assume it has been constructed, and then changes the nesting-level *afterwards*.
       It would need more complex macro-programming to enable syntax like
       OKLIB_TEST_RETHROW(::OKlib::Module::tests::Test);
       OKLIB_TEST_RETHROW(::OKlib::Module::tests::Test, x);
       OKLIB_TEST_RETHROW(::OKlib::Module::tests::Test, x, y);
       set_depth is used by OKLIB_TEST_RETHROW to (re)set the new testobject, and thus must be public
       (although it does not really belong to the interface of TestBase).
    */

    class TestBase : public ::OKlib::TestSystem::Test {

      //! No copying of objects of this class
      TestBase(const TestBase&);
      //! No assignment for objects of this class
      TestBase& operator=(const TestBase&);

      typedef std::tr1::shared_ptr< ::OKlib::Messages::MessagesPrePost> MessagePointer;

      //! Source-code file containing the test-meta-function (fixed at construction time).
      const MessagePointer file;
      //! Line number where the test-meta-function declaration starts (w.r.t. file_name; fixed at construction time).
      const MessagePointer line;

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

      //! Pointer to the test-level object (a static constant, so no ownership issues)
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
               ::OKlib::Messages::Utilities::FileIdentification* const file_name,
               ::OKlib::Messages::Utilities::LineIdentification* const line_number,
               const char* const test_function) :
        file(file_name),
        line(line_number),
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
      //! Changing the (assumed) test-function-nesting-level.
      TestBase& set_depth(const ::OKlib::TestSystem::depth_number_type d) { depth_ = d; return *this; }

    private :

      /*
        The following block of overloaded functions implements the pure virtual member functions
        from the base class Test, delegating the task in each case to the member function
        template perform_and_catch.
      */

      void perform_(Basic, std::ostream& log) { perform_and_catch(Basic(), log); }
      void perform_(Full, std::ostream& log) { perform_and_catch(Full(), log); }
      void perform_(Extensive, std::ostream& log) { perform_and_catch(Extensive(), log); }

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

        log_indent << description();
        try {
          test(TestLevel());
        }
        catch(const OKlib::TestSystem::messages::TestException&) {
          throw;
        }
        catch(const std::exception& e) {
          OKlib::TestSystem::messages::TestException e_new(std::string("std::exception\n   what = \"") + e.what() + "\"\n   type = " + typeid(e).name());
          e_new.add(OKLIB_TESTDESCRIPTION);
          throw e_new;
        }
        catch(...) {
          OKlib::TestSystem::messages::TestException e("unknown exception");
          e.add(OKLIB_TESTDESCRIPTION);
          throw e;
        }
      }

      /*
        The following virtual member functions actually perform the test, and are to be implemented by the
        test-meta-functions (that is, only the Basic-versions needs implementation, while the rest has default
        implementations).
      */

      virtual void test(Basic) = 0;
      virtual void test(Full) {
        using OKlib::Messages::Utilities::trivial_message;
        log(trivial_message("Warning: test level \"Full\" not available, by default retrograding to test level \"Basic\"")); // use Messages
        test(Basic());
      }
      virtual void test(Extensive) {
        using OKlib::Messages::Utilities::trivial_message;
        log(trivial_message("Warning: test level \"Extensive\" not available, by default retrograding to test level \"Basic\"")); // use Messages
        test(Full());
      }

    protected :

      //! Test-meta-functions create short log-messages via this member function.

      void log(const ::OKlib::Messages::MessagesBase& m) {
        assert(log_p);
        *log_p << m;
      }

      //! Test-meta-functions create full log-messages via this member function:
      void log(const ::OKlib::Messages::MessagesBase& m, ::OKlib::Messages::Utilities::LineIdentification* const line, ::OKlib::Messages::Utilities::FileIdentification* const file) const {
        assert(line);
        assert(file);
        *log_p << ::OKlib::TestSystem::messages::LogDescription(description(), file, line) << m << std::endl;
      }

      //! Access to the log-stream
      std::ostream& log_stream() { return *log_p; }

      //! The basic description of the test class
      ::OKlib::TestSystem::messages::BasicTestDescription description() const {
        assert(level_p);
        return ::OKlib::TestSystem::messages::BasicTestDescription(test_function_type_name, file, line, depth_, *level_p);
      }

    };

    /*!
      \def OKLIB_TEST_CLASS
      \brief Macro for the declaration of all test classes

      Used together with OKLIB_TEST_CLASS_C.
    */
#define OKLIB_TEST_CLASS(X) class X : public ::OKlib::TestSystem::TestBase

    /*!
      \def OKLIB_TEST_CLASS_C
      \brief Macro for the default constructor declaration of test classes

      Use as follows:

      OKLIB_TEST_CLASS(TestClass) :
        OKLIB_TEST_CLASS_C(TestClass) {}
      private :

        void test(::OKlib::TestSystem::Basic) { ... }
      };

      Remarks:
      <ol>
       <li> There is still something to write, since only in this way the class
            definition is "understandable" for XEmacs (regarding indentation and
            bracketing).
       </li>
       <li> If the test class has parameters, then
        <ul>
         <li> define a member function to set these (default constructed) parameters
              after construction, or </li>
         <li> use OKLIB_TEST_CLASS_C1, which allows for one parameter, or </li>
         <li> just define the constructors without using the macros (copying
              the typedef for base_type and the base-class construction). </li>
        </ul>
       </li>
      </ol>
    */
#define OKLIB_TEST_CLASS_C(TC) typedef ::OKlib::TestSystem::TestBase base_type; \
  public : TC() : base_type(OKLIB_FILE_ID, OKLIB_LINE, typeid(TC).name())

    /*!
      \def OKLIB_TEST_CLASS_C1
      \brief Macro for a constructor declaration of a test class with one parameter

      Use as follows:

      OKLIB_TEST_CLASS(TestClass) :
         OKLIB_TEST_CLASS_C1(TestClass, ParameterType, parameter_name) {
            // ...
            // use parameter_name for the argument of the constructor, and
            // this -> parameter_name for the (private) class member
         }
      private :
        void test(::OKlib::TestSystem::Basic) { ... }
      };

      Remarks:
      <ol>
       <li> Defines the private data member parameter_name of TestClass with type ParameterType. </li>
       <li> Defines exactly one constructor of TestClass; this constructor has one parameter called
            parameter_name, which is of type ParameterType; construction of the data member parameter_name
            is by copy-construction. </li>
       <li> The constructor is declared as explicit. </li>
      </ol>
    */

#define OKLIB_TEST_CLASS_C1(TC, PT, PN) typedef ::OKlib::TestSystem::TestBase base_type; \
  private : PT PN; \
  public : explicit TC(PT PN) : base_type(OKLIB_FILE_ID, OKLIB_LINE, typeid(TC).name()), PN(PN)

    /*!
      \def OKLIB_FULL_LOG
      \brief Macro for full log messages

      Use as follows:

      OKLIB_FULL_LOG(m);

      where m is a message object.
    */

#define OKLIB_FULL_LOG(m) log(m, OKLIB_LINE, OKLIB_FILE_ID)

  }
  
}

# undef OKLIB_FILE_ID
#endif

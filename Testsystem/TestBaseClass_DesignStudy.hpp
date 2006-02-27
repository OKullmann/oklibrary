// Oliver Kullmann, 5.12.2005

/*!
  \file Messages/TestBaseClass_DesignStudy.hpp
  \brief Design studies for the new test hierarchy.
  \todo The log-file is not passed to TestBase::print, but testobjects
  pass message objects to the base class if they want to print a message.
  \todo Use Messages for all output.
  \todo Try to extend the level hierarchy.
  \todo In case an unknown exception is thrown, there should be a global option to
  let this exception through.
*/

#ifndef TESTBASECLASSTEMPORARY_8uXXzs

#define TESTBASECLASSTEMPORARY_8uXXzs

#include <ostream>
#include <exception>

#include <boost/iostreams/filtering_stream.hpp>

#include <Transitional/GeneralInputOutput/IOStreamFilters.hpp>
#include <Transitional/Messages/MessagesBase.hpp>

#include <Transitional/Testsystem/BasicDeclarations.hpp>
#include <Transitional/Testsystem/TestFondement.hpp>
#include <Transitional/Testsystem/TestExceptions_DesignStudy.hpp>
#include <Transitional/Testsystem/messages/TestBaseClass.hpp>

namespace OKlib {

  namespace TestSystem {

    /*!
      \class TestBase
      \brief Derived from class Test, to be used as immediate base class for test-meta-functions.
    */

    class TestBase : public ::OKlib::TestSystem::Test {

      const char* const file_name;
      const ::OKlib::TestSystem::line_number_type line_number;
      ::OKlib::TestSystem::depth_number_type depth_;

      typedef ::OKlib::GeneralInputOutput::IndentLines indent_lines_type;
      indent_lines_type::size_type indentation;

      std::ostream* log_p;

     protected :

      const ::OKlib::TestSystem::TestLevel* level_p;

      const char* const test_function_type_name;

      TestBase(const char* const file_name, const ::OKlib::TestSystem::line_number_type line_number, const char* const test_function) : file_name(file_name), line_number(line_number), depth_(0), indentation(1), log_p(0), level_p(0), test_function_type_name(test_function) {}

      ::OKlib::TestSystem::depth_number_type depth() const { return depth_; }

    public :

      TestBase& set_depth(const ::OKlib::TestSystem::depth_number_type d) { depth_ = d; return *this; }

    private :

      void perform_(Basic, std::ostream& log) {
        perform_and_catch(Basic(), log);
      }
      void perform_(Full, std::ostream& log) {
        perform_and_catch(Full(), log);
      }
      void perform_(Extensive, std::ostream& log) {
        perform_and_catch(Extensive(), log);
      }

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
      virtual void test(Basic, std::ostream& log) = 0;
      virtual void test(Full, std::ostream& log) {
        log << "Warning: test level \"Full\" not available, retrograding to test level \"Basic\"" << std::endl;
        test(Basic(), log);
      }
      virtual void test(Extensive, std::ostream& log) {
        log << "Warning: test level \"Extensive\" not available, retrograding to test level \"Full\"" << std::endl;
        test(Full(), log);
      }

    protected :

      void log(const ::OKlib::Messages::MessagesBase* const m, ::OKlib::TestSystem::line_number_type const line, const char* const file) {
        assert(log_p);
        *log_p << *m;
      }
      std::ostream& log_message(std::ostream& log, ::OKlib::TestSystem::line_number_type line) {
        return log << "log at line " << line << ": ";
      }
    };

  }
  
}

#endif

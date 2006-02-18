// Oliver Kullmann, 5.12.2005

/*!
  \file TestBaseClass_DesignStudy.hpp
  \brief Design studies for the new test hierarchy.
  \todo Try to extend the level hierarchy.
  \todo In case an unknown exception is thrown, there should be a global option to
  let this exception through.
*/

#ifndef TESTBASECLASSTEMPORARY_8uXXzs

#define TESTBASECLASSTEMPORARY_8uXXzs

#include <ostream>
#include <exception>


#include <boost/iostreams/filtering_stream.hpp>

#include "IOStreamFilters.hpp"
#include "MessagesBase.hpp"

#include "BasicDeclarations.hpp"
#include "TestFondement.hpp"
#include "TestLevel_Explanations.hpp"
#include "TestExceptions_DesignStudy.hpp"

namespace OKlib {

  namespace TestSystem {

    /*!
      \class TestBase
      \brief Derived from class Test, to be used as immediate base class for test-meta-functions.
    */

    template <class TestFunction>
    class TestBase : public ::OKlib::TestSystem::Test {

      const char* const file_name;
      typedef unsigned long int line_number_type;
      const line_number_type line_number;
      ::OKlib::TestSystem::depth_number_type depth_;
      typedef ::OKlib::GeneralInputOutput::IndentLines indent_lines_type;
      indent_lines_type::size_type indentation;

     protected :

      const ::OKlib::TestSystem::TestLevel* level_p;

      typedef TestFunction test_type;

      TestBase(const char* const file_name, const line_number_type line_number) : file_name(file_name), line_number(line_number), depth_(0), indentation(1), level_p(0) {
        assert(level_p == 0);
      }

      ::OKlib::TestSystem::depth_number_type depth() const { return depth_; }

    public :

      TestBase& set_depth(const ::OKlib::TestSystem::depth_number_type d) { depth_ = d; return *this; }

    private :

      void perform_(Basic, std::ostream& log, const ::OKlib::TestSystem::TestLevel& level) {
        level_p = &level;
        assert(level_p);
        perform_and_catch(Basic(), log);
      }
      void perform_(Full, std::ostream& log, const ::OKlib::TestSystem::TestLevel& level) {
        level_p = &level;
        assert(level_p);
        perform_and_catch(Full(), log);
      }
      void perform_(Extensive, std::ostream& log, const ::OKlib::TestSystem::TestLevel& level) {
        level_p = &level;
        assert(level_p);
        perform_and_catch(Extensive(), log);
      }


      template <class TestLevel>
      void perform_and_catch(TestLevel, std::ostream& log) {
        typedef TestLevel level_type;
        ::OKlib::GeneralInputOutput::IndentLines indent_lines;
        indent_lines.indentation() = indentation;
        boost::iostreams::filtering_ostream log_indent;
        log_indent.push(indent_lines);
        log_indent.push(log);
        ::OKlib::Messages::MessagesBase::set(log_indent, ::OKlib::Messages::MessagesBase::language(log));
        ::OKlib::Messages::MessagesBase::set(log_indent, ::OKlib::Messages::MessagesBase::level(log));
        log_indent << "Test function = " << typeid(TestFunction).name() << "\n";
        log_indent << "File name = " << file_name << "\n";
        log_indent << "Line number = " << line_number << "\n";
        log_indent << "Test level = " << ::OKlib::TestSystem::Documentation::TestLevelDescriptions(::OKlib::TestSystem::test_level(TestLevel())) << "\n";
        log_indent << "Test depth = " << depth_ << std::endl;
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
      std::ostream& log_message(std::ostream& log, ::OKlib::TestSystem::line_number_type line) {
        return log << "log at line " << line << ": ";
      }
    };

  }
  
}

#endif

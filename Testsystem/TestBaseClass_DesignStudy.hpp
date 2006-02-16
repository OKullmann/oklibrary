// Oliver Kullmann, 5.12.2005

/*!
  \file TestBaseClass_DesignStudy.hpp
  \brief Design studies for the new test hierarchy.
  \todo RunTest should go to its own file.
  \todo Try to extend the level hierarchy.
  \todo In case an unknown exception is thrown, there should be a global option to
  let this exception through.
*/

#ifndef TESTBASECLASSTEMPORARY_8uXXzs

#define TESTBASECLASSTEMPORARY_8uXXzs

#include <cstdlib>
#include <exception>
#include <cassert>
#include <string>
#include <iomanip>

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/timer.hpp>
#include <boost/iostreams/filtering_stream.hpp>

#include "TimeHandling.hpp"
#include "IOStreamFilters.hpp"
#include "MessagesBase.hpp"

#include "BasicDeclarations.hpp"
#include "TestFondement.hpp"
#include "TestExceptions_DesignStudy.hpp"
#include "TestLevel_Explanations.hpp"

namespace OKlib {

  namespace TestSystem {

    /*!
      \class TestBase
      \brief Derived from clas Test, to be used as immediate base class for test-meta-functions.
    */

    template <class TestFunction>
    class TestBase : public Test {
      const char* const file_name;
      typedef unsigned long int line_number_type;
      const line_number_type line_number;
      ::OKlib::TestSystem::depth_number_type depth_;
      typedef ::OKlib::GeneralInputOutput::IndentLines indent_lines_type;
      indent_lines_type::size_type indentation;

     protected :

      typedef TestFunction test_type;

      TestBase(const char* const file_name, const line_number_type line_number) : file_name(file_name), line_number(line_number), depth_(0), indentation(1) {}
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


    // ###################################################


    
    class RunTest {
      RunTest(const RunTest&); // not available
      RunTest& operator =(const RunTest&); // not available

    public :
      typedef Test test_type;
      typedef Test* test_pointer_type;

      typedef boost::ptr_list<test_type> container_type;

    private :

      static container_type& handle_test_objects(const test_pointer_type p = 0) {
        static container_type test_objects;
        if (p)
          test_objects.push_back(p);
        return test_objects;
      }

    public :

      RunTest(Test* const test) {
        // takes over ownership of test object
        handle_test_objects(test);
      }

      static int run_tests(std::ostream& err, std::ostream& messages, std::ostream& log, TestLevel& level) {
        return run_tests(err, messages, log, level, handle_test_objects());
      }
        
      static int run_tests(std::ostream& err, std::ostream& messages, std::ostream& log, TestLevel& level, container_type& test_objects) {

        messages << "\n" << banner_messages() << "\nOKlib::TestSystem::RunTest\n\n";
        messages << TimeHandling::currentDateTime("%A, %B %e, %Y; %H:%M:%S%n");
        messages << ::OKlib::TestSystem::Documentation::TestLevelDescriptions(level) << "\n";
        typedef container_type::size_type size_type;
        {
          const size_type& size(test_objects.size());
          messages << size << " testobject";
          if (size != 1) messages << "s";
          messages << ". \n";
        }
        messages << std::endl;

        int return_value = 0;
        boost::timer timer;
        TimeHandling::WallTime total_time;

        typedef container_type::iterator iterator;
        const iterator& end(test_objects.end());
        size_type counter = 1;
        size_type err_counter = 0;
        for (iterator i(test_objects.begin()); i != end; ++i, ++counter) {
          bool failed = false;
          log << banner_log() << "\n" << "Test No. " << counter << std::endl;
          try {
            level.perform(*i, log);
          }
          catch(const OKlib::TestSystem::TestException& e) {
            ++err_counter;
            failed = true;
            log.flush();
            err << e << std::endl;
            return_value = EXIT_FAILURE;
          }
          log << "Test No. " << counter << " ";
          if (failed)
            log << "FAILED";
          else
            log << "SUCCEEDED";
          log <<"\n" << banner_log() << "\n\n";
        }
        log.flush();

        {
          messages << err_counter << " error";
          if (err_counter != 1) messages << "s";
          messages << ".\n";
        }
        {
          const size_type& size(test_objects.size());
          messages << size << " testobject";
          if (size != 1) messages << "s";
          messages << ". \n";
        }
        messages << ::OKlib::TestSystem::Documentation::TestLevelDescriptions(level) << "\n";
        messages << "\nElapsed system time: " << timer.elapsed() << "s\n";
        messages << "Elapsed total time: " << double(total_time) << "s\n";
        messages << TimeHandling::currentDateTime("%A, %B %e, %Y; %H:%M:%S%n");
        messages << "\nOKlib::TestSystem::RunTest " << "\n" << banner_messages() << std::endl;
        return return_value;
      }

    private :

      static std::string banner_log() {
        return std::string(40, '#');
      }
      static std::string banner_messages() {
        return std::string(40, '+');
      }
    };

  }
  
}

#endif

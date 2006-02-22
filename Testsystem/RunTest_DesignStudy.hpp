// Oliver Kullmann, 16.2.2006 (Swansea)

/*!
  \file RunTest_DesignStudy.hpp
  \brief Components for running tests
*/

#ifndef RUNTESTDESIGNSTUDY_kkJJ6t

#define RUNTESTDESIGNSTUDY_kkJJ6t

#include <ostream>
#include <string>

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/timer.hpp>

#include <Transitional/General/TimeHandling.hpp>

#include <Transitional/Testsystem/TestFondement.hpp>
#include <Transitional/Testsystem/TestExceptions_DesignStudy.hpp>
#include <Transitional/Testsystem/TestLevel_Explanations.hpp>

namespace OKlib {

  namespace TestSystem {

    class RunTest {
      RunTest(const RunTest&); // not available
      RunTest& operator =(const RunTest&); // not available

    public :
      typedef ::OKlib::TestSystem::Test test_type;
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

      RunTest(::OKlib::TestSystem::Test* const test) {
        // takes over ownership of test object
        handle_test_objects(test);
      }

      static int run_tests(std::ostream& err, std::ostream& messages, std::ostream& log, ::OKlib::TestSystem::TestLevel& level) {
        return run_tests(err, messages, log, level, handle_test_objects());
      }
        
      static int run_tests(std::ostream& err, std::ostream& messages, std::ostream& log, ::OKlib::TestSystem::TestLevel& level, container_type& test_objects) {

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
            i -> perform(level, log);
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

// Oliver Kullmann, 16.2.2006 (Swansea)

/*!
  \file RunTest_DesignStudy.hpp
  \brief Components for running tests
  \todo Use messages throughout.
*/

#ifndef RUNTESTDESIGNSTUDY_kkJJ6t

#define RUNTESTDESIGNSTUDY_kkJJ6t

#include <boost/timer.hpp>

#include <Transitional/General/TimeHandling.hpp>

#include <Transitional/Testsystem/TestExceptions_DesignStudy.hpp>

#include <Transitional/Testsystem/RunTest_Declarations.hpp>
#include <Transitional/Testsystem/RunTest_Messages.hpp>

namespace OKlib {

  namespace TestSystem {

    inline ::OKlib::TestSystem::RunTest::container_type& ::OKlib::TestSystem::RunTest::handle_test_objects(const test_pointer_type p) {
      static container_type test_objects;
      if (p)
        test_objects.push_back(p);
      return test_objects;
    }

    inline ::OKlib::TestSystem::RunTest::RunTest(::OKlib::TestSystem::Test* const test) {
      // takes over ownership of test object
      handle_test_objects(test);
    }

    inline int ::OKlib::TestSystem::RunTest::run_tests(std::ostream& err, std::ostream& messages, std::ostream& log, ::OKlib::TestSystem::TestLevel& level) {
      return run_tests(err, messages, log, level, handle_test_objects());
    }
        
    inline int ::OKlib::TestSystem::RunTest::run_tests(std::ostream& err, std::ostream& messages, std::ostream& log, ::OKlib::TestSystem::TestLevel& level, container_type& test_objects) {

      messages << ::OKlib::TestSystem::Documentation::RunTestOpening(level, test_objects.size());

      int return_value = 0;
      boost::timer timer;
      TimeHandling::WallTime total_time;
      
      typedef container_type::iterator iterator;
      const iterator& end(test_objects.end());
      typedef container_type::size_type size_type;
      size_type counter = 1;
      size_type err_counter = 0;
      for (iterator i(test_objects.begin()); i != end; ++i, ++counter) {
        bool failed = false;
        log << banner_log() << "\n" << "Test No. " << counter << std::endl; // MESSAGES
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
        log << "Test No. " << counter << " "; // MESSAGES
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

    inline std::string OKlib::TestSystem::RunTest::banner_log() {
      return std::string(40, '#');
    }
    inline std::string OKlib::TestSystem::RunTest::banner_messages() {
      return std::string(40, '+');
    }

  }

}

#endif

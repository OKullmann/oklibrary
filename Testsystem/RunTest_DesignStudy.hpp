// Oliver Kullmann, 16.2.2006 (Swansea)

/*!
  \file Testsystem/RunTest_DesignStudy.hpp
  \brief Components for running tests

  In fact this file contains only the implementation of the class TestSystem::RunTest, declared in
  Testsystem/RunTest_Declarations.hpp.
*/

#ifndef RUNTESTDESIGNSTUDY_kkJJ6t
#define RUNTESTDESIGNSTUDY_kkJJ6t

#include <cstdlib> // for EXIT_FAILURE

#include <boost/timer.hpp>

#include <Transitional/General/TimeHandling.hpp>

#include <Transitional/Testsystem/TestExceptions_DesignStudy.hpp>

#include <Transitional/Testsystem/RunTest_Declarations.hpp>
#include <Transitional/Testsystem/messages/RunTest.hpp>

namespace OKlib {

  namespace TestSystem {

    inline ::OKlib::TestSystem::RunTest::container_type& ::OKlib::TestSystem::RunTest::handle_test_objects(const test_pointer_type p) {
      static container_type test_objects;
      if (p) test_objects.push_back(p);
      return test_objects;
    }

    inline ::OKlib::TestSystem::RunTest::RunTest(test_pointer_type const test) {
      handle_test_objects(test);
    }

    inline int ::OKlib::TestSystem::RunTest::run_tests(std::ostream& err, std::ostream& messages, std::ostream& log, ::OKlib::TestSystem::TestLevel& level) {
      return run_tests(err, messages, log, level, handle_test_objects());
    }
        
    inline int ::OKlib::TestSystem::RunTest::run_tests(std::ostream& err, std::ostream& messages, std::ostream& log, ::OKlib::TestSystem::TestLevel& level, container_type& test_objects) {

      messages << ::OKlib::TestSystem::messages::RunTestOpening(level, test_objects.size());

      int return_value = 0;
      boost::timer timer;
      TimeHandling::WallTime total_time;

      // loop through all test objects:
      typedef container_type::size_type size_type;
      size_type err_counter = 0;
      {
        typedef container_type::iterator iterator;
        const iterator& end(test_objects.end());
        size_type counter = 1;
        for (iterator i(test_objects.begin()); i != end; ++i, ++counter) {
          bool failed = false;
          messages << ::OKlib::TestSystem::messages::SingleTestOpening(counter);
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
          messages << ::OKlib::TestSystem::messages::SingleTestClosing(counter, failed);
        }
      }

      log.flush();      
      messages << ::OKlib::TestSystem::messages::RunTestClosing(level, err_counter, test_objects.size(), timer.elapsed(), double(total_time));

      return return_value;
    }

  }

}

#endif

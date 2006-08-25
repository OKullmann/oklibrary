// Oliver Kullmann, 25.2.2006 (Swansea)

/*!
  \file Testsystem/RunTest_Declarations.hpp
  \brief Declarations for ::OKlib::TestSystem::RunTest

  With this class tests are first inserted into a global container, and then executed.
*/

#ifndef RUNTESTDECLARATIONS_jJJhhBc5
#define RUNTESTDECLARATIONS_jJJhhBc5

#include <ostream>
#include <string>

#include <boost/ptr_container/ptr_list.hpp>

#include <Transitional/Testsystem/TestFondement.hpp>

namespace OKlib {

  namespace TestSystem {

    /*!
      \class RunTest
      \brief Responsible for running tests

      Objects of type RunTest are initialised with a pointer to a TestSystem::Test object,
      and the pointer is put into a global container.
      Via the static member function RunTest::run_test all these tests are executed.
    */

    class RunTest {
      RunTest(const RunTest&); // not available
      RunTest& operator =(const RunTest&); // not available

    public :

      typedef ::OKlib::TestSystem::Test test_type;
      typedef Test* test_pointer_type;
      typedef boost::ptr_list<test_type> container_type;

      RunTest(::OKlib::TestSystem::Test* const test);

      static int run_tests(std::ostream& err, std::ostream& messages, std::ostream& log, ::OKlib::TestSystem::TestLevel& level);
      static int run_tests(std::ostream& err, std::ostream& messages, std::ostream& log, ::OKlib::TestSystem::TestLevel& level, container_type& test_objects);

    private :

      static container_type& handle_test_objects(const test_pointer_type p = 0);

      static std::string banner_log(); 
      static std::string banner_messages();
    };

  }

}

#endif

// Oliver Kullmann, 5.12.2005

/*!
  \file TestBaseClass_DesignStudy.hpp
  \brief Design studies for the new test hierarchy.
  \todo All tests should derive from Test (so that they can be reused)!
  An independent (parameterised, see next todo) class RunTest is then responsible
  for the insertion of test objects into a global container via the
  constructor RunTest(Test*). Thus Testobjects contains instead of
  TestingClassTemplate<TestedClassTemplate> test;
  now
  RunTest(new TestingClassTemplate<TestedClassTemplate>) test;
  where RunTest is a typedef for RunTests<typename T> as discussed
  in the next item.
  \todo Perhaps we should use RunTest<n> ?
  Then the test system could test itself (using RunTest<0> and RunTest<1>).
  Perhaps one should use RunTest<T> for a typename T --- then the name
  of T could provide the meaning. But likely we should be able to
  register the instances of RunTest, and then numbers are easier? Perhaps
  this registration needs to happen at run time? Too complicated.
  We need a type which can be defined only once. Better a value ---
  a function pointer, exploiting the ODR?! Perhaps this "registration function"
  yields the name of the testsystem instance, and there is a default
  registration function. The default registration function is defined
  in a .cpp file in module Testsystem (declared in TestBaseClass.hpp).
  \todo Three streams: error, messages, log.
  Every output is assigned to exactly one of these three streams. It's the users responsibility
  to provide appropriate synchronisation between these three streams.
  For the log messages and the normal messages there are member functions in Test,
  using appropriate macro support adding automatically file name, line number
  etc. (perhaps just like OKLIB_TESTDESCRIPTION).
  There is a RAII class, by which the output of log or normal messages can be turned off
  (if for example we call a test function very often). For all these messages we
  should use the Messages module, and then there are different levels of
  verbosity.
  \todo For convenience three standard instantiations of the three streams are provided:
   - normal run:
     error = std::cerr, messages = log = std::cout, normal messages on, log messages off
   - nightly run:
     error = error-file, messages = log = log-file, normal messages on, log messages on,
     error is copied to log-file
   - normal run with file storage:
     error = error-file, messages = std::cout, log = log-file, normal messages on, log messages on,
     error is copied to std::cerr and log_file, messages is copied to log_file.
   To implement these, we must investigate the standard iostream and buffer classes, and the
   boost iostreams library.
*/

#ifndef TESTBASECLASSTEMPORARY_8uXXzs

#define TESTBASECLASSTEMPORARY_8uXXzs

#include <cstdlib>
#include <exception>
#include <cassert>

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/timer.hpp>

#include "TimeHandling.hpp"

#include "TestExceptions_DesignStudy.hpp"

namespace OKlib {

  namespace TestSystem {

    class Basic;

    struct Test {
      virtual ~Test() {}
      template <class TestLevel>
      void perform(TestLevel, std::ostream& log) {
        perform_(TestLevel(), log);
      }
      
    private :
      virtual void perform_(Basic, std::ostream& log) = 0;
    };

    // ###################################################

    struct TestLevel {
      virtual void perform(Test& test, std::ostream& log) const = 0;
      virtual const char* description() const = 0;
      virtual ~TestLevel() {}
    };

    struct Basic : TestLevel {
      void perform(Test& test, std::ostream& log) const {
        test.perform(Basic(), log);
      }
      const char* description() const { return "Basic test level"; }
    };
    struct Full : Basic {
      void perform(Test& test, std::ostream& log) const {
        test.perform(Full(), log);
      }
      const char* description() const { return "Full test level"; }
    };
    struct Extensive : Full {
      void perform(Test& test, std::ostream& log) const {
        test.perform(Extensive(), log);
      }
      const char* description() const { return "Extensive test level"; }
    };

    inline const TestLevel* test_level(const char* const level_description) {
      switch (level_description[0]) {
      case 'f' : return new Full;
      case 'e' : return new Extensive;
      default : return new Basic;
      }
    }

    // ###################################################

    template <class TestFunction>
    class TestBase : public Test {
    protected :
      typedef TestFunction test_type;
    private :
      void perform_(Basic, std::ostream& log) {
        perform_and_catch(Basic(), log);
      }
      template <class TestLevel>
      void perform_and_catch(TestLevel, std::ostream& log) {
        typedef TestLevel level_type;
        try {
          test(TestLevel(), log);
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
      static container_type test_objects_default;

      enum Modes { insert, extract };
      static void handle_test_objects(const Modes mode, const test_pointer_type p = 0) {
        static container_type test_objects;
        switch (mode) {
        case insert :
          test_objects.push_back(p);
          return;
        case extract :
          test_objects.swap(test_objects_default);
        }
      }

    public :

      RunTest(Test* const test) {
        // takes over ownership of test object
        handle_test_objects(insert, test);
      }

      static int run_tests(std::ostream& err, std::ostream& messages, std::ostream& log, const TestLevel* const level) {
        handle_test_objects(extract);
        return run_tests(err, messages, log, level, test_objects_default);
      }
        
      static int run_tests(std::ostream& err, std::ostream& messages, std::ostream& log, const TestLevel* const level, container_type& test_objects) {
        // does not take ownership of level object
        assert(level);

        messages << "OKlib::TestSystem::RunTest " << "\n";
        messages << TimeHandling::currentDateTime("%A, %B %e, %Y; %H:%M:%S%n");
        messages << level -> description() << "\n";
        typedef container_type::size_type size_type;
        {
          const size_type& size(test_objects.size());
          messages << size << " testobject";
          if (size != 1) messages << "s";
          messages << ".\n";
        }
        messages << std::endl;

        int return_value = 0;
        boost::timer timer;
        TimeHandling::WallTime total_time;

        typedef container_type::iterator iterator;
        const iterator& end(test_objects.end());
        size_type counter = 1;
        for (iterator i(test_objects.begin()); i != end; ++i, ++counter) {
          log << "No. " << counter << std::endl;
          try {
            level -> perform(*i, log);
          }
          catch(const OKlib::TestSystem::TestException& e) {
            err << e << std::endl;
            return_value = EXIT_FAILURE;
          }
        }

        messages << "\nElapsed system time: " << timer.elapsed() << "s\n";
        messages << "Elapsed total time: " << double(total_time) << "s\n";
        return return_value;
      }
    };

  }
  
}

#endif

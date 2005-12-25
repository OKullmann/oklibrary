// Oliver Kullmann, 5.12.2005

/*!
  \file TestBaseClass_DesignStudy.hpp
  \brief Design studies for the new test hierarchy.
  \todo Perhaps we should use Test<n> and TestBase<n> ?
  Then the test system could test itself (using Test<0> and Test<1>).
  Perhaps one should use Test<T> for a typename T --- then the name
  of T could provide the meaning. But likely we should be able to
  register the instances of Test, and then numbers are easier? Perhaps
  this registration needs to happen at run time? Too complicated.
  We need a type which can be defined only once. Better a value ---
  a function pointer, exploiting the ODR?! Perhaps this "registration function"
  yields the name of the testsystem instance, and there is a default
  registration function. The default registration function is defined
  in a .cpp file in module Testsystem (declared in TestBaseClass.hpp).
  Every module then contains a TestIncludes_Module.cpp file, which
  gathers all the the testobject files. By linking them together and providing
  one main-function (which calls the run_tests_default or what else is needed)
  we can run all tests together from a variety of modules. If Test is a template,
  then using it in different translation units does not violate the ODR.
  \todo Three streams: error, messages, log.
  error and messages as now (exceptions thrown, module names and basic
  statistics). For the log messages there is a member function in Test;
  using appropriate macro support automatically the file name, line number
  etc. are included in the log message (perhaps just like OKLIB_TESTDESCRIPTION).
  There is a RAII class, by which the output of log messages can be turned off
  (if for example we call a test function very often). For all these messages we
  should use the Messages module, and then there are different levels of
  verbosity. By default error ist std::cerr, while messages and log are std::cout;
  furthermore by default log messages are turned off.
*/

#ifndef TESTBASECLASSTEMPORARY_8uXXzs

#define TESTBASECLASSTEMPORARY_8uXXzs

namespace OKlib {

  namespace Testsystem {

    class Test;

    struct Basic {
      virtual ~Basic() {}
      static void test(Test* const t) {
        t.test_basic();
      }
      virtual void test_dynamic(Test* const t) const {
        t.test(Basic);
      }
    };
    struct Full : Basic {
      static void test(Test* const t) {
        t.test_full();
      }
      void test_dynamic(Test* const t) const {
        t.test(Full);
      }
    };
    struct Extensive : Full {
      static void test(Test* const t) {
        t.test_extensive();
      }
      void test_dynamic(Test* const t) const {
        t.test(Extensive);
      }
    };

    class Test {
    public :
      virtual ~Test() {}

      typedef Test test_type;

      template <class Level>
      void test(Level) {
        try {
          Level::test(this);
        }
        catch(const TestException&) {
          throw;
        }
        catch(const std::exception& e) {
          TestException e_new(std::string("std::exception: what = ") + e.what() + "\ntype = " + typeid(e).name());
          e_new.add(OKLIB_TESTDESCRIPTION);
          throw e_new;
        }
        catch(...) {
          TestException e("exception ...\n ");
          e.add(OKLIB_TESTDESCRIPTION);
          throw e;
        }
      }

      void test_dynamic(const Basic& b) {
        b.test_dynamic(this);
      }

    private :

      virtual void test_basic() = 0;
      virtual void test_full() { test_basic(); }
      virtual void test_extensive() { test_full(); }

    };

    // ###################################################

    class TestBase : public Test {

      typedef std::list<TestBase*> List;
      // List does not take ownership of the elements pointed to by its members.
      static List test_list;
      mutable List::iterator it;
      mutable bool inserted;

      TestBase (const TestBase&); // not available
      TestBase& operator =(const TestBase&); // not available

    protected :

      void insert(TestBase* const p) const {
        test_list.push_back(p);
        it = --test_list.end();
        inserted = true;
      }

    public :

      template <class Level>
      static int run_tests(std::ostream& out, std::ostream& err, Level l) {
        out << "\nrun_tests_default:\n\n";
        int return_value = 0;
        boost::timer timer;
        for (List::iterator i = test_list.begin(); i != test_list.end(); ++i) {
          try {
            (*i) -> test(l);
          }
          catch (const TestException& e) {
            err << e;
            return_value = EXIT_FAILURE;
          }
        }
        const double elapsed(timer.elapsed());
        out << "\nElapsed: " << elapsed << "s\n";
        const List::size_type& size(test_list.size());
        out << size << " testobject";
        if (size != 1) out << "s";
        out << ".\n";
        return return_value;
      }

      TestBase() : inserted(false) {}

      virtual ~TestBase() {
        if (inserted)
          test_list.erase(it);
        // does not throw
      }

      

    };
    TestBase::List TestBase::test_list;
 
  }


  }

}

#endif

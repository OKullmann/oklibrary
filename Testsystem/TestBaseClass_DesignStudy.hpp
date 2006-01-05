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

#endif

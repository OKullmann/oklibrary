// Oliver Kullmann, 5.12.2005

/*!
  \file TestBaseClass_DesignStudy.hpp
  \brief Design studies for the new test hierarchy.
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

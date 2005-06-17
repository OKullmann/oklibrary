// Oliver Kullmann, 8.5.2005 (Swansea)

#ifndef TESTBASECLASS_kkLLkbV5I

#define TESTBASECLASS_kkLLkbV5I

#include <vector>
#include <exception>
#include <string>
#include <list>
#include <ostream>
#include <cstdlib>

#include <boost/timer.hpp>

#include "TestExceptions.hpp"

namespace OKlib {

  namespace TestSystem {

    typedef double TestSingleParameter;
    typedef std::vector<TestSingleParameter> TestParameter;

    class Test {
    public :
      virtual ~Test() {}
      typedef Test test_type;
      void perform_test(const TestParameter& P = TestParameter()) {
        // throws only TestException
        try {
          if (P.empty())
            perform_test_trivial();
          else
            perform_test_nontrivial(P);
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
    private :
      virtual void perform_test_trivial() = 0;
      virtual void perform_test_nontrivial(const TestParameter&) {
        perform_test_trivial();
      }
    };


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
      static int run_tests_default(std::ostream& out) {
        out << "\nrun_tests_default:\n\n";
        int return_value = 0;
        boost::timer timer;
        for (List::iterator i = test_list.begin(); i != test_list.end(); ++i) {
          try {
            (*i) -> perform_test();
          }
          catch (const TestException& e) {
            out << e;
            return_value = EXIT_FAILURE;
          }
        }
        const double elapsed(timer.elapsed());
        out << "\nElapsed: " << elapsed << "s\n";
        return return_value;
      }
      TestBase() : inserted(false) {}
      virtual ~TestBase() {
        if (inserted)
          test_list.erase(it);
      }
      // does not throw
    private :
      virtual void perform_test_trivial() = 0;
      virtual void perform_test_nontrivial(const TestParameter&) {
        perform_test_trivial();
      }
    };
    TestBase::List TestBase::test_list;
  
  }

}

#endif

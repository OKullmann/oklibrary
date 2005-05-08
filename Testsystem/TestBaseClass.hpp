// Oliver Kullmann, 8.5.2005 (Swansea)

#ifndef TESTBASECLASS_kkLLkbV5I

#define TESTBASECLASS_kkLLkbV5I

#include <vector>
#include <exception>
#include <string>

#include "TestExceptions.hpp"

namespace TestBaseClass {

  typedef double TestSingleParameter;
  typedef std::vector<TestSingleParameter> TestParameter;

  class TestBase {
  public :
    typedef TestBase test_type;
    void perform_test(const TestParameter& P = TestParameter()) {
      // throws only TestExceptions::TestException
      try {
        if (P.empty())
          perform_test_trivial();
        else
          perform_test_nontrivial(P);
      }
      catch(const TestExceptions::TestException&) {
        throw;
      }
      catch(const std::exception& e) {
        TestExceptions::TestException e_new(std::string("std::exception: what = ") + e.what() + "\ntype = " + typeid(e).name());
        e_new.add(OKLIB_TESTDESCRIPTION);
        throw e_new;
      }
      catch(...) {
        TestExceptions::TestException e("exception ...\n ");
        e.add(OKLIB_TESTDESCRIPTION);
        throw e;
      }
    }
      virtual ~TestBase() {}
    // does not throw
  protected :
    virtual void perform_test_trivial() = 0;
    virtual void perform_test_nontrivial(const TestParameter&) = 0;
  };
}

#endif

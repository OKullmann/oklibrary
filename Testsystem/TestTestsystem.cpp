#include <cassert>
#include <sstream>
#include <iostream> // #######################

#include "TestExceptions.hpp"
#include "TestBaseClass.hpp"

struct Test1 : public TestBaseClass::TestBase {
  typedef Test1 test_type;
  TestExceptions::ErrorDescription d;

  void perform_test_trivial() {
    TestExceptions::TestException e("test_trivial");
    d = OKLIB_TESTDESCRIPTION;
    e.add(d);
    throw e;
  }
  void perform_test_nontrivial(const TestBaseClass::TestParameter& P) {
    TestExceptions::TestException e("test_nontrivial");
    d = OKLIB_TESTDESCRIPTION;
    e.add(d);
    throw e;
  }
  
};


int main() {
  const char* const l1 = "xxx1";
  const char* const f1 = "yyy1";
  const char* const type1 = "zzz1";

  const char* const l2 = "xxx2";
  const char* const f2 = "yyy2";
  const char* const type2 = "zzz2";

  const char* const message = "mmm";

  TestExceptions::TestException e(message);
  assert(e.what() == std::string(message));
  std::string out(__DATE__ ", "__TIME__ "\n");
  out += std::string(message) + "\n";
  {
    std::stringstream s;
    s << e;
    assert(s.str() == out);
  }
  TestExceptions::ErrorDescription d1(f1, l1, type1);
  TestExceptions::ErrorDescription d2(f2, l2, type2);
  e.add(d1);
  assert(e.what() == std::string(message));
  out += std::string(f1) + ": " + l1 + "\n" + type1 + "\n\n";
  {
    std::stringstream s;
    s << e;
    assert(s.str() == out);
  }
  e.add(d2);
  assert(e.what() == std::string(message));
  out += std::string(f2) + ": " + l2 + "\n" + type2 + "\n\n";
  {
    std::stringstream s;
    s << e;
    assert(s.str() == out);
  }

  {
    Test1 test1;
    try {
      test1.perform_test();
    }
    catch(const TestExceptions::TestException& e) {
      std::cerr << e;
      assert(std::string(e.what()) == std::string("test_trivial"));
    }
  }
}


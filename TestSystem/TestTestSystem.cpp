// Oliver Kullmann, 2005 (Swansea)
/* Copyright 2005 - 2007, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TestSystem/TestTestSystem.cpp
  \brief Test of the (old) test system itself (cannot use the old test system).
  \deprecated
*/

#include <cassert>
#include <sstream>
#include <cstdlib>

#include <OKlib/TestSystem/TestExceptions.hpp>
#include <OKlib/TestSystem/TestBaseClass.hpp>

class Test1 : public OKlib::TestSystem::TestBase {
  typedef Test1 test_type;
public :
  bool test_trivial_throws;
  OKlib::TestSystem::ErrorDescription d;
  Test1() : test_trivial_throws(true) {
    insert(this);
  }
private :
  void perform_test_trivial() {
    if (test_trivial_throws) {
      OKlib::TestSystem::TestException e("test_trivial");
      d = OKLIB_TESTDESCRIPTION;
      e.add(d);
      throw e;
    }
  }
  void perform_test_nontrivial(const OKlib::TestSystem::TestParameter& P) {
    OKlib::TestSystem::TestException e("test_nontrivial");
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

  OKlib::TestSystem::TestException e(message);
  assert(e.what() == std::string(message));
  std::string out(__DATE__ ", "__TIME__ "\n");
  out += std::string(message) + "\n";
  {
    std::stringstream s;
    s << e;
    assert(s.str() == out);
  }
  OKlib::TestSystem::ErrorDescription d1(f1, l1, type1);
  OKlib::TestSystem::ErrorDescription d2(f2, l2, type2);
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

  OKlib::TestSystem::ErrorDescription d;
  {
    Test1 test1;
    OKlib::TestSystem::TestBase& test = test1;
    try {
      test.perform_test();
    }
    catch(const OKlib::TestSystem::TestException& e) {
      std::stringstream s;
      s << e;
      assert(std::string(e.what()) == std::string("test_trivial"));
      std::stringstream s2;
      s2 << __DATE__  ", " __TIME__ "\n" "test_trivial" "\n";
      d = test1.d;
      s2 << test1.d << "\n";
      assert(s.str() == s2.str());
    }
    try {
      test1.test_trivial_throws = false;
      test1.perform_test(OKlib::TestSystem::TestParameter(1));
    }
    catch(const OKlib::TestSystem::TestException& e) {
      std::stringstream s;
      s << e;
      assert(std::string(e.what()) == std::string("test_nontrivial"));
      std::stringstream s2;
      s2 << __DATE__  ", " __TIME__ "\n" "test_nontrivial" "\n";
      s2 << test1.d << "\n";
      assert(s.str() == s2.str());
    }
  }

  {
    Test1 test1; Test1 test2;
    std::stringstream s;
    assert(OKlib::TestSystem::TestBase::run_tests_default(s) == EXIT_FAILURE);
    std::stringstream s2;
    s2 << "\nrun_tests_default:\n\n";
    s2 << __DATE__  ", " __TIME__ "\n" "test_trivial" "\n";
    s2 << d << "\n";
    s2 << __DATE__  ", " __TIME__ "\n" "test_trivial" "\n";
    s2 << d << "\n";
    std::stringstream s2alt; s2alt << s2.str();
    s2 << "\nElapsed: 0s\n2 testobjects.\n";
    s2alt << "\nElapsed: 0.01s\n2 testobjects.\n";
    assert(s.str() == s2.str() or s.str() == s2alt.str());
  }
}

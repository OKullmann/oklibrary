// Oliver Kullmann, 8.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TestSystem/TestBaseClass.hpp
  \brief Base classes Test and TestBase, from which all test classes
  are to be derived (Test without auto-insertion, TestBase with).
  \deprecated
*/

#ifndef TESTBASECLASS_kkLLkbV5I
#define TESTBASECLASS_kkLLkbV5I

#include <vector>
#include <exception>
#include <string>
#include <list>
#include <ostream>
#include <cstdlib>

#include <boost/timer.hpp>

#include <OKlib/TestSystem/TestExceptions.hpp>

namespace OKlib {

  namespace TestSystem {

    typedef double TestSingleParameter;
    typedef std::vector<TestSingleParameter> TestParameter;

    /*!
      \class Test
      \brief Root of the test class hierarchy. From this class test classes are to be
      derived, which are used by other test classes.

      One public member function perform_test(P), where P is of type
      TestParameter; delegates to private virtual member function perform_test_trivial(P),
      and then, if P is non-empty, to perform_test_nontrivial(P).
    */

    class Test {
    public :
      virtual ~Test() {}
      typedef Test test_type;
      void perform_test(const TestParameter& P = TestParameter()) {
        // throws only TestException
        try {
          perform_test_trivial();
          if (not P.empty())
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
      virtual void perform_test_nontrivial(const TestParameter&) {};
    };

    /*!
      \class TestBase
      \brief Base class for tests with auto-insertion. Derived from Test; from this class test
      classes are to be derived, which shall be executed on their own.

      Adds protected member function insert (for the self-insertion of the test object)
      and static member function run_tests_default.
      \todo Use Messages.
    */

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

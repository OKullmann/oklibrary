// Oliver Kullmann, 8.5.2005 (Swansea)

/*!
  \file TestBaseClass.hpp
  \brief Base classes Test and TestBase, from which all test classes
  are to be derived (Test without auto-insertion, TestBase with).
  \todo Two main weaknesses currently:
   - Even is just one submodule changed, the whole testprogram
     has to be recompiled.
   - Test functions (derived from TestBase) cannot be reused.
   First problem: "Module_Testobjects.hpp" shall become "Module_Testobjects.cpp",
   compiled independently. There is only one TestProgram.cpp in OKlib/TestSystem,
   which for building a test program is linked with all the *new* object-files in some
   appropriate module (and below; executable is called "TestProgram_Module").
   We must also investigate the possibilities for dynamic link libraries, so that
   we can avoid (if necessary) gigantic test program files at higher levels (copying
   again and again the same object files).
   Second problem: All test functions just derive from Test, and creating a static test
   object happens via a class RunTest (having a constructor RunTest(Test*)).
  \todo The TestParameter shall vanish; instead we have a tag hierarchy
  Basic -> Full -> Extensiv, with the meaning:
   - Basic : "permanent building"
   - Full : "nightly build"
   - Extensive : "weekly build".
  \todo Normal output to std::cout, error messages to std::cerr (thus 2 streams,
  as in Aeryn); optionally also log messages, to a third stream (copying
  also the normal output).
  Definitely for levels full and extensive we need logging facilities.
  \todo We need the possibility to obtain as much information about the runs of the tests as possible if we wish so.
  \todo With the three levels then also the timestamps test and testop each
  exists in three versions.
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

#include "TestExceptions.hpp"

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

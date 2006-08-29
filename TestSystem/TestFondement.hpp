// Oliver Kullmann, 15.2.2006 (Swansea)

/*!
  \file TestSystem/TestFondement.hpp
  \brief The abstract base classes for the test system (only for internal purposes)

  Provides the abstract base classes TestSystem::Test and TestSystem::TestLevel:

  Given a (test) object t of type TestSystem::Test and a (level) object l of type TestSystem::TestLevel,
  via t.(l, log_stream) the test at level l is performed, with log-messages output to log_stream.

  Technics:
   - The polymorphic double-dispatch (on t and l) is realised via the visitor design pattern.
   - The TestSystem::TestLevel hierarchy is equipped with a generic visitor facility itself.
   - Singleton objects are provided for the classes derived from TestSystem::TestLevel.
*/

#ifndef TESTFONDEMENT_bbrq28
#define TESTFONDEMENT_bbrq28

#include <cassert>

namespace OKlib {

  namespace TestSystem {

    class TestLevel; class Basic; class Full; class Extensive;

    /*!
      \class Test
      \brief The abstract base class of the (polymorphic) test hierarchy; containers of testobjects are
      containers of pointers to objects of type Test.

      The (only public) member function Test::perform is to be called with a (polymorphic) test-level object
      and a log-stream.

      The private overloaded member functions "perform_" are to be called by a "visitor" from the 
      TestLevel-hierarchy. For every test-level type which shall be passed to derived classes an overload
      of "perform_" needs to be available (otherwise the closest match with existing versions of
      perform_ is used).
    */

    struct Test {

      virtual ~Test() {}
      void perform(const TestLevel& level, std::ostream& log);
      // implemented below as { level.perform(*this, log); }

    private :

      friend class Basic;
      virtual void perform_(Basic, std::ostream&) = 0;
      friend class Full;
      virtual void perform_(Full, std::ostream&) = 0;
      friend class Extensive;
      virtual void perform_(Extensive, std::ostream&) = 0;
    };

    // ###################################################

    /*!
      \class VisitorTestLevel
      \brief Abstract base class for generic visitors (helper classes) for TestLevel (to enable
      "polymorphic switches" on objects of type TestLevel).

      If some action is to be performed on polymorphic objects of type TestLevel,
      then derive from VisitorTestLevel a class performing the desired action in
      the appropriate bracket-operator member function.
    */

    struct VisitorTestLevel {
      virtual void operator()(Basic) const = 0;
      virtual void operator()(Full) const = 0;
      virtual void operator()(Extensive) const = 0;
      virtual ~VisitorTestLevel() {}
    };

    /*!
      \class TestLevel
      \brief Abstract base class of the (polymorphic) test-level hierarchy.

      - Objects of type TestLevel are visitors for the TestSystem::Test hierarchy; the TestLevel::perform
        member function is used to call the right overload of TestSystem::Test::perform_.
      - By the TestLevel::operator()(const VisitorTestLevel&) member function a visitor can be employed.
    */

    struct TestLevel {
      virtual void perform(Test& test, std::ostream& log) const = 0;
      virtual ~TestLevel() {}
      virtual void operator()(const VisitorTestLevel&) const = 0;
    };

    inline void Test::perform(const TestLevel& level, std::ostream& log) {
      assert(&level);
      level.perform(*this, log);
    }

    struct Basic : TestLevel {
      void perform(Test& test, std::ostream& log) const  {
        test.perform_(Basic(), log);
      }
      void operator()(const VisitorTestLevel& vis) const { vis(*this); }
    };
    struct Full : Basic {
      void perform(Test& test, std::ostream& log) const {
        test.perform_(Full(), log);
      }
      void operator()(const VisitorTestLevel& vis) const { vis(*this); }
    };
    struct Extensive : Full {
      void perform(Test& test, std::ostream& log) const {
        test.perform_(Extensive(), log);
      }
      void operator()(const VisitorTestLevel& vis) const { vis(*this); }
    };

    /*!
      \brief Factory for static test-level objects
    */

    template <class TestLevelDerived>
    inline TestLevel& test_level(TestLevelDerived) {
      static TestLevelDerived d;
      return d;
    }

  }

}

#endif

// Oliver Kullmann, 15.2.2006 (Swansea)

/*!
  \file TestFondement.hpp
  \brief The very base classes for the test system
*/

#ifndef TESTFONDEMENT_bbrq28

#define TESTFONDEMENT_bbrq28

#include <cassert>

namespace OKlib {

  namespace TestSystem {

    class TestLevel; class Basic; class Full; class Extensive;

    /*!
      \class Test
      \brief The root of the (polymorphic) test hierarchy; containers of testobjects are
      containers of Test pointers.

      The private member function template "perform" is to be called by a "visitor" from the level
      hierarchy. For every test-level type which shall be passed to derived classes an overload
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
      \brief Generic visitor class for TestLevel

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
      \brief The root of the (polymorphic) test-level hierarchy.

      Test level objects are visitors for the Test hierarchy, calling the right
      overload of the perform member function of Test.
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
      \brief Factory for static test level objects
    */

    template <class TestLevelDerived>
    inline TestLevel& test_level(TestLevelDerived) {
      static TestLevelDerived d;
      return d;
    }

  }

}

#endif

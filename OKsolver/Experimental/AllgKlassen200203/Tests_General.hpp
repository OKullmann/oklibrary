// Oliver Kullmann, 3.10.2003 (Swansea)

#ifndef TESTSGENERALWAECHTER_3619gdEQ

#define TESTSGENERALWAECHTER_3619gdEQ

#include <iostream>

#include "Concepts_General.hpp"

#include "Traits_General.hpp"


namespace Tests_General {

  template <class C, class T>
  class Test {
  public :
    typedef C Class;
    typedef T Tag;
    Test() {
      std::cout << "\nTest: EMPTY\n";
      assert(0 == 1);
    }
    void test() {}
  };
  template <class C>
  void RunTest() {
    Test<C, typename Traits_General::MetaData<C>::concept_tag>().test();
  }

  template <class C>
  class Test<C, Concepts_General::Root_tag> {
  public :
    typedef C Class;
    typedef Concepts_General::Root_tag Tag;
    Test() {
      std::cout << "\nTest\nClass " << typeid(C).name() << "\n";
    }
    void test() {}
    virtual ~Test() {};
  };
  // Inherited classes: Only private data!

}


#endif


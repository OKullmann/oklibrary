// Oliver Kullmann, 3.10.2003 (Swansea)
/* Copyright 2003 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef TESTSGENERALWAECHTER_3619gdEQ

#define TESTSGENERALWAECHTER_3619gdEQ

#include <iostream>

#include <Transitional/OKsolver/Experimental/AllgKlassen200203/Concepts_General.hpp>

#include <Transitional/OKsolver/Experimental/AllgKlassen200203/Traits_General.hpp>


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


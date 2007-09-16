// Oliver Kullmann, 15.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <Transitional/TestSystem/TestBaseClass.hpp>

#include <Transitional/Concepts/std_Iterators_Testobjects.hpp>
#include <Transitional/Concepts/std_Basics_Testobjects.hpp>
#include <Transitional/Concepts/Basics_Testobjects.hpp>
#include <Transitional/Concepts/ConceptsMetafunctions_Testobjects.hpp>
#include <Transitional/Concepts/LibraryBasics_Testobjects.hpp>
#include <Transitional/Concepts/Variables_Testobjects.hpp>
#include <Transitional/Concepts/ResultElements_Testobjects.hpp>
#include <Transitional/Concepts/std_Container_Testobjects.hpp>
#include <Transitional/Concepts/RefactoringIncludeHandling_Testobjects.hpp>
#include <Transitional/Concepts/InputOutput_Testobjects.hpp>

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
}

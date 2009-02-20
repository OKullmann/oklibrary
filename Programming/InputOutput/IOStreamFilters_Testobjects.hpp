// Oliver Kullmann, 10.10.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef IOSTREAMFILTERSTESTOBJECTS_gGVVc5
#define IOSTREAMFILTERSTESTOBJECTS_gGVVc5

#include <OKlib/Programming/InputOutput/IOStreamFilters.hpp>
#include <OKlib/Programming/InputOutput/IOStreamFilters_Tests.hpp>

namespace OKlib {
  namespace GeneralInputOutput {

    ::OKlib::GeneralInputOutput::Test_BasicCounter< ::OKlib::GeneralInputOutput::BasicCounter> test_basic_counter;

    ::OKlib::GeneralInputOutput::Test_BasicIndentLines< ::OKlib::GeneralInputOutput::BasicIndentLines> test_basic_indentlines;
    
  }
}

#endif

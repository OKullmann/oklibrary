// Oliver Kullmann, 12.6.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Structures/Sets/SetAlgorithms/SequenceOperations_Tests.hpp
  \brief Tests for sequence operations.

  \todo Completing Test_Sum_sizes:
   - using boost::assign (and a general loop)
   - more test cases.
*/

#ifndef SEQUENCEOPERATIONSTESTS_kavBv4
#define SEQUENCEOPERATIONSTESTS_kavBv4

#include <vector>

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

namespace OKlib {
  namespace SetAlgorithms {

    template <template <class Range> class Sum_sizes>
    class Test_Sum_sizes : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_Sum_sizes test_type;
      Test_Sum_sizes() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        {
          typedef int value_type;
          typedef std::vector<value_type> range_type;
          typedef std::vector<range_type> seq_type;
          seq_type seq;
          { range_type v; v.push_back(1); v.push_back(1); seq.push_back(v); }
          { range_type v; v.push_back(1); seq.push_back(v); }
          { range_type v; v.push_back(0); v.push_back(1); v.push_back(2); seq.push_back(v); }
          typedef Sum_sizes<typename seq_type::iterator> sum_sizes;
          sum_sizes sz;
          OKLIB_TEST_EQUAL(sz(seq.begin(), seq.end()), 2 + 1 + 3);
        }
      }
    };

  }

}

#endif

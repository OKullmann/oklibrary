// Oliver Kullmann, 22.11.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ActiveClauses/InjectivityConstraints/plans/AllDifferent.hpp
  \brief Plans for all-different constraint.


  \todo Finish implementation of InjectivityConstraints::TrivialAllDifferent


  \todo What is the concept for TrivialAllDifferent<PASS> ?
  <ul>
   <li> An "active clauses" ! </li>
   <li> Especially the eval-member-function needs
   an overhaul (see Concepts/plans/ClauseSets.hpp; perhaps it makes
   sense to always have a trivial evaluation?). </li>
   <li> The connection to the partial
   assignment is critical; there are more return-possibilities (at least in
   general; perhaps not for the trivial evaluation).
   Perhaps a reference to phi should be stored at construction. </li>
  </ul>


  \todo See also Concepts/plans/Variables.hpp and Concepts/plans/Literals.hpp.
  (Perhaps the value_type should always be order-comparable?)


  \todo Test InjectivityConstraints::TrivialAllDifferent


  \todo Move the relevant parts from InjectivityConstraints/plans/general.hpp to here.


  \todo Old implementations:
  Perhaps the following tests can be reused?
  \code
#include <set>
#include <algorithm>

#include <boost/range/value_type.hpp>
#include <boost/range/const_iterator.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/size.hpp>

namespace OKlib {
  namespace InjectivityConstraints {

    class AllDifferentBySetSizeComparison {
    public:
      template <typename Range>
      bool operator() (Range R) {
     }
    };

}

#include <vector>
#include <iostream>
#include <cassert>
#include <exception>
#include <list>

#include <OKlib/TestSystem/TestBaseClass_DesignStudy.hpp>
#include <OKlib/TestSystem/TestExceptions_DesignStudy.hpp>
#include <OKlib/InjectivityConstraints/AllDifferent.hpp>

#include <boost/assign/std/vector.hpp> 
#include <boost/assign/std/list.hpp> 
#include <boost/assert.hpp>
#include <boost/range/const_iterator.hpp>

namespace OKlib {
  namespace InjectivityConstraints {
    namespace tests {

      void all_different_vector_adhoc_test() {

        std::vector<int> v,v2;
        using namespace boost::assign;

        v += 1,2;
        v2 += 1,1,2;

        assert(all_different(v));
        assert(! all_different(v2));

      }

      void all_different_list_adhoc_test() {

        std::list<int> l,l1;
        using namespace boost::assign;

        l += 1,2,3;
        l1 += 1,1,2;

        assert(all_different(l));
        assert(! all_different(l1));

      }

      void all_different_adhoc_test() {
        all_different_vector_adhoc_test();
        all_different_list_adhoc_test();
      }

      class TestAllDifferentAdhoc : public ::OKlib::TestSystem::TestBase {
        typedef ::OKlib::TestSystem::TestBase base_type;
      public :
        TestAllDifferentAdhoc() : base_type(__FILE__, __LINE__, typeid(TestAllDifferentAdhoc).name()) {}
      private :
        void test(::OKlib::TestSystem::Basic, std::ostream& log) {
          typedef ::OKlib::TestSystem::Basic level_type;
          all_different_adhoc_test();
        }

      };

      // -------------------------------------------------------------------

      template <class AllDifferent, class Range>
      void all_different_positive_test(Range TestData) {
        typedef typename boost::range_const_iterator<Range>::type iterator;
        const iterator& end(boost::end(TestData));
        for(iterator begin(boost::begin(TestData));begin!=end;++begin) {
          assert(AllDifferent()(*begin));
        }
      }

      template <class AllDifferent, class Range>
      void all_different_negative_test(Range TestData) {
        typedef typename boost::range_const_iterator<Range>::type iterator;
        const iterator& end(boost::end(TestData));
        for(iterator begin(boost::begin(TestData));begin!=end;++begin) {
          assert(!AllDifferent()(*begin));
        }
      }

      struct VectorOfIntPositive {
        std::vector<int> p01,p02,p03,p04,p05,p06,p07,p08,p09,p10;
        std::vector<std::vector<int> > data;

        VectorOfIntPositive() {
          using namespace boost::assign;
          p02 += 1;
          p03 += 1,2;
          p04 += 1,2,3;
          p05 += 1,2,3,4;
          p06 += 1,2,3,4,5;
          p07 += 1,2,3,4,5,6;
          p08 += 1,2,3,4,5,6,7;
          p09 += 1,2,3,4,5,6,7,8;
          p10 += 1,2,3,4,5,6,7,8,9; 
          data += p01,p02,p03,p04,p05,p06,p07,p08,p09,p10;
        }
      };

      struct VectorOfIntNegative {
        std::vector<int> n01,n02,n03,n04,n05,n06,n07,n08,n09,n10,
          n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,
          n21,n22,n23,n24,n25,n26,n27,n28,n29,n30,
          n31,n32,n33,n34;
        std::vector<std::vector<int> > data;
        
        VectorOfIntNegative() {
          using namespace boost::assign;
          
          n01 += 1,1;
          
          n02 += 1,1,1;
          n03 += 1,2,2;
          n04 += 2,1,2;
          n05 += 2,2,1;
          
          n06 += 1,2,2,2;
          n07 += 2,1,2,2;
          n08 += 2,2,1,2;
          n09 += 2,2,2,1;
          
          n10 += 1,1,2,2;
          n11 += 1,2,1,2;
          n12 += 1,2,2,1;
          
          n10 += 1,2,3,3;
          n11 += 1,3,2,3;
          n12 += 1,3,3,2;
          n13 += 3,1,2,3;
          n14 += 3,1,3,2;
          n15 += 3,3,1,2;
          
          n16 += 1,2,2,2,2;
          n17 += 2,1,2,2,2;
          n18 += 2,2,1,2,2;
          n19 += 2,2,2,1,2;
          n20 += 2,2,2,2,1;

          n21 += 1,1,2,2,2;
          n22 += 1,2,1,2,2;
          n23 += 1,2,2,1,2;
          n24 += 1,2,2,2,1;
          n24 += 2,1,1,2,2;
          n25 += 2,1,2,1,2;
          n26 += 2,1,2,2,1;
          n27 += 2,2,1,1,2;
          n28 += 2,2,1,2,1;
          n29 += 2,2,2,1,1;
          n30 += 1,1,2,1,1;
          n31 += 1,1,1,2,1;
          n32 += 1,2,1,1,1;
          n33 += 1,2,2,2,2;
          n34 += 1,1,1,1,2;
          
          data += n01,n02,n03,n04,n05,n06,n07,n08,n09,n10,n11,n12,n13,n14,n15,n16,n17,n18,n19,n20,n21,n22,n23,n24,n25,n26,n27,n28,n29,n30,n31,n32,n33,n34;
        }
      };
      template <class AllDifferent>
      class TestAllDifferent : public ::OKlib::TestSystem::TestBase {
      typedef ::OKlib::TestSystem::TestBase base_type;
      public :
      TestAllDifferent() : base_type(__FILE__, __LINE__, typeid(TestAllDifferent).name()) {}
      private :
      void test(::OKlib::TestSystem::Basic, std::ostream& log) {
        typedef ::OKlib::TestSystem::Basic level_type;
        all_different_positive_test<AllDifferent>(::OKlib::InjectivityConstraints::tests::VectorOfIntPositive().data);
        all_different_negative_test<AllDifferent>(::OKlib::InjectivityConstraints::tests::VectorOfIntNegative().data);
      }
     };
    }
   }
 }

namespace OKlib {
  namespace InjectivityConstraints {
    namespace testobjects {

      ::OKlib::TestSystem::RunTest test_all_different_adhoc(new ::OKlib::InjectivityConstraints::tests::TestAllDifferentAdhoc);
      ::OKlib::TestSystem::RunTest test_all_different_by_set_size_comparison(new ::OKlib::InjectivityConstraints::tests::TestAllDifferent<AllDifferentBySetSizeComparison>);

    }
  }
} 

  \endcode


*/


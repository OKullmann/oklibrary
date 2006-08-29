// Matthew Henderson, 24.4.2006 (Paderborn)

/*!
  \file InjectivityConstraints/testobjects/AllDifferent.cpp
  \brief Objects for testing of algorithms for alldifferent constraint.
*/

#include <Transitional/TestSystem/RunTest_DesignStudy.hpp>
#include <Transitional/InjectivityConstraints/tests/AllDifferent.hpp>

namespace OKlib {
  namespace InjectivityConstraints {
    namespace testobjects {

      ::OKlib::TestSystem::RunTest test_all_different_adhoc(new ::OKlib::InjectivityConstraints::tests::TestAllDifferentAdhoc);
      ::OKlib::TestSystem::RunTest test_all_different_by_set_size_comparison(new ::OKlib::InjectivityConstraints::tests::TestAllDifferent<AllDifferentBySetSizeComparison>);
      ::OKlib::TestSystem::RunTest test_all_different_by_seen_before_vector_find(new ::OKlib::InjectivityConstraints::tests::TestAllDifferent<AllDifferentBySeenBeforeVectorFind>);

    }
  }
} 

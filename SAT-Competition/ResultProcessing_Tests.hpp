// Oliver Kullmann, 18.5.2005 (Swansea)

#ifndef RESULTPROCESSINGTESTS_905Tgb

#define RESULTPROCESSINGTESTS_905Tgb

#include <vector>

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

#include "SingleResult.hpp"

namespace OKlib {

  namespace SATCompetition {

    template <template <typename ResultIterator> class ResultDatabase>
    class Test_ResultDatabase_ResultIterator : public ::OKlib::TestSystem::TestBase {
      void perform_test_trivial() {
        typedef std::vector<Result> VectorResult;
        VectorResult test_vector;

        typedef ResultDatabase<VectorResult::const_iterator> ResultDatabase_vector;
        ResultDatabase_vector db(test_vector.begin(), test_vector.end());

        // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
      }
    public :
      Test_ResultDatabase_ResultIterator() {
        insert(this);
      }
    };

    
  }

}

#endif

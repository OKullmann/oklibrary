// Oliver Kullmann, 7.6.2005 (Swansea)

#ifndef ANALYSISTOOLSTESTS_37yh6fR4

#define ANALYSISTOOLSTESTS_37yh6fR4

#include "SingleResult.hpp"
#include "ParsingSingleResult.hpp"

namespace OKlib {

  namespace SATCompetition {

    template <template <class Database> class ElementaryAnalysis>
    class Test_ElementaryAnalysis : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ElementaryAnalysis test_type;
      Test_ElementaryAnalysis() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        // ToDo: Test for series_in_superseries()
      }
    };

  }

}

#endif

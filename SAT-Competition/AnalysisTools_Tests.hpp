// Oliver Kullmann, 7.6.2005 (Swansea)

#ifndef ANALYSISTOOLSTESTS_37yh6fR4

#define ANALYSISTOOLSTESTS_37yh6fR4

#include <cassert>

#include "SingleResult.hpp"
#include "ParsingSingleResult.hpp"
#include "ParsingResultSequences_Tests.hpp"
#include "ResultProcessing.hpp"

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

        test_result<Result>(filename_large_industrial, line_count_large_industrial);

        // ToDo: Test for series_in_superseries()
      }

      template <class result_type>
      void test_result(const std::string& filename, const int line_count) {
        typedef  Result_database_from_file<ParserResult, result_type> result_database;
        result_database rdb(filename);
        assert(rdb.result_sequence.size() == line_count);
        typedef typename result_database::database_type database;
        typedef ElementaryAnalysis<database> elementary_analysis;
        elementary_analysis ea(rdb.db);

        OKLIB_TEST_EQUAL(ea.series_in_superseries().size(), rdb.db.super_series().size());
        
      }

    };

  }

}

#endif

// Oliver Kullmann, 17.6.2005 (Swansea)

#ifndef SCORINGTESTS_kdkdnvc1

#define SCORINGTESTS_kdkdnvc1

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

#include "AnalysisTools.hpp"
#include "ParsingResultSequences_Tests.hpp"

namespace OKlib {

  namespace SATCompetition {

    template <template <class IndexedDatabase, typename NumberType = double> class  PurseScoring>
    class Test_PurseScoring : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_PurseScoring test_type;
      Test_PurseScoring() {
        insert(this);
      }
    private :

      void perform_test_trivial() {

        test_result<Result>(filename_large_industrial, line_count_large_industrial);
        test_result<ResultRandomSat>(filename_large_random, line_count_large_random);

      }

      template <class result_type>
      void test_result(const std::string& filename, const unsigned int line_count) {

        typedef Result_database_from_file<ParserResult, result_type> result_database;
        typedef typename result_database::database_type database;
        typedef ElementaryAnalysis<database> indexed_database;
        typedef PurseScoring<indexed_database> purse_scoring_type;

        result_database rdb(filename);
        assert(rdb.result_sequence.size() == line_count);
        indexed_database idb(rdb.db);
        purse_scoring_type purse_scoring(idb);

        typedef typename purse_scoring_type::number_type number_type;
        
      }

    };

  }

}

#endif

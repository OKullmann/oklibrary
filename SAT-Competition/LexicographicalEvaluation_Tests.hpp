// Oliver Kullmann, 25.10.2005 (Swansea)

/*!
  \file LexicographicalEvaluation_Tests.hpp
  \brief Tests for tools evaluating an "indexed database" by providing comparison tools.
  \todo Basic tests involving the times are needed.
*/

#ifndef LEXICOGRAPHICALEVALUATIONTESTS_bzklapao0

#define LEXICOGRAPHICALEVALUATIONTESTS_bzklapao0

#include <string>
#include <cassert>

#include <boost/lexical_cast.hpp>

#include "IteratorHandling.hpp"

#include "BasicMapOperations.hpp"

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

#include "SingleResult.hpp"
#include "ResultProcessing.hpp"
#include "ParsingSingleResult.hpp"
#include "AnalysisTools.hpp"
#include "ParsingResultSequences_Tests.hpp"

namespace OKlib {

  namespace SATCompetition {

    template < template <class IndexedDatabase, typename NumberType = double> class LexicographicalEvaluation >
    class Test_LexicographicalEvaluation : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_LexicographicalEvaluation test_type;
      Test_LexicographicalEvaluation() {
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
        typedef LexicographicalEvaluation<indexed_database> lexicographical_evaluation_type;
        typedef typename lexicographical_evaluation_type::size_type size_type;
        typedef typename lexicographical_evaluation_type::number_type number_type;
        typedef typename lexicographical_evaluation_type::map_solver_evaluation_for_series_type map_solver_evaluation_for_series_type;
        typedef typename lexicographical_evaluation_type::numerics_solver_on_series_type numerics_solver_on_series_type;

        result_database rdb(filename);
        assert(rdb.result_sequence.size() == line_count);
        indexed_database idb(rdb.db);
        lexicographical_evaluation_type lexicographical_evaluation(idb);

        typedef typename indexed_database::map_solver_benchmarks map_solver_benchmarks;
        typedef typename IteratorHandling::IteratorFirst<typename map_solver_benchmarks::const_iterator>::type iterator;
        const map_solver_benchmarks& map(idb.solved_benchmarks());
        const iterator end(map.end());
        for (iterator i(map.begin()); i != end; ++i) { // loop over all solvers
          const Solver& solver(*i);
          
          const map_solver_evaluation_for_series_type& map2(lexicographical_evaluation.evaluation(solver));
          OKLIB_TEST_EQUAL(map2.size(), OKlib::SetAlgorithms::map_value(idb.solved_series(), solver).size());
          size_type sum = 0;
          typedef typename map_solver_evaluation_for_series_type::const_iterator iterator;
          const iterator& end(map2.end());
          for (iterator j(map2.begin()); j != end; ++j) {
            const numerics_solver_on_series_type& n(j -> second);
            if (n.first <= 0)
              OKLIB_THROW("n.first <= 0, namely n.first = " + boost::lexical_cast<std::string>(n.first));
            sum += n.first;
            if (n.second < 0)
              OKLIB_THROW("n.second < 0, namely n.second = " + boost::lexical_cast<std::string>(n.second));
          }
          if (sum != OKlib::SetAlgorithms::map_value(map, solver).size())
            OKLIB_THROW("sum != OKlib::SetAlgorithms::map_value(map, solver).size(), namely sum = " +  boost::lexical_cast<std::string>(sum) + ", while map_value(map, solver).size() = " + boost::lexical_cast<std::string>(OKlib::SetAlgorithms::map_value(map, solver).size()) + "\nContext: solver = " + boost::lexical_cast<std::string>(solver));
          OKLIB_TEST_EQUAL(sum, OKlib::SetAlgorithms::map_value(map, solver).size());
        }
                
      }

    };

  }

}

#endif

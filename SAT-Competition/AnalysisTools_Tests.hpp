// Oliver Kullmann, 7.6.2005 (Swansea)

#ifndef ANALYSISTOOLSTESTS_37yh6fR4

#define ANALYSISTOOLSTESTS_37yh6fR4

#include <cassert>
#include <iterator>
#include <algorithm>
#include <iostream> // ####################

#include <boost/iterator/transform_iterator.hpp>
#include <boost/range/iterator_range.hpp>

#include "FunctionHandling.hpp"
#include "BasicSetOperations.hpp"
#include "SequenceOperations.hpp"

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

      }

      template <class result_type>
      void test_result(const std::string& filename, const unsigned int line_count) {

        typedef  Result_database_from_file<ParserResult, result_type> result_database;
        result_database rdb(filename);
        assert(rdb.result_sequence.size() == line_count);
        typedef typename result_database::database_type database;
        typedef ElementaryAnalysis<database> elementary_analysis;
        elementary_analysis ea(rdb.db);

        typedef typename elementary_analysis::SpecSeries SpecSeries;

        typedef typename elementary_analysis::map_superseries_series map_superseries_series;
        typedef typename elementary_analysis::map_series_benchmarks map_series_benchmarks;
        typedef typename elementary_analysis::map_benchmark_series map_benchmark_series;
        typedef typename elementary_analysis::seq_series seq_series;
        typedef typename elementary_analysis::seq_benchmarks seq_benchmarks;

        { // testing ea.series_in_superseries()
          {
            typedef FunctionHandling::First<typename map_superseries_series::value_type> first1;
            typedef FunctionHandling::First<MapSuperSeries::value_type> first2;
            OKLIB_TEST_EQUAL_RANGES(
                                    boost::make_iterator_range(boost::make_transform_iterator<first1>(ea.series_in_superseries().begin()), (boost::make_transform_iterator<first1>(ea.series_in_superseries().end()))),
                                    boost::make_iterator_range(boost::make_transform_iterator<first2>(rdb.db.super_series().begin()), boost::make_transform_iterator<first2>(rdb.db.super_series().end())));
          }
          {
            typedef FunctionHandling::Second<typename map_superseries_series::value_type> second;
            seq_series s;
            OKlib::SetAlgorithms::union_sets(boost::make_transform_iterator<second>(ea.series_in_superseries().begin()), boost::make_transform_iterator<second>(ea.series_in_superseries().end()), std::back_inserter(s));
            typedef FunctionHandling::First<MapSeries::value_type> first;
            OKLIB_TEST_EQUAL_RANGES(s, boost::make_iterator_range(boost::make_transform_iterator<first>(rdb.db.series().begin()), boost::make_transform_iterator<first>(rdb.db.series().end())));
          }
        }

       { // testing ea.benchmarks_in_series()
         typedef FunctionHandling::Second<typename map_series_benchmarks::value_type> second;
         typedef boost::transform_iterator<second, typename map_series_benchmarks::const_iterator> transform_iterator;
         const map_series_benchmarks& map(ea.benchmarks_in_series());
         const transform_iterator& begin(transform_iterator(map.begin()));
         const transform_iterator& end(transform_iterator(map.end()));
         OKLIB_TEST_EQUAL(OKlib::SetAlgorithms::sum_sizes(begin, end), rdb.db.benchmark().size());
         seq_benchmarks seq;
         OKlib::SetAlgorithms::union_sets(begin, end, std::back_inserter(seq));
         typedef FunctionHandling::First<MapBenchmark::value_type> first;
         OKLIB_TEST_EQUAL_RANGES(seq, boost::make_iterator_range(boost::make_transform_iterator<first>(rdb.db.benchmark().begin()), boost::make_transform_iterator<first>(rdb.db.benchmark().end())));
       }

       { // testing ea.series_of_benchmark()
         const map_benchmark_series& map1(ea.series_of_benchmark());
         {
           typedef FunctionHandling::First<typename map_benchmark_series::value_type> first1;
           typedef FunctionHandling::First<MapBenchmark::value_type> first2;
           typedef boost::transform_iterator<first1, typename map_benchmark_series::const_iterator> transform_iterator1;
           typedef boost::transform_iterator<first2, MapBenchmark::const_iterator> transform_iterator2;
           const MapBenchmark& map2(rdb.db.benchmark());
           OKLIB_TEST_EQUAL_RANGES(
                                   boost::make_iterator_range(transform_iterator1(map1.begin()), transform_iterator1(map1.end())),
                                   boost::make_iterator_range(transform_iterator2(map2.begin()), transform_iterator2(map2.end())));
         }
         {
           typedef typename map_series_benchmarks::const_iterator iterator_series;
           typedef typename map_benchmark_series::const_iterator iterator_benchmark;
           const map_series_benchmarks& map2(ea.benchmarks_in_series());
           const iterator_series& end_series(map2.end());
           const iterator_benchmark& end_benchmarks(map1.end());
           for (iterator_series i = map2.begin(); i != end_series; ++i) {
             const SpecSeries& series(i -> first);
             const seq_benchmarks& benchs(i -> second);
             typedef typename seq_benchmarks::const_iterator iterator;
             const iterator& end(benchs.end());
             for (iterator j =benchs.begin(); j != end; ++j) {
               const Benchmark& bench(*j);
               const iterator_benchmark& position(map1.find(bench));
               OKLIB_TEST_NOTEQUAL(position, end_benchmarks);
               OKLIB_TEST_EQUAL(position -> second, series);
             }
           }
         }
         // ToDo: Testing whether elementary_analysis::AmbigueBenchmark is thrown.
       }

       {// testing ea.solved_benchmarks()
         typedef MapSolver::const_iterator iterator;
         const MapSolver& map_solver(rdb.db.solver());
         const iterator end_map(map_solver.end());
         for (iterator i = map_solver.begin(); i != end_map; ++i) {
           const Solver& solver(i -> first);
           const SetResultNodesP* result_nodes_p = i -> second;
           rdb.db.vector_of_sets.clear();
           rdb.db.vector_of_sets.push_back(result_nodes_p);
           rdb.db.vector_of_sets.push_back(OKlib::SetAlgorithms::map_value(rdb.db.sat_status(), SATStatus(sat)));
           const VectorResultNodesP sat_result_nodes(rdb.db.intersection());
           rdb.db.vector_of_sets.pop_back();
           rdb.db.vector_of_sets.push_back(OKlib::SetAlgorithms::map_value(rdb.db.sat_status(), SATStatus(unsat)));
           const VectorResultNodesP& unsat_result_nodes(rdb.db.intersection());
           VectorResultNodesP sat_and_unsat; sat_and_unsat.reserve(sat_result_nodes.size() + unsat_result_nodes.size());
           std::set_union(sat_result_nodes.begin(), sat_result_nodes.end(), unsat_result_nodes.begin(), unsat_result_nodes.end(), std::back_inserter(sat_and_unsat));
           OKLIB_TEST_EQUAL(sat_and_unsat.size(), OKlib::SetAlgorithms::map_value(ea.solved_benchmarks(), solver).size()); 
         }
       }

       { // testing ea.solved_series()
         
       }

       // ToDo: to be completed
      }

    };

  }

}

#endif

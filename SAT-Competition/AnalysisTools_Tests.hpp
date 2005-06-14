// Oliver Kullmann, 7.6.2005 (Swansea)

#ifndef ANALYSISTOOLSTESTS_37yh6fR4

#define ANALYSISTOOLSTESTS_37yh6fR4

#include <cassert>
#include <iterator>
#include <algorithm>

#include <boost/range/iterator_range.hpp>

#include "IteratorHandling.hpp"
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
        typedef typename elementary_analysis::map_solver_benchmarks map_solver_benchmarks;
        typedef typename elementary_analysis::map_solver_series map_solver_series;

        typedef typename elementary_analysis::seq_series seq_series;
        typedef typename elementary_analysis::seq_spec_series seq_spec_series;
        typedef typename elementary_analysis::seq_benchmarks seq_benchmarks;

        { // testing ea.series_in_superseries()
          OKLIB_TEST_EQUAL_RANGES(
                                  IteratorHandling::range_first(ea.series_in_superseries()),
                                  IteratorHandling::range_first(rdb.db.super_series()));
          {
            seq_series s;
            OKlib::SetAlgorithms::union_sets(
                                             IteratorHandling::iterator_second(ea.series_in_superseries().begin()),
                                             IteratorHandling::iterator_second(ea.series_in_superseries().end()),
                                             std::back_inserter(s));
            OKLIB_TEST_EQUAL_RANGES(
                                    s,
                                    IteratorHandling::range_first(rdb.db.series()));
          }
        }

       { // testing ea.benchmarks_in_series()
         typedef typename IteratorHandling::IteratorSecond<typename map_series_benchmarks::const_iterator>::type transform_iterator;
         const map_series_benchmarks& map(ea.benchmarks_in_series());
         const transform_iterator& begin(transform_iterator(map.begin()));
         const transform_iterator& end(transform_iterator(map.end()));
         OKLIB_TEST_EQUAL(OKlib::SetAlgorithms::sum_sizes(begin, end), rdb.db.benchmark().size());
         seq_benchmarks seq;
         OKlib::SetAlgorithms::union_sets(begin, end, std::back_inserter(seq));
         OKLIB_TEST_EQUAL_RANGES(
                                 seq,
                                 IteratorHandling::range_first(rdb.db.benchmark()));
       }

       { // testing ea.series_of_benchmark()
         const map_benchmark_series& map1(ea.series_of_benchmark());
         {
           const MapBenchmark& map2(rdb.db.benchmark());
           OKLIB_TEST_EQUAL_RANGES(
                                   IteratorHandling::range_first(map1),
                                   IteratorHandling::range_first(map2));
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
           // ToDo: Testing the sequences for equality (after appropriately sorting the second sequence).
         }
       }

       { // testing ea.solved_series()
         typedef typename map_solver_series::const_iterator iterator_solver_series;
         const map_solver_series& map(ea.solved_series());
         const iterator_solver_series& end_map(map.end());
         {
           const map_solver_benchmarks& map2(ea.solved_benchmarks());
           OKLIB_TEST_EQUAL_RANGES(
                                   boost::make_iterator_range(IteratorHandling::iterator_first(map.begin()), IteratorHandling::iterator_first(end_map)),
                                   IteratorHandling::range_first(map2));
         }
         {
           for (iterator_solver_series i = map.begin(); i != end_map; ++i) {
             const Solver& solver(i -> first);
             const seq_spec_series& series_seq(i -> second);
             typedef typename seq_spec_series::const_iterator iterator;
             const iterator& end_series_seq(series_seq.end());
             for (iterator j = series_seq.begin(); j != end_series_seq; ++j) {
               const SpecSeries series(*j);
               rdb.db.vector_of_sets.clear();
               rdb.db.vector_of_sets.push_back(OKlib::SetAlgorithms::map_value(rdb.db.solver(), solver));
               rdb.db.vector_of_sets.push_back(OKlib::SetAlgorithms::map_value(rdb.db.super_series(), series.first));
               rdb.db.vector_of_sets.push_back(OKlib::SetAlgorithms::map_value(rdb.db.series(), series.second));
               rdb.db.vector_of_sets.push_back(OKlib::SetAlgorithms::map_value(rdb.db.sat_status(), SATStatus(sat)));
               if (rdb.db.intersection().empty()) {
                 rdb.db.vector_of_sets.pop_back();
                 rdb.db.vector_of_sets.push_back(OKlib::SetAlgorithms::map_value(rdb.db.sat_status(), SATStatus(unsat)));
                 if (rdb.db.intersection().empty())
                   OKLIB_THROW("Solver " + boost::lexical_cast<std::string>(solver) + " has not solved series " +  boost::lexical_cast<std::string>(series.second) + " in super-series " + boost::lexical_cast<std::string>(series.first));
               }
             }
             seq_spec_series unsolved_series;
             std::set_difference(IteratorHandling::iterator_first(ea.benchmarks_in_series().begin()), IteratorHandling::iterator_first(ea.benchmarks_in_series().end()), series_seq.begin(), series_seq.end(), std::back_inserter(unsolved_series));
             const iterator& end_unsolved_series(unsolved_series.end());
             for (iterator j = unsolved_series.begin(); j != end_unsolved_series; ++j) {
               const SpecSeries series(*j);
               rdb.db.vector_of_sets.clear();
               rdb.db.vector_of_sets.push_back(OKlib::SetAlgorithms::map_value(rdb.db.solver(), solver));
               rdb.db.vector_of_sets.push_back(OKlib::SetAlgorithms::map_value(rdb.db.super_series(), series.first));
               rdb.db.vector_of_sets.push_back(OKlib::SetAlgorithms::map_value(rdb.db.series(), series.second));
               rdb.db.vector_of_sets.push_back(OKlib::SetAlgorithms::map_value(rdb.db.sat_status(), SATStatus(sat)));
               if (not rdb.db.intersection().empty())
                 OKLIB_THROW("Solver " + boost::lexical_cast<std::string>(solver) + " has solved series " +  boost::lexical_cast<std::string>(series.second) + " in super-series " + boost::lexical_cast<std::string>(series.first) + "as satisfiable");
               rdb.db.vector_of_sets.pop_back();
               rdb.db.vector_of_sets.push_back(OKlib::SetAlgorithms::map_value(rdb.db.sat_status(), SATStatus(unsat)));
               if (not rdb.db.intersection().empty())
                 OKLIB_THROW("Solver " + boost::lexical_cast<std::string>(solver) + " has solved series " +  boost::lexical_cast<std::string>(series.second) + " in super-series " + boost::lexical_cast<std::string>(series.first) + "as unsatisfiable");
             }
           }
         }
       }

       // ToDo: to be completed
      }

    };

  }

}

#endif

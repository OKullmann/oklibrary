// Oliver Kullmann, 7.6.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef ANALYSISTOOLSTESTS_37yh6fR4
#define ANALYSISTOOLSTESTS_37yh6fR4

#include <cassert>
#include <iterator>
#include <algorithm>
#include <functional>
#include <string>

#include <boost/range/iterator_range.hpp>
#include <boost/lexical_cast.hpp>

#include <OKlib/General/IteratorHandling.hpp>

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

#include <OKlib/Structures/Sets/SetAlgorithms/BasicSetOperations.hpp>
#include <OKlib/Structures/Sets/SetAlgorithms/SequenceOperations.hpp>

#include <OKlib/Experimentation/Competition/SingleResult.hpp>
#include <OKlib/Experimentation/Competition/ParsingSingleResult.hpp>
#include <OKlib/Experimentation/Competition/ParsingResultSequences_Tests.hpp>
#include <OKlib/Experimentation/Competition/ResultProcessing.hpp>

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

        test_result<Result>(filename_large_industrial, line_count_large_industrial, true);
        test_result<ResultRandomSat>(filename_large_random + "_inconsistency", line_count_large_random + 2, false);
      }

      template <class result_type>
      void test_result(const std::string& filename, const unsigned int line_count, const bool consistent) {

        typedef Result_database_from_file<ParserResult, result_type> result_database;
        result_database rdb(filename);
        assert(rdb.result_sequence.size() == line_count);
        typedef typename result_database::database_type database;
        typedef ElementaryAnalysis<database> elementary_analysis;
        elementary_analysis ea(rdb.db);

        typedef typename elementary_analysis::SolvedBenchmark SolvedBenchmark;

        typedef typename elementary_analysis::map_superseries_series map_superseries_series;
        typedef typename elementary_analysis::map_series_benchmarks map_series_benchmarks;
        typedef typename elementary_analysis::map_benchmark_series map_benchmark_series;
        typedef typename elementary_analysis::map_solver_benchmarks map_solver_benchmarks;
        typedef typename elementary_analysis::map_solver_series map_solver_series;
        typedef typename elementary_analysis::map_benchmark_solvers map_benchmark_solvers;
        typedef typename elementary_analysis::map_benchmark_satstatus map_benchmark_satstatus;

        typedef typename elementary_analysis::seq_series seq_series;
        typedef typename elementary_analysis::seq_spec_series seq_spec_series;
        typedef typename elementary_analysis::seq_benchmarks seq_benchmarks;
        typedef typename elementary_analysis::seq_solved_benchmarks seq_solved_benchmarks;
        typedef typename elementary_analysis::seq_solvers seq_solvers;

        { // testing ea.series_in_superseries()
          const map_superseries_series& map(ea.series_in_superseries());
          test_order(map);
          OKLIB_TEST_EQUAL_RANGES(
                                  IteratorHandling::range_first(map),
                                  IteratorHandling::range_first(rdb.db.super_series()));
          {
            seq_series s;
            OKlib::SetAlgorithms::union_sets(
                                             IteratorHandling::iterator_second(map.begin()),
                                             IteratorHandling::iterator_second(map.end()),
                                             std::back_inserter(s));
            OKLIB_TEST_EQUAL_RANGES(
                                    s,
                                    IteratorHandling::range_first(rdb.db.series()));
          }
          // ToDo: stronger testing
        }

       { // testing ea.benchmarks_in_series()
         const map_series_benchmarks& map(ea.benchmarks_in_series());
         test_order(map);
         typedef typename IteratorHandling::IteratorSecond<typename map_series_benchmarks::const_iterator>::type transform_iterator;
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
         const map_solver_benchmarks& map(ea.solved_benchmarks());;
         test_order(map);
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
           OKLIB_TEST_EQUAL(sat_and_unsat.size(), OKlib::SetAlgorithms::map_value(map, solver).size());
           // ToDo: Testing the sequences for equality (after appropriately sorting the second sequence).
         }
         // ToDo: testing for exception AmbigueSolution
       }

       { // testing ea.solved_series()
         const map_solver_series& map(ea.solved_series());
         test_order(map);
         typedef typename map_solver_series::const_iterator iterator_solver_series;
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
             std::set_difference(
                                 IteratorHandling::iterator_first(ea.benchmarks_in_series().begin()), IteratorHandling::iterator_first(ea.benchmarks_in_series().end()),
                                 series_seq.begin(), series_seq.end(),
                                 std::back_inserter(unsolved_series));
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

       { // testing ea.succesful_solvers()
         const map_benchmark_solvers& map(ea.succesful_solvers());
         test_order(map);
         OKLIB_TEST_EQUAL_RANGES(
                                 IteratorHandling::range_first(map),
                                 IteratorHandling::range_first(ea.series_of_benchmark()));
         typedef typename map_benchmark_solvers::const_iterator iterator;
         const iterator& end_map(map.end());
         for (iterator i = map.begin(); i != end_map; ++i) {
           const SolvedBenchmark& bench(i -> first);
           const seq_solvers& solvers(i -> second);
           typedef typename seq_solvers::const_iterator iterator_solvers;
           const iterator_solvers& end_solvers(solvers.end());
           for (iterator_solvers j = solvers.begin(); j != end_solvers; ++j) {
             const Solver& solver(*j);
             const seq_solved_benchmarks& benchs(OKlib::SetAlgorithms::map_value(ea.solved_benchmarks(), solver));
             if (not std::binary_search(benchs.begin(), benchs.end(), bench))
               OKLIB_THROW("Benchmark " + boost::lexical_cast<std::string>(bench) + " not solved by solver " + boost::lexical_cast<std::string>(solver));
           }
         }
         typedef typename map_solver_benchmarks::const_iterator iterator_solvers;
         const map_solver_benchmarks& map2(ea.solved_benchmarks());
         const iterator_solvers& end_solvers(map2.end());
         for (iterator_solvers i = map2.begin(); i != end_solvers; ++i) {
           const Solver& solver(i -> first);
           const seq_solved_benchmarks& benchs(i -> second);
           typedef typename seq_solved_benchmarks::const_iterator iterator_benchmarks;
           const iterator_benchmarks end_benchs(benchs.end());
           for (iterator_benchmarks j = benchs.begin(); j != end_benchs; ++j) {
             const SolvedBenchmark& bench(*j);
             const seq_solvers& succesful(OKlib::SetAlgorithms::map_value(map, bench));
             if (not std::binary_search(succesful.begin(), succesful.end(), solver))
               OKLIB_THROW("Benchmark " + boost::lexical_cast<std::string>(bench) + " solved by solver " + boost::lexical_cast<std::string>(solver));
           }
         }
       }

       { // testing ea.sat_status()
         const map_benchmark_satstatus& map(ea.sat_status());
         {
           const map_benchmark_solvers& map2(ea.succesful_solvers());
           const seq_benchmarks& inconsistent_results(ea.inconsistent_results());
           if (consistent) {
             if (not inconsistent_results.empty())
               OKLIB_THROW("Test case should not contain inconsistent results");
             OKLIB_TEST_EQUAL_RANGES(
                                     IteratorHandling::range_first(map),
                                     IteratorHandling::range_first(map2));
           }
           else {
             if (inconsistent_results.empty())
               OKLIB_THROW("Test case should contain inconsistent results");
             seq_benchmarks all_benchs;
             std::set_union(
                            IteratorHandling::iterator_first(map.begin()), IteratorHandling::iterator_first(map.end()),
                            inconsistent_results.begin(), inconsistent_results.end(),
                            std::back_inserter(all_benchs));
             OKLIB_TEST_EQUAL_RANGES(
                                     all_benchs,
                                     IteratorHandling::range_first(map2));
               }
         }
         typedef typename map_benchmark_satstatus::const_iterator iterator;
         const iterator& end_map(map.end());
         for (iterator i = map.begin(); i != end_map; ++i) {
           const Benchmark& bench(i -> first);
           const SATStatus& sat_status(i -> second);
           switch (sat_status.result()) {
           case unknown :
             if (not OKlib::SetAlgorithms::map_value(ea.succesful_solvers(), bench).empty())
               OKLIB_THROW("Benchmark " + boost::lexical_cast<std::string>(bench) + " was solved");
             break;
           case sat :
             rdb.db.vector_of_sets.clear();
             rdb.db.vector_of_sets.push_back(OKlib::SetAlgorithms::map_value(rdb.db.benchmark(), bench));
             rdb.db.vector_of_sets.push_back(OKlib::SetAlgorithms::map_value(rdb.db.sat_status(), sat_status));
             if (rdb.db.intersection().empty())
               OKLIB_THROW("Benchmark " + boost::lexical_cast<std::string>(bench) + " was not found satisfiable");
             rdb.db.vector_of_sets.pop_back();
             rdb.db.vector_of_sets.push_back(OKlib::SetAlgorithms::map_value(rdb.db.sat_status(), SATStatus(unsat)));
             if (not rdb.db.intersection().empty())
               OKLIB_THROW("Benchmark " + boost::lexical_cast<std::string>(bench) + " was found unsatisfiable");
             break;
           case unsat :
             rdb.db.vector_of_sets.clear();
             rdb.db.vector_of_sets.push_back(OKlib::SetAlgorithms::map_value(rdb.db.benchmark(), bench));
             rdb.db.vector_of_sets.push_back(OKlib::SetAlgorithms::map_value(rdb.db.sat_status(), sat_status));
             if (rdb.db.intersection().empty())
               OKLIB_THROW("Benchmark " + boost::lexical_cast<std::string>(bench) + " was not found unsatisfiable");
             rdb.db.vector_of_sets.pop_back();
             rdb.db.vector_of_sets.push_back(OKlib::SetAlgorithms::map_value(rdb.db.sat_status(), SATStatus(sat)));
             if (not rdb.db.intersection().empty())
               OKLIB_THROW("Benchmark " + boost::lexical_cast<std::string>(bench) + " was found satisfiable");
             break;
           case error :
             OKLIB_THROW("Benchmark " + boost::lexical_cast<std::string>(bench) + " sat status was \"error\"");
           }
         }
       }

      }

      template <class Map>
      void test_order(const Map& map) {
        typedef typename Map::const_iterator iterator_map;
        const iterator_map& end_map(map.end());
        for (iterator_map i = map.begin(); i != end_map; ++i) {
          typedef typename Map::value_type value_type_pairs;
          typedef typename value_type_pairs::second_type seq_type;
          const seq_type& seq(i -> second);
          typedef typename seq_type::const_iterator iterator_seq;
          const iterator_seq& end_seq(seq.end());
          typedef typename seq_type::value_type value_type;
          if (std::adjacent_find(seq.begin(), end_seq, std::greater_equal<value_type>()) != end_seq)
            OKLIB_THROW(std::string("Vector with elements of type ") + typeid(value_type).name() + " not sorted");
        }
      }

    };

  }

}

#endif

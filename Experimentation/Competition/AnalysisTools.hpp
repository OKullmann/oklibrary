// Oliver Kullmann, 7.6.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Competition/AnalysisTools.hpp
  \brief Class ElementaryAnalysis<Database> (supplying Database with some "indices") and related constructs.
*/

#ifndef ANALYSISTOOLS_946TglQ
#define ANALYSISTOOLS_946TglQ

#include <vector>
#include <utility>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <string>
#include <ostream>

#include <boost/lexical_cast.hpp>

#include <OKlib/General/IteratorHandling.hpp>

#include <OKlib/Structures/Sets/SetAlgorithms/BasicMapOperations.hpp>

#include <OKlib/Experimentation/Competition/SingleResult.hpp>
#include <OKlib/Experimentation/Competition/ResultProcessing.hpp>

namespace OKlib {

  namespace SATCompetition {

    /*!
      \class ElementaryAnalysis
      \brief Given a "database" of results, computes an "indexed database", providing elementary maps, for example assiging to a solver the set of solved benchmarks, and elementary sets, for example the set of "complete" solvers.

      ElementaryAnalysis provides direct access to
       - the sequence of series in a superseries
       - the sequence of benchmarks in a pair of (superseries, series)
       - the pair (superseries, series) associated with a benchmark
       - the sequence of "solved benchmarks" (which are structures containing a benchmark and a pointer to a result node)
       - the sequence of (superseries, series) solved by a solver
       - the sequence of solvers which solved a benchmark
       - the satisfiability status of a benchmark
       - the set of benchmarks where inconsistent results exist.

      \todo Enable access to the sets of SuperSeries, SpecSeries, Benchmark, Solver (via iterators to corresponding maps), and provide computation of the set of "complete" solvers (having solved at least one sat and one unsat instance).
      \todo A concept is needed.
      \todo Should there be an exception base class for OKlib ?
    */

    template <class Database>
    class ElementaryAnalysis {
      ElementaryAnalysis(const ElementaryAnalysis&);
      ElementaryAnalysis& operator =(const ElementaryAnalysis&);

    public :

      ElementaryAnalysis(const Database& db) : db(db) {
        fill_series_in_superseries();
        fill_benchmarks_in_series();
        fill_series_of_benchmark();
        fill_solved_benchmarks();
        fill_solved_series();
        fill_succesful_solvers();
        fill_sat_status();
      }

      typedef Database database_type;
      const Database& db;

      struct SolvedBenchmark {
        const Benchmark bench;
        const ResultNode* const node;
        SolvedBenchmark(const Benchmark& bench) : bench(bench), node(0) {}
        SolvedBenchmark(const Benchmark& bench, const ResultNode* node) : bench(bench), node(node) {}
        operator Benchmark() const { return bench; }
        operator ResultNode*() const { return node; }
      };
      friend std::ostream& operator <<(std::ostream& out, const SolvedBenchmark sb) {
        return out << sb.bench;
      }
      friend inline bool operator ==(const SolvedBenchmark& b1, const SolvedBenchmark& b2) {
        return b1.bench == b2.bench;
      }
      friend inline bool operator <(const SolvedBenchmark& b1, const SolvedBenchmark& b2) {
        return b1.bench < b2.bench;
      }
      OKLIB_DERIVED_RELATIONS_FRIENDS(SolvedBenchmark);

      typedef std::vector<Series> seq_series;
      typedef std::vector<Benchmark> seq_benchmarks;
      typedef std::vector<Solver> seq_solvers;
      // such vectors are always sorted

      typedef std::vector<SpecSeries> seq_spec_series;
      typedef std::vector<SolvedBenchmark> seq_solved_benchmarks;

      typedef std::map<SuperSeries, seq_series> map_superseries_series;
      typedef std::map<SpecSeries, seq_benchmarks> map_series_benchmarks;
      typedef std::map<Benchmark, SpecSeries> map_benchmark_series;
      typedef std::map<Solver, seq_solved_benchmarks> map_solver_benchmarks;
      typedef std::map<Solver, seq_spec_series> map_solver_series;
      typedef std::map<Benchmark, seq_solvers> map_benchmark_solvers;
      typedef std::map<Benchmark, SATStatus> map_benchmark_satstatus;

      const map_superseries_series& series_in_superseries() const { return series_in_superseries_; }
      const map_series_benchmarks & benchmarks_in_series() const { return benchmarks_in_series_; }
      const map_benchmark_series& series_of_benchmark() const { return series_of_benchmark_; }
      const map_solver_benchmarks& solved_benchmarks() const { return solved_benchmarks_; }
      const map_solver_series& solved_series() const { return solved_series_; }
      const map_benchmark_solvers& succesful_solvers() const { return succesful_solvers_; }
      const map_benchmark_satstatus& sat_status() const { return sat_status_; }

      const seq_benchmarks& inconsistent_results() const { return inconsistent_results_; }

      struct ErrorElementaryAnalysis : std::runtime_error {
        ErrorElementaryAnalysis(const std::string& message) : std::runtime_error(message) {}
      };
      struct AmbigueBenchmark : ErrorElementaryAnalysis {
        AmbigueBenchmark(const Benchmark& benchmark, const SpecSeries& series1, const SpecSeries& series2) : ErrorElementaryAnalysis(boost::lexical_cast<std::string>(benchmark) + " in " + boost::lexical_cast<std::string>(series1) + " and " + boost::lexical_cast<std::string>(series2)) {}
      };
      struct AmbigueSolution : ErrorElementaryAnalysis {
        AmbigueSolution(const ResultNode* const solution1, const ResultNode* const solution2) : ErrorElementaryAnalysis("Ambigue solutions:\n" + boost::lexical_cast<std::string>(solution1 -> rb) + "\n" + boost::lexical_cast<std::string>(solution2 -> rb)) {}
      };

    private :

      map_superseries_series series_in_superseries_;
      map_series_benchmarks benchmarks_in_series_;
      map_benchmark_series series_of_benchmark_;
      map_solver_benchmarks solved_benchmarks_;
      map_solver_series solved_series_;
      map_benchmark_solvers succesful_solvers_;
      map_benchmark_satstatus sat_status_;

      seq_benchmarks inconsistent_results_;

      typedef SetResultNodesP::iterator set_iterator;

      void fill_series_in_superseries() {
        typedef map_superseries_series::iterator iterator;
        iterator current = series_in_superseries_.begin();
        typedef MapSuperSeries::const_iterator other_iterator;
        const other_iterator end_other(db.super_series().end());
        for (other_iterator i(db.super_series().begin()); i != end_other; ++i) {
          const SuperSeries super_series = i -> first;
          assert(i -> second);
          const SetResultNodesP& result_nodes_p = *(i -> second);
          std::set<Series> set_series;
          const set_iterator end_set(result_nodes_p.end());
          for (set_iterator j(result_nodes_p.begin()); j != end_set; ++j) {
            assert((*j) -> rb);
            set_series.insert((*j) -> rb -> series()); // ToDO: Using STL
            assert((*j) -> rb -> super_series() == super_series);
          }
          current = series_in_superseries_.insert(current, std::make_pair(super_series, seq_series(set_series.begin(), set_series.end())));
          assert(current -> first == super_series);
          assert((current -> second).size() == set_series.size());
        }
      }
      void fill_benchmarks_in_series() {
        typedef map_series_benchmarks::iterator iterator;
        typedef map_superseries_series::const_iterator iterator_superseries;
        typedef seq_series::const_iterator iterator_series;
        iterator current = benchmarks_in_series_.begin();
        const iterator_superseries& end_superseries(series_in_superseries().end());
        for (iterator_superseries i = series_in_superseries().begin(); i != end_superseries; ++i) {
          const SuperSeries& super_series(i -> first);
          const seq_series& vec_series(i -> second);
          const iterator_series& end_series(vec_series.end());
          for (iterator_series j = vec_series.begin(); j != end_series; ++j) {
            const Series& series(*j);
            std::set<Benchmark> set_benchmarks;
            const SetResultNodesP& result_nodes_p = *OKlib::SetAlgorithms::map_value(db.series(), series);
            const set_iterator& end_results(result_nodes_p.end());
            for (set_iterator k = result_nodes_p.begin(); k != end_results; ++k) {
              const ResultNode& node(**k);
              const ResultBasis& result(*node.rb);
              if (result.super_series() == super_series)
                set_benchmarks.insert(result.benchmark());
            }
            current = benchmarks_in_series_.insert(current, std::make_pair(std::make_pair(super_series, series), seq_benchmarks(set_benchmarks.begin(), set_benchmarks.end())));
          }
        }
      }
      void fill_series_of_benchmark() {
        typedef map_series_benchmarks::const_iterator iterator_series;
        typedef seq_benchmarks::const_iterator iterator_benchmarks;
        typedef map_benchmark_series::iterator iterator;
        const iterator_series& end_series(benchmarks_in_series().end());
        const iterator& end_map(series_of_benchmark_.end());
        for (iterator_series i = benchmarks_in_series().begin(); i != end_series; ++i) {
          const SpecSeries& series(i -> first);
          const seq_benchmarks& benchs(i -> second);
          const iterator_benchmarks& end_benchmarks(benchs.end());
          for (iterator_benchmarks j = benchs.begin(); j != end_benchmarks; ++j) {
            const Benchmark& bench(*j);
            const iterator position(series_of_benchmark_.find(bench));
            if (position == end_map)
              series_of_benchmark_.insert(std::make_pair(bench, series));
            else {
              const SpecSeries& series2(position -> second);
              assert(series2 != series);
              throw AmbigueBenchmark(bench, series, series2);
            }
              
            series_of_benchmark_[*j] = series;
          }
        }
      }
      void fill_solved_benchmarks() {
        typedef typename map_solver_benchmarks::iterator iterator;
        iterator current = solved_benchmarks_.begin();
        std::set<Solver> set_solvers;
        const MapSolver& map(db.solver());
        typedef MapSolver::const_iterator other_iterator;
        const other_iterator end(map.end());
        for (other_iterator i = map.begin(); i != end; ++i) {
          const Solver& solver(i -> first);
          const SetResultNodesP& result_nodes_p = *(i -> second);
          typedef std::set<SolvedBenchmark> set_benchmarks_type;
          set_benchmarks_type set_benchmarks;
          const set_iterator& end_results(result_nodes_p.end());
          for (set_iterator j = result_nodes_p.begin(); j != end_results; ++j) {
            const ResultNode* node_p(*j);
            const ResultNode& node(*node_p);
            const ResultBasis& result(*node.rb);
            const SolverResult& solv_res(result.sat_status().result());
            if (solv_res == sat or solv_res == unsat) {
              typedef typename set_benchmarks_type::iterator set_benchmarks_iterator;
              typedef std::pair<set_benchmarks_iterator, bool> insert_return_type;
              const Benchmark& bench(result.benchmark());
              const SolvedBenchmark new_solved_benchmark(bench, node_p);
              const insert_return_type& insert_result(set_benchmarks.insert(new_solved_benchmark));
              if (not insert_result.second) {
                throw AmbigueSolution(node_p, insert_result.first -> node);
              }
            }
          }
          current = solved_benchmarks_.insert(current, std::make_pair(solver, seq_solved_benchmarks(set_benchmarks.begin(), set_benchmarks.end())));
        }
      }
      void fill_solved_series() {
        typedef map_solver_series::iterator iterator;
        iterator current = solved_series_.begin();
        typedef typename map_solver_benchmarks::const_iterator iterator_solvers;
        const iterator_solvers& end_solvers(solved_benchmarks().end());
        for (iterator_solvers i = solved_benchmarks().begin(); i != end_solvers; ++i) {
          std::set<SpecSeries> set_series;
          const Solver& solver(i -> first);
          const seq_solved_benchmarks& benchs(i -> second);
          typedef typename seq_solved_benchmarks::const_iterator iterator_benchmarks;
          const iterator_benchmarks end_benchs(benchs.end());
          for (iterator_benchmarks j = benchs.begin(); j != end_benchs; ++j) {
            const Benchmark& bench(*j);
            const SpecSeries& series(OKlib::SetAlgorithms::map_value(series_of_benchmark(), bench));
            set_series.insert(series);
          }
          current = solved_series_.insert(current, std::make_pair(solver, seq_spec_series(set_series.begin(), set_series.end())));
        }
      }
      void fill_succesful_solvers() {
        typedef typename map_solver_benchmarks::const_iterator iterator_solvers;
        typedef typename seq_solved_benchmarks::const_iterator iterator_solved_benchmarks;
        const iterator_solvers& end_solvers(solved_benchmarks().end());
        for (iterator_solvers i = solved_benchmarks().begin(); i != end_solvers; ++i) {
          const Solver& solver(i -> first);
          const seq_solved_benchmarks& benchs(i -> second);
          const iterator_solved_benchmarks& end_benchmarks(benchs.end());
          for (iterator_solved_benchmarks j = benchs.begin(); j != end_benchmarks; ++j) {
            const Benchmark& bench(*j);
            succesful_solvers_[bench].push_back(solver);
          }
        }
        seq_benchmarks unsolved;
        std::set_difference(
                            IteratorHandling::iterator_first(series_of_benchmark().begin()), IteratorHandling::iterator_first(series_of_benchmark().end()),
                            IteratorHandling::iterator_first(succesful_solvers_.begin()), IteratorHandling::iterator_first(succesful_solvers_.end()),
                            std::back_inserter(unsolved));
        typedef map_benchmark_solvers::iterator iterator;
        iterator current(succesful_solvers_.begin());
        typedef typename seq_benchmarks::const_iterator iterator_benchmarks;
        const iterator_benchmarks& end_benchmarks(unsolved.end());
        for (iterator_benchmarks i = unsolved.begin(); i != end_benchmarks; ++i) {
          const Benchmark& bench(*i);
          current = succesful_solvers_.insert(current, std::make_pair(bench, seq_solvers()));
          // ToDo: Ideally, the temporary store unsolved would not be needed.
        }
      }
      
      void fill_sat_status() {
        typedef map_benchmark_solvers::const_iterator iterator_benchmarks;
        const iterator_benchmarks end_benchmarks(succesful_solvers().end());
        for (iterator_benchmarks i = succesful_solvers().begin(); i != end_benchmarks; ++i) {
          const Benchmark& bench(i -> first);
          {
            const seq_solvers& solvers(i -> second);
            if (solvers.empty()) {
              sat_status_.insert(std::make_pair(bench, SATStatus(unknown))); 
              continue;
            }
          }
          typedef std::set<SATStatus> set_result_type;
          set_result_type set_results;
          const MapBenchmark& map(db.benchmark());
          const SetResultNodesP& set(*OKlib::SetAlgorithms::map_value(map, bench));
          typedef SetResultNodesP::iterator iterator;
          const iterator& end_set(set.end());
          for (iterator j = set.begin(); j != end_set; ++j) {
            const ResultNode& result_node(**j);
            const ResultBasis& result(*result_node.rb);
            const SATStatus& sat_status(result.sat_status());
            switch (sat_status.result()) {
            case sat :
              set_results.insert(SATStatus(sat)); break;
            case unsat :
              set_results.insert(SATStatus(unsat)); break;
            case error : break;
            case unknown : break;
            default :
              assert(false);
            }
          }
          assert(not set_results.empty());
          assert(set_results.size() <= 2);
          if (set_results.size() == 1)
            sat_status_.insert(std::make_pair(bench, *set_results.begin()));
          else
            inconsistent_results_.push_back(bench);
        }
      }

    };
    
  }

}

#endif

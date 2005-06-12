// Oliver Kullmann, 7.6.2005 (Swansea)

#ifndef ANALYSISTOOLS_946TglQ

#define ANALYSISTOOLS_946TglQ

#include <vector>
#include <utility>
#include <algorithm>
#include <iterator>

#include "BasicMapOperations.hpp"

#include "SingleResult.hpp"
#include "ResultProcessing.hpp"

namespace OKlib {

  namespace SATCompetition {

    template <class Database>
    class ElementaryAnalysis {
      ElementaryAnalysis(const ElementaryAnalysis&);
      ElementaryAnalysis& operator =(const ElementaryAnalysis&);
      const Database& db;

    public :

      ElementaryAnalysis(const Database& db) : db(db) {
        fill_series_in_superseries();
        fill_benchmarks_in_series();
        fill_solved_benchmarks();
        fill_solved_series();
        fill_succesful_solvers();
        fill_sat_status();
      }

      typedef std::pair<SuperSeries, Series> SpecSeries;

      typedef std::vector<Series> seq_series;
      typedef std::vector<SpecSeries> seq_spec_series;
      typedef std::vector<Benchmark> seq_benchmarks;
      typedef std::vector<Solver> seq_solvers;
      // such vectors are always sorted

      typedef std::map<SuperSeries, seq_series> map_superseries_series;
      typedef std::map<SpecSeries, seq_benchmarks> map_series_benchmarks;
      typedef std::map<Solver, seq_benchmarks> map_solver_benchmarks;
      typedef std::map<Solver, seq_spec_series> map_solver_series;
      typedef std::map<Benchmark, seq_solvers> map_benchmark_solvers;
      typedef std::map<Benchmark, SATStatus> map_benchmark_satstatus;

      const map_superseries_series& series_in_superseries() const { return series_in_superseries_; }
      const map_series_benchmarks & benchmarks_in_series() const { return benchmarks_in_series_; }
      const map_solver_benchmarks& solved_benchmarks() const { return solved_benchmarks_; }
      const map_solver_series& solved_series() const { return solved_series_; }
      const map_benchmark_solvers& succesful_solvers() const { return succesful_solvers_; }
      const map_benchmark_satstatus& sat_status() const { return sat_status_; }

      const seq_benchmarks& inconsistent_results() const { return inconsistent_results; }

    private :

      map_superseries_series series_in_superseries_;
      map_series_benchmarks benchmarks_in_series_;
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
      void fill_solved_benchmarks() {
        // ToDo: to be completed
      }
      void fill_solved_series() {

      }
      void fill_succesful_solvers() {

      }
      void fill_sat_status() {

      }
    };

  }

}

#endif

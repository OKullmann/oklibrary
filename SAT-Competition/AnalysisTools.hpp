// Oliver Kullmann, 7.6.2005 (Swansea)

#ifndef ANALYSISTOOLS_946TglQ

#define ANALYSISTOOLS_946TglQ

#include <vector>
#include <utility>
#include <algorithm>
#include <iterator>

#include "SingleResult.hpp"
#include "ResultProcessing.hpp"

namespace OKlib {

  namespace SATCompetition {

    template <class Database>
    class ElementaryAnalysis {
      Database& db;

    public :

      ElementaryAnalysis(Database& db) : db(db) {
        fill_series_in_superseries();
        fill_benchmarks_in_series();
        fill_solved_benchmarks();
        fill_solved_series();
        fill_succesful_solvers();
        fill_sat_status();
      }

      typedef std::vector<Series> seq_series;
      typedef std::vector<Benchmark> seq_benchmarks;
      typedef std::vector<Solver> seq_solvers;
      // such vectors are always sorted

      typedef std::map<SuperSeries, seq_series> map_superseries_series;
      typedef std::map<Series, seq_benchmarks> map_series_benchmarks;
      typedef std::map<Solver, seq_benchmarks> map_solver_benchmarks;
      typedef std::map<Solver, seq_series> map_solver_series;
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
          const SetResultNodesP& result_nodes_p = i -> second;
          typedef std::set<Series> set;
          set set_series;
          const set_iterator end_set(result_nodes_p.end());
          for (set_iterator j(result_nodes_p.begin()); j != end_set; ++j)
            set_series.insert(*((*j) -> rb)); // ToDO: Using STL
          current = series_in_superseries_.insert(current, std::make_pair(super_series, seq_series()));
          std::copy(set_series.begin(), set_series.end(), std::back_inserter(current -> second));
        }
      }
      void fill_benchmarks_in_series() {

      }
      void fill_solved_benchmarks() {

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

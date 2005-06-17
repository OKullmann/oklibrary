// Oliver Kullmann, 17.6.2005 (Swansea)

#ifndef SCORING_567UyTr

#define SCORING_567UyTr

#include <algorithm>
#include <cassert>

#include "IteratorHandling.hpp"

#include "BasicSetOperations.hpp"

#include "SingleResult.hpp"
#include "ResultProcessing.hpp"

namespace OKlib {

  namespace SATCompetition {

    template <class IndexedDatabase, typename NumberType = double>
    class PurseScoring {
      const IndexedDatabase& idb;
    public :

      typedef NumberType number_type;
      typedef IndexedDatabase database_type;

      const number_type standard_problem_purse;
      const number_type standard_speed_factor;
      const number_type standard_series_factor;

      const number_type standard_speed_purse;
      const number_type standard_series_purse;

      PurseScoring(const IndexedDatabase& idb, const number_type standard_problem_purse = 1000, const number_type standard_speed_factor = 1, const number_type standard_series_factor = 3) :
        idb(idb), standard_problem_purse(standard_problem_purse), standard_speed_factor(standard_speed_factor), standard_series_factor(standard_series_factor), standard_speed_purse(standard_speed_factor * standard_problem_purse), standard_series_purse(standard_series_factor * standard_problem_purse) {}

      virtual ~PurseScoring() {}

      number_type problem_purse(const Solver& solver, const Benchmark& bench) const {
        return problem_purse_(solver, bench);
      }
      number_type problem_purse(const Solver& solver) const {
        return problem_purse_(solver);
      }

      number_type speed_factor(const Solver& solver, const Benchmark& bench) const {
        return speed_factor_(solver, bench);
      }
      number_type speed_factor(const Benchmark& bench) const {
        return speed_factor_(bench);
      }
      
      number_type speed_award(const Solver& solver, const Benchmark& bench) const {
        return speed_award_(solver, bench);
      }
      number_type speed_award(const Solver& solver) const {
        return speed_award_(solver);
      }

      number_type series_purse(const SpecSeries& series) const {
        return series_purse_(series);
      }
      number_type series_purse(const Solver& solver, const SpecSeries& series) const {
        return series_purse_(solver, series);
      }
      number_type series_purse(const Solver& solver) const {
        return series_purse_(solver);
      }

      number_type score(const Solver& solver) const {
        return score_(solver);
      }

    private :

      typedef typename database_type::seq_solved_benchmarks seq_solved_benchmarks;
      typedef typename seq_solved_benchmarks::size_type size_type_solved_benchmarks;
      typedef typename database_type::seq_solvers seq_solvers;
      typedef typename seq_solvers::size_type size_type_solvers;
      typedef typename database_type::seq_benchmarks seq_benchmarks;
      typedef typename seq_benchmarks::size_type size_type_benchmarks;


      typedef typename database_type::seq_spec_series seq_spec_series;

      typedef typename database_type::SolvedBenchmark SolvedBenchmark;


      bool solved(const Solver& solver, const Benchmark& bench) const {
        const seq_solvers& seq(OKlib::SetAlgorithms::map_value(idb.succesful_solvers(), bench));
        return std::binary_search(seq.begin(), seq.end(), solver);
      }

      bool solved(const Solver& solver, const SpecSeries& series) const {
        const seq_spec_series& seq(OKlib::SetAlgorithms::map_value(idb.solved_series(), solver));
        return std::binary_search(seq.begin(), seq.end(), series);
      }


      virtual number_type problem_purse_(const Solver& solver, const Benchmark& bench) const {
        if (solved(solver, bench)) {
          const seq_solvers& seq(OKlib::SetAlgorithms::map_value(idb.succesful_solvers(), bench));
          const size_type_solvers& count(seq.size());
          assert(count > 0);
          return standard_problem_purse / count;
        }
        else
          return 0;
      }

      virtual number_type problem_purse_(const Solver& solver) const {
        const seq_solved_benchmarks& seq(OKlib::SetAlgorithms::map_value(idb.solved_benchmarks(), solver));
        number_type sum = 0;
        typedef typename seq_solved_benchmarks::const_iterator iterator;
        const iterator end(seq.end());
        for (iterator i = seq.begin(); i != end; ++i) {
          const Benchmark& bench(*i);
          sum += problem_purse(solver, bench);
        }
        return sum;
      }

      virtual number_type speed_factor_(const Solver& solver, const Benchmark& bench) const {
        if (solved(solver, bench)) {
          const seq_solved_benchmarks& seq(OKlib::SetAlgorithms::map_value(idb.solved_benchmarks(), solver));
          typedef typename seq_solved_benchmarks::const_iterator iterator;
          const iterator& solution_it(std::lower_bound(seq.begin(), seq.end(), SolvedBenchmark(bench)));
          assert(solution_it != seq.end());
          const SolvedBenchmark& solution(*solution_it);
          assert(solution.bench == bench);
          typedef ResultElement::floating_point_type floating_point_type;
          const floating_point_type& time_needed(solution.node -> rb -> average().average());
          typedef ResultElement::natural_number_type natural_number_type;
          const natural_number_type& time_out(solution.node -> rb -> time_out().time_out());
          return time_out / (1 + time_needed);
        }
        else
          return 0;
      }

      virtual number_type speed_factor_(const Benchmark& bench) const {
        const seq_solvers& seq(OKlib::SetAlgorithms::map_value(idb.succesful_solvers(), bench));
        typedef typename seq_solvers::const_iterator iterator;
        const iterator end(seq.end());
        number_type sum = 0;
        for (iterator i = seq.begin(); i != end; ++i) {
          const Solver& solver(*i);
          sum += speed_factor(solver, bench);
        }
        return sum;
      }

      virtual number_type speed_award_(const Solver& solver, const Benchmark& bench) const {
        if (solved(solver, bench))
          return standard_speed_purse * speed_factor(solver, bench) / speed_factor(bench);
        else
          return 0;
      }

      virtual number_type speed_award_(const Solver& solver) const {
        const seq_solved_benchmarks& seq(OKlib::SetAlgorithms::map_value(idb.solved_benchmarks(), solver));
        number_type sum = 0;
        typedef typename seq_solved_benchmarks::const_iterator iterator;
        const iterator end(seq.end());
        for (iterator i = seq.begin(); i != end; ++i) {
          const Benchmark& bench(*i);
          sum += speed_award(solver, bench);
        }
        return sum;
      }
     
      virtual number_type series_purse_(const SpecSeries& series) const {
        const seq_benchmarks& seq(OKlib::SetAlgorithms::map_value(idb.benchmarks_in_series(), series));
        const size_type_benchmarks& size(seq.size());
        const size_type_benchmarks size_threshold = 5;
        if (size >= size_threshold)
          return standard_series_purse;
        else {
          const number_type weight = number_type(1) / number_type(3);
          return standard_series_purse * weight;
        }
      }

      virtual number_type series_purse_(const Solver& solver, const SpecSeries& series) const {
        if (solved(solver, series)) {
          number_type count = 0;
          typedef typename database_type::map_solver_benchmarks map_solver_benchmarks;
          typedef typename IteratorHandling::IteratorFirst<typename map_solver_benchmarks::const_iterator>::type iterator;
          const map_solver_benchmarks& map(idb.solved_benchmarks());
          const iterator end(map.end());
          for (iterator i(map.begin()); i != end; ++i) {
            const Solver& running_solver(*i);
            count += solved(running_solver, series);
          }
          assert(count >= 1);
          return series_purse(series) / count;
        }
        else
          return 0;
      }

      virtual number_type series_purse_(const Solver& solver) const {
        const seq_spec_series& seq(OKlib::SetAlgorithms::map_value(idb.solved_series(), solver));
        number_type sum = 0;
        typedef typename seq_spec_series::const_iterator iterator;
        const iterator end(seq.end());
        for (iterator i = seq.begin(); i != end; ++i) {
          const SpecSeries& series(*i);
          sum += series_purse(solver, series);
        }
        return sum;
      }

      virtual number_type score_(const Solver& solver) const {
        return problem_purse(solver) + speed_award(solver) + series_purse(solver);
      }

    };

  }

}

#endif

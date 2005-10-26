// Oliver Kullmann, 25.10.2005 (Swansea)

/*!
  \file LexicographicalEvaluation.hpp
  \brief Evaluation of an "indexed database" by providing comparison tools.

  The evaluation member function of LexicographicalEvaluation provides a map,
  which gives access for each solver to the map from succesfully solved series
  to solve-count and average running time.
*/

#ifndef LEXICOGRAPHICALEVALUATION_9fsfs41917

#define LEXICOGRAPHICALEVALUATION_9fsfs41917

#include <vector>
#include <map>
#include <utility>

#include "BasicMapOperations.hpp"

#include "SingleResult.hpp"

namespace OKlib {

  namespace SATCompetition {

    /*!
      \class LexicographicalEvaluation
      \brief Given an "indexed database" of results, map each solver to the map, which
      assigns to each solved series the pair of solved-count and average running time.
    */

    template <class IndexedDatabase, typename NumberType = double>
    class LexicographicalEvaluation {
    public :

      typedef IndexedDatabase database_type;
      typedef NumberType number_type;

    private :

      typedef typename database_type::seq_solved_benchmarks seq_solved_benchmarks;

    public :

      typedef typename seq_solved_benchmarks::size_type size_type;

      typedef std::pair<size_type, number_type> numerics_solver_on_series_type;
      typedef std::map<SpecSeries, numerics_solver_on_series_type> map_solver_evaluation_for_series_type;

      const database_type& idb;

      LexicographicalEvaluation(const IndexedDatabase& idb) : idb(idb) {

        typedef typename database_type::map_solver_benchmarks map_solver_benchmarks;
        typedef typename map_solver_benchmarks::const_iterator iterator;
        const map_solver_benchmarks& solver_benchmarks(idb.solved_benchmarks());
        const iterator& end(solver_benchmarks.end());
        for (iterator i(solver_benchmarks.begin()); i != end; ++i) { // loop over all solvers
          const Solver& solver(i -> first);
          typedef typename database_type::seq_solved_benchmarks seq_solved_benchmarks;
          const seq_solved_benchmarks& solved_benchmarks(i -> second);
          map_solver_evaluation_for_series_type result_map;
          typedef typename seq_solved_benchmarks::const_iterator iterator;
          const iterator& end(solved_benchmarks.end());
          for (iterator j(solved_benchmarks.begin()); j != end; ++j) { // loop over all solved benchs
            typedef typename database_type::SolvedBenchmark solved_benchmark_type;
            const solved_benchmark_type& solved_bench(*j);
            const SpecSeries& series(OKlib::SetAlgorithms::map_value(idb.series_of_benchmark(), solved_bench.bench));
            numerics_solver_on_series_type& ref(result_map[series]);
            ++ref.first;
            ref.second += solved_bench.node -> rb -> average().average();
          }
          typedef typename map_solver_evaluation_for_series_type::iterator iterator2;
          const iterator2 end2(result_map.end());
          for (iterator2 j(result_map.begin()); j != end2; ++j) {
            numerics_solver_on_series_type& ref(j -> second);
            assert(ref.first > 0);
            ref.second /= ref.first;
          }
          map.insert(std::make_pair(solver, result_map));

        }
      }
      ~LexicographicalEvaluation() {}

      const map_solver_evaluation_for_series_type& evaluation(const Solver& solver) const {
        return OKlib::SetAlgorithms::map_value(map, solver);
      }

    private :

      typedef std::map<Solver, map_solver_evaluation_for_series_type> map_solver_map_evaluation_type;
      map_solver_map_evaluation_type map;
      
    };

  }

}

#endif

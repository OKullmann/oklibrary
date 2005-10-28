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

#include <map>
#include <utility>

#include "BasicMapOperations.hpp"
#include "DerivedRelations.hpp"

#include "SingleResult.hpp"

#include "LexicographicalEvaluationPolicies.hpp"

namespace OKlib {

  namespace SATCompetition {

    /*!
      \class LexicographicalEvaluation
      \brief Given an "indexed database" of results, map each solver to the map, which
      assigns to each solved super-series the map, which assigns to each solved series
      the pair of solved-count and average running time.
      \todo Write two function objects for sorting of LexicographicalEvaluation::map_series_numerics_type.
      Or better this 
    */

    template <class IndexedDatabase, class SeriesPolicy = LexicographicalSeriesPolicy<Result>, typename NumberType = double>
    class LexicographicalEvaluation {
    public :

      typedef IndexedDatabase database_type;
      typedef NumberType number_type;

    private :

      typedef typename database_type::seq_solved_benchmarks seq_solved_benchmarks;

    public :

      typedef typename seq_solved_benchmarks::size_type size_type;

      typedef std::pair<size_type, number_type> numerics_solver_on_series_type;
      friend bool operator <(const numerics_solver_on_series_type& lhs, const numerics_solver_on_series_type& rhs) {
        return lhs.first > rhs.first or (lhs.first == rhs.first and lhs.first != 0 and lhs.second < rhs.second);
      }
      // a < b for a, b of type numerics_solver_on_series_type means "a is better than b"
      OKLIB_DERIVED_RELATIONS_FRIENDS(numerics_solver_on_series_type);

      typedef typename SeriesPolicy::super_series_type super_series_type;
      typedef typename SeriesPolicy::series_type series_type;
      typedef std::map<series_type, numerics_solver_on_series_type> map_series_numerics_type;
      typedef std::map<super_series_type, map_series_numerics_type> map_solver_evaluation_type;

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
          map_solver_evaluation_type result_map;

          {
            typedef typename seq_solved_benchmarks::const_iterator iterator;
            const iterator& end(solved_benchmarks.end());
            for (iterator j(solved_benchmarks.begin()); j != end; ++j) { // loop over all solved benchs
              typedef typename database_type::SolvedBenchmark solved_benchmark_type;
              const solved_benchmark_type& solved_bench(*j);
              const super_series_type& super_series(SeriesPolicy::super_series(solved_bench));
              const series_type& series(SeriesPolicy::series(solved_bench));
              numerics_solver_on_series_type& ref(result_map[super_series][series]);
              ++ref.first;
              ref.second += solved_bench.node -> rb -> average().average();
            }
          }
          {
            typedef typename map_solver_evaluation_type::iterator iterator;
            const iterator& end(result_map.end());
            for (iterator j(result_map.begin()); j != end; ++j) {
              map_series_numerics_type& ref(j -> second);
              typedef typename map_series_numerics_type::iterator iterator2;
              const iterator2& end2(ref.end());
              for (iterator2 k(ref.begin()); k != end2; ++k) {
                numerics_solver_on_series_type& ref2(k -> second);
                assert(ref2.first > 0);
                ref2.second /= ref2.first;
              }
            }
            map.insert(std::make_pair(solver, result_map));
          }

        }
      }
      ~LexicographicalEvaluation() {}

      const map_solver_evaluation_type& evaluation(const Solver& solver) const {
        return OKlib::SetAlgorithms::map_value(map, solver);
      }

    private :

      typedef std::map<Solver, map_solver_evaluation_type> map_solver_map_evaluation_type;
      map_solver_map_evaluation_type map;
      
    };

    // ###########################################################

    

  }

}

#endif

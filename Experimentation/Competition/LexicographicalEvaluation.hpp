// Oliver Kullmann, 25.10.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Competition/LexicographicalEvaluation.hpp
  \brief Evaluation of an "indexed database" by providing comparison tools based on aggregated
  series evaluations.

  Tools providing for each solver the map from succesfully solved series
  to solve-count and average running time, and in case the series can be ordered
  by size, providing lexicographical evaluation.
*/

#ifndef LEXICOGRAPHICALEVALUATION_9fsfs41917
#define LEXICOGRAPHICALEVALUATION_9fsfs41917

#include <map>
#include <utility>
#include <vector>
#include <algorithm>
#include <cassert>
#include <ostream>
#include <iterator>
#include <string>
#include <ios>
#include <iomanip>

#include <boost/functional.hpp>

#include <OKlib/Structures/Sets/SetAlgorithms/BasicMapOperations.hpp>
#include <OKlib/Programming/Utilities/OrderRelations/DerivedRelations.hpp>

#include <OKlib/Experimentation/Competition/SingleResult.hpp>
#include <OKlib/Experimentation/Competition/AnalysisTools.hpp>

#include <OKlib/Experimentation/Competition/LexicographicalEvaluationPolicies.hpp>

namespace OKlib {
  namespace SATCompetition {

    /*!
      \class LexicographicalEvaluation
      \brief Given an "indexed database" of results, map each solver to the map, which
      assigns to each solved super-series the map, which assigns to each solved series
      the pair of solved-count and average running time.
      \todo Actually, "LexicographicalEvaluation" is a misleading --- this class provides
      access to aggregated series results for each solver. So we should rename this class
      here; LexicographicalEvaluationRandom then is really the lexicographical evaluation
      (which should be generalised for "Series_with_n" (where "n" should be read as
      "size")).
      \todo The nested typedef numerics_solver_on_series_type should become a freestanding
      class template with an associated linear order (we should not just have a typedef for
      a pair-type here, since for pair-types we should always assume the natural definitions
      of == and <). A conversion to pair should be given. A concept is needed here (at
      least FullyConstructibleLinearOrder and the library basics).
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

      struct numerics_solver_on_series_type {
        typedef size_type first_type;
        typedef number_type second_type;
        size_type first;
        number_type second;
        numerics_solver_on_series_type() : first(0), second(0) {}
        numerics_solver_on_series_type(const size_type first, const number_type second) : first(first), second(second) {}
      };
      friend bool operator ==(const numerics_solver_on_series_type& lhs, const numerics_solver_on_series_type& rhs) {
        return lhs.first == rhs.first and (lhs.first == 0 or lhs.second == rhs.second);
      }
      friend bool operator <(const numerics_solver_on_series_type& lhs, const numerics_solver_on_series_type& rhs) {
        return lhs.first > rhs.first or (lhs.first == rhs.first and lhs.first != 0 and lhs.second < rhs.second);
      }
      // a < b for a, b of type numerics_solver_on_series_type means "a is better than b"
      OKLIB_DERIVED_RELATIONS_FRIENDS(numerics_solver_on_series_type);
      friend std::ostream& operator <<(std::ostream& out, const numerics_solver_on_series_type& pair) {
        const std::streamsize field_width_count = 3;
        out.width(field_width_count);
        out << pair.first << ":";
        const std::streamsize field_width_time = 5;
        out.width(field_width_time);
        if (pair.first != 0) {
          const std::streamsize old_precision(out.precision());
          const std::streamsize new_precision_time = (pair.second < 10) ? 1 : 0;
          out.precision(new_precision_time);
          out << pair.second;
          out.precision(old_precision);
        }
        else
          out << "NaN";
        return out;
      }

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

      const map_solver_evaluation_type& evaluation(const Solver& solver) const {
        return OKlib::SetAlgorithms::map_value(map, solver);
      }

    public : // ToDo: Design

      typedef std::map<Solver, map_solver_evaluation_type> map_solver_map_evaluation_type;
      map_solver_map_evaluation_type map;
      
    };

    // ###########################################################

    /*!
      \class LexicographicalEvaluationRandom
      \brief Given an "indexed database" of results, map each super-series to the set of
      solver-resultvector pairs, where the resultvectors contain the series evaluations
      computed by LexicographicalEvaluation, and the order of these pairs is some
      variation of lexicographical order.
    */

    template <class IndexedDatabase, template <class solver_evaluation_pair_type> class SortingPolicy = LexicographicalSortingPolicy_unfolded_lexicographical, typename NumberType = double>
    class LexicographicalEvaluationRandom {
    public :

      typedef IndexedDatabase database_type;
      typedef NumberType number_type;

      typedef LexicographicalEvaluation<database_type, LexicographicalSeriesPolicy<ResultRandomSat>, number_type> lexicographical_evaluation_type;
      typedef typename lexicographical_evaluation_type::numerics_solver_on_series_type numerics_solver_on_series_type;
      typedef typename lexicographical_evaluation_type::size_type size_type;

      typedef std::vector<numerics_solver_on_series_type> evaluation_vector_type;
      typedef std::pair<Solver, evaluation_vector_type> solver_evaluation_pair_type;

      typedef typename SortingPolicy<solver_evaluation_pair_type>::comparison_type comparison_type;
      typedef std::set<solver_evaluation_pair_type, comparison_type> set_of_evaluations_type;

      const database_type& idb;

      LexicographicalEvaluationRandom(const IndexedDatabase& idb) : idb(idb) {
        lexicographical_evaluation_type eval(idb);
        typedef typename lexicographical_evaluation_type::map_solver_map_evaluation_type map_solver_map_evaluation_type;
        const map_solver_map_evaluation_type& map_solver(eval.map);
        typedef typename map_solver_map_evaluation_type::const_iterator iterator_solver;
        const iterator_solver& solver_end(map_solver.end());

        typedef typename database_type::map_superseries_series map_superseries_series;
        const map_superseries_series& map_superseries(idb.series_in_superseries());
        typedef typename map_superseries_series::const_iterator iterator;
        const iterator& end(map_superseries.end());
        for (iterator i = map_superseries.begin(); i != end; ++i) {
          const SuperSeries& super_series(i -> first);
          const RandomKSat& random_k_sat(get_derived_super_series(super_series));
          set_of_evaluations_type& set(map[random_k_sat]);
          
          typedef typename database_type::seq_series seq_series_type;
          const seq_series_type& seq_series(i -> second);

          typedef std::vector<RandomKSat_n> hash_map_type;
          hash_map_type hash_map;
          hash_map.reserve(seq_series.size());
          {
            typedef typename seq_series_type::const_iterator iterator;
            const iterator& end(seq_series.end());
            for (iterator i = seq_series.begin(); i != end; ++i)
              hash_map.push_back(get_derived_series(*i));
          }
          std::sort(hash_map.begin(), hash_map.end(), sort_by_n());
          assert(std::adjacent_find(hash_map.begin(), hash_map.end(), boost::not2(sort_by_n())) == hash_map.end());

          for (iterator_solver j(map_solver.begin()); j != solver_end; ++j) {
            const Solver& solver(j -> first);
            evaluation_vector_type evaluation_vector(hash_map.size());
            typedef typename lexicographical_evaluation_type::map_solver_evaluation_type map_solver_evaluation_type;
            const map_solver_evaluation_type map_solver_all_evaluations(j -> second);
            {
              typedef typename evaluation_vector_type::iterator iterator;
              const iterator& end(evaluation_vector.end());
              for (iterator i(evaluation_vector.begin()); i != end; ++i)
                i -> first = 0;
            }
            {
              typedef typename map_solver_evaluation_type::const_iterator iterator_map_solver_evaluation;
              const iterator_map_solver_evaluation& aux(map_solver_all_evaluations.find(random_k_sat));
              if (aux == map_solver_all_evaluations.end()) continue;
              assert(aux != map_solver_all_evaluations.end());
              typedef typename lexicographical_evaluation_type::map_series_numerics_type map_series_numerics_type;
              const map_series_numerics_type& map_solver_evaluations(aux -> second);
              typedef typename map_series_numerics_type::const_iterator iterator_map_series_numerics;
              const iterator_map_series_numerics& end(map_solver_evaluations.end());
              for (iterator_map_series_numerics k(map_solver_evaluations.begin()); k != end; ++k) {
                const RandomKSat_n& random_k_sat_n(k -> first);
                {
                  const typename hash_map_type::iterator& found(std::lower_bound(hash_map.begin(), hash_map.end(), random_k_sat_n, sort_by_n()));
                  assert(found != hash_map.end());
                  const typename hash_map_type::iterator::difference_type& index(found - hash_map.begin());
                  assert(index >= 0);
                  assert((typename hash_map_type::size_type)index < hash_map.size());
                  evaluation_vector[index] = k -> second;
                  assert(*found == random_k_sat_n);
                }
              }
            }

            set.insert(solver_evaluation_pair_type(solver, evaluation_vector));
          }
        }
      }

      const set_of_evaluations_type& evaluation(const SuperSeries& super_series) const {
        return OKlib::SetAlgorithms::map_value(map, super_series);
      }

    public : // ToDo: Design

      typedef std::map<RandomKSat, set_of_evaluations_type> map_super_series_to_evaluations_type;
      map_super_series_to_evaluations_type map;

      void print(std::ostream& out) const {
        const bool fixed = out.flags() & std::ios_base::fixed; // ToDo: RAII
        if (not fixed)
          out.setf(std::ios_base::fixed);
        typedef typename map_super_series_to_evaluations_type::const_iterator iterator;
        const iterator& end(map.end());
        for (iterator i = map.begin(); i != end; ++i) {
          print(out, *i);
          out << "\n";
        }
        if (not fixed)
          out.unsetf(std::ios_base::fixed);
      }
    private :

      typedef typename map_super_series_to_evaluations_type::value_type value_type;

      static void print(std::ostream& out, const value_type& pair) {
        out << pair.first << "\t:\n\n";
        print(out, pair.second);
        out << "\n";
      }
      static void print(std::ostream& out, const set_of_evaluations_type& set) {
        typedef typename set_of_evaluations_type::const_iterator iterator;
        const iterator& end(set.end());
        for (iterator i = set.begin(); i != end; ++i) {
          print(out, *i);
          out << "\n";
        }
      }
      static void print(std::ostream& out, const solver_evaluation_pair_type& pair) {
        const std::streamsize field_width_solvers(6 + 3);
        out.width(field_width_solvers);
        out << std::left << pair.first <<std::right << "\t:\t";
        print(out, pair.second);
        out << "\n";
      }
      static void print(std::ostream& out, const evaluation_vector_type& vec) {
        std::copy(vec.begin(), vec.end(), std::ostream_iterator<numerics_solver_on_series_type>(out, " | "));
      }


      RandomKSat get_derived_super_series(const SuperSeries& s) {
        return (static_cast<const ResultRandomSat*>((*OKlib::SetAlgorithms::map_value(idb.db.super_series(), s) -> begin()) -> rb)) -> super_series_random();
      }
      RandomKSat_n get_derived_series(const Series& s) {
        return (static_cast<const ResultRandomSat*>((*OKlib::SetAlgorithms::map_value(idb.db.series(), s) -> begin()) -> rb)) -> series_random();
      }

      struct sort_by_n : std::binary_function<const RandomKSat_n&, const RandomKSat_n&, bool> {
        bool operator() (const RandomKSat_n& lhs, const RandomKSat_n& rhs) const {
          return lhs.count_variables() > rhs.count_variables();
        }
      };

    };

    // ###########################################################

    template <template <typename CharT, typename ParseIterator> class ParserExtension = ParserEmpty>
    struct LexicographicalEvaluationRandom_from_file {

      typedef ResultRandomSat result_type;
      typedef Result_database_from_file<ParserResult, result_type, ParserExtension> result_database_from_file;
      typedef typename result_database_from_file::database_type database;
      typedef ElementaryAnalysis<database> indexed_database;

      typedef LexicographicalEvaluationRandom<indexed_database> lexicographical_evaluation_unfolded_type;
      typedef LexicographicalEvaluationRandom<indexed_database, LexicographicalSortingPolicy_induced_lexicographical> lexicographical_evaluation_induced_type;

      result_database_from_file rdb;
      database rdb_sat;
      database rdb_unsat;

      bool dummy;

      indexed_database idb_all, idb_sat, idb_unsat;

      lexicographical_evaluation_unfolded_type evaluation_unfolded_all;
      lexicographical_evaluation_unfolded_type evaluation_unfolded_sat;
      lexicographical_evaluation_unfolded_type evaluation_unfolded_unsat;

      lexicographical_evaluation_induced_type evaluation_induced_all;
      lexicographical_evaluation_induced_type evaluation_induced_sat;
      lexicographical_evaluation_induced_type evaluation_induced_unsat;

      LexicographicalEvaluationRandom_from_file(const std::string& file_name)  : rdb(file_name), rdb_sat(rdb.db), rdb_unsat(rdb.db), dummy(restrict_database()),  idb_all(rdb.db), idb_sat(rdb_sat), idb_unsat(rdb_unsat), evaluation_unfolded_all(idb_all),  evaluation_unfolded_sat(idb_sat), evaluation_unfolded_unsat(idb_unsat), evaluation_induced_all(idb_all),  evaluation_induced_sat(idb_sat), evaluation_induced_unsat(idb_unsat) {
        assert(dummy);
      }

    private :

      bool restrict_database() {
        rdb_sat.vector_of_sets.push_back(OKlib::SetAlgorithms::map_value(rdb_sat.sat_status(), SATStatus(sat)));
        rdb_sat.intersection();
        rdb_sat.restrict();
        rdb_unsat.vector_of_sets.push_back(OKlib::SetAlgorithms::map_value(rdb_unsat.sat_status(), SATStatus(unsat)));
        rdb_unsat.intersection();
        rdb_unsat.restrict();
        return true;
      }

    };

  }

}

#endif

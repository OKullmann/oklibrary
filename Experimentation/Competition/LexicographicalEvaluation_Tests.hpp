// Oliver Kullmann, 25.10.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Competition/LexicographicalEvaluation_Tests.hpp
  \brief Tests for tools evaluating an "indexed database" by providing comparison tools.
  \todo Distinguishinging between basic and enhanced tests is needed;
  running through these big files is "enhanced testing".
*/

#ifndef LEXICOGRAPHICALEVALUATIONTESTS_bzklapao0
#define LEXICOGRAPHICALEVALUATIONTESTS_bzklapao0

#include <string>
#include <cassert>

#include <boost/lexical_cast.hpp>

#include <OKlib/General/IteratorHandling.hpp>

#include <OKlib/Structures/Sets/SetAlgorithms/BasicMapOperations.hpp>

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

#include <OKlib/Structures/Sets/SetAlgorithms/SequenceOperations.hpp>
#include <OKlib/General/IteratorHandling.hpp>

#include <OKlib/Experimentation/Competition/SingleResult.hpp>
#include <OKlib/Experimentation/Competition/ResultProcessing.hpp>
#include <OKlib/Experimentation/Competition/ParsingSingleResult.hpp>
#include <OKlib/Experimentation/Competition/AnalysisTools.hpp>
#include <OKlib/Experimentation/Competition/ParsingResultSequences_Tests.hpp>

#include <OKlib/Experimentation/Competition/LexicographicalEvaluationPolicies.hpp>

namespace OKlib {
  namespace SATCompetition {

    /*!
      \class Test_RepresentationSolverSeries
      \brief Tests representations of the run of a solver on a series.
      \todo Complete the tests as outlined below (using the extended test
      machinery).
    */

    template < template <class IndexedDatabase, class SeriesPolicy = LexicographicalSeriesPolicy<Result>, typename NumberType = double> class LexicographicalEvaluation >
    class Test_RepresentationSolverSeries : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_RepresentationSolverSeries test_type;
      Test_RepresentationSolverSeries() {
        insert(this);
      }
    private :

      void perform_test_trivial() {
        typedef Result result_type;
        typedef Result_database_from_file<ParserResult, result_type> result_database;
        typedef typename result_database::database_type database;
        typedef ElementaryAnalysis<database> indexed_database;
        typedef LexicographicalEvaluation<indexed_database, LexicographicalSeriesPolicy<result_type> > lexicographical_evaluation_type;
        typedef typename lexicographical_evaluation_type::size_type size_type;
        typedef typename lexicographical_evaluation_type::number_type number_type;
        typedef typename lexicographical_evaluation_type::numerics_solver_on_series_type numerics_solver_on_series_type;

        // Actually, like below, we should have a function template test_result, which
        // allows for testing of the two main cases (it should not depend on the
        // result type, but we can test it).

        // We have a model of FullyConstructibleLinearOrder.

        // Using the systematic sporadic test for the representative cases
        // [(0,0), (0,0.5), (0, 1)], [(1,0), (1,0)], [(1,0.5), (1,0.5)], [(1,1), (1,1)], [(2,0)], [(2,0.5)], [(2,1)]
      }

    };



    // ##################################################

    /*!
      \class Test_LexicographicalEvaluation
      \brief Testing the basic facility for lexicographical evaluation.
      \todo Checking the average running times.
    */

    template < template <class IndexedDatabase, class SeriesPolicy = LexicographicalSeriesPolicy<Result>, typename NumberType = double> class LexicographicalEvaluation >
    class Test_LexicographicalEvaluation : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_LexicographicalEvaluation test_type;
      Test_LexicographicalEvaluation() {
        insert(this);
      }
    private :

      void perform_test_trivial() {

        test_result<Result>(filename_large_industrial, line_count_large_industrial);
        test_result<ResultRandomSat>(filename_large_random, line_count_large_random);

      }

      template <class result_type>
      void test_result(const std::string& filename, const unsigned int line_count) {

        typedef Result_database_from_file<ParserResult, result_type> result_database;
        typedef typename result_database::database_type database;
        typedef ElementaryAnalysis<database> indexed_database;
        typedef LexicographicalEvaluation<indexed_database, LexicographicalSeriesPolicy<result_type> > lexicographical_evaluation_type;
        typedef typename lexicographical_evaluation_type::size_type size_type;
        typedef typename lexicographical_evaluation_type::number_type number_type;
        typedef typename lexicographical_evaluation_type::map_solver_evaluation_type map_solver_evaluation_type;
        typedef typename lexicographical_evaluation_type::numerics_solver_on_series_type numerics_solver_on_series_type;

        result_database rdb(filename);
        assert(rdb.result_sequence.size() == line_count);
        indexed_database idb(rdb.db);
        lexicographical_evaluation_type lexicographical_evaluation(idb);

        typedef typename indexed_database::map_solver_benchmarks map_solver_benchmarks;
        typedef typename IteratorHandling::IteratorFirst<typename map_solver_benchmarks::const_iterator>::type iterator;
        const map_solver_benchmarks& map_benchmarks(idb.solved_benchmarks());
        const iterator end(map_benchmarks.end());
        for (iterator i(map_benchmarks.begin()); i != end; ++i) { // loop over all solvers
          const Solver& solver(*i);
          
          const map_solver_evaluation_type& map(lexicographical_evaluation.evaluation(solver));
          typedef typename map_solver_evaluation_type::const_iterator iterator;
          typedef typename IteratorHandling::IteratorSecond<iterator>::type iterator_second;
          OKLIB_TEST_EQUAL(OKlib::SetAlgorithms::sum_sizes(iterator_second(map.begin()), iterator_second(map.end())), OKlib::SetAlgorithms::map_value(idb.solved_series(), solver).size());
          size_type sum = 0;
          const iterator& end(map.end());
          for (iterator j(map.begin()); j != end; ++j) {
            typedef typename lexicographical_evaluation_type::map_series_numerics_type map_series_numerics_type;
            const map_series_numerics_type& map(j -> second);

            typedef typename map_series_numerics_type::const_iterator iterator;
            const iterator& end(map.end());
            for (iterator k(map.begin()); k != end; ++k) {
              const numerics_solver_on_series_type& n(k -> second);

              if (n.first <= 0)
                OKLIB_THROW("n.first <= 0, namely n.first = " + boost::lexical_cast<std::string>(n.first));
              sum += n.first;
              if (n.first != 0 and n.second < 0)
                OKLIB_THROW("n.first = " +  boost::lexical_cast<std::string>(n.first) + ", and n.second < 0, namely n.second = " + boost::lexical_cast<std::string>(n.second));
            }
          }
          if (sum != OKlib::SetAlgorithms::map_value(map_benchmarks, solver).size())
            OKLIB_THROW("sum != OKlib::SetAlgorithms::map_value(map_benchmarks, solver).size(), namely sum = " +  boost::lexical_cast<std::string>(sum) + ", while map_value(map_benchmarks, solver).size() = " + boost::lexical_cast<std::string>(OKlib::SetAlgorithms::map_value(map_benchmarks, solver).size()) + "\nContext: solver = " + boost::lexical_cast<std::string>(solver));
        }
                
      }

    };

    // ##################################################

    /*!
      \class Test_LexicographicalEvaluationRandom
      \brief Testing lexicographical evaluation.
      \todo Completing like Test_LexicographicalEvaluation.
      \todo Checking the average running times.
      \todo Checking the order details of the variations on lexicographical ordering.
    */

    template < template <class IndexedDatabase, template <class solver_evaluation_pair_type> class SortingPolicy = LexicographicalSortingPolicy_unfolded_lexicographical, typename NumberType = double> class LexicographicalEvaluationRandom >
    class Test_LexicographicalEvaluationRandom : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_LexicographicalEvaluationRandom test_type;
      Test_LexicographicalEvaluationRandom() {
        insert(this);
      }
    private :
      
      void perform_test_trivial() {
        test_result(filename_large_random, line_count_large_random);
      }

      void test_result(const std::string& filename, const unsigned int line_count) {

        typedef ResultRandomSat result_type;

        typedef Result_database_from_file<ParserResult, result_type> result_database;
        typedef typename result_database::database_type database;
        typedef ElementaryAnalysis<database> indexed_database;
        typedef LexicographicalEvaluationRandom<indexed_database> lexicographical_evaluation_random_unfolded_type;
        typedef typename lexicographical_evaluation_random_unfolded_type::size_type size_type;
        typedef typename lexicographical_evaluation_random_unfolded_type::number_type number_type;
        typedef typename lexicographical_evaluation_random_unfolded_type::numerics_solver_on_series_type numerics_solver_on_series_type;

        result_database rdb(filename);
        assert(rdb.result_sequence.size() == line_count);
        indexed_database idb(rdb.db);
        lexicographical_evaluation_random_unfolded_type lexicographical_evaluation(idb);

        // ###############################################                
      }

    };

  }

}

#endif

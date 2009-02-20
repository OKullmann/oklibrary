// Oliver Kullmann, 28.10.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Competition/LexicographicalEvaluationPolicies.hpp
  \brief Policies used by LexicographicalEvaluation.hpp

  \todo Write a concept.
  \todo Write accompanying Test and Testobjects files.
*/

#ifndef LEXICOGRAPHICALEVALUATIONPOLICIES_j57llkkK
#define LEXICOGRAPHICALEVALUATIONPOLICIES_j57llkkK

#include <functional>

#include <OKlib/General/IteratorHandling.hpp>

#include <OKlib/Experimentation/Competition/SingleResult.hpp>
#include <OKlib/Experimentation/Competition/ResultProcessing.hpp>

namespace OKlib {
  namespace SATCompetition {

    // Policies for LexicographicalEvaluation ###########################

    /*!
      \class LexicographicalSeriesPolicy
      \brief Class template governing the use of series and super-series for LexicographicalEvaluation.

      LexicographicalSeriesPolicy::super_series_type and LexicographicalSeriesPolicy::series_type specify the respective types, and LexicographicalSeriesPolicy::super_series, LexicographicalSeriesPolicy::series return the (super-)series in in solved benchmark.

      \todo Write a concept.
    */

    template <class ResultType>
    struct LexicographicalSeriesPolicy;

    template <>
    struct LexicographicalSeriesPolicy<Result> {
      typedef SuperSeries super_series_type;
      typedef Series series_type;

      template <class SolvedBenchmark>
      static super_series_type super_series(const SolvedBenchmark& sb) {
        const ResultBasis* const rb(sb.node -> rb);
        return rb -> super_series();
      }
      template <class SolvedBenchmark>
      static series_type series(const SolvedBenchmark& sb) {
        const ResultBasis* const rb(sb.node -> rb);
        return rb -> series();
      }
    };

    template <>
    struct LexicographicalSeriesPolicy<ResultRandomSat> {
      typedef RandomKSat super_series_type;
      typedef RandomKSat_n series_type;

      template <class SolvedBenchmark>
      static super_series_type super_series(const SolvedBenchmark& sb) {
        const ResultBasis* const rb(sb.node -> rb);
        return *static_cast<const super_series_type*>(rb -> super_series().self());
      }
      template <class SolvedBenchmark>
      static series_type series(const SolvedBenchmark& sb) {
        const ResultBasis* const rb(sb.node -> rb);
        return *static_cast<const series_type*>(rb -> series().self());
      }
    };

    // Policies for LexicographicalEvaluationRandom #####################

    /*!
      \class LexicographicalSortingPolicy_induced_lexicographical
      \brief Policy for LexicographicalEvaluationRandom yielding the order of evaluation
      vectors which is the lexicographical order based on the order given for
      numerics_solver_on_series_type.
    */

    template <class solver_evaluation_pair_type>
    struct LexicographicalSortingPolicy_induced_lexicographical {
      struct comparison_type : std::binary_function<const solver_evaluation_pair_type&, const solver_evaluation_pair_type&, bool> {
        bool operator()(const solver_evaluation_pair_type& lhs, const solver_evaluation_pair_type& rhs) const {
          return lhs.second < rhs.second;
        }
      };
    };

    /*!
      \class LexicographicalSortingPolicy_unfolded_lexicographical
      \brief Policy for LexicographicalEvaluationRandom yielding the order of evaluation
      vectors which is the lexicographical order when unfolding the pairs, that is,
      first all first components, then all second components.
    */

    template <class solver_evaluation_pair_type>
    struct LexicographicalSortingPolicy_unfolded_lexicographical {
      struct comparison_type : std::binary_function<const solver_evaluation_pair_type&, const solver_evaluation_pair_type&, bool> {
        bool operator()(const solver_evaluation_pair_type& lhs, const solver_evaluation_pair_type& rhs) const {
          assert(lhs.second.size() == rhs.second.size());
          typedef typename solver_evaluation_pair_type::second_type second_type;
          typedef typename second_type::const_iterator iterator;
          typedef typename IteratorHandling::IteratorFirst<iterator>::type iterator_first;
          typedef typename IteratorHandling::IteratorSecond<iterator>::type iterator_second;
          const second_type& lhs_s(lhs.second), rhs_s(rhs.second);
          const iterator_first f_lhs_b(lhs_s.begin()), f_lhs_e(lhs_s.end()), f_rhs_b(rhs_s.begin()), f_rhs_e(rhs_s.end());
          const iterator_second s_lhs_b(lhs_s.begin()), s_lhs_e(lhs_s.end()), s_rhs_b(rhs_s.begin()), s_rhs_e(rhs_s.end());
          typedef typename iterator_first::value_type size_type;
          return std::lexicographical_compare(f_lhs_b, f_lhs_e, f_rhs_b, f_rhs_e, std::greater<size_type>()) or (std::equal(f_lhs_b, f_lhs_e, f_rhs_b) and std::lexicographical_compare(s_lhs_b, s_lhs_e, s_rhs_b, s_rhs_e));
        }
      };
    };

  }

}

#endif

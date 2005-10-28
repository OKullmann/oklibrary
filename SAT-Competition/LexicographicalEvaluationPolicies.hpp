// Oliver Kullmann, 28.10.2005 (Swansea)

/*!
  \file LexicographicalEvaluationPolicies.hpp
  \brief Policies used by LexicographicalEvaluation.hpp
  \todo Write a concept.
  \todo Write accompanying Test and Testobjects files.
*/

#ifndef LEXICOGRAPHICALEVALUATIONPOLICIES_j57llkkK

#define LEXICOGRAPHICALEVALUATIONPOLICIES_j57llkkK

#include "SingleResult.hpp"
#include "ResultProcessing.hpp"

namespace OKlib {

  namespace SATCompetition {

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


  }

}

#endif

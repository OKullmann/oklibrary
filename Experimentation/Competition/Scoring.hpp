// Oliver Kullmann, 17.6.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Competition/Scoring.hpp
  \brief Given an "indexed database", computing the solver scores.
*/

#ifndef SCORING_567UyTr
#define SCORING_567UyTr

#include <algorithm>
#include <cassert>
#include <vector>
#include <string>
#include <ostream>
#include <functional>

#include <boost/format.hpp>

#include <OKlib/General/IteratorHandling.hpp>

#include <OKlib/Structures/Sets/SetAlgorithms/BasicMapOperations.hpp>
#include <OKlib/Programming/Utilities/OrderRelations/DerivedRelations.hpp>

#include <OKlib/Experimentation/Competition/SingleResult.hpp>
#include <OKlib/Experimentation/Competition/ParsingSingleResult.hpp>
#include <OKlib/Experimentation/Competition/ParsingResultSequences.hpp>
#include <OKlib/Experimentation/Competition/ResultProcessing.hpp>
#include <OKlib/Experimentation/Competition/AnalysisTools.hpp>

namespace OKlib {
  namespace SATCompetition {

    /*!
      \class ConstantSeriesPurse
      \brief Policy for PurseScoring, which does not change the series purse.
    */
    struct ConstantSeriesPurse {
      template <typename Number, typename Int>
      static Number purse(const Number initial_purse, const Int count) {
        return initial_purse;
      }
    };
    /*!
      \class SAT2005SeriesPurse
      \brief Policy for PurseScoring, which does divides the series purse by 3, if only 4 or less
      benchmarks are in the series.
    */
    struct SAT2005SeriesPurse {
      template <typename Number, typename Int>
      static Number purse(const Number initial_purse, const Int count) {
        const Int count_threshold = 5;
        if (count >= count_threshold)
          return initial_purse;
        else {
          const Number weight = Number(1) / Number(3);
          return weight * initial_purse;
        }
      }
    };

    // ###############

    /*!
      \class PurseScoring
      \brief Computing score(solver) = problem_purse(solver) + speed_award(solver) + series_purse(solver).
      \todo Concepts for IndexedDatabase and SeriesPursePolicy are needed.
      \todo Make the implementation more efficient by storing intermediate results.
    */

    template <class IndexedDatabase, class SeriesPursePolicy = SAT2005SeriesPurse, typename NumberType = double>
    class PurseScoring {

    public :

      const IndexedDatabase& idb;

      typedef NumberType number_type;
      typedef IndexedDatabase database_type;
      typedef SeriesPursePolicy series_policy_type;

      const number_type standard_problem_purse;
      const number_type standard_speed_factor;
      const number_type standard_series_factor;

      const number_type standard_speed_purse;
      const number_type standard_series_purse;

      PurseScoring(const IndexedDatabase& idb, const number_type standard_problem_purse = 1000, const number_type standard_speed_factor = 1, const number_type standard_series_factor = 3) :
        idb(idb), standard_problem_purse(standard_problem_purse), standard_speed_factor(standard_speed_factor), standard_series_factor(standard_series_factor), standard_speed_purse(standard_speed_factor * standard_problem_purse), standard_series_purse(standard_series_factor * standard_problem_purse) {}

      virtual ~PurseScoring() {}
      
      bool solved(const Solver& solver, const Benchmark& bench) const {
        return solved_(solver, bench);
      }

      bool solved(const Solver& solver, const SpecSeries& series) const {
        return solved_(solver, series);
      }

    private :

      typedef typename database_type::seq_solvers seq_solvers;

    public :

      typedef typename seq_solvers::size_type size_type_solvers;

      size_type_solvers solved(const Benchmark& bench) const {
        return solved_(bench);
      }

      size_type_solvers solved(const SpecSeries& series) const {
        return solved_(series);
      }

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

      number_type total_time(const Benchmark& bench) const {
        return total_time_(bench);
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
      typedef typename database_type::seq_benchmarks seq_benchmarks;
      typedef typename seq_benchmarks::size_type size_type_benchmarks;


      typedef typename database_type::seq_spec_series seq_spec_series;

      typedef typename database_type::SolvedBenchmark SolvedBenchmark;

      // --------------------------------------------------------------

      virtual bool solved_(const Solver& solver, const Benchmark& bench) const {
        const seq_solvers& seq(OKlib::SetAlgorithms::map_value(idb.succesful_solvers(), bench));
        return std::binary_search(seq.begin(), seq.end(), solver);
      }

      virtual bool solved_(const Solver& solver, const SpecSeries& series) const {
        const seq_spec_series& seq(OKlib::SetAlgorithms::map_value(idb.solved_series(), solver));
        return std::binary_search(seq.begin(), seq.end(), series);
      }

      virtual size_type_solvers solved_(const Benchmark& bench) const {
        const seq_solvers& seq(OKlib::SetAlgorithms::map_value(idb.succesful_solvers(), bench));
        const size_type_solvers& count(seq.size());
        return count;
      }

      virtual size_type_solvers solved_(const SpecSeries& series) const {
        size_type_solvers count = 0;
        typedef typename database_type::map_solver_benchmarks map_solver_benchmarks;
        typedef typename IteratorHandling::IteratorFirst<typename map_solver_benchmarks::const_iterator>::type iterator;
        const map_solver_benchmarks& map(idb.solved_benchmarks());
        const iterator end(map.end());
        for (iterator i(map.begin()); i != end; ++i) { // loop over all solvers
          const Solver& solver(*i);
            count += solved(solver, series);
          }
        return count;
      }

      // --------------------------------------------------------------

      virtual number_type problem_purse_(const Solver& solver, const Benchmark& bench) const {
        if (solved(solver, bench)) {
          const size_type_solvers& count(solved(bench));
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
        for (iterator i = seq.begin(); i != end; ++i) { // loop over all benchmarks the solver solved
          const Benchmark& bench(*i);
          sum += problem_purse(solver, bench);
        }
        return sum;
      }

      // --------------------------------------------------------------

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
        const iterator& end(seq.end());
        number_type sum = 0;
        for (iterator i = seq.begin(); i != end; ++i) {
          const Solver& solver(*i);
          sum += speed_factor(solver, bench); // ToDo: Here the first test in speed_factor(solver, bench) is not necessary
        }
        return sum;
      }

      virtual number_type total_time_(const Benchmark& bench) const {
        const seq_solvers& seq(OKlib::SetAlgorithms::map_value(idb.succesful_solvers(), bench));
        typedef typename seq_solvers::const_iterator iterator;
        const iterator& end(seq.end());
        number_type sum = 0;
        for (iterator i = seq.begin(); i != end; ++i) {
          const Solver& solver(*i);
          const seq_solved_benchmarks& seq(OKlib::SetAlgorithms::map_value(idb.solved_benchmarks(), solver));
          typedef typename seq_solved_benchmarks::const_iterator iterator;
          const iterator& solution_it(std::lower_bound(seq.begin(), seq.end(), SolvedBenchmark(bench)));
          assert(solution_it != seq.end());
          const SolvedBenchmark& solution(*solution_it);
          assert(solution.bench == bench);
          typedef ResultElement::floating_point_type floating_point_type;
          const floating_point_type& time_needed(solution.node -> rb -> average().average());
          sum += time_needed;
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
     
      // --------------------------------------------------------------

      virtual number_type series_purse_(const SpecSeries& series) const {
        const seq_benchmarks& seq(OKlib::SetAlgorithms::map_value(idb.benchmarks_in_series(), series));
        const size_type_benchmarks& size(seq.size());
        return series_policy_type::purse(standard_series_purse, size);
      }

      virtual number_type series_purse_(const Solver& solver, const SpecSeries& series) const {
        if (solved(solver, series)) {
          const number_type count = solved(series);
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
        for (iterator i = seq.begin(); i != end; ++i) { // loop over all series' the solver solved
          const SpecSeries& series(*i);
          sum += series_purse(solver, series);
        }
        return sum;
      }

      // --------------------------------------------------------------

      virtual number_type score_(const Solver& solver) const {
        return problem_purse(solver) + speed_award(solver) + series_purse(solver);
      }

    };

    // #######################################################

    /*!
      \class Scoring_from_file
      \brief Given a file with competition results, computes sorted lists of scores for the SAT, UNSAT and SAT+UNSAT categories.
      \todo PROBLEM: When restricting to the SAT or to the UNSAT case (see objects rdb_sat
      and rdb_unsat), then it might happen that in case less then 5 but at least one of the
      benchmarks in the series has been solved, that we apply the rule for
      diminishing the series purse by the factor of three to the series,
      while actually the series had enough SAT resp. UNSAT instances in them.
      \todo Additional to the current computation for the SAT+UNSAT category, compute the scores when considering only "complete" solvers.
      \todo This class is a "blob" --- many things are done here, which should be split over several classes.
    */

    template <template <typename CharT, typename ParseIterator> class ParserExtension = ParserEmpty, class ResultClass = Result, class SeriesPursePolicy = SAT2005SeriesPurse>
    struct Scoring_from_file {
      typedef Result_database_from_file<ParserResult, ResultClass, ParserExtension> result_database_from_file;
      typedef typename result_database_from_file::database_type database;
      typedef ElementaryAnalysis<database> indexed_database;
      typedef PurseScoring<indexed_database, SeriesPursePolicy> purse_scoring_type;

      result_database_from_file rdb;
      indexed_database idb;

      typedef typename purse_scoring_type::number_type number_type;

      struct series_info {
        SpecSeries series;
        number_type total_series_purse;
        number_type solver_series_purse;
        series_info (const SpecSeries& series, const number_type total_series_purse, const number_type solver_series_purse) : series(series), total_series_purse(total_series_purse), solver_series_purse(solver_series_purse) {}
        bool solved() const { return solver_series_purse != 0; }
        friend bool operator <(const series_info& lhs, const series_info& rhs) {
          return lhs.solver_series_purse < rhs.solver_series_purse;
        }
        friend bool operator ==(const series_info& lhs, const series_info& rhs) {
          return lhs.solver_series_purse == rhs.solver_series_purse;
        }
        OKLIB_DERIVED_RELATIONS_FRIENDS(series_info);
        friend std::ostream& operator <<(std::ostream& out, const series_info& s) {
          if (s.solved()) {
            out << boost::format("  %-60s : %10.1f %10.1f") % s.series % s.solver_series_purse % s.total_series_purse;
            return out << "\n";
          }
          else
            return out;
        }
      };

      struct solved_series : std::unary_function<const series_info&, bool> {
        bool operator() (const series_info& si) const {
          return si.solved();
        }
      };

      /*!
        \class OKlib::SATCompetition::Scoring_from_file::scoring
        \brief Contains scoring related data for one solver.
        \todo Not only the number of instances solved, but also the number of sat and unsat instances solved.
      */
      
      struct scoring {
        Solver solver;
        number_type score;
        number_type problem_purse;
        number_type speed_award;
        number_type series_purse;

        typedef std::vector<series_info> vector_type;
        vector_type series_info_vector;

        typedef unsigned int natural_number_type;
        natural_number_type instances_solved;
        //ToDo: Finer: number sat/unsat solved.

        scoring(const Solver& solver, const number_type score, const number_type problem_purse, const number_type speed_award, const number_type series_purse, const natural_number_type instances_solved) : solver(solver), score(score), problem_purse(problem_purse), speed_award(speed_award), series_purse(series_purse), instances_solved(instances_solved) {}
        friend bool operator <(const scoring& lhs, const scoring& rhs) {
          return lhs.score < rhs.score;
        }
        friend bool operator ==(const scoring& lhs, const scoring& rhs) {
          return lhs.score == rhs.score;
        }
        OKLIB_DERIVED_RELATIONS_FRIENDS(scoring);
        friend std::ostream& operator <<(std::ostream& out, const scoring& s) {
          out << boost::format("%-8s : %10.1f %10.1f %10.1f %10.1f\n") % s.solver % s.score % s.problem_purse % s.speed_award % s.series_purse;
          out << boost::format("Number of instances solved: %5u\n") % s.instances_solved;
          out << boost::format("Number of series solved: %4u\n") % std::count_if(s.series_info_vector.begin(), s.series_info_vector.end(), solved_series());
          out << "Composition of series purse:\n";
          std::copy(s.series_info_vector.begin(), s.series_info_vector.end(), std::ostream_iterator<series_info>(out, ""));
          return out;
        }
      };

      typedef std::vector<scoring> scoring_vector;
      scoring_vector scores_all;
      scoring_vector scores_sat;
      scoring_vector scores_unsat;

      const number_type standard_problem_purse;
      const number_type standard_speed_factor;
      const number_type standard_series_factor;

      Scoring_from_file(const std::string file_name, const number_type standard_problem_purse = 1000, const number_type standard_speed_factor = 1, const number_type standard_series_factor = 3) : rdb(file_name), idb(rdb.db), standard_problem_purse(standard_problem_purse), standard_speed_factor(standard_speed_factor), standard_series_factor(standard_series_factor) {

        {
          purse_scoring_type purse_scoring_all(idb, standard_problem_purse, standard_speed_factor, standard_series_factor);
          fill_scoring_vectors(scores_all, purse_scoring_all);
        }
        {
          database rdb_sat = rdb.db;
          rdb_sat.vector_of_sets.push_back(OKlib::SetAlgorithms::map_value(rdb_sat.sat_status(), SATStatus(sat)));
          rdb_sat.intersection();
          rdb_sat.restrict();
          indexed_database idb_sat(rdb_sat);
          purse_scoring_type purse_scoring_sat(idb_sat, standard_problem_purse, standard_speed_factor, standard_series_factor);
          fill_scoring_vectors(scores_sat, purse_scoring_sat);
        }
        {
          database rdb_unsat = rdb.db;
          rdb_unsat.vector_of_sets.push_back(OKlib::SetAlgorithms::map_value(rdb_unsat.sat_status(), SATStatus(unsat)));
          rdb_unsat.intersection();
          rdb_unsat.restrict();
          indexed_database idb_unsat(rdb_unsat);
          purse_scoring_type purse_scoring_unsat(idb_unsat, standard_problem_purse, standard_speed_factor, standard_series_factor);
          fill_scoring_vectors(scores_unsat, purse_scoring_unsat);
        }
      }

    private :

      void fill_scoring_vectors(scoring_vector& vector, purse_scoring_type& purse_scoring) {
        typedef typename indexed_database::map_solver_benchmarks map_solver;
        const map_solver& solvers(purse_scoring.idb.solved_benchmarks());
        vector.reserve(solvers.size());
        typedef typename map_solver::const_iterator iterator_solver;
        const iterator_solver& end_solvers(solvers.end());
        for (iterator_solver i = solvers.begin(); i != end_solvers; ++i) {
          const Solver& solver(i -> first);
          vector.push_back(scoring(solver, purse_scoring.score(solver), purse_scoring.problem_purse(solver), purse_scoring.speed_award(solver), purse_scoring.series_purse(solver), i -> second.size()));
          scoring& current(vector.back());
          typedef typename indexed_database::map_series_benchmarks map_series;
          const map_series& all_series(purse_scoring.idb.benchmarks_in_series());
          current.series_info_vector.reserve(all_series.size());
          typedef typename map_series::const_iterator iterator_series;
          const iterator_series& end_series(all_series.end());
          for (iterator_series j = all_series.begin(); j != end_series; ++j) {
            const SpecSeries& series(j -> first);
            current.series_info_vector.push_back(series_info(series,  purse_scoring.series_purse(series), purse_scoring.series_purse(solver, series)));
          }
          std::sort(current.series_info_vector.begin(), current.series_info_vector.end());
        }
        std::sort(vector.begin(), vector.end());
      }

    };

  }

}

#endif

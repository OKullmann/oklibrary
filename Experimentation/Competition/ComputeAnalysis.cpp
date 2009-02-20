// Oliver Kullmann, 5.11.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Competition/ComputeAnalysis.cpp
  \brief Analyse a file with competition data by printing out for all benchmarks
  the number of succesful solvers, the result problem purse, the speed factor and the
  SAT status (as determined by the solver results).

  \todo Usage of ProgramOptions.
  \todo Classes PrintStandard and PrintLatex should inherit from some common base class.
*/

#include <cassert>
#include <iostream>
#include <utility>
#include <ios>
#include <ostream>

#include <OKlib/Structures/Sets/SetAlgorithms/BasicMapOperations.hpp>

#include <OKlib/Experimentation/Competition/SingleResult.hpp>
#include <OKlib/Experimentation/Competition/ParsingSingleResult.hpp>
#include <OKlib/Experimentation/Competition/Scoring.hpp>
#include <OKlib/Experimentation/Competition/AnalysisTools.hpp>

namespace {

  template <bool with_extension>
  struct result_database {
    typedef OKlib::SATCompetition:: Result_database_from_file<OKlib::SATCompetition::ParserResult, OKlib::SATCompetition::Result, OKlib::SATCompetition::ParserThreeElements> type;
  };
  template <>
  struct result_database<false> {
    typedef OKlib::SATCompetition:: Result_database_from_file<OKlib::SATCompetition::ParserResult> type;
  };

  // ###################

  struct PrintStandard {
    std::ostream& out;
    const std::streamsize precision;
    const bool fixed;
    
    PrintStandard(std::ostream& out) : out(out), precision(out.precision()), fixed(out.flags() & std::ios_base::fixed) {
      const std::streamsize new_precision = 1;
      out.precision(new_precision);
      if (not fixed)
        out.setf(std::ios_base::fixed);
    }
    ~PrintStandard() {
      out.precision(precision);
      if (not fixed)
        out.unsetf(std::ios_base::fixed);
    }

    template <class Benchmark, typename Counter, typename Number, class SatStatus>
    void operator() (const Benchmark& bench, Counter solved, const Number standard_problem_purse, const Number total_time, const SatStatus sat_status) const {
      const std::streamsize field_width_bench(5 + 5 + 1);
      out.width(field_width_bench);
      out << bench << ": ";
      const std::streamsize field_width_count(3);
      out.width(field_width_count);
      out << solved << " ";
      const std::streamsize field_width_score(4+2);
      out.width(field_width_score);
      if (not solved)
        out << "NaN" << "\n";
      else {
        out << standard_problem_purse / solved << "; ";
        const std::streamsize field_width_average_time(6+2);
        out.width(field_width_average_time);
        out << total_time / solved << "; ";
        assert(sat_status != OKlib::SATCompetition::error);
        assert(sat_status != OKlib::SATCompetition::unknown);
        switch (sat_status) {
        case OKlib::SATCompetition::sat :
          out << "SAT"; break;
        default :
          out << "UNSAT"; break;
        }
        out << "\n";
      }
    }
  };
  
  struct PrintLatex {
    std::ostream& out;
    const std::streamsize precision;
    const bool fixed;
    
    PrintLatex(std::ostream& out) : out(out), precision(out.precision()), fixed(out.flags() & std::ios_base::fixed) {
      const std::streamsize new_precision = 1;
      out.precision(new_precision);
      if (not fixed)
        out.setf(std::ios_base::fixed);
    }
    ~PrintLatex() {
      out.precision(precision);
      if (not fixed)
        out.unsetf(std::ios_base::fixed);
    }
    
    template <class Benchmark, typename Counter, typename Number, class SatStatus>
    void operator() (const Benchmark& bench, Counter solved, const Number standard_problem_purse, const Number total_time, const SatStatus sat_status) const {
      const int length_word_bench(5);
      assert(bench.name().size() >= length_word_bench + 1);
      out << " & " << bench.name().substr(length_word_bench) << " & $";
      if (not solved)
        out << "?$ & NaN";
      else {
        assert(sat_status != OKlib::SATCompetition::error);
        assert(sat_status != OKlib::SATCompetition::unknown);
        switch (sat_status) {
        case OKlib::SATCompetition::sat :
          out << "1"; break;
        case OKlib::SATCompetition::unsat :
          out << "0"; break;
        }
        out << "$ & $" << solved << "$ & $" << standard_problem_purse / solved << "$ & $" << total_time / solved << "$";
      }
      out << "\\\\\n";
    }
  };

  // ###################

#ifdef LATEXOUTPUT
  typedef PrintLatex DefaultPolicy;
#else
  typedef PrintStandard DefaultPolicy;
#endif

  template <bool with_extension, class DataLinePolicy = DefaultPolicy>
  struct Evaluation {
    const std::string& filename;
    const char* const specifier;
    typedef typename result_database<with_extension>::type result_database_from_file_type;
    typedef typename result_database_from_file_type::database_type result_database_type;
    typedef OKlib::SATCompetition::ElementaryAnalysis<result_database_type> indexed_database_type;
    typedef OKlib::SATCompetition::PurseScoring<indexed_database_type> scoring_type;
    typedef typename scoring_type::number_type number_type;
    Evaluation(const std::string& filename, const char* const specifier) : filename(filename), specifier(specifier) {
      if (with_extension)
        assert(specifier);
    }
    void operator() (const number_type standard_problem_purse = 1000, const number_type standard_speed_factor = 1, const number_type standard_series_factor = 3) const {
      
      const result_database_from_file_type rdb(filename);
      const indexed_database_type idb(rdb.db);
      scoring_type scores(idb, standard_problem_purse, standard_speed_factor, standard_series_factor);
      
      std::cout << "\nFile name = " << filename;
      if (specifier)
        std::cout << "\nsyntax specifier = " << specifier << "\n";
      std::cout << "\n";
      std::cout << "Standard problem purse = " << standard_problem_purse << "\n";
      std::cout << "Standard speed factor = " << standard_speed_factor << "\n";
      std::cout << "Standard series factor = " << standard_series_factor << "\n\n";
      
      DataLinePolicy out(std::cout);
      
      typedef typename indexed_database_type::map_superseries_series map_superseries_series;
      const map_superseries_series& map_superseries(idb.series_in_superseries());
      typedef typename map_superseries_series::const_iterator iterator_superseries;
      const iterator_superseries& end_superseries(map_superseries.end());
      for (iterator_superseries i = map_superseries.begin(); i != end_superseries; ++i) {
        const OKlib::SATCompetition::SuperSeries& superseries(i -> first);
        std::cout << "###########################################\n\n" << superseries << "\n\n";
        typedef typename indexed_database_type:: seq_series seq_series_type;
        const seq_series_type& seq_series(i -> second);
        typedef typename seq_series_type::const_iterator iterator_series;
        const iterator_series& end_series(seq_series.end());
        for (iterator_series j = seq_series.begin(); j != end_series; ++j) {
          const OKlib::SATCompetition::Series& series(*j);
          std::cout << "#######\n" << series << "\n\n";
          typedef typename indexed_database_type::seq_benchmarks seq_benchmarks_type;
          const seq_benchmarks_type& seq_benchmarks(OKlib::SetAlgorithms::map_value(idb.benchmarks_in_series(), std::make_pair(superseries, series)));
          typedef typename seq_benchmarks_type::const_iterator iterator_benchmarks;
          const iterator_benchmarks end_benchmarks(seq_benchmarks.end());
          for (iterator_benchmarks k = seq_benchmarks.begin(); k != end_benchmarks; ++k) {
            const OKlib::SATCompetition::Benchmark& bench(*k);
            typedef typename scoring_type::size_type_solvers size_type_solvers;
            const size_type_solvers& solved(scores.solved(bench));
            const number_type& total_time(scores.total_time(bench));
            const OKlib::SATCompetition::SolverResult sat_status(OKlib::SetAlgorithms::map_value(idb.sat_status(), bench).result());
            out(bench, solved, standard_problem_purse, total_time, sat_status);
          }
          std::cout << "\n";
        }
      }
    }
  };
}

// ##########################################

int main(const int argc, const char* const argv[]) {

  if (argc <= 1 or argc >= 4) {
    std::cerr << "One or two arguments required (the name of the file, optionally a syntax specifier \"syntax=...\").\n";
    return EXIT_FAILURE;
  }
  bool specifier = false;
  const char* specifier_text = (const char*)(0);
  const std::string& file_name(argv[1]);
  for (int i = 2; i < argc; ++i) {
    const int prefix_length = 6 + 1;
    assert(prefix_length >= 0);
    if (std::strlen(argv[i]) <= (unsigned int)prefix_length) {
      std::cerr << "Parameter \"" << argv[i] << "\" is not \"syntax=+\".\n";
      return EXIT_FAILURE;
    }
    if (std::strncmp(argv[i], "syntax=", prefix_length) == 0) {
      specifier = true;
      specifier_text = argv[i] + prefix_length;
    }
    else {
      std::cerr << "Parameter \"" << argv[i] << "\" does not start with \"syntax=\".\n";
      return EXIT_FAILURE;
    }
  }

  if (specifier)
      Evaluation<true>(file_name, specifier_text)();
  else
      Evaluation<false>(file_name, specifier_text)();
}

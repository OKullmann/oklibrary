// Oliver Kullmann, 17.6.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Competition/ComputeScores.cpp
  \brief Outputs the sorted scores for all solvers from competition data (given as a file).
  With the optional parameter syntax=... (currently it doesn't matter, but it must be
  non-empty) the extended syntax is used, and with the optional parameter
  series=... (currently it doesn't matter, but it must be non-empty) the series purse
  is computed without performing case distinctions.

  \todo The output should also contain date and time of the computation; and
  information about the program which produced the output (we need a general
  convention of how to make information about the compilation available to
  the program, so that via "--version" we obtain as much information as possible).
  \todo The output should contain a legend.
  \todo See ComputeLexicographicalEvaluation.cpp for common functionality.
  \todo Using the new module ProgramOptions (together with Messages).
  \todo Perhaps the structure Scoring_from_file (in this file) should be generalised (it can be shared
  at least with ComputeAnalysis.cpp); and also the option handling could be shared.
*/

#include <string>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <cassert>

#include <OKlib/Experimentation/Competition/ParsingSingleResult.hpp>
#include <OKlib/Experimentation/Competition/Scoring.hpp>

namespace {

  template <bool with_extension, bool without_exception>
  struct Scoring_from_file {
    typedef OKlib::SATCompetition::Scoring_from_file<OKlib::SATCompetition::ParserThreeElements, OKlib::SATCompetition::Result, OKlib::SATCompetition::ConstantSeriesPurse> type;
  };
  template <>
  struct Scoring_from_file<false, false> {
    typedef OKlib::SATCompetition::Scoring_from_file<> type;
  };
  template <>
  struct Scoring_from_file<true, false> {
    typedef OKlib::SATCompetition::Scoring_from_file<OKlib::SATCompetition::ParserThreeElements> type;
  };
  template <>
  struct Scoring_from_file<false, true> {
    typedef OKlib::SATCompetition::Scoring_from_file<OKlib::SATCompetition::ParserEmpty, OKlib::SATCompetition::Result, OKlib::SATCompetition::ConstantSeriesPurse> type;
  };
  
  
  template <typename InputIterator>
  void output_sequence(const InputIterator& begin, const InputIterator& end) {
    typedef typename InputIterator::value_type value_type;
    std::copy(begin, end, std::ostream_iterator<value_type>(std::cout, "\n"));
  }
  
  template <bool with_extension, bool without_exception>
  struct Evaluation {
    const std::string& filename;
    const char* const specifier;
    const char* const policy;
    typedef typename Scoring_from_file<with_extension, without_exception>::type scoring_from_file;
    typedef typename scoring_from_file::number_type number_type;
    Evaluation(const std::string& filename, const char* const specifier, const char* const policy) : filename(filename), specifier(specifier), policy(policy) {
      if (with_extension)
        assert(specifier);
      if (without_exception)
        assert(policy);
    }
    void operator() (const number_type standard_problem_purse = 1000, const number_type standard_speed_factor = 1, const number_type standard_series_factor = 3) const {
      
      const scoring_from_file scores(filename, standard_problem_purse, standard_speed_factor, standard_series_factor);
      std::cout << "\nFile name = " << filename;
      if (specifier)
        std::cout << "\nsyntax specifier = " << specifier << "\n";
      if (policy)
        std::cout << "series policy = " << policy << "\n";
      std::cout << "\n";
      std::cout << "Standard problem purse = " << standard_problem_purse << "\n";
      std::cout << "Standard speed factor = " << standard_speed_factor << "\n";
      std::cout << "Standard series factor = " << standard_series_factor << "\n";
      std::cout << "\nALL results: ---------------------------------------------------------------------------------------------------------------------\n\n";
      output_sequence(scores.scores_all.begin(), scores.scores_all.end());
      std::cout << "\nSAT results: ---------------------------------------------------------------------------------------------------------------------\n\n";
      output_sequence(scores.scores_sat.begin(), scores.scores_sat.end());
      std::cout << "\nUNSAT results: ---------------------------------------------------------------------------------------------------------------------\n\n";
      output_sequence(scores.scores_unsat.begin(), scores.scores_unsat.end());
    }
  };
}

int main(const int argc, const char* const argv[]) {

  if (argc <= 1 or argc >= 5) {
    std::cerr << "One, two or three arguments required (the name of the file, optionally a syntax specifier \"syntax=...\", optionally a series policy specifier \"series=...\").\n";
    return EXIT_FAILURE;
  }
  bool specifier = false;
  const char* specifier_text = (const char*)(0);
  bool policy = false;
  const char* policy_text = (const char*)(0);
  const std::string& file_name(argv[1]);
  for (int i = 2; i < argc; ++i) {
    const int prefix_length = 6 + 1;
    assert(prefix_length >= 0);
    if (std::strlen(argv[i]) <= (unsigned int)prefix_length) {
      std::cerr << "Parameter \"" << argv[i] << "\" is not \"syntax=+\" or \"series=+\".\n";
      return EXIT_FAILURE;
    }
    if (std::strncmp(argv[i], "syntax=", prefix_length) == 0) {
      specifier = true;
      specifier_text = argv[i] + prefix_length;
    }
    else if (std::strncmp(argv[i], "series=", prefix_length) == 0) {
      policy = true;
      policy_text = argv[i] + prefix_length;
    }
    else {
      std::cerr << "Parameter \"" << argv[i] << "\" does not start with \"syntax=\" or \"series=\".\n";
      return EXIT_FAILURE;
    }
  }

  if (specifier) {
    if (policy)
      Evaluation<true, true>(file_name, specifier_text, policy_text)();
    else
      Evaluation<true, false>(file_name, specifier_text, policy_text)();
  }
  else {
    if (policy)
      Evaluation<false, true>(file_name, specifier_text, policy_text)();
    else
      Evaluation<false, false>(file_name, specifier_text, policy_text)();
  }
}

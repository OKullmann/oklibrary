// Oliver Kullmann, 17.6.2005 (Swansea)

/*!
  \file ComputeScores.cpp
  \brief Outputs the sorted scores for all solvers from competition data (given as a file).
  If an additional second input is given (currently it doesn't matter what), then the
  extended syntax of round 2 is assumed (otherwise the syntax of round 1).
  \todo It must be possible to specify on the command line which policy for the
  computation of the series purse is to be used (see class template PurseScoring), and how
  to apply this to the SAT/UNSAT sub-competitions.
  \todo The output should also contain data and time of the computation.
  \todo The output should contain a legend.
  \todo Using the new module ProgramOptions (together with Messages).
*/
#include <string>
#include <iterator>
#include <algorithm>
#include <iostream>

#include "ParsingSingleResult.hpp"
#include "Scoring.hpp"

template <bool with_extension>
struct Scoring_from_file {
  typedef OKlib::SATCompetition::Scoring_from_file<OKlib::SATCompetition::ParserThreeElements> type;
};
template <>
struct Scoring_from_file<false> {
  typedef OKlib::SATCompetition::Scoring_from_file<> type;
};

template <typename InputIterator>
void output_sequence(const InputIterator& begin, const InputIterator& end) {
  typedef typename InputIterator::value_type value_type;
  std::copy(begin, end, std::ostream_iterator<value_type>(std::cout, "\n"));
}

template <bool with_extension>
struct Evaluation {
  const std::string& filename;
  const char* const specifier;
  typedef typename Scoring_from_file<with_extension>::type scoring_from_file;
  typedef typename scoring_from_file::number_type number_type;
  Evaluation(const std::string& filename, const char* const specifier) : filename(filename), specifier(specifier) {}
  void operator() (const number_type standard_problem_purse = 1000, const number_type standard_speed_factor = 1, const number_type standard_series_factor = 3) {
    
    const scoring_from_file scores(filename, standard_problem_purse, standard_speed_factor, standard_series_factor);
    std::cout << "\nFile name = " << filename;
    if (specifier)
      std::cout << "\nspecifier = " << specifier;
    std::cout << "\n\n";
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

int main(const int argc, char* const argv[]) {

  if (argc <= 1 or argc >= 4) {
    std::cerr << "One or two arguments required (the name of the file, and optionally a syntax specifier).\n";
    return EXIT_FAILURE;
  }
  const std::string& file_name(argv[1]);
  const bool specifier = (argc == 3);
  const char* const specifier_text = (specifier) ? argv[2] : (const char*)(0);

  if (specifier) {
    Evaluation<true>(file_name, specifier_text)();
  }
  else {
    Evaluation<false>(file_name, specifier_text)();
  }
}

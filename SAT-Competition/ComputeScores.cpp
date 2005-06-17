// Oliver Kullmann, 17.6.2005 (Swansea)

#include <string>
#include <cstdlib>
#include <iostream>
#include <map>

#include "SingleResult.hpp"
#include "ParsingSingleResult.hpp"
#include "ParsingResultSequences.hpp"
#include "ResultProcessing.hpp"
#include "AnalysisTools.hpp"
#include "Scoring.hpp"

int main(const int argc, char* const argv[]) {

  if (argc != 2) {
    std::cerr << "Exactly one argument required (the name of the file).\n";
    return EXIT_FAILURE;
  }
  const std::string& file_name(argv[1]);

  typedef OKlib::SATCompetition::Result_database_from_file<OKlib::SATCompetition::ParserResult, OKlib::SATCompetition::Result> result_database;
  typedef result_database::database_type database;
  typedef OKlib::SATCompetition::ElementaryAnalysis<database> indexed_database;
  typedef OKlib::SATCompetition::PurseScoring<indexed_database> purse_scoring_type;

  result_database rdb(file_name);
  indexed_database idb(rdb.db);
  purse_scoring_type purse_scoring(idb);

  
}

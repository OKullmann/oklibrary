// Oliver Kullmann, 13.5.2005 (Swansea)

#ifndef PARSINGSINGLERESULTTESTOBJECTSS_992ttgBa

#define PARSINGSINGLERESULTTESTOBJECTSS_992ttgBa

#include "ParsingSingleResult_Tests.hpp"

namespace OKlib {

  namespace SATCompetition {

    namespace Testobjects {

      Test_ParserResultElement_SuperSeries<SuperSeries> test_SuperSeries;

      Test_ParserResultElement_RandomKSat<RandomKSat> test_RandomKSat;

      Test_ParserResultElement_Series<Series> test_Series;

      Test_ParserResultElement_RandomKSat_n<RandomKSat_n> test_RandomKSat_n;

      Test_ParserResultElement_Benchmark<Benchmark> test_Benchmark;

      Test_ParserResultElement_Solver<Solver> test_Solver;

      Test_ParserResultElement_SATStatus<SATStatus> test_SATStatus;

      Test_ParserResultElement_AverageTime<AverageTime> test_AverageTime;

      Test_ParserResultElement_TimeOut<TimeOut> test_TimeOut;

      Test_ParserResult_Result<Result> test_Result;
      Test_ParserResult_ResultRandomSat<ResultRandomSat> test_ResultRandomSat;
    }

  }

}

#endif



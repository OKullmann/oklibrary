// Oliver Kullmann, 8.5.2005 (Swansea)

#ifndef SINGLERESULT_jdj277yYt

#define SINGLERESULT_jdj277yYt

#include <string>

namespace SingleResult {

  class ResultElement {
  public :
    virtual ~ResultElement() {}
  };

  typedef std::string String;

  class ResultElement_with_name : public ResultElement {
    const String name_;
  public :
    ResultElement_with_name(const String& name) : name_(name) {}
    const String& name() const { return name_; }
  };

  class SuperSeries : public ResultElement_with_name {
  public :
    SuperSeries(const String& name) : ResultElement_with_name(name) {}
  };
  class Series  : public ResultElement_with_name {
  public :
    Series(const String& name) : ResultElement_with_name(name) {}
  };
  class Benchmark  : public ResultElement_with_name {
  public :
    Benchmark(const String& name) : ResultElement_with_name(name) {}
  };
  class Solver  : public ResultElement_with_name {
  public :
    Solver(const String& name) : ResultElement_with_name(name) {}
  };

  enum SolverResult { unknown, sat, unsat };
  // ToDo: This should come from the general library

  class SATStatus : public ResultElement {
    const SolverResult result_;
  public :
    SATStatus(const SolverResult result) : result_(result) {}
    SolverResult result() const { return result_; }
  };

  typedef double FloatingPoint;

  class AverageTime : public ResultElement {
    const FloatingPoint average_;
  public :
    AverageTime(const FloatingPoint average) : average_(average) {}
    FloatingPoint average() const { return average_; }
  };

  typedef unsigned int NaturalNumber;

  class TimeOut : public ResultElement {
    const NaturalNumber time_out_;
  public :
    TimeOut(const NaturalNumber time_out) : time_out_(time_out) {}
    NaturalNumber time_out() const { return time_out_; }
  };

}

#endif

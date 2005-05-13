// Oliver Kullmann, 8.5.2005 (Swansea)

#ifndef SINGLERESULT_jdj277yYt

#define SINGLERESULT_jdj277yYt

#include <string>

namespace OKlib {

  namespace SATCompetition {

    class ResultElement {
    public :
      virtual ~ResultElement() {}
    };

    typedef std::string String;

    class ResultElement_with_name : public ResultElement {
      String name_;
    public :
      ResultElement_with_name() {}
      ResultElement_with_name(const String& name) : name_(name) {}
      const String& name() const { return name_; }
    };

    typedef unsigned int NaturalNumber;
    
    class SuperSeries : public ResultElement_with_name {
    public :
      SuperSeries() {}
      SuperSeries(const String& name) : ResultElement_with_name(name) {}
    };
    class RandomKSat : public SuperSeries {
      NaturalNumber k;
    public :
      RandomKSat() {}
      RandomKSat(const String& name, const NaturalNumber k) : SuperSeries(name), k(k) {}
      NaturalNumber clause_length() const { return k; }
    };

    class Series  : public ResultElement_with_name {
    public :
      Series(const String& name) : ResultElement_with_name(name) {}
    };
    class Series_with_n : public Series {
      NaturalNumber n;
    public :
      Series_with_n(const String& name, const NaturalNumber n) : Series(name), n(n) {}
      NaturalNumber count_variables() const { return n; }
    };
    class RandomKSat_n : public Series_with_n {
      RandomKSat_n(const String& name, const NaturalNumber n) : Series_with_n(name, n) {}
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
      SolverResult result_;
    public :
      SATStatus(const SolverResult result) : result_(result) {}
      SolverResult result() const { return result_; }
    };

    typedef double FloatingPoint;

    class AverageTime : public ResultElement {
      FloatingPoint average_;
    public :
      AverageTime(const FloatingPoint average) : average_(average) {}
      FloatingPoint average() const { return average_; }
    };

    class TimeOut : public ResultElement {
      NaturalNumber time_out_;
    public :
      TimeOut(const NaturalNumber time_out) : time_out_(time_out) {}
      NaturalNumber time_out() const { return time_out_; }
    };

  }

}

#endif

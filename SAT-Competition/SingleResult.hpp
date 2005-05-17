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
    typedef double FloatingPoint;
    typedef unsigned int NaturalNumber;

    class ResultElement_with_name : public ResultElement {
      String name_;
    public :
      ResultElement_with_name() {}
      ResultElement_with_name(const String& name) : name_(name) {}
      const String& name() const { return name_; }
    };

    // ---------------------------------------------------------------------------------------------------------------
    
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

    // ---------------------------------------------------------------------------------------------------------------

    class Series  : public ResultElement_with_name {
    public :
      Series() {}
      Series(const String& name) : ResultElement_with_name(name) {}
    };
    class Series_with_n : public Series {
      NaturalNumber n;
    public :
      Series_with_n() {}
      Series_with_n(const String& name, const NaturalNumber n) : Series(name), n(n) {}
      NaturalNumber count_variables() const { return n; }
    };
    class RandomKSat_n : public Series_with_n {
    public :
      RandomKSat_n() {}
      RandomKSat_n(const String& name, const NaturalNumber n) : Series_with_n(name, n) {}
    };

    // ---------------------------------------------------------------------------------------------------------------

    class Benchmark  : public ResultElement_with_name {
    public :
      Benchmark() {}
      Benchmark(const String& name) : ResultElement_with_name(name) {}
    };

    // ---------------------------------------------------------------------------------------------------------------

    class Solver  : public ResultElement_with_name {
    public :
      Solver() {}
      Solver(const String& name) : ResultElement_with_name(name) {}
    };

    // ---------------------------------------------------------------------------------------------------------------

    enum SolverResult { unknown, sat, unsat };
    // ToDo: This should come from the general library

    class SATStatus : public ResultElement {
      SolverResult result_;
    public :
      SATStatus() {}
      SATStatus(const SolverResult result) : result_(result) {}
      SolverResult result() const { return result_; }
    };

    // ---------------------------------------------------------------------------------------------------------------

    class AverageTime : public ResultElement {
      FloatingPoint average_;
    public :
      AverageTime() {}
      AverageTime(const FloatingPoint average) : average_(average) {}
      FloatingPoint average() const { return average_; }
    };

    // ---------------------------------------------------------------------------------------------------------------

    class TimeOut : public ResultElement {
      NaturalNumber time_out_;
    public :
      TimeOut() {}
      TimeOut(const NaturalNumber time_out) : time_out_(time_out) {}
      NaturalNumber time_out() const { return time_out_; }
    };
 
    // ---------------------------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------------------------

    class ResultBasis {
    public :
      const SuperSeries& super_series() const { return super_series_(); };
      const Series& series() const { return series_(); }
      const Benchmark& benchmark() const { return benchmark_(); }
      const Solver& solver() const { return solver_(); }
      const SATStatus& sat_status() const { return sat_status_(); }
      const AverageTime& average() const { return average_(); }
      const TimeOut& time_out() const { return time_out_(); }
      ~ResultBasis() {}
    private :
      virtual const SuperSeries& super_series_() const = 0;
      virtual const Series& series_() const = 0;
      virtual const Benchmark& benchmark_() const = 0;
      virtual const Solver& solver_() const = 0;
      virtual const SATStatus& sat_status_() const = 0;
      virtual const AverageTime& average_() const = 0;
      virtual const TimeOut& time_out_() const = 0;
    };
    template <class> class ParserResult;
    class Result : public ResultBasis {
      friend class ParserResult<Result>;
      SuperSeries* sup_ser;
      Series* ser;
      Benchmark* bench;
      Solver* solv;
      SATStatus* sat_stat;
      AverageTime* avg;
      TimeOut* tmo;
      const SuperSeries& super_series_() const { return *sup_ser; };
      const Series& series_() const { return *ser; }
      const Benchmark& benchmark_() const { return *bench; }
      const Solver& solver_() const { return *solv; }
      const SATStatus& stat_status_() const { return *sat_stat; }
      const AverageTime& average_() const { return *avg; }
      const TimeOut& time_out_() const { return *tmo; }
    public :
      Result() : sup_ser(0), ser(0), bench(0), solv(0), sat_stat(0), avg(0), tmo(0) {}
    };

    class ResultRandomSatBasis : ResultBasis {
    public :
    private :
      virtual const RandomKSat& super_series_random_() const = 0;
      const SuperSeries& super_series_() const { return super_series_random_(); }
      virtual const RandomKSat_n& series_random_() const = 0;
      virtual const Series& series_() const { return series_random_(); }
    };
  }

}

#endif

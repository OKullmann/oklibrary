// Oliver Kullmann, 8.5.2005 (Swansea)

#ifndef SINGLERESULT_jdj277yYt

#define SINGLERESULT_jdj277yYt

#include <string>
#include <ostream>

#include <boost/tuple/tuple.hpp>

namespace OKlib {

  namespace SATCompetition {

    class ResultElement {
    public :
      virtual ~ResultElement() {}
    };

    typedef std::string String;
    typedef double FloatingPoint;
    typedef unsigned int NaturalNumber;

    // ---------------------------------------------------------------------------------------------------------------
    
    class ResultElement_with_name : public ResultElement {
      String name_;
    public :
      ResultElement_with_name() {}
      ResultElement_with_name(const String& name) : name_(name) {}
      const String& name() const { return name_; }
    };

    std::ostream& operator <<(std::ostream& out, const ResultElement_with_name& e) {
      return out << e.name();
    }

    bool operator ==(const ResultElement_with_name& lhs, const ResultElement_with_name& rhs) {
      return lhs.name() == rhs.name();
    }
    bool operator !=(const ResultElement_with_name& lhs, const ResultElement_with_name& rhs) {
      return not (lhs == rhs);
    }

    bool operator <(const ResultElement_with_name& lhs, const ResultElement_with_name& rhs) {
      return lhs.name() < rhs.name();
    }
    bool operator >(const ResultElement_with_name& lhs, const ResultElement_with_name& rhs) {
      return rhs < lhs;
    }
    bool operator <=(const ResultElement_with_name& lhs, const ResultElement_with_name& rhs) {
      return not(lhs > rhs);
    }
    bool operator >=(const ResultElement_with_name& lhs, const ResultElement_with_name& rhs) {
      return not(lhs < rhs);
    }

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

    enum SolverResult { unknown = 0, sat = 10, unsat = 20, error = 1 };
    // ToDo: This should come from the general library ?!

    class SATStatus : public ResultElement {
      SolverResult result_;
    public :
      SATStatus() {}
      SATStatus(const SolverResult result) : result_(result) {}
      SolverResult result() const { return result_; }
    };

    std::ostream& operator <<(std::ostream& out, const SATStatus& e) {
      return out << e.result();
    }

    bool operator ==(const SATStatus& lhs, const SATStatus& rhs) {
      return lhs.result() == rhs.result();
    }
    bool operator !=(const SATStatus& lhs, const SATStatus& rhs) {
      return not (lhs == rhs);
    }

    bool operator <(const SATStatus& lhs, const SATStatus& rhs) {
      return lhs.result() < rhs.result();
    }
    bool operator >(const SATStatus& lhs, const SATStatus& rhs) {
      return rhs < lhs;
    }
    bool operator <=(const SATStatus& lhs, const SATStatus& rhs) {
      return not(lhs > rhs);
    }
    bool operator >=(const SATStatus& lhs, const SATStatus& rhs) {
      return not(lhs < rhs);
    }


    // ---------------------------------------------------------------------------------------------------------------

    class AverageTime : public ResultElement {
      FloatingPoint average_;
    public :
      AverageTime() {}
      AverageTime(const FloatingPoint average) : average_(average) {}
      FloatingPoint average() const { return average_; }
    };

    std::ostream& operator <<(std::ostream& out, const AverageTime& e) {
      return out << e.average();
    }

    bool operator ==(const AverageTime& lhs, const AverageTime& rhs) {
      return lhs.average() == rhs.average();
    }
    bool operator !=(const AverageTime& lhs, const AverageTime& rhs) {
      return not (lhs == rhs);
    }
    // ---------------------------------------------------------------------------------------------------------------

    class TimeOut : public ResultElement {
      NaturalNumber time_out_;
    public :
      TimeOut() {}
      TimeOut(const NaturalNumber time_out) : time_out_(time_out) {}
      NaturalNumber time_out() const { return time_out_; }
    };

    std::ostream& operator <<(std::ostream& out, const TimeOut& e) {
      return out << e.time_out();
    }
 
    bool operator ==(const TimeOut& lhs, const TimeOut& rhs) {
      return lhs.time_out() == rhs.time_out();
    }
    bool operator !=(const TimeOut& lhs, const TimeOut& rhs) {
      return not (lhs == rhs);
    }
    
    bool operator <(const TimeOut& lhs, const TimeOut& rhs) {
      return lhs.time_out() < rhs.time_out();
    }
    bool operator >(const TimeOut& lhs, const TimeOut& rhs) {
      return rhs < lhs;
    }
    bool operator <=(const TimeOut& lhs, const TimeOut& rhs) {
      return not(lhs > rhs);
    }
    bool operator >=(const TimeOut& lhs, const TimeOut& rhs) {
      return not(lhs < rhs);
    }

    // ---------------------------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------------------------
    
    typedef boost::tuple<SuperSeries, Series, Benchmark, Solver, SATStatus, AverageTime, TimeOut> TupleResult;

    inline std::ostream& operator <<(std::ostream& out, const TupleResult& t) {
      return out << t.get<0>() << " " << t.get<1>() << " " << t.get<2>() << " " << t.get<3>() << " " << t.get<4>() << " " << t.get<5>() << " " << t.get<6>();
    }

    typedef boost::tuple<RandomKSat, RandomKSat_n, Benchmark, Solver, SATStatus, AverageTime, TimeOut> TupleResultRandomSat;

    inline std::ostream& operator <<(std::ostream& out, const TupleResultRandomSat& t) {
      return out << t.get<0>() << " " << t.get<1>() << " " << t.get<2>() << " " << t.get<3>() << " " << t.get<4>() << " " << t.get<5>() << " " << t.get<6>();
    }

    template <class result_type>
    struct tuple_type;

    // ---------------------------------------------------------------------------------------------------------------

    class ResultBasis {
      // ToDo: This should be an instance of a general pattern.
      // Instead of all the name duplications, the elements should be
      // accessible via the type name, e.g., element<SuperSeries>().
      // Template metaprogramming should yield a generic result container, for a given typelist.
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

    std::ostream& operator <<(std::ostream& out, const ResultBasis& r) {
      return out << r.super_series() << " " <<r.series() << " " <<r.benchmark() << " " << r.solver() << " " << r.sat_status() << " " << r.average() << " " <<r.time_out();
    }

    // ---------------------------------------------------------------------------------------------------------------

    template <class Result, typename CharT = char, typename ParseIterator = const CharT*> class ParserResult;

    class Result : public ResultBasis {
      template <class, typename, typename>
      friend class ParserResult; // ToDo: Only when the first template parameter is this class Result, we should have a friend --- but this seems not to be possible?
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
      const SATStatus& sat_status_() const { return *sat_stat; }
      const AverageTime& average_() const { return *avg; }
      const TimeOut& time_out_() const { return *tmo; }
    public :
      Result() : sup_ser(new SuperSeries), ser(new Series), bench(new Benchmark), solv(new Solver), sat_stat(new SATStatus), avg(new AverageTime), tmo(new TimeOut) {}
      Result(const TupleResult& r) : sup_ser(new SuperSeries(r.get<0>())), ser(new Series(r.get<1>())), bench(new Benchmark(r.get<2>())), solv(new Solver(r.get<3>())), sat_stat(new SATStatus(r.get<4>())), avg(new AverageTime(r.get<5>())), tmo(new TimeOut(r.get<6>())) {}
      Result(const Result& r) : sup_ser(new SuperSeries(*r.sup_ser)), ser(new Series(*r.ser)), bench(new Benchmark(*r.bench)), solv(new Solver(*r.solv)), sat_stat(new SATStatus(*r.sat_stat)), avg(new AverageTime(*r.avg)), tmo(new TimeOut(*r.tmo)) {}
      // ToDo: Also copy assignment with deep copying!!
      ~Result() {
        delete sup_ser; delete ser; delete bench; delete solv; delete sat_stat; delete avg; delete tmo;
      }
    };

    bool operator ==(const TupleResult& lhs, const Result& rhs) {
      return lhs.get<0>() == rhs.super_series() and lhs.get<1>() == rhs.series() and lhs.get<2>() == rhs.benchmark() and lhs.get<3>() == rhs.solver() and lhs.get<4>() == rhs.sat_status() and lhs.get<5>() == rhs.average() and lhs.get<6>() == rhs.time_out();
    }
    bool operator ==(const Result& lhs, const TupleResult& rhs) {
      return rhs == lhs;
    }
    bool operator !=(const TupleResult& lhs, const Result& rhs) {
      return not (lhs == rhs);
    }
    bool operator !=(const Result& lhs, const TupleResult& rhs) {
      return not (lhs == rhs);
    }

    template <>
    struct tuple_type<Result> {
      typedef TupleResult type;
    };

    // ---------------------------------------------------------------------------------------------------------------

    class ResultRandomSatBasis : public ResultBasis {
    public :
      const RandomKSat& super_series_random() const { return super_series_random_(); }
      const RandomKSat_n& series_random() const { return series_random_(); }
    private :
      virtual const RandomKSat& super_series_random_() const = 0;
      const SuperSeries& super_series_() const { return super_series_random_(); }
      virtual const RandomKSat_n& series_random_() const = 0;
      virtual const Series& series_() const { return series_random_(); }
    };

    class ResultRandomSat : public ResultRandomSatBasis {
      template <class, typename, typename>
      friend class ParserResult; // ToDo: Only when the first template parameter is this class Result, we should have a friend --- but this seems not to be possible?
      RandomKSat* sup_ser;
      RandomKSat_n* ser;
      Benchmark* bench;
      Solver* solv;
      SATStatus* sat_stat;
      AverageTime* avg;
      TimeOut* tmo;
      const RandomKSat& super_series_random_() const { return *sup_ser; };
      const RandomKSat_n& series_random_() const { return *ser; }
      const Benchmark& benchmark_() const { return *bench; }
      const Solver& solver_() const { return *solv; }
      const SATStatus& sat_status_() const { return *sat_stat; }
      const AverageTime& average_() const { return *avg; }
      const TimeOut& time_out_() const { return *tmo; }
    public :
      ResultRandomSat() : sup_ser(new RandomKSat), ser(new RandomKSat_n), bench(new Benchmark), solv(new Solver), sat_stat(new SATStatus), avg(new AverageTime), tmo(new TimeOut) {}
      ResultRandomSat(const ResultRandomSat& r) : sup_ser(new RandomKSat(*r.sup_ser)), ser(new RandomKSat_n(*r.ser)), bench(new Benchmark(*r.bench)), solv(new Solver(*r.solv)), sat_stat(new SATStatus(*r.sat_stat)), avg(new AverageTime(*r.avg)), tmo(new TimeOut(*r.tmo)) {}
      ~ResultRandomSat() {
	delete sup_ser; delete ser; delete bench; delete solv; delete sat_stat; delete avg; delete tmo;
      }
      // ToDo: Also copy assignment with deep copying!!
    };

    bool operator ==(const TupleResultRandomSat& lhs, const ResultRandomSat& rhs) {
      return lhs.get<0>() == rhs.super_series() and lhs.get<1>() == rhs.series() and lhs.get<2>() == rhs.benchmark() and lhs.get<3>() == rhs.solver() and lhs.get<4>() == rhs.sat_status() and lhs.get<5>() == rhs.average() and lhs.get<6>() == rhs.time_out();
    }
    bool operator ==(const ResultRandomSat& lhs, const TupleResultRandomSat& rhs) {
      return rhs == lhs;
    }
    bool operator !=(const TupleResultRandomSat& lhs, const ResultRandomSat& rhs) {
      return not (lhs == rhs);
    }
    bool operator !=(const ResultRandomSat& lhs, const TupleResultRandomSat& rhs) {
      return not (lhs == rhs);
    }

    template <>
    struct tuple_type<ResultRandomSat> {
      typedef TupleResultRandomSat type;
    };

  }

}

#endif

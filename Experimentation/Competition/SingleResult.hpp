// Oliver Kullmann, 8.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Competition/SingleResult.hpp
  \brief Classes for representing results of one solver on one problem instance

  A polymorphic class hierarchy with ResultElement at the base, with derived class
  ResultElement_with_name, from which SuperSeries, Series, Benchmark and
  Solver are derived, while SATStatus, AverageTime and TimeOut are directly
  derived from ResultElement.

  Combining all these classes we have a polymorphic hierarchy with ResultBasis
  as the abstract base class, and derived from it Result and ResultRandomSatBasis,
  where the latter is again an abstract base class, from which ResultRandomSat is
  derived.

  \todo Writing doxygen comments for all classes in here.
*/

#ifndef SINGLERESULT_jdj277yYt
#define SINGLERESULT_jdj277yYt

#include <string>
#include <ostream>
#include <cassert>
#include <algorithm>

#include <boost/tuple/tuple.hpp>

#include <OKlib/Concepts/ResultElements.hpp>

#include <OKlib/Programming/Utilities/OrderRelations/DerivedRelations.hpp>

namespace OKlib {
  namespace SATCompetition {

    /*!
      \class ResultElement
      \brief Polymorphic base class for the representation of various forms of "results".

      A model of concept Concepts::ResultElement.
      Contains only nested types for the representation of strings, floating point numbers
      and natural numbers.
    */

    class ResultElement {
    public :
      typedef OKlib::Concepts::ResultElement_tag concept_tag;
      typedef std::string string_type;
      typedef double floating_point_type;
      typedef unsigned int natural_number_type;
      virtual const ResultElement* self() const { return this; }
      virtual ~ResultElement() {}
    };

    // #################

    /*!
      \class ResultElement_with_name
      \brief Addition of a name to SATCompetition::ResultElement, by which result elements
      can be alphabetically sorted.

      A model of concept Concepts::ResultElementWithName (thus the requirement of the
      linear order is fulfilled by using the lexicographical order on the names).
    */
    
    class ResultElement_with_name : public ResultElement {
      string_type name_;
    public :
      typedef OKlib::Concepts::ResultElementWithName_tag concept_tag;
      ResultElement_with_name() {}
      ResultElement_with_name(const string_type& name) : name_(name) {}
      string_type name() const { return name_; }
      const ResultElement_with_name* self() const { return this; }
    };

    std::ostream& operator <<(std::ostream& out, const ResultElement_with_name& e) {
      return out << e.name();
    }

    bool operator ==(const ResultElement_with_name& lhs, const ResultElement_with_name& rhs) {
      return lhs.name() == rhs.name();
    }
    OKLIB_DERIVED_UNEQUAL(ResultElement_with_name)

    bool operator <(const ResultElement_with_name& lhs, const ResultElement_with_name& rhs) {
      return lhs.name() < rhs.name();
    }
    OKLIB_DERIVED_ORDERRELATIONS(ResultElement_with_name)

    // #################
    
    class SuperSeries : public ResultElement_with_name {
    public :
      SuperSeries() {}
      SuperSeries(const string_type& name) : ResultElement_with_name(name) {}
      const SuperSeries* self() const { return this; }
    };
    class RandomKSat : public SuperSeries {
      natural_number_type k;
    public :
      RandomKSat() {}
      RandomKSat(const string_type& name, const natural_number_type k) : SuperSeries(name), k(k) {}
      natural_number_type clause_length() const { return k; }
      const RandomKSat* self() const { return this; }
    };

    // #################

    class Series  : public ResultElement_with_name {
    public :
      Series() {}
      Series(const string_type& name) : ResultElement_with_name(name) {}
      const Series* self() const { return this; }
    };
    class Series_with_n : public Series {
      natural_number_type n;
    public :
      Series_with_n() {}
      Series_with_n(const string_type& name, const natural_number_type n) : Series(name), n(n) {}
      natural_number_type count_variables() const { return n; }
      const Series_with_n* self() const { return this; }
    };
    class RandomKSat_n : public Series_with_n {
    public :
      RandomKSat_n() {}
      RandomKSat_n(const string_type& name, const natural_number_type n) : Series_with_n(name, n) {}
      const RandomKSat_n* self() const { return this; }
    };

    // #################

    typedef std::pair<SuperSeries, Series> SpecSeries;

    std::ostream& operator <<(std::ostream& out, const SpecSeries& series) {
      return out << series.first << "::" << series.second;
    }

    typedef std::pair<RandomKSat, RandomKSat_n> SpecRandomKSat;

    // #################

    class Benchmark  : public ResultElement_with_name {
    public :
      Benchmark() {}
      Benchmark(const string_type& name) : ResultElement_with_name(name) {}
      const Benchmark* self() const { return this; }
    };

    // #################

    class Solver  : public ResultElement_with_name {
    public :
      Solver() {}
      Solver(const string_type& name) : ResultElement_with_name(name) {}
      const Solver* self() const { return this; }
    };

    // #################

    /*!
      \enum SolverResult
      \brief Codes for return values of SAT solvers
      \todo This should come from the general library ?!
    */

    enum SolverResult { unknown = 0, sat = 10, unsat = 20, error = 1 };

    /*!
      \class SATStatus
      \brief Special result element representing the return value of the solver
    */

    class SATStatus : public ResultElement {
      SolverResult result_;
    public :
      SATStatus() {}
      SATStatus(const SolverResult result) : result_(result) {}
      SolverResult result() const { return result_; }
      const SATStatus* self() const { return this; }
    };

    std::ostream& operator <<(std::ostream& out, const SATStatus& e) {
      return out << e.result();
    }

    bool operator ==(const SATStatus& lhs, const SATStatus& rhs) {
      return lhs.result() == rhs.result();
    }
    OKLIB_DERIVED_UNEQUAL(SATStatus)

    bool operator <(const SATStatus& lhs, const SATStatus& rhs) {
      return lhs.result() < rhs.result();
    }
    OKLIB_DERIVED_ORDERRELATIONS(SATStatus)

    // #################

    class AverageTime : public ResultElement {
      floating_point_type average_;
    public :
      AverageTime() {}
      AverageTime(const floating_point_type average) : average_(average) {}
      floating_point_type average() const { return average_; }
      const AverageTime* self() const { return this; }
    };

    std::ostream& operator <<(std::ostream& out, const AverageTime& e) {
      return out << e.average();
    }

    bool operator ==(const AverageTime& lhs, const AverageTime& rhs) {
      return lhs.average() == rhs.average();
    }
    OKLIB_DERIVED_UNEQUAL(AverageTime)

    // #################

    class TimeOut : public ResultElement {
      natural_number_type time_out_;
    public :
      TimeOut() {}
      TimeOut(const natural_number_type time_out) : time_out_(time_out) {}
      natural_number_type time_out() const { return time_out_; }
      const TimeOut* self() const { return this; }
    };

    std::ostream& operator <<(std::ostream& out, const TimeOut& e) {
      return out << e.time_out();
    }
 
    bool operator ==(const TimeOut& lhs, const TimeOut& rhs) {
      return lhs.time_out() == rhs.time_out();
    }
    OKLIB_DERIVED_UNEQUAL(TimeOut)

    bool operator <(const TimeOut& lhs, const TimeOut& rhs) {
      return lhs.time_out() < rhs.time_out();
    }
    OKLIB_DERIVED_ORDERRELATIONS(TimeOut)

    // ######################################################
    // ######################################################

    // ToDo: This should go into a separate file
    
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

    // #################

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
      virtual ~ResultBasis() {}
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
    std::ostream& operator <<(std::ostream& out, const ResultBasis* const r) {
      return out << r -> super_series() << " " <<r -> series() << " " <<r -> benchmark() << " " << r -> solver() << " " << r -> sat_status() << " " << r -> average() << " " <<r -> time_out();
    }

    // #################

    template <typename CharT, typename ParseIterator>
    struct ParserEmpty;
    template <class Result, typename CharT = char, typename ParseIterator = const CharT*, class ParserExtension = ParserEmpty<CharT, ParseIterator> > class ParserResult;

    class Result : public ResultBasis {
      template <class, typename, typename, class>
      friend class ParserResult; // ToDo: Only when the first template parameter is this class Result, we should have a friend --- but this seems not to be possible?

      SuperSeries* sup_ser;
      Series* ser;
      Benchmark* bench;
      Solver* solv;
      SATStatus* sat_stat;
      AverageTime* avg;
      TimeOut* tmo;

      const SuperSeries& super_series_() const {
        assert(sup_ser);
        return *sup_ser;
      };
      const Series& series_() const {
        assert(ser);
        return *ser;
      }
      const Benchmark& benchmark_() const {
        assert(bench);
        return *bench;
      }
      const Solver& solver_() const {
        assert(solv);
        return *solv;
      }
      const SATStatus& sat_status_() const {
        assert(sat_stat);
        return *sat_stat;
      }
      const AverageTime& average_() const {
        assert(avg);
        return *avg;
      }
      const TimeOut& time_out_() const {
        assert(tmo);
        return *tmo;
      }
    public :

      Result() : sup_ser(new SuperSeries), ser(new Series), bench(new Benchmark), solv(new Solver), sat_stat(new SATStatus), avg(new AverageTime), tmo(new TimeOut) {}

      Result(const TupleResult& r) : sup_ser(new SuperSeries(r.get<0>())), ser(new Series(r.get<1>())), bench(new Benchmark(r.get<2>())), solv(new Solver(r.get<3>())), sat_stat(new SATStatus(r.get<4>())), avg(new AverageTime(r.get<5>())), tmo(new TimeOut(r.get<6>())) {}

      Result(const Result& r) : sup_ser(new SuperSeries(*r.sup_ser)), ser(new Series(*r.ser)), bench(new Benchmark(*r.bench)), solv(new Solver(*r.solv)), sat_stat(new SATStatus(*r.sat_stat)), avg(new AverageTime(*r.avg)), tmo(new TimeOut(*r.tmo)) {}

      Result& operator =(const Result& rhs) {
        Result new_r(rhs);
        std::swap(new_r.sup_ser, sup_ser);
        std::swap(new_r.ser, ser);
        std::swap(new_r.bench, bench);
        std::swap(new_r.solv, solv);
        std::swap(new_r.sat_stat, sat_stat);
        std::swap(new_r.avg, avg);
        std::swap(new_r.tmo, tmo);
        return *this;
      }

      ~Result() {
        assert(sup_ser); delete sup_ser; 
        assert(ser); delete ser;
        assert(bench); delete bench;
        assert(solv); delete solv;
        assert(sat_stat); delete sat_stat;
        assert(avg); delete avg;
        assert(tmo); delete tmo;
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

    // #################

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
      template <class, typename, typename, class>
      friend class ParserResult; // ToDo: Only when the first template parameter is this class Result, we should have a friend --- but this seems not to be possible?

      RandomKSat* sup_ser;
      RandomKSat_n* ser;
      Benchmark* bench;
      Solver* solv;
      SATStatus* sat_stat;
      AverageTime* avg;
      TimeOut* tmo;

      const RandomKSat& super_series_random_() const {
        assert(sup_ser);
        return *sup_ser;
      };
      const RandomKSat_n& series_random_() const {
        assert(ser);
        return *ser;
      }
      const Benchmark& benchmark_() const {
        assert(bench);
        return *bench;
      }
      const Solver& solver_() const {
        assert(solv);
        return *solv;
      }
      const SATStatus& sat_status_() const { assert(sat_stat);
        return *sat_stat;
      }
      const AverageTime& average_() const {
        assert(avg);
        return *avg;
      }
      const TimeOut& time_out_() const {
        assert(tmo);
        return *tmo;
      }
    public :

      ResultRandomSat() : sup_ser(new RandomKSat), ser(new RandomKSat_n), bench(new Benchmark), solv(new Solver), sat_stat(new SATStatus), avg(new AverageTime), tmo(new TimeOut) {}

      ResultRandomSat(const ResultRandomSat& r) : sup_ser(new RandomKSat(*r.sup_ser)), ser(new RandomKSat_n(*r.ser)), bench(new Benchmark(*r.bench)), solv(new Solver(*r.solv)), sat_stat(new SATStatus(*r.sat_stat)), avg(new AverageTime(*r.avg)), tmo(new TimeOut(*r.tmo)) {}

      ResultRandomSat& operator =(const ResultRandomSat& rhs) {
        ResultRandomSat new_r(rhs);
        std::swap(new_r.sup_ser, sup_ser);
        std::swap(new_r.ser, ser);
        std::swap(new_r.bench, bench);
        std::swap(new_r.solv, solv);
        std::swap(new_r.sat_stat, sat_stat);
        std::swap(new_r.avg, avg);
        std::swap(new_r.tmo, tmo);
        return *this;
      }

      ~ResultRandomSat() {
        assert(sup_ser); delete sup_ser; 
        assert(ser); delete ser;
        assert(bench); delete bench;
        assert(solv); delete solv;
        assert(sat_stat); delete sat_stat;
        assert(avg); delete avg;
        assert(tmo); delete tmo;
      }
      
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

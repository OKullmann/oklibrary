// Oliver Kullmann, 18.5.2005 (Swansea)

#ifndef RESULTPROCESSINGTESTS_905Tgb

#define RESULTPROCESSINGTESTS_905Tgb

#include <vector>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <string>
#include <set>
#include <iterator>

#include <boost/lexical_cast.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include "FunctionHandling.hpp"

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

#include "SingleResult.hpp"
#include "ParsingSingleResult_Tests.hpp"

namespace OKlib {

  namespace SATCompetition {

    template <class Map, typename Size>
    class Check_sizes : public ::OKlib::TestSystem::TestBase {
      const Map& m;
      const Size s;
    public :
      typedef Check_sizes test_type;
      Check_sizes(const Map& m, const Size s) : m(m), s(s) {}
      Check_sizes(const Check_sizes& cs) : m(cs.m), s(cs.s) {}
    private :
      void perform_test_trivial() {
        Size total = 0;
        for (typename Map::const_iterator i = m.begin(); i != m.end(); ++i)
          total += i -> second -> size();
        if (total != s)
          OKLIB_THROW("Total count is " + boost::lexical_cast<std::string>(total) + ", and not " + boost::lexical_cast<std::string>(s));
      }
    };

    template <class Map, typename Size>
    inline Check_sizes<Map, Size> check_sizes(const Map& m, const Size s) {
      return Check_sizes<Map, Size>(m, s);
    }

    // -------------------------------------------------------------------------------------------------------------------------------

    template <class Map, class Set>
    class Check_set : public ::OKlib::TestSystem::TestBase {
      // ToDo: Make a general test tool out of it (with concepts etc.)
      const Map& m;
      const Set& s;
    public :
      typedef Check_set test_type;
      Check_set(const Map& m, const Set& s) : m(m), s(s) {}
      Check_set(const Check_set& cs) : m(cs.m), s(cs.s) {}
    private :
      void perform_test_trivial() {
        typedef typename Map::value_type Pair;
        typedef typename Map::const_iterator map_iterator;
        typedef FunctionHandling::First<Pair> First;
        typedef boost::transform_iterator<First, map_iterator> iterator;
        typedef typename Map::value_type::first_type value_type_map;
        typedef typename Set::value_type value_type_set;
        
        if (m.size() != s.size())
          OKLIB_THROW("Size is " + boost::lexical_cast<std::string>(m.size()) + ", and not " + boost::lexical_cast<std::string>(s.size()));
        if (not std::equal(s.begin(), s.end(), iterator(m.begin(), First()))) {
          std::stringstream message;
          message << "Domain of map is ";
          std::copy(iterator(m.begin(), First()), iterator(m.end(), First()), std::ostream_iterator<value_type_map>(message, ","));
          message << ", and not ";
          std::copy(s.begin(), s.end(), std::ostream_iterator<value_type_set>(message, ","));
          OKLIB_THROW(message.str());
        }
      }
    };

    template <class Map, class Set>
    inline Check_set<Map, Set> check_set(const Map& m, const Set& s) {
      return Check_set<Map, Set>(m, s);
    }

    // -------------------------------------------------------------------------------------------------------------------------------

    template <template <typename ResultIterator> class ResultDatabase>
    class Test_ResultDatabase_ResultIterator : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ResultDatabase_ResultIterator test_type;
      Test_ResultDatabase_ResultIterator() {
        insert(this);
      }

    private :
      
      void perform_test_trivial() {
        typedef ParserResult<Result> Parser;
        typedef std::vector<TupleResult> VectorTupleResult;
        VectorTupleResult pre_test_vector;
        add_positive_results(pre_test_vector);
        const VectorTupleResult::size_type pre_test_vector_size = pre_test_vector.size();

        std::stringstream input;
        std::copy(pre_test_vector.begin(), pre_test_vector.end(), std::ostream_iterator<TupleResult>(input, "\n"));

        typedef std::vector<Result> VectorResult;
        VectorResult test_vector;
        assert(copy_results<Parser>(input.str().c_str(), std::back_inserter(test_vector)).full);
        const VectorResult::size_type test_vector_size = test_vector.size();
        assert(test_vector_size == pre_test_vector_size);

        std::set<SuperSeries> ref_set_super_series;
        std::set<Series> ref_set_series;
        std::set<Benchmark> ref_set_benchmark;
        std::set<Solver> ref_set_solver;
        std::set<SATStatus> ref_set_sat_status;
        std::set<TimeOut> ref_set_time_out;

        for (VectorResult::const_iterator i = test_vector.begin(); i != test_vector.end(); ++i) {
          ref_set_super_series.insert(i -> super_series());
          ref_set_series.insert(i -> series());
          ref_set_benchmark.insert(i -> benchmark());
          ref_set_solver.insert(i -> solver());
          ref_set_sat_status.insert(i -> sat_status());
          ref_set_time_out.insert(i -> time_out());
        }

        typedef ResultDatabase<VectorResult::const_iterator> ResultDatabase_vector;
        ResultDatabase_vector db(test_vector.begin(), test_vector.end());

        if (not db.number_results() == test_vector_size)
          OKLIB_THROW("Element count is " + boost::lexical_cast<std::string>(db.number_results()) + ", and not " + boost::lexical_cast<std::string>(test_vector_size));

        const MapSuperSeries& super_series(db.super_series());
        const MapSeries& series(db.series());
        const MapBenchmark& benchmark(db.benchmark());
        const MapSolver& solver(db.solver());
        const MapSATStatus& sat_status(db.sat_status());
        const MapTimeOut& time_out(db.time_out());

        OKLIB_TESTTRIVIAL_RETHROW((check_set(super_series, ref_set_super_series)));
        OKLIB_TESTTRIVIAL_RETHROW((check_set(series, ref_set_series)));
        OKLIB_TESTTRIVIAL_RETHROW((check_set(benchmark, ref_set_benchmark)));
        OKLIB_TESTTRIVIAL_RETHROW((check_set(solver, ref_set_solver)));
        OKLIB_TESTTRIVIAL_RETHROW((check_set(sat_status, ref_set_sat_status)));
        OKLIB_TESTTRIVIAL_RETHROW((check_set(time_out, ref_set_time_out)));

        OKLIB_TESTTRIVIAL_RETHROW((check_sizes(super_series, test_vector_size)));
        OKLIB_TESTTRIVIAL_RETHROW((check_sizes(series, test_vector_size)));
        OKLIB_TESTTRIVIAL_RETHROW((check_sizes(benchmark, test_vector_size)));
        OKLIB_TESTTRIVIAL_RETHROW((check_sizes(solver, test_vector_size)));
        OKLIB_TESTTRIVIAL_RETHROW((check_sizes(sat_status, test_vector_size)));
        OKLIB_TESTTRIVIAL_RETHROW((check_sizes(time_out, test_vector_size)));
      }

    };

    
  }

}

#endif

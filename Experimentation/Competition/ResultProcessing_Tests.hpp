// Oliver Kullmann, 18.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef RESULTPROCESSINGTESTS_905Tgb
#define RESULTPROCESSINGTESTS_905Tgb

#include <vector>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <string>
#include <set>
#include <iterator>
#include <utility>

#include <boost/lexical_cast.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include <OKlib/General/FunctionHandling.hpp>
#include <OKlib/Structures/Sets/SetAlgorithms/BasicMapOperations.hpp>

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

#include <OKlib/Experimentation/Competition/SingleResult.hpp>
#include <OKlib/Experimentation/Competition/ParsingSingleResult_Tests.hpp>
#include <OKlib/Experimentation/Competition/ParsingResultSequences.hpp>
#include <OKlib/Experimentation/Competition/ParsingResultSequences_Tests.hpp>

namespace OKlib {
  namespace SATCompetition {

    template <class Map, typename Size>
    class Check_sizes : public ::OKlib::TestSystem::Test {
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
    class Check_set : public ::OKlib::TestSystem::Test {
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

    template <class ResultDatabase, class ResultContainer>
    class Test_Database_vs_Container : public ::OKlib::TestSystem::Test {
      ResultDatabase& rdb;
      ResultContainer& rc;
    public :
      typedef Test_Database_vs_Container test_type;
      Test_Database_vs_Container(ResultDatabase& rdb, ResultContainer& rc) : rdb(rdb), rc(rc) {}
      typedef typename ResultContainer::value_type result_type;
    private :
      typedef typename ResultContainer::const_iterator const_iterator;
      typedef typename ResultContainer::size_type size_type;

      void perform_test_trivial() {

        const size_type rc_size = rc.size();
        if (rdb.number_results() != rc_size)
          OKLIB_THROW("Element count is " + boost::lexical_cast<std::string>(rdb.number_results()) + ", and not " + boost::lexical_cast<std::string>(rc_size));

        std::set<SuperSeries> ref_set_super_series;
        std::set<Series> ref_set_series;
        std::set<Benchmark> ref_set_benchmark;
        std::set<Solver> ref_set_solver;
        std::set<SATStatus> ref_set_sat_status;
        std::set<TimeOut> ref_set_time_out;

        for (const_iterator i = rc.begin(); i != rc.end(); ++i) {
            ref_set_super_series.insert(i -> super_series());
            ref_set_series.insert(i -> series());
            ref_set_benchmark.insert(i -> benchmark());
            ref_set_solver.insert(i -> solver());
            ref_set_sat_status.insert(i -> sat_status());
            ref_set_time_out.insert(i -> time_out());
          }

        const MapSuperSeries& super_series(rdb.super_series());
        const MapSeries& series(rdb.series());
        const MapBenchmark& benchmark(rdb.benchmark());
        const MapSolver& solver(rdb.solver());
        const MapSATStatus& sat_status(rdb.sat_status());
        const MapTimeOut& time_out(rdb.time_out());

        OKLIB_TESTTRIVIAL_RETHROW((check_set(super_series, ref_set_super_series)));
        OKLIB_TESTTRIVIAL_RETHROW((check_set(series, ref_set_series)));
        OKLIB_TESTTRIVIAL_RETHROW((check_set(benchmark, ref_set_benchmark)));
        OKLIB_TESTTRIVIAL_RETHROW((check_set(solver, ref_set_solver)));
        OKLIB_TESTTRIVIAL_RETHROW((check_set(sat_status, ref_set_sat_status)));
        OKLIB_TESTTRIVIAL_RETHROW((check_set(time_out, ref_set_time_out)));

        OKLIB_TESTTRIVIAL_RETHROW((check_sizes(super_series, rc_size)));
        OKLIB_TESTTRIVIAL_RETHROW((check_sizes(series, rc_size)));
        OKLIB_TESTTRIVIAL_RETHROW((check_sizes(benchmark, rc_size)));
        OKLIB_TESTTRIVIAL_RETHROW((check_sizes(solver, rc_size)));
        OKLIB_TESTTRIVIAL_RETHROW((check_sizes(sat_status, rc_size)));
        OKLIB_TESTTRIVIAL_RETHROW((check_sizes(time_out, rc_size)));

        typedef VectorResultNodesP intersection_type;
        const ResultNode* result_p = 0;
        {
          const intersection_type& res(rdb.intersection());
          if (res.size() != rc_size)
            OKLIB_THROW("Result size is " + boost::lexical_cast<std::string>(res.size()) + ", and not " + boost::lexical_cast<std::string>(rc_size));
          if (not res.empty())
            result_p = *res.begin();
        }
        if (rc_size == 0) return;
        assert(result_p);

        const SuperSeries first_super_series = result_p -> rb -> super_series();
        const Series first_series = result_p -> rb -> series();
        const Benchmark first_benchmark = result_p -> rb -> benchmark();

        typedef typename ResultDatabase::VectorOfSetsP Sets;
        Sets& query(rdb.vector_of_sets);
        if (not query.empty())
          OKLIB_THROW("Query vector is not empty, but has size " + boost::lexical_cast<std::string>(query.size()));

        const SetResultNodesP* const res_super_series(OKlib::SetAlgorithms::map_value(rdb.super_series(), first_super_series));
        const SetResultNodesP::iterator res_super_series_end = res_super_series -> end();
        if (res_super_series -> find(result_p) == res_super_series_end)
          OKLIB_THROW("Resulting set of super series' does not contain " + boost::lexical_cast<std::string>(*result_p));
        query.push_back(res_super_series);
        if (query.size() != 1)
          OKLIB_THROW("Query vector has size " + boost::lexical_cast<std::string>(query.size()));

        const SetResultNodesP* const res_series(OKlib::SetAlgorithms::map_value(rdb.series(), first_series));
        const SetResultNodesP::iterator res_series_end = res_series -> end();
        if (res_series -> find(result_p) == res_series_end)
          OKLIB_THROW("Resulting set of series' does not contain " + boost::lexical_cast<std::string>(*result_p));
        query.push_back(res_series);
        if (query.size() != 2)
          OKLIB_THROW("Query vector has size " + boost::lexical_cast<std::string>(query.size()));

        const SetResultNodesP* const res_benchmark(OKlib::SetAlgorithms::map_value(rdb.benchmark(), first_benchmark));
        const SetResultNodesP::iterator res_benchmark_end = res_benchmark -> end();
        if (res_benchmark -> find(result_p) == res_benchmark_end)
          OKLIB_THROW("Resulting set of benchmarks does not contain " + boost::lexical_cast<std::string>(*result_p));
        query.push_back(res_benchmark);
        if (not query.size() == 3)
          OKLIB_THROW("Query vector has size " + boost::lexical_cast<std::string>(query.size()));

        if (query[0] != res_super_series)
          OKLIB_THROW("Query vector[0] has changed");
        if (query[1] != res_series)
          OKLIB_THROW("Query vector[1] has changed");
        if (query[2] != res_benchmark)
          OKLIB_THROW("Query vector[2] has changed");
        const intersection_type& res(rdb.intersection());
        if (not std::binary_search(res.begin(), res.end(), result_p)) {
          std::stringstream message;
          message <<"Result does not contain element\n" << *result_p << "\nResult contains " << res.size() << " elements";
          OKLIB_THROW(message.str());
        }
        {
          intersection_type ref1;
          ref1.reserve(std::min(res_super_series -> size(), res_series -> size()));
          typedef typename intersection_type::iterator iterator;
          std::set_intersection(res_super_series -> begin(), res_super_series_end, res_series -> begin(), res_series_end, std::back_inserter(ref1));
          intersection_type ref2;
          ref2.reserve(std::min(ref1.size(), res_benchmark -> size()));
          std::set_intersection(ref1.begin(), ref1.end(), res_benchmark -> begin(), res_benchmark_end, std::back_inserter(ref2));
          OKLIB_TEST_EQUAL(res, ref2);
        }
        {
          typedef VectorResultNodesP::const_iterator const_iterator;
          const const_iterator end = res.end();
          for (const_iterator i = res.begin(); i != end; ++i) {
            const ResultNode& node(**i);

            OKLIB_TEST_EQUAL(node.rb -> super_series(), first_super_series);
            OKLIB_TEST_EQUAL(node.rb -> series(), first_series);
            OKLIB_TEST_EQUAL(node.rb -> benchmark(), first_benchmark);

            const MapSuperSeries::value_type node_sup_ser_it(*node.m_sup_ser_it);
            OKLIB_TEST_EQUAL(node_sup_ser_it.first, first_super_series);
            OKLIB_TEST_EQUAL(node_sup_ser_it.second, res_super_series);
            const MapSeries::value_type node_ser_it(*node.m_ser_it);
            OKLIB_TEST_EQUAL(node_ser_it.first, first_series);
            OKLIB_TEST_EQUAL(node_ser_it.second, res_series);
            const MapBenchmark::value_type node_bench_it(*node.m_bench_it);
            OKLIB_TEST_EQUAL(node_bench_it.first, first_benchmark);
            OKLIB_TEST_EQUAL(node_bench_it.second, res_benchmark);
          }
        }
      }
    };

    template <class ResultDatabase, class ResultContainer>
    inline Test_Database_vs_Container<ResultDatabase, ResultContainer> test_Database_vs_Container(ResultDatabase& rdb, ResultContainer& rc) {
      return Test_Database_vs_Container<ResultDatabase, ResultContainer>(rdb, rc);
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

        test_result<Result>(filename_large_industrial, line_count_large_industrial);
        test_result<ResultRandomSat>(filename_large_random, line_count_large_random);

      }

      template <class result_type>
      void test_result(const std::string& filename, const unsigned int line_count) {

        typedef typename tuple_type<result_type>::type TupleResult;
        typedef std::vector<result_type> VectorResult;
        typedef typename VectorResult::const_iterator const_iterator;
        typedef typename VectorResult::iterator iterator;
        typedef typename VectorResult::size_type size_type;
        typedef ResultDatabase<const_iterator> result_database;

        {
          typedef ParserResult<result_type> Parser;
          typedef std::vector<TupleResult> VectorTupleResult;
          typedef typename VectorTupleResult::size_type size_type_tuple;
          VectorTupleResult pre_test_vector;
          add_positive_result_tuples(pre_test_vector);

          std::stringstream input;
          std::copy(pre_test_vector.begin(), pre_test_vector.end(), std::ostream_iterator<TupleResult>(input, "\n"));

          VectorResult test_vector;
          assert(copy_results<Parser>(input.str().c_str(), std::back_inserter(test_vector)).full);
          assert(test_vector.size() == pre_test_vector.size());

          result_database db(test_vector.begin(), test_vector.end());

          OKLIB_TESTTRIVIAL_RETHROW(test_Database_vs_Container(db, test_vector));
        }
        {
          VectorResult test_vector;
          typedef std::back_insert_iterator<VectorResult> OutputIterator;
          typedef Copy_results_from_file<ParserResult, OutputIterator, result_type> Copy;
          assert(Copy()(filename, std::back_inserter(test_vector)).full);
          assert(test_vector.size() == line_count);
          result_database db(test_vector.begin(), test_vector.end());
          OKLIB_TESTTRIVIAL_RETHROW(test_Database_vs_Container(db, test_vector));
        }

      }

    };
    
  }

}

#endif

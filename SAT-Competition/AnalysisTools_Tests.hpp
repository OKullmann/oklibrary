// Oliver Kullmann, 7.6.2005 (Swansea)

#ifndef ANALYSISTOOLSTESTS_37yh6fR4

#define ANALYSISTOOLSTESTS_37yh6fR4

#include <cassert>
#include <iterator>

#include <boost/iterator/transform_iterator.hpp>
#include <boost/range/iterator_range.hpp>

#include "FunctionHandling.hpp"
#include "BasicSetOperations.hpp"
#include "SequenceOperations.hpp"

#include "SingleResult.hpp"
#include "ParsingSingleResult.hpp"
#include "ParsingResultSequences_Tests.hpp"
#include "ResultProcessing.hpp"

namespace OKlib {

  namespace SATCompetition {

    template <template <class Database> class ElementaryAnalysis>
    class Test_ElementaryAnalysis : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ElementaryAnalysis test_type;
      Test_ElementaryAnalysis() {
        insert(this);
      }
    private :

      void perform_test_trivial() {

        test_result<Result>(filename_large_industrial, line_count_large_industrial);

      }

      template <class result_type>
      void test_result(const std::string& filename, const unsigned int line_count) {

        typedef  Result_database_from_file<ParserResult, result_type> result_database;
        result_database rdb(filename);
        assert(rdb.result_sequence.size() == line_count);
        typedef typename result_database::database_type database;
        typedef ElementaryAnalysis<database> elementary_analysis;
        elementary_analysis ea(rdb.db);
        typedef typename elementary_analysis::map_superseries_series map_superseries_series;
        typedef typename elementary_analysis::map_series_benchmarks map_series_benchmarks;
        typedef typename elementary_analysis::seq_series seq_series;
        typedef typename elementary_analysis::seq_benchmarks seq_benchmarks;

        { // testing ea.series_in_superseries()
          {
            typedef FunctionHandling::First<typename map_superseries_series::value_type> first1;
            typedef FunctionHandling::First<MapSuperSeries::value_type> first2;
            OKLIB_TEST_EQUAL_RANGES(
                                    boost::make_iterator_range(boost::make_transform_iterator<first1>(ea.series_in_superseries().begin()), (boost::make_transform_iterator<first1>(ea.series_in_superseries().end()))),
                                    boost::make_iterator_range(boost::make_transform_iterator<first2>(rdb.db.super_series().begin()), boost::make_transform_iterator<first2>(rdb.db.super_series().end())));
          }
          {
            typedef FunctionHandling::Second<typename map_superseries_series::value_type> second;
            seq_series s;
            OKlib::SetAlgorithms::union_sets(boost::make_transform_iterator<second>(ea.series_in_superseries().begin()), boost::make_transform_iterator<second>(ea.series_in_superseries().end()), std::back_inserter(s));
            typedef FunctionHandling::First<MapSeries::value_type> first;
            OKLIB_TEST_EQUAL_RANGES(s, boost::make_iterator_range(boost::make_transform_iterator<first>(rdb.db.series().begin()), boost::make_transform_iterator<first>(rdb.db.series().end())));
          }
        }

       { // testing ea.benchmarks_in_series()
         typedef FunctionHandling::Second<typename map_series_benchmarks::value_type> second;
         typedef boost::transform_iterator<second, typename map_series_benchmarks::const_iterator> transform_iterator;
         const map_series_benchmarks& map(ea.benchmarks_in_series());
         const transform_iterator& begin(transform_iterator(map.begin()));
         const transform_iterator& end(transform_iterator(map.end()));
         OKLIB_TEST_EQUAL(OKlib::SetAlgorithms::sum_sizes(begin, end), rdb.db.benchmark().size());
         seq_benchmarks seq;
         OKlib::SetAlgorithms::union_sets(begin, end, std::back_inserter(seq));
         typedef FunctionHandling::First<MapBenchmark::value_type> first;
         OKLIB_TEST_EQUAL_RANGES(seq, boost::make_iterator_range(boost::make_transform_iterator<first>(rdb.db.benchmark().begin()), boost::make_transform_iterator<first>(rdb.db.benchmark().end())));
       }

       // ToDo: to be completed
      }

    };

  }

}

#endif

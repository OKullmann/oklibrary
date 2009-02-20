// Oliver Kullmann, 3.6.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef PARSINGRESULTSEQUENCESTESTS_k324RtG
#define PARSINGRESULTSEQUENCESTESTS_k324RtG

#include <string>

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

#include <OKlib/Programming/Parsing/ParserBase_Tests.hpp>

#include <OKlib/Experimentation/Competition/SingleResult.hpp>
#include <OKlib/Experimentation/Competition/ParsingSingleResult.hpp>
#include <OKlib/Experimentation/Competition/ParsingResultSequences.hpp>
#include <OKlib/Experimentation/Competition/ParsingSingleResult_Tests.hpp>

namespace OKlib {
  namespace SATCompetition {

    template <class Container>
    class Test_Copy_results_fill_from_file : public ::OKlib::TestSystem::Test {
      const boost::filesystem::path filename;
      Container& C;
    public :
      typedef Test_Copy_results_fill_from_file test_type;
      Test_Copy_results_fill_from_file(const boost::filesystem::path& filename, Container& C) : filename(filename), C(C) {}
      typedef typename Container::value_type result_type;
    private :
      void perform_test_trivial() {
        typedef std::back_insert_iterator<Container> OutputIterator;
        typedef Copy_results_from_file<ParserResult, OutputIterator, result_type> Copy;
        typedef typename Copy::ParseIterator ParseIterator;
        OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingResult_Positional<ParseIterator>(Copy()(filename, std::back_inserter(C))));
      }
    };
    template <class Container>
    inline Test_Copy_results_fill_from_file<Container> test_Copy_results_fill_from_file(const boost::filesystem::path& filename, Container& C) {
      return Test_Copy_results_fill_from_file<Container>(filename, C);
    }

    // -----------------------------------------------------------------------------------------------------------------------------
    // -----------------------------------------------------------------------------------------------------------------------------

    class Test_Copy_results_ParserResult_Result_positive_cases : public ::OKlib::TestSystem::Test {
    public :
      typedef Test_Copy_results_ParserResult_Result_positive_cases test_type;
    private :
      void perform_test_trivial() {
        typedef ParserResult<Result> Parser;
        typedef std::list<Result> List_output;
        List_output output;
        typedef std::vector<TupleResult> Vector;
        Vector test_vector;
        add_positive_result_tuples(test_vector);
        std::stringstream input;
        std::copy(test_vector.begin(), test_vector.end(), std::ostream_iterator<TupleResult>(input, "\n"));
        if (not copy_results<Parser>(input.str().c_str(), std::back_inserter(output)).full)
          OKLIB_THROW("Sequence\n" + input.str() + "was not accepted.");
        if (output.size() != test_vector.size())
          OKLIB_THROW("Sequence size is " + boost::lexical_cast<std::string>(output.size()) + ", and not " + boost::lexical_cast<std::string>(test_vector.size()));
        Vector::size_type index = 0;
        for (List_output::const_iterator i = output.begin(); i != output.end(); ++i, ++index) {
          if (*i != test_vector[index]) {
            std::stringstream message;
            message << "Result \"" << *i << "\" different from input \"" << test_vector[index] << "\"";
            OKLIB_THROW(message.str());
          }
        }
      }
    };

    class Test_Copy_results_ParserResult_Result_negative_cases : public ::OKlib::TestSystem::Test {
    public :
      typedef Test_Copy_results_ParserResult_Result_negative_cases test_type;
    private :
      void perform_test_trivial() {
        typedef ParserResult<Result> Parser;
        typedef std::list<Result> List_output;
        List_output output;
        {
          typedef std::vector<std::string> Vector;
          Vector test_vector;
          
          test_vector.push_back("3SAT random/MediumSizeBenches/k3-r4.263-v300 bench1902 solver1 20 7.09 1319");
           test_vector.push_back("3SAT random/MediumSizeBenches/k3-r4.263-v300 bench1902 solver1 20 7.09 1319\n3SAT random/MediumSizeBenches/k3-r4.263-v300 bench1902 solver1 20 7.09 1319");
          for (Vector::const_iterator i = test_vector.begin(); i != test_vector.end(); ++i)
            if (copy_results<Parser>(i -> c_str(), std::back_inserter(output)).full)
              OKLIB_THROW("Sequence\n" + *i + "was accepted.");
        }
      }
    };

    const std::string filename_large_industrial = "Data/export-industrial_2005_Round1.txt";
    const unsigned int line_count_large_industrial = 17168;

    class Test_Copy_results_ParserResult_Result : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_Copy_results_ParserResult_Result test_type;
      Test_Copy_results_ParserResult_Result() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        {
          OKLIB_TESTTRIVIAL_RETHROW(Test_Copy_results_ParserResult_Result_positive_cases());
          OKLIB_TESTTRIVIAL_RETHROW(Test_Copy_results_ParserResult_Result_negative_cases());
        }
        {
          // ToDo: This should belong to the more time-consuming testing.
          typedef std::list<Result> List_output;
          List_output output;
          OKLIB_TESTTRIVIAL_RETHROW(test_Copy_results_fill_from_file(filename_large_industrial, output));
          const List_output::size_type output_size = output.size();
          OKLIB_TEST_EQUAL(output_size, line_count_large_industrial);
        }
       }
    };

    // -----------------------------------------------------------------------------------------------------------------------------
    // -----------------------------------------------------------------------------------------------------------------------------

    class Test_Copy_results_ParserResultRandomSat_ResultRandomSat_positive_cases : public ::OKlib::TestSystem::Test {
    public :
      typedef Test_Copy_results_ParserResultRandomSat_ResultRandomSat_positive_cases test_type;
    private :
      void perform_test_trivial() {
        typedef ParserResult<ResultRandomSat> Parser;
        typedef std::list<ResultRandomSat> List_output;
        List_output output;
        typedef std::vector<TupleResultRandomSat> Vector;
        Vector test_vector;
        add_positive_result_tuples(test_vector);
        std::stringstream input;
        std::copy(test_vector.begin(), test_vector.end(), std::ostream_iterator<TupleResultRandomSat>(input, "\n"));
        if (not copy_results<Parser>(input.str().c_str(), std::back_inserter(output)).full)
          OKLIB_THROW("Sequence\n" + input.str() + "was not accepted.");
        if (output.size() != test_vector.size())
          OKLIB_THROW("Sequence size is " + boost::lexical_cast<std::string>(output.size()) + ", and not " + boost::lexical_cast<std::string>(test_vector.size()));
        Vector::size_type index = 0;
        for (List_output::const_iterator i = output.begin(); i != output.end(); ++i, ++index) {
          if (*i != test_vector[index]) {
            std::stringstream message;
            message << "ResultRandomSat \"" << *i << "\" different from input \"" << test_vector[index] << "\"";
            OKLIB_THROW(message.str());
          }
        }
      }
    };

    const std::string filename_large_random = "Data/export-random_2005_Round1_corrected.txt";
    const unsigned int line_count_large_random = 11700;

    class Test_Copy_results_ParserResultRandomSat_ResultRandomSat : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_Copy_results_ParserResultRandomSat_ResultRandomSat test_type;
      Test_Copy_results_ParserResultRandomSat_ResultRandomSat() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(Test_Copy_results_ParserResultRandomSat_ResultRandomSat_positive_cases());
        OKLIB_TESTTRIVIAL_RETHROW(Test_Copy_results_ParserResult_Result_negative_cases());
        {
          typedef std::list<ResultRandomSat> List_output;
          List_output output;
          OKLIB_TESTTRIVIAL_RETHROW(test_Copy_results_fill_from_file(filename_large_random, output));
          const List_output::size_type output_size = output.size();
          OKLIB_TEST_EQUAL(output_size, line_count_large_random);
        }
      }
    };

  }

}

#endif

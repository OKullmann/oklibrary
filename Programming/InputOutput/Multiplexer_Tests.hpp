// Oliver Kullmann, 19.1.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/InputOutput/Multiplexer_Tests.hpp
  \brief Testing of components for gathering and distributing input and output from and to different streams
*/

#ifndef MULTIPLEXERTESTS_oKnCd2
#define MULTIPLEXERTESTS_oKnCd2

#include <set>
#include <vector>
#include <string>
#include <cassert>

#include <boost/assign/std/vector.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/lexical_cast.hpp>

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

namespace OKlib {
  namespace GeneralInputOutput {

    /*!
      \class Test_OStreamDescriptor
      \brief Test models of OStreamDescriptor
    */

    template <class OStreamDescriptor>
    class Test_OStreamDescriptor : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_OStreamDescriptor test_type;
      Test_OStreamDescriptor() {
        insert(this);
      }
    private :
      void perform_test_trivial() {

        typedef typename OStreamDescriptor::label_type label_type;
        OKLIB_TEST_EQUAL(OStreamDescriptor::nullstream, 0);
        OKLIB_TEST_EQUAL(OStreamDescriptor::stdcerr, 1);
        OKLIB_TEST_EQUAL(OStreamDescriptor::stdcout, 2);
        OKLIB_TEST_EQUAL(OStreamDescriptor::stdofstreamappend, 3);
        OKLIB_TEST_EQUAL(OStreamDescriptor::stdofstreamoverwrite, 4);
        OKLIB_TEST_EQUAL(OStreamDescriptor::stdostringstream, 5);

        typedef typename OStreamDescriptor::string_type string_type;
        {
          const OStreamDescriptor od("NULL");
          OKLIB_TEST_EQUAL(od.label, OStreamDescriptor::nullstream);
          OKLIB_TEST_EQUAL(od.name, string_type());
        }
        {
          const OStreamDescriptor od("cerr");
          OKLIB_TEST_EQUAL(od.label, OStreamDescriptor::stdcerr);
          OKLIB_TEST_EQUAL(od.name, string_type());
        }
        {
          const OStreamDescriptor od("cout");
          OKLIB_TEST_EQUAL(od.label, OStreamDescriptor::stdcout);
          OKLIB_TEST_EQUAL(od.name, string_type());
        }
        {
          const OStreamDescriptor od("ofstream-app=x");
          OKLIB_TEST_EQUAL(od.label, OStreamDescriptor::stdofstreamappend);
          OKLIB_TEST_EQUAL(od.name, string_type("x"));
        }
        {
          const OStreamDescriptor od("ofstream-w=y x");
          OKLIB_TEST_EQUAL(od.label, OStreamDescriptor::stdofstreamoverwrite);
          OKLIB_TEST_EQUAL(od.name, string_type("y x"));
        }
        {
          const OStreamDescriptor od("ostringstream=9");
          OKLIB_TEST_EQUAL(od.label, OStreamDescriptor::stdostringstream);
          OKLIB_TEST_EQUAL(od.name, string_type("9"));
        }
        {
          bool thrown = false;
          const std::string desc("abcd 9o =xcd ");
          try {
            const OStreamDescriptor od(desc);
          }
          catch(const typename OStreamDescriptor::UnknownDescription& e) {
            thrown = true;
            OKLIB_TEST_EQUAL(e.what(), desc);
          }
          OKLIB_TEST_EQUAL(thrown, true);
        }
        {
          bool thrown = false;
          const std::string desc("=\n");
          try {
            const OStreamDescriptor od(desc);
          }
          catch(const typename OStreamDescriptor::UnknownDescription& e) {
            thrown = true;
            OKLIB_TEST_EQUAL(e.what(), desc);
          }
          OKLIB_TEST_EQUAL(thrown, true);
        }
        {
          bool thrown = false;
          const std::string desc("ostringstream=");
          try {
            const OStreamDescriptor od(desc);
          }
          catch(const typename OStreamDescriptor::EmptyAfterEqDescription& e) {
            thrown = true;
            OKLIB_TEST_EQUAL(e.what(), desc);
          }
          OKLIB_TEST_EQUAL(thrown, true);
        }
        {
          bool thrown = false;
          const std::string desc("ostringstream abc");
          try {
            const OStreamDescriptor od(desc);
          }
          catch(const typename OStreamDescriptor::NoEqInDescription& e) {
            thrown = true;
            OKLIB_TEST_EQUAL(e.what(), desc);
          }
          OKLIB_TEST_EQUAL(thrown, true);
        }
        {
          bool thrown = false;
          const std::string desc("");
          try {
            const OStreamDescriptor od(desc);
          }
          catch(const typename OStreamDescriptor::EmptyDescription& e) {
            thrown = true;
            OKLIB_TEST_EQUAL(e.what(), desc);
          }
          OKLIB_TEST_EQUAL(thrown, true);
        }
        {
          const std::string name("x");
          for (label_type i = OStreamDescriptor::nullstream; i < OStreamDescriptor::stdofstreamappend; ++reinterpret_cast<int&>(i)) {
            bool thrown = false;
            try {
              const OStreamDescriptor od(i, name);
            }
            catch(const typename OStreamDescriptor::NonEmptyName& e) {
              thrown = true;
              OKLIB_TEST_EQUAL(e.what(), "label = " + boost::lexical_cast<std::string>(i) + ", name = " + name);
            }
            OKLIB_TEST_EQUAL(thrown, true);
          }
        }
        {
          for (label_type i = OStreamDescriptor::stdofstreamappend; i <= OStreamDescriptor::stdostringstream; ++reinterpret_cast<int&>(i)) {
            bool thrown = false;
            try {
              const OStreamDescriptor od(i, "");
            }
            catch(const typename OStreamDescriptor::EmptyName& e) {
              thrown = true;
              OKLIB_TEST_EQUAL(e.what(), "label = " + boost::lexical_cast<std::string>(i));
            }
            OKLIB_TEST_EQUAL(thrown, true);
          }
        }
      }
    };

    // ##################################################

    /*!
      \class Test_OStreamMultiplexer
      \brief Testing an OStreamMulitplexer model
      \todo Use test for OStreamDescriptor as subtest.
    */

    template <class OStreamMultiplexer>
    class Test_OStreamMultiplexer : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_OStreamMultiplexer test_type;
      Test_OStreamMultiplexer() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        typedef typename OStreamMultiplexer::ostream_descriptor_type ostream_descriptor_type;
        typedef typename OStreamMultiplexer::label_type label_type;
        typedef typename OStreamMultiplexer::string_type string_type;
        typedef typename OStreamMultiplexer::stringstream_map_type stringstream_map_type;
        typedef typename OStreamMultiplexer::ofstream_map_type ofstream_map_type;
        typedef typename OStreamMultiplexer::fostream_vector_type fostream_vector_type;

        typedef std::set<ostream_descriptor_type> descriptor_set_type;
        typedef std::vector<descriptor_set_type> test_vector_type;

        typedef ostream_descriptor_type odt;
        typedef descriptor_set_type dst;

        {
          test_vector_type test_vector;
          OStreamMultiplexer multiplexer(test_vector);
          OKLIB_TEST_EQUAL(multiplexer.stringstream_map.empty(), true);
          OKLIB_TEST_EQUAL(multiplexer.ofstream_map.empty(), true);
          OKLIB_TEST_EQUAL(multiplexer.fostream_vector.empty(), true);
        }
        {
          test_vector_type test_vector;
          using namespace boost::assign;
          test_vector +=
            list_of(odt(odt::nullstream, ""));
          OStreamMultiplexer multiplexer(test_vector);
          OKLIB_TEST_EQUAL(multiplexer.stringstream_map.empty(), true);
          OKLIB_TEST_EQUAL(multiplexer.ofstream_map.empty(), true);
          OKLIB_TEST_EQUAL(multiplexer.fostream_vector.size(), 1U);
          std::ostream& out(multiplexer.fostream_vector[0]);
          const int max_iterations = 10000;
          for (int i = 0; i < max_iterations; ++i)
            out << "NO OUTPUT\n";
        }
        {
          test_vector_type test_vector;
          using namespace boost::assign;
          test_vector +=
            list_of(odt(odt::nullstream, "")), list_of(odt(odt::stdostringstream, "s1"))(odt(odt::stdostringstream, "s2")), list_of(odt(odt::stdostringstream, "s1"))(odt(odt::stdostringstream, "s3"));
          OStreamMultiplexer multiplexer(test_vector);
          OKLIB_TEST_EQUAL(multiplexer.stringstream_map.size(), 3U);
          OKLIB_TEST_EQUAL(multiplexer.ofstream_map.empty(), true);
          OKLIB_TEST_EQUAL(multiplexer.fostream_vector.size(), 3U);
          std::ostream& out0(multiplexer.fostream_vector[0]);
          const int max_iterations = 1000;
          for (int i = 0; i < max_iterations; ++i)
            out0 << "NO OUTPUT\n";
          std::ostream& out1(multiplexer.fostream_vector[1]);
          const char* const m1 = "M1a\nM1b";
          out1 << m1;
          std::ostream& out2(multiplexer.fostream_vector[2]);
          const char* const m2 = "M2a\nM2b";
          out2 << m2;
          out1.flush(); out2.flush();
          OKLIB_TEST_EQUAL(multiplexer.stringstream_map["s1"].str(), std::string(m1) + std::string(m2));
          OKLIB_TEST_EQUAL(multiplexer.stringstream_map["s2"].str(), m1);
          OKLIB_TEST_EQUAL(multiplexer.stringstream_map["s3"].str(), m2);
        }
        {
          test_vector_type test_vector;
          using namespace boost::assign;
          test_vector +=
            list_of(odt("ofstream-app=f1"))(odt("ofstream-w=f2")),
            list_of(odt("ostringstream=s1"))(odt(odt::stdostringstream, "s2"))(odt("ofstream-w=f1")),
            list_of(odt("ostringstream=s1"))(odt("ostringstream=s3"))(odt("ofstream-app=f2"));

          struct file1_t {
            const boost::filesystem::path p;
            explicit file1_t(const std::string& name) : p(name) {}
            operator const boost::filesystem::path&() { return p; }
            ~file1_t() { boost::filesystem::remove(p); }
          } test_file_1("f1");
          assert(not boost::filesystem::exists(test_file_1));
          struct file2_t {
            const boost::filesystem::path p;
            file2_t(const std::string& name) : p(name) {}
            operator const boost::filesystem::path&() { return p; }
            ~file2_t() { boost::filesystem::remove(p); }
          } test_file_2("f2");
          assert(not boost::filesystem::exists(test_file_2));
          const std::string string1("This must stay.\n");
          {
            boost::filesystem::ofstream f1(test_file_1);
            assert(f1);
            f1 << string1;
          }
          assert(boost::filesystem::file_size(test_file_1) == string1.size());
          const std::string string2("This must go.\n");
          {
            boost::filesystem::ofstream f2(test_file_2);
            assert(f2);
            f2 << string2;
          }
          assert(boost::filesystem::file_size(test_file_2) == string2.size());

          OStreamMultiplexer multiplexer(test_vector);
          OKLIB_TEST_EQUAL(multiplexer.stringstream_map.size(), 3U);
          OKLIB_TEST_EQUAL(multiplexer.ofstream_map.size(), 2U);
          OKLIB_TEST_EQUAL(multiplexer.fostream_vector.size(), 3U);
          OKLIB_TEST_EQUAL(boost::filesystem::exists(test_file_1), true);
          OKLIB_TEST_EQUAL(boost::filesystem::exists(test_file_2), true);

          std::ostream& out0(multiplexer.fostream_vector[0]);
          const char* const m0 = "M0\n\n";
          out0 << m0;
          std::ostream& out1(multiplexer.fostream_vector[1]);
          const char* const m1 = "M1a\nM1b";
          out1 << m1;
          std::ostream& out2(multiplexer.fostream_vector[2]);
          const char* const m2 = "M2a\nM2b";
          out2 << m2;
          out0.flush(); out1.flush(); out2.flush();
          OKLIB_TEST_EQUAL(multiplexer.stringstream_map["s1"].str(), std::string(m1) + std::string(m2));
          OKLIB_TEST_EQUAL(multiplexer.stringstream_map["s2"].str(), m1);
          OKLIB_TEST_EQUAL(multiplexer.stringstream_map["s3"].str(), m2);
          {
            boost::filesystem::ifstream f1(test_file_1);
            assert(f1);
            std::ostringstream out;
            out << f1.rdbuf();
            OKLIB_TEST_EQUAL(out.str(), string1 + m0 + m1);
          }
          {
            boost::filesystem::ifstream f2(test_file_2);
            assert(f2);
            std::ostringstream out;
            out << f2.rdbuf();
            OKLIB_TEST_EQUAL(out.str(), std::string(m0) + m2);
          }
          const std::string string3("addition1");
          multiplexer.ofstream_map[test_file_1.p.string()] << string3;
          multiplexer.ofstream_map[test_file_1.p.string()].flush();
          const std::string string4("addition2");
          multiplexer.ofstream_map[test_file_2.p.string()] << string4;
          multiplexer.ofstream_map[test_file_2.p.string()].flush();
          {
            boost::filesystem::ifstream f1(test_file_1);
            assert(f1);
            std::ostringstream out;
            out << f1.rdbuf();
            OKLIB_TEST_EQUAL(out.str(), string1 + m0 + m1 + string3);
          }
          {
            boost::filesystem::ifstream f2(test_file_2);
            assert(f2);
            std::ostringstream out;
            out << f2.rdbuf();
            OKLIB_TEST_EQUAL(out.str(), std::string(m0) + m2 + string4);
          }
        }
      }
    };

  }
}

#endif

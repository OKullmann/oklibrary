// Matthew Henderson, 25.10.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/InputOutput/RecursiveDirectoryIteration_Tests.hpp
  \brief Tests for recursive directory iteration.

  \bug Checking for directories should not involve CVS-files!
  So the code commented out below must be corrected.

  \todo There is now a recursive directory-iterator in boost!
*/

#ifndef RECURSIVEDIRECTORYITERATIONTESTS_oui453
#define RECURSIVEDIRECTORYITERATIONTESTS_oui453

#include <iostream>
#include <cassert>
#include <string>
#include <set>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp> 
#include <boost/filesystem/fstream.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/tuple/tuple.hpp>
 
#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

#include <OKlib/Concepts/LibraryBasics.hpp>
#include <OKlib/Concepts/std_Iterators.hpp>

namespace OKlib {
  namespace GeneralInputOutput {

   /*!
      \class TestData
      \brief Provides the names of directories and files within the 
      TestDirectory and their status as directory (1) or file (0).
    */

    class TestData {
     
      typedef std::string value_type;
      typedef boost::tuple<value_type,bool> tuple_type;
      
    public:

      typedef std::vector<tuple_type> checked_pathname_vector_type;
      typedef checked_pathname_vector_type::const_iterator const_iterator;
      static checked_pathname_vector_type checked_pathname_vector;

      TestData() {

        using namespace boost::assign;
        typedef tuple_type el_t;
        checked_pathname_vector +=
          el_t("TestDirectory",1),
          el_t("TestDirectory/Test_001.hpp",0),
          el_t("TestDirectory/TestSubDirectory",1),
          el_t("TestDirectory/TestSubDirectory/Test_002.hpp",0),
          el_t("CVS",1),
          el_t("CVS/Entries",0),
          el_t("CVS/Repository",0),
          el_t("CVS/Root",0);
      }

      value_type checked_pathname(const_iterator i) {
        return i -> get<0>();
      }

      bool is_directory(const_iterator i) {
        return i -> get<1>();
      }

    };

    TestData::checked_pathname_vector_type TestData::checked_pathname_vector;

    /*!
      \class Test_DirectoryIterator
      \brief Testing the concept of a directory iterator.
      \todo Using path-equality or path-equivalence ?
      \todo Instead of OKlib::Concepts::InputIterator we should require OKlib::Concepts::InputIteratorDefault and MultiPassInputIterator (???).
      \todo Testing should also cover the case of symbolic links.
      \todo Provide testing of star operator in private member function test_star_operator.
      \todo Provide testing of increment operator in private member function test_increment_operator.
      \todo Extend testing of equality and inequality operators to non-trivial cases.
    */
    
    template <class DirectoryIterator>
    class Test_DirectoryIterator : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_DirectoryIterator test_type;
      Test_DirectoryIterator() {
        insert(this);
      }
    private :

      TestData test_data;

      void test_concepts() {
        OKLIB_MODELS_CONCEPT_REQUIRES(DirectoryIterator, OKlib::Concepts::InputIterator);
      }

      void test_equality_inequality() {
          DirectoryIterator dir_it;
          if (not (dir_it == dir_it))
            OKLIB_THROW("not dir_it == dir_it");
          if (dir_it != dir_it)
            OKLIB_THROW("dir_it != dir_it");
      }

      void perform_test_trivial() {
        
        test_concepts();
        test_equality_inequality();


        // ##################################################################################

        {

          typedef DirectoryIterator DirIt;
          typedef boost::filesystem::path path;
                   
          const std::string str_test_1("TestDirectory");
          const path path_test_1(str_test_1);
          assert(boost::filesystem::exists(path_test_1));
	  assert(boost::filesystem::is_directory(path_test_1));
	  
          const:: std::string str_test_2("TestDirectory/Test_001.hpp");
          const path path_test_2(str_test_2);
          assert(boost::filesystem::exists(path_test_2));
          assert(not boost::filesystem::is_directory(path_test_2));

//           const:: std::string str_test_3("CVS");
//           const path path_test_3(str_test_3);
//           assert(boost::filesystem::exists(path_test_3));
//           assert(boost::filesystem::is_directory(path_test_3));

//           const:: std::string str_test_4("CVS/Entries");
//           const path path_test_4(str_test_4);
//           assert(boost::filesystem::exists(path_test_4));
//           assert(not boost::filesystem::is_directory(path_test_4));

//           const:: std::string str_test_5("CVS/Repository");
//           const path path_test_5(str_test_5);
//           assert(boost::filesystem::exists(path_test_5));
//           assert(not boost::filesystem::is_directory(path_test_5));

//           const:: std::string str_test_6("CVS/Root");
//           const path path_test_6(str_test_6);
//           assert(boost::filesystem::exists(path_test_6));
//           assert(not boost::filesystem::is_directory(path_test_6));

          DirIt dir_it(path_test_1);
          if (not (dir_it == dir_it))
            OKLIB_THROW("not dir_it == dir_it");
          if (dir_it != dir_it)
            OKLIB_THROW("dir_it != dir_it");

          if (boost::filesystem::equivalent(path_test_1,*dir_it))
            OKLIB_THROW("boost::filesystem::equivalent(path_test_1,*dir_it)");

//           typedef std::set<std::string> set_of_names;
//           set_of_names cvs_file_names_ref;
//           cvs_file_names_ref.insert("Entries"); cvs_file_names_ref.insert("Repository"); cvs_file_names_ref.insert("Root");
//           set_of_names file_names_found;
//           for (int i = 0; i != 3; ++i)
//             file_names_found.insert((*(dir_it++)).leaf());
//           OKLIB_TEST_EQUAL_W(cvs_file_names_ref, file_names_found);

          // \todo Replace tests below with concept tests.

          DirIt dir_it_0;
          DirIt dir_it_1;
          if (not (dir_it == dir_it))
            OKLIB_THROW("not dir_it_0 == dir_it_1 (Default constructor)");

          DirIt dir_it_2(dir_it);
          if (not (dir_it == dir_it_2))
            OKLIB_THROW("dir_it == dir_it_2 (Copy constructor)");

          DirIt dir_it_3;
          dir_it_3 = dir_it_2;
          if (not (dir_it_2 == dir_it_3))
            OKLIB_THROW("dir_it_2 == dir_it_3 (Assignment)");

          DirIt dir_it_4;
          DirIt dir_it_5;
          assert(dir_it_4 == dir_it_5);
          if (not (dir_it_5 == dir_it_4))
            OKLIB_THROW("dir_it_5 == dir_it_4 (Symmetry of equality)");

          DirIt dir_it_6;
          assert(dir_it_5 == dir_it_6);
          if (not (dir_it_4 == dir_it_6))
            OKLIB_THROW("dir_it_4 == dir_it_6 (Transitivity of equality)");

          DirIt dir_it_7;
          DirIt dir_it_8;
          if (not (dir_it_7==dir_it_8)!=(dir_it_7!=dir_it_8))
            OKLIB_THROW("(dir_it_7==dir_it_8)!=(dir_it_7!=dir_it_8)");   
          
        }
      }
    };

  }
}

#endif

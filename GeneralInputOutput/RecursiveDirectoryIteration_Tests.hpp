/*!
  \file RecursiveDirectoryIteration_Tests.hpp
  \brief Tests for recursive directory iteration.
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
 
#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

#include "LibraryBasics.hpp"
#include "std_Iterators.hpp"

namespace OKlib {

  namespace RecursiveDirectoryIteration {

    /*!
      \class Test_DirectoryIterator
      \brief Testing the concept of a directory iterator.
      \todo Using path-equality or path-equivalence ?
      \todo Instead of OKlib::Concepts::InputIterator we should require OKlib::Concepts::InputIteratorDefault and MultiPassInputIterator (???).
      \todo Testing should also cover the case of symbolic links.
    */
    
    template <class DirectoryIterator>
    class Test_DirectoryIterator : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_DirectoryIterator test_type;
      Test_DirectoryIterator() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        OKLIB_MODELS_CONCEPT_REQUIRES(DirectoryIterator, OKlib::Concepts::InputIterator);
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

          const:: std::string str_test_3("CVS");
          const path path_test_3(str_test_3);
          assert(boost::filesystem::exists(path_test_3));
          assert(boost::filesystem::is_directory(path_test_3));

          const:: std::string str_test_4("CVS/Entries");
          const path path_test_4(str_test_4);
          assert(boost::filesystem::exists(path_test_4));
          assert(not boost::filesystem::is_directory(path_test_4));

          const:: std::string str_test_5("CVS/Repository");
          const path path_test_5(str_test_5);
          assert(boost::filesystem::exists(path_test_5));
          assert(not boost::filesystem::is_directory(path_test_5));

          const:: std::string str_test_6("CVS/Root");
          const path path_test_6(str_test_6);
          assert(boost::filesystem::exists(path_test_6));
          assert(not boost::filesystem::is_directory(path_test_6));

          DirIt dir_it(path_test_1);
          if (not (dir_it == dir_it))
            OKLIB_THROW("not dir_it == dir_it");
          if (dir_it != dir_it)
            OKLIB_THROW("dir_it != dir_it");

          if (boost::filesystem::equivalent(path_test_1,*dir_it))
            OKLIB_THROW("boost::filesystem::equivalent(path_test_1,*dir_it)");

          typedef std::set<std::string> set_of_names;
          set_of_names cvs_file_names_ref;
          cvs_file_names_ref.insert("Entries"); cvs_file_names_ref.insert("Repository"); cvs_file_names_ref.insert("Root");
          set_of_names file_names_found;
          for (int i = 0; i != 3; ++i)
            file_names_found.insert((*(dir_it++)).leaf());
          OKLIB_TEST_EQUAL_W(cvs_file_names_ref, file_names_found);

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

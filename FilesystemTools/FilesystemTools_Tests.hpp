/*!
  \file FilesystemTools_Tests.hpp
  \brief Tests for filesystem tools.
*/

#ifndef FILESYSTEMTOOLSTESTS_oui453

#define FILESYSTEMTOOLSTESTS_oui453

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

  namespace FilesystemTools {
    
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
                    
          const std::string TestDirectory("TestDirectory");
          const path test_path(TestDirectory);
          assert(boost::filesystem::exists(test_path));
	  assert(boost::filesystem::is_directory(test_path));
	  
          const:: std::string TestFile("TestDirectory/Test_001.hpp");
          const path test_path_2(TestFile);
          assert(boost::filesystem::exists(test_path_2));
          assert(not boost::filesystem::is_directory(test_path_2));

          // ToDO: adding asserts for the cvs-files
          
	  DirIt dir_it(test_path);
          if (not (dir_it == dir_it))
            OKLIB_THROW("not dir_it == dir_it");
          if (dir_it != dir_it)
            OKLIB_THROW("dir_it != dir_it");
          if (boost::filesystem::equivalent(test_path,*dir_it))
            OKLIB_THROW("boost::filesystem::equivalent(test_path,*dir_it)");
          typedef std::set<std::string> set_of_names;
          set_of_names cvs_file_names_ref;
          cvs_file_names_ref.insert("Entries"); cvs_file_names_ref.insert("Repository"); cvs_file_names_ref.insert("Root");
          set_of_names file_names_found;
          for (int i = 0; i != 3; ++i)
            file_names_found.insert((*(dir_it++)).leaf());
          OKLIB_TEST_EQUAL_W(cvs_file_names_ref, file_names_found);
        }
      }
    };

  }

}

#endif

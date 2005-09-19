/*!
  \file FilesystemTools_Tests.hpp
  \brief Tests for filesystem tools.
*/

#ifndef FILESYSTEMTOOLSTESTS_oui453

#define FILESYSTEMTOOLSTESTS_oui453

#include <iostream>
#include <cassert>
#include <string>

#include <boost/filesystem/path.hpp> 
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
          typedef boost::filesystem::directory_iterator DirIt_Boost;
          const std::string TestDirectory("TestDirectory");
          path test_path(TestDirectory);
          assert(boost::filesystem::exists(test_path));
          const std::string TestFile("TestDirectory/Test_001.hpp");
          path test_path_2(TestFile);
          assert(boost::filesystem::exists(test_path_2));
          DirIt dir_it(test_path);
          DirIt_Boost boost_dir_it(test_path);
	  DirIt dir_it_2(boost_dir_it);
        }
      }
    };

  }

}

#endif

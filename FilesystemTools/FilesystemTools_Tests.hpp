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
          typedef DirectoryIterator DirIterator;
          typedef boost::filesystem::path path;
          const std::string TestDirectory("TestDirectory");
          path test_path(TestDirectory);
          assert(boost::filesystem::exists(test_path));
	  
        }
      }
    };

  }

}

#endif

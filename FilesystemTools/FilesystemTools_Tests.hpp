/*!
  \file AssociativeContainers_Tests.hpp
  \brief Tests for search data structures.
*/

#ifndef FILESYSTEMTOOLSTESTS_oui453

#define FILESYSTEMTOOLSTESTS_oui453

#include <boost/filesystem/path.hpp> 
#include <iostream>

#include "boost/filesystem/fstream.hpp" 
#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

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
        {
//           typedef DirectoryIterator DirIterator;
//  	  boost::filesystem::path path("/home/csmatthew/test.hpp");
//  	  boost::filesystem::ofstream file( path );
//           file << "Test\n";
//           file.close();	
	  
        }
      }
    };

  }

}

#endif

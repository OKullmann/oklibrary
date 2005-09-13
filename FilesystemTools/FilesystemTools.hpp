// Matthew Henderson, 4.9.2005 (Swansea)

/*!
  \file Filesystemtools.hpp
  \brief Tools related to the filesystem.
*/

#ifndef FILESYSTEMTOOLS_98gsyf

#define FILESYSTEMTOOLS_98gsyf

#include <iostream>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>

#include "AssociativeContainers.hpp"
                   
namespace OKlib {

  namespace FilesystemTools {

    /*!
      \class DirectoryIterator
      \brief Class which provides iterators over the contents of a directory.
    */

    class DirectoryIterator : public boost::filesystem::directory_iterator {
      
    };

  }

}

#endif

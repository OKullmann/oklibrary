// Matthew Henderson, 4.9.2005 (Swansea)

/*!
  \file FilesystemTools.hpp
  \brief Tools related to the filesystem.
*/

#ifndef FILESYSTEMTOOLS_98gsyf

#define FILESYSTEMTOOLS_98gsyf

#include <iostream>
#include <stack>
#include <iterator>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>

#include "DerivedRelations.hpp"
#include "AssociativeContainers.hpp"
                   
namespace OKlib {

  namespace FilesystemTools {

    /*!
      \class DirectoryIterator
      \brief Class which provides recursive input iterators over the non-directory files within a directory.
    */

    class DirectoryIterator {
      typedef boost::filesystem::directory_iterator directory_iterator;
      directory_iterator current_dir_it;
      typedef std::stack<directory_iterator> stack_directory_iterators;
      stack_directory_iterators dir_it_history;

    public :
      typedef std::input_iterator_tag iterator_category;
      typedef directory_iterator::value_type value_type;
      typedef directory_iterator::difference_type difference_type;
      typedef directory_iterator::pointer pointer;
      typedef directory_iterator::reference reference;

      DirectoryIterator() {}

      DirectoryIterator( const directory_iterator& dir_it ) : current_dir_it(dir_it) {}

      DirectoryIterator( const boost::filesystem::path& directory_ph ) {
        directory_iterator dir_it(directory_ph);
	//DirectoryIterator(dir_it);
      }

      value_type operator* () const {
        return *current_dir_it;
      }

      DirectoryIterator& operator ++() {
        // ########################
        return *this;
      }

      DirectoryIterator operator ++(int) {
        DirectoryIterator tmp = *this;
	operator++();
	return tmp;
      }

      friend bool operator ==(const DirectoryIterator& lhs, const DirectoryIterator& rhs) {
        return lhs.current_dir_it == rhs.current_dir_it and lhs.dir_it_history == rhs.dir_it_history;
      }
    };

    OKLIB_DERIVED_UNEQUAL(DirectoryIterator);

  }

}

#endif

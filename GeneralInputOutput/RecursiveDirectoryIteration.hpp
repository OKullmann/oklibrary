// Matthew Henderson, 4.9.2005 (Swansea)

/*!
  \namespace GeneralInputOutput
  \brief Tools for handling input and output.
*/

/*!
  \file RecursiveDirectoryIteration.hpp
  \brief Recursive iteration over the files and subdirectories 
  contained within a filesystem directory.
*/

#ifndef RECURSIVEDIRECTORYITERATION_938ypbn3

#define RECURSIVEDIRECTORYITERATION_938ypbn3

#include <iostream>
#include <stack>
#include <iterator>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>

#include "DerivedRelations.hpp"
#include "AssociativeContainers.hpp"
                   
namespace OKlib {

  namespace GeneralInputOutput {

    /*!
      \class DirectoryIterator
      \brief Class which provides recursive input iterators over the non-directory files within a directory.
      \todo A more complete explanation is needed.
      \todo Handling of links needs to be customised (and documented).
      \todo Designing a concept; the domain of == allows comparison with the unique past-the-end iterator. The concept InputIterator should be refined InputIteratorDefault and MultiPassInputIterator (???).
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

      DirectoryIterator(const boost::filesystem::path& directory_ph) : current_dir_it(directory_ph) {
        while (current_dir_it != directory_iterator() and boost::filesystem::is_directory(*current_dir_it)) {
          dir_it_history.push(current_dir_it);
          current_dir_it = directory_iterator(*current_dir_it);
        } 
      }

      value_type operator* () const {
        return *current_dir_it;
      }

      DirectoryIterator& operator ++() {
        while (++current_dir_it == directory_iterator() and not dir_it_history.empty()) {
          current_dir_it = dir_it_history.top();
          dir_it_history.pop();
        }
        while (current_dir_it != directory_iterator() and boost::filesystem::is_directory(*current_dir_it)) {
          dir_it_history.push(current_dir_it);
          current_dir_it = directory_iterator(*current_dir_it);
        }
        return *this;
      }

    private :

      class post_increment_proxy {
        value_type x;
      public :
        post_increment_proxy(const value_type& x) :x(x) {}
        value_type operator*() const {
          return x;
        }
      };

    public :

      post_increment_proxy operator ++(int) {
        post_increment_proxy p(*current_dir_it);
	operator++();
        return p;
      }

      friend bool operator ==(const DirectoryIterator& lhs, const DirectoryIterator& rhs) {
        return lhs.current_dir_it == rhs.current_dir_it and lhs.dir_it_history == rhs.dir_it_history;
      }
    };

    OKLIB_DERIVED_UNEQUAL(DirectoryIterator)

  }

}

#endif

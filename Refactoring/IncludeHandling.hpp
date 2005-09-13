// Oliver Kullmann, 28.8.2005 (Swansea)

/*!
  \namespace OKlib::Refactoring
  \brief Tools for refactoring C++ code.
*/

/*!
  \file IncludeHandling.hpp
  \brief Refactoring of include directives.
*/

#ifndef INCLUDEHANDLING_9yhNbv

#define INCLUDEHANDLING_9yhNbv

#include <string>
#include <cassert>
//include <iostream>
#include <ostream>
#include <sstream>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>

#include "FilesystemTools.hpp"
#include "AssociativeContainers.hpp"


namespace OKlib {

  namespace Refactoring {

    enum Include_forms { system_header, source_code_header };

    /*!
      \class IncludeDirective
      \brief Representation of one include directive.
      \todo Enable element access.
      \todo Enable output streaming.
      \todo Can number_spaces_after_include be zero ?
    */
    
    template <class String = std::string>
    class IncludeDirective {
    public :
      typedef typename String::size_type size_type;
    private :
      String header_file_;
      size_type number_spaces_after_hash_, number_spaces_after_include_;
      Include_forms include_form_;
    public :
      IncludeDirective(const String& header_file, const size_type number_spaces_after_hash,  const size_type number_spaces_after_include, const Include_forms include_form) : header_file_(header_file), number_spaces_after_hash_(number_spaces_after_hash), number_spaces_after_include_(number_spaces_after_include), include_form_(include_form) {
        assert(not header_file.empty());
      }
      String header_file() const {
        return header_file_;
      }
      String& header_file() {
        return header_file_;
      }
      size_type number_spaces_after_hash() const { 
        return number_spaces_after_hash_; 
      }
      size_type& number_spaces_after_hash() { 
        return number_spaces_after_hash_; 
      }
      size_type number_spaces_after_include() const { 
        return number_spaces_after_include_; 
      }
      size_type& number_spaces_after_include() { 
        return number_spaces_after_include_; 
      }
      Include_forms include_form() const { 
        return include_form_; 
      }
      Include_forms& include_form() { 
        return include_form_; 
      }
    };


template<class T, class charT, class traits>
std::basic_ostream<charT, traits>&
operator<<(std::basic_ostream<charT, traits>& out, const IncludeDirective<T>& include_directive)
{
  std::basic_ostringstream<charT, traits> o_string_stream;
  o_string_stream.flags(out.flags());
  o_string_stream.imbue(out.getloc());
  o_string_stream.precision(out.precision());
  char enclosing_mark;
  Include_forms include_form = include_directive.include_form();
  if (include_form==system_header)
    enclosing_mark = '<';
  else
    enclosing_mark = '"';
  o_string_stream << '#' << "include" << enclosing_mark << include_directive.header_file << enclosing_mark;
  out << o_string_stream.str();
  return out;
}

    /*!
      \class Extract_include_directives
      \brief Enables read/write iterator access to the include directives from an istream.

      Extracting all include directives from an istream via a single pass forward iterator over IncludeDirective elements
      --- writing to these elements changes the underlying include directives. An ostream object contains the output. 
      Include directives resulting from macro replacement are not handled (since this depends on the context of the
      translation).

      \todo Design and implement. The algorithmic idea is to first check for lines of the form
      #include "*" or
      #include <*>
      (with spaces allowed according to  IncludeDirective); if found, then this is the value of the current 
      iterator, and the rest of the line is copied. Otherwise the line is scanned for an opening C-comment token:
      If found then the text is scanned for the closing token, and then as well as when the opening token is not found,
      that line is read until the end-of-line (and copied). Using ++ on the iterator writes the
      IncludeDirective to the ostream object.
    */

    class Extract_include_directives {};

    /*!
      \class Extend_include_directives
      \brief Functor for adding suitable prefixes to the include directives of an istream.

      The input is an istream and a form of an "associative prefix container". All include directives are extracted,
      checked whether they have a unique extension via the prefix container, if yes, they are extended, if not,
      a policy-controlled alternative action takes place.

      \todo Design and implement.
    */

    class Extend_include_directives {};

    /*!
      \class Extend_include_directives_Two_directories
      \brief Extend all include directives in files below some working directories with pathes relative to a working directory.

      Given a reference directory and a working directory, in all suitable files below the working
      directory suitable include directives including a file given via PATH are replaced by the similar
      include directive now including PREFIX/PATH, if, using the reference directory as base, this new
      path uniquely determines an existing file.

      \todo Design and implement. First the reference directory is recursively iterated (using a suitable
      iterator to be written an a new module "FileSystemTools") and the boost::path's obtained are
      put into an associative prefix container (regarding a path as a sequence, but in reverse
      order(!); the associative prefix container can be implemented in a new module "SearchDataStructures"
      as a simple wrapper around std::set, using lexicographical order and searching for an extension via
      std::set::lower_bound). Once that associative prefix container is filled, all suitable files under
      the working directory are handled by Extend_include_directives.
    */

    class Extend_include_directives_Two_directories {};

  }

}

#endif

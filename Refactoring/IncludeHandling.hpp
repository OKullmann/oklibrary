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
#include <ostream>
#include <vector>
#include <utility>
#include <boost/spirit.hpp>

#include "FilesystemTools.hpp"
#include "AssociativeContainers.hpp"


namespace OKlib {

  namespace Refactoring {

    enum Include_forms { system_header, source_code_header };

    /*!
      \class IncludeDirective
      \brief Representation of one include directive.
    */
    
    template <class String = std::string>
    class IncludeDirective {
    public :
      typedef String string_type;
      typedef typename String::size_type size_type;
      std::string opening() const {
        switch (include_form_) {
        case system_header :
          return "<";
        case source_code_header :
          return "\"";
        }
      }
      std::string closing() const {
        switch (include_form_) {
        case system_header :
          return ">";
        case source_code_header :
          return "\"";
        }

      }
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


template<class T>
    std::ostream& operator <<(std::ostream& out, const IncludeDirective<T>& include_directive)
    {
      return out << "#" << std::string(include_directive.number_spaces_after_hash(), ' ') << "include" << std::string(include_directive.number_spaces_after_include(), ' ') << include_directive.opening() << include_directive.header_file() << include_directive.closing();
    }


    /*!
      \class Extract_include_directives
      \brief Enables read/write iterator access to the include directives from an istream.

      Extracting all include directives from an istream via a single pass "input-output" iterator over IncludeDirective elements
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
      "Input-output iterators" means just the combination of input iterator and output iterator; it is allowed that two iterators are equal iff either both are dereferencable or both are past-the-end iterators --- this is what we do here (using perhaps a boolean data member for past-the-end indication).
    */
    template <class String>
    class Extract_include_directives {
    private:
      typedef IncludeDirective<String> IncludeDirective;
      typedef std::pair<IncludeDirective,String> Pair;
      typedef std::vector<Pair> Vector;
      String preface_;
      Vector vec_incl_text_pairs;

    // \todo Enable read/write element access.
    public:
      Extract_include_directives() {};
      String preface() const {
        return preface_;
      }
      String& preface() {
        return preface_;
      }
      IncludeDirective incl(int i) const {
        return vec_incl_text_pairs[i].first;
      }
      String following_text(int i) const {
        return vec_incl_text_pairs[i].second;
      }
      void push_back(Pair pair){
        vec_incl_text_pairs.push_back(pair);
      }

    };

    template<class String>
    std::ostream& operator <<(std::ostream& out, const Extract_include_directives<String>& extract_incldir)
    {
      // \todo Implementation.
    }

    template<class String>
    std::istream& operator >>(std::istream& in, const Extract_include_directives<String>& extract_incldir)
    {
      bool inside_multiline_comment=false;
      bool past_preface=false;
      String str_input_line;
      String current_following_text;
      while (getline(in,str_input_line)) {
	if (inside_multiline_comment) {
          String end_of_multiline_comment;
          String rest_of_line;
          boost::spirit::parse_info<> comment_result;
          // \todo Implement matching of end of multiline comment. This will involve a boost::spirit rule Comment which matches the end of a multiline comment and, as a semantic action, assigns this to end_of_multiline_comment and the rest of the line to rest_of_line. 
          // \todo Then, if matching successful, depending on whether past_preface is true or false the line is copied to either past_preface or current_following_text and the value of inside_multiline_comment is changed depending upon the status of comments in rest_of_line. (rest_of_line doesn't need to be parsed for include directive)
          // \todo Otherwise, it matching fails, current_following_text+=str_input_line (i.e.comment not closed on this line)
        }
	else {
          String result_whitespace;
          // IncludeDirective<String> result_include;
          String result_following_text;
	  // \todo Implement matching of include directive. This involves a boost::spirit rule Include which matches (whitespace)(#include<*>)(following_text) and has a semantic action which assigns whitespace to result_whitespace, the include directive to result_include and following_text to result_following_text.
          // \todo Then, if matching successful, append result_following_text to current_following_text, do extract_incldir.push_back(<result_include,current_following_text>) and reset current_following_text to the empty string.
          // \todo Update inside_multiline_comment.
	}
      }
    }
  

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

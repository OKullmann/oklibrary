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
#include <istream>
#include <vector>
#include <utility>
#include <memory>
#include <iterator>

#include <boost/spirit/core.hpp>
#include <boost/spirit/utility/confix.hpp>
#include <boost/spirit/utility/chset.hpp>
#include <boost/spirit/utility/regex.hpp>
#include <boost/spirit/utility/escape_char.hpp>

#include "FilesystemTools.hpp"
#include "AssociativeContainers.hpp"


namespace OKlib {

  namespace Refactoring {

    enum Include_forms { system_header, source_code_header, undefined_include_form };

    /*!
      \class IncludeDirective
      \brief Representation of one include directive.
      \todo A concept is needed.
      \todo Test it.
    */
    
    template <class String = std::string>
    class IncludeDirective {
    public :

      typedef String string_type;
      typedef typename string_type::size_type size_type;

      string_type opening() const {
        assert(include_form_ != undefined_include_form);
        switch (include_form_) {
        case system_header :
          return "<";
        case source_code_header :
          return "\"";
        }
      }
      string_type closing() const {
        assert(include_form_ != undefined_include_form);
        switch (include_form_) {
        case system_header :
          return ">";
        case source_code_header :
          return "\"";
        }

      }
    private :
      string_type header_file_;
      size_type number_spaces_after_hash_, number_spaces_after_include_;
      Include_forms include_form_;

    public :
      IncludeDirective() : header_file_(""), number_spaces_after_hash_(0), number_spaces_after_include_(0), include_form_(undefined_include_form) {}
      IncludeDirective(const string_type& header_file, const size_type number_spaces_after_hash,  const size_type number_spaces_after_include, const Include_forms include_form) : header_file_(header_file), number_spaces_after_hash_(number_spaces_after_hash), number_spaces_after_include_(number_spaces_after_include), include_form_(include_form) {
        assert(not header_file_.empty());
      }

      const string_type header_file() const {
        return header_file_;
      }
      string_type& header_file() {
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
      const Include_forms include_form() const { 
        return include_form_; 
      }
      Include_forms& include_form() { 
        return include_form_; 
      }
    };
    
    template<class T>
    inline std::ostream& operator <<(std::ostream& out, const IncludeDirective<T>& include_directive)
    {
      return out << "#" << std::string(include_directive.number_spaces_after_hash(), ' ') << "include" << std::string(include_directive.number_spaces_after_include(), ' ') << include_directive.opening() << include_directive.header_file() << include_directive.closing();
    }

    template<class T>
    inline bool operator ==(const IncludeDirective<T>& lhs, const IncludeDirective<T>& rhs) {
      return lhs.header_file() == rhs.header_file() and lhs.number_spaces_after_hash() == rhs.number_spaces_after_hash() and lhs.number_spaces_after_include() == rhs.number_spaces_after_include() and lhs.include_form() == rhs.include_form();
    }

    
    // ####################################################################################

    /*!
      \class ProgramRepresentationIncludes
      \brief Class for representing the include-directives within a program
      \todo A concept is needed
      \todo Test it.
    */

    template <class charT = char, class traits = std::char_traits<charT>, class Allocator = std::allocator<charT> >
    class ProgramRepresentationIncludes {
    public :
      typedef std::basic_string<charT, traits, Allocator> string_type;
      typedef std::vector<std::pair<IncludeDirective<string_type>, string_type> > container_type;
      typedef typename container_type::value_type value_type;
      typedef typename container_type::iterator iterator;
      typedef typename container_type::const_iterator const_iterator;

      string_type prefix;
      container_type include_directives_with_context;

      ProgramRepresentationIncludes& operator() (const string_type& s) {
        prefix = s;
        return *this;
      }
      ProgramRepresentationIncludes& operator() (const value_type& v) {
        include_directives_with_context.push_back(v);
        return *this;
      }

      void clear() {
        prefix.clear(); include_directives_with_context.clear();
      }

      friend std::ostream& operator <<(std::ostream& out, const ProgramRepresentationIncludes& pr) {
        out << pr.prefix;
        const const_iterator& end(pr.include_directives_with_context.end());
        for (const_iterator i = pr.include_directives_with_context.begin(); i != end; ++i)
          out << i -> first << i -> second;
        return out;
      }

      friend bool operator ==(const ProgramRepresentationIncludes& lhs, const ProgramRepresentationIncludes& rhs) {
        return lhs.prefix == rhs.prefix and lhs.include_directives_with_context == rhs.include_directives_with_context;
      }

    };


    // ####################################################################################
    
    /*!
      \class IncludeParsingGrammar
      \brief Defines grammar of a C++ program as far as is needed for parsing of include directives in a file.
      \todo Write istream >> ProgramRepresentationIncludes.
      \todo Yet we use a fixed ProgramRepresentationIncludes form (with fixed character type), ignoring the scanner type (the template parameter of the nested class definition). A more perfect solution would create a new parse-function which then instantiates the IncludeParsingGrammar and ProgramRepresentationIncludes accordingly to the character type actually used.
      \todo A concept is needed.
    */

    struct IncludeParsingGrammar : public boost::spirit::grammar<IncludeParsingGrammar> {
      typedef ProgramRepresentationIncludes<> program_representation_type;

      program_representation_type& pr;
      
      typedef program_representation_type::string_type string_type;
      typedef IncludeDirective<string_type> include_directive_type;
      mutable include_directive_type id;
      mutable string_type s;

      IncludeParsingGrammar(program_representation_type& pr) : pr(pr), blanks_hash(id), blanks_include(id), header_file(id), include_form_system(id, system_header), include_form_source(id, source_code_header), other(pr), context(s), push_back(pr, id, s) {}

      struct action_blanks_hash {
        include_directive_type& id;
        action_blanks_hash(include_directive_type& id) : id(id) {}
        template <typename Iterator>
        void operator() (const Iterator& begin, const Iterator& end) const {
          id.number_spaces_after_hash() = std::distance(begin, end);
        }
      };
      action_blanks_hash blanks_hash;
      

      struct action_blanks_include {
        include_directive_type& id;
        action_blanks_include(include_directive_type& id) : id(id) {}
        template <typename Iterator>
        void operator() (const Iterator& begin, const Iterator& end) const {
          id.number_spaces_after_include() = std::distance(begin, end);
        }
      };
      action_blanks_include blanks_include;

      struct action_header_file {
        include_directive_type& id;
        action_header_file(include_directive_type& id) : id(id) {}
        template <typename Iterator>
        void operator() (const Iterator& begin, const Iterator& end) const {
          id.header_file().assign(begin, end);
        }
      };
      action_header_file header_file;

      struct action_include_form {
        include_directive_type& id;
        Include_forms inc_f;
        action_include_form(include_directive_type& id, Include_forms inc_f) : id(id), inc_f(inc_f) {}
        template <typename Iterator>
        void operator() (const Iterator&, const Iterator&) const {
          id.include_form() = inc_f;
         }
      };
      action_include_form include_form_system, include_form_source;

      struct action_other {
        program_representation_type& pr;
        action_other(program_representation_type& pr) : pr(pr) {}
        template <typename Iterator>
        void operator() (const Iterator& begin, const Iterator& end) const {
          pr.prefix.assign(begin, end);
        }
      };
      action_other other;
      
      struct action_context {
        string_type& s;
        action_context(string_type& s) : s(s) {}
        template <typename Iterator>
        void operator() (const Iterator& begin, const Iterator& end) const {
          s.assign(begin, end);
        }
      };
      action_context context;

      struct action_push_back {
        program_representation_type& pr;
        include_directive_type& id;
        string_type& s;
        action_push_back(program_representation_type& pr, include_directive_type& id, string_type& s) : pr(pr), id(id), s(s) {}
        template <typename Iterator>
        void operator() (const Iterator&, const Iterator&) const {
          pr.include_directives_with_context.push_back(std::make_pair(id, s));
        }
      };
      action_push_back push_back;

      template <typename ScannerT>
      struct definition {

        typedef typename boost::spirit::rule<ScannerT> rule;
        rule program;
        rule include_directive_begin, include_directive_system, include_directive_source, include_directive, comment, string, other_statements;

        definition(const IncludeParsingGrammar& self) {

          include_directive_begin = boost::spirit::regex_p("^#") >> (*boost::spirit::blank_p)[self.blanks_hash] >> boost::spirit::str_p("include") >> (*boost::spirit::blank_p)[self.blanks_include];

          include_directive_system = include_directive_begin >> boost::spirit::confix_p("<", (*boost::spirit::anychar_p)[self.header_file], ">");
          include_directive_source = include_directive_begin >> boost::spirit::confix_p("\"",(*boost::spirit::anychar_p)[self.header_file], "\"");

          include_directive = include_directive_system[self.include_form_system] | include_directive_source[self.include_form_source];
          
          comment = boost::spirit::comment_p("/*", "*/") | boost::spirit::comment_p("//");

          string = boost::spirit::confix_p("\"", *boost::spirit::c_escape_ch_p, "\"");
          
          other_statements = *(comment | string | (boost::spirit::anychar_p - include_directive));
          
          program = other_statements[self.other] >> *(include_directive >> other_statements[self.context])[self.push_back];
        }

        const rule& start() const { return program; }
      };
    };
    
    // ####################################################################################
    

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

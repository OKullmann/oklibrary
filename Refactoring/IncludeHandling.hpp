// Oliver Kullmann, 28.8.2005 (Swansea)

/*!
  \namespace OKlib::Refactoring
  \brief Tools for refactoring C++ code.
  \todo Investigating existing tools for parsing and refactoring C++ code.
  Asking on the boost e-mail list.
*/

/*!
  \file IncludeHandling.hpp
  \brief Refactoring of include directives.
  \todo Notifying the boost e-mail list about the problem with the
  filesystem library (see below).

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
#include <ios>
#include <stdexcept>

#include <boost/spirit/core.hpp>
#include <boost/spirit/utility/confix.hpp>
#include <boost/spirit/utility/chset.hpp>
#include <boost/spirit/utility/regex.hpp>
#include <boost/spirit/utility/escape_char.hpp>
#include <boost/spirit/iterator/multi_pass.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

#include <boost/range/const_iterator.hpp> // Fix for errenous Boost library filesystem ##################################################################
#include <boost/range/size_type.hpp> // Fix for errenous Boost library filesystem ##################################################################

#include "RecursiveDirectoryIteration.hpp"
#include "AssociativeContainers.hpp"
#include "DerivedRelations.hpp"


// Fix for errenous Boost library filesystem ##########################################################################

namespace boost {

  template <>
  struct range_const_iterator<boost::filesystem::path> {
    typedef boost::filesystem::path::iterator type;
  };
  template <>
  struct range_size<boost::filesystem::path> {
    typedef boost::filesystem::path::iterator::difference_type type;
  };

  namespace filesystem {

    inline boost::range_size<boost::filesystem::path>::type boost_range_size(const boost::filesystem::path& p) {

    }

  }

}

// ##########################################################################


namespace OKlib {

  namespace Refactoring {

    enum Include_forms { system_header, source_code_header, undefined_include_form };

    /*!
      \class IncludeDirective
      \brief Representation of one include directive.
      \todo A concept is needed.
      \todo Test inequality operator.
    */
    
    template <class String = std::string>
    class IncludeDirective {
    public :

      typedef String string_type;
      typedef typename string_type::size_type size_type;

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
    };
    
    template<class String>
    inline std::ostream& operator <<(std::ostream& out, const IncludeDirective<String>& include_directive)
    {
      return out << "#" << std::string(include_directive.number_spaces_after_hash(), ' ') << "include" << std::string(include_directive.number_spaces_after_include(), ' ') << include_directive.opening() << include_directive.header_file() << include_directive.closing();
    }

    template<class String>
    inline bool operator ==(const IncludeDirective<String>& lhs, const IncludeDirective<String>& rhs) {
      return lhs.header_file() == rhs.header_file() and lhs.number_spaces_after_hash() == rhs.number_spaces_after_hash() and lhs.number_spaces_after_include() == rhs.number_spaces_after_include() and lhs.include_form() == rhs.include_form();
    }

    template<class String>
    OKLIB_DERIVED_UNEQUAL(IncludeDirective<String>)
    
    // ####################################################################################

    /*!
      \class ProgramRepresentationIncludes
      \brief Class for representing the include-directives within a program
      \todo std::pair<IncludeDirective<string_type>, string_type> should be a typedef.
      \todo An extended explanation is needed.
      \todo A concept is needed.
      \todo Test inequality operator. (OK: ?? Always test equality and inequality together, applying
      the appropriate generic test facilities from the concept definitions.)
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

      friend OKLIB_DERIVED_UNEQUAL(ProgramRepresentationIncludes)

    };



    // ####################################################################################
    
    /*!
      \class IncludeParsingGrammar
      \brief Defines grammar of a C++ program as far as necessary for parsing include directives in a file.
      \todo Extended explanation.
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

          include_directive_begin = boost::spirit::ch_p("#") >> (*boost::spirit::blank_p)[self.blanks_hash] >> boost::spirit::str_p("include") >> (*boost::spirit::blank_p)[self.blanks_include];

          include_directive_system = include_directive_begin >> boost::spirit::confix_p("<", (*boost::spirit::anychar_p)[self.header_file], ">");
          include_directive_source = include_directive_begin >> boost::spirit::confix_p("\"",(*boost::spirit::anychar_p)[self.header_file], "\"");

          include_directive = include_directive_system[self.include_form_system] | include_directive_source[self.include_form_source];
          
          comment = boost::spirit::comment_p("/*", "*/") | boost::spirit::comment_p("//");

          string = boost::spirit::confix_p("\"", *boost::spirit::c_escape_ch_p, "\"");
          
          other_statements = *(comment | string | (boost::spirit::anychar_p - (boost::spirit::eol_p >> boost::spirit::ch_p("#")))) >> !boost::spirit::eol_p;
          
          program = ((include_directive >> other_statements[self.context])[self.push_back] | other_statements[self.other]) >> *(include_directive >> other_statements[self.context])[self.push_back];
        }

        const rule& start() const { return program; }
      };
    };

    // ####################################################################################

    /*!
      \class StreamExtractor_by_istream_iterator
      \brief Functor class for extracting include directives from input streams, using
      multipass-iterator-wrappers around istream-iterators.
      \todo Extended explanation should mention the other implementation.
      \todo Management of stream-formatflags-resources should be handled by (RAII) object.
      \todo Write a test for it.
      \todo Create a concept.
    */

    struct StreamExtractor_by_istream_iterator {

      std::istream& in;
      const bool skipws;

      explicit StreamExtractor_by_istream_iterator(std::istream& in) : in(in), skipws(in.flags() & std::ios_base::skipws) {
        if (skipws)
          in.unsetf(std::ios_base::skipws);
      }
      ~StreamExtractor_by_istream_iterator() {
        if (skipws)
          in.setf(std::ios_base::skipws);
      }
        
      template <typename charT, class traits, class Allocator>
      std::istream& operator() (ProgramRepresentationIncludes<charT, traits, Allocator>& pr) {
        typedef boost::spirit::multi_pass<std::istream_iterator<charT> > iterator_t;
        const iterator_t& begin(boost::spirit::make_multi_pass(std::istream_iterator<charT>(in)));
        const iterator_t& end(boost::spirit::make_multi_pass(std::istream_iterator<charT>()));
        IncludeParsingGrammar g(pr);
        boost::spirit::parse(begin, end, g);
        return in;
      }

    };
    
    /*!
      \class StreamExtractor_by_copy
      \brief Functor class for extracting include directives from input streams,
      copying the stream content.
      \todo Extended explanation as above.
    */

    struct StreamExtractor_by_copy {

      std::istream& in;

      explicit StreamExtractor_by_copy(std::istream& in) : in(in) {}
        
      template <class charT, class traits, class Allocator>
      std::istream& operator() (ProgramRepresentationIncludes<charT, traits, Allocator>& pr) {
        std::ostringstream input;
        input << in.rdbuf();
        std::string input_string(input.str());
        std::string::iterator begin(input_string.begin());
        std::string::iterator end(input_string.end());
        IncludeParsingGrammar g(pr);
        boost::spirit::parse(begin, end, g);
        return in;
      }

    };
    
    // ######################

    // ToDo: Write Doxygen comment here
    // ToDo: Explore which implementation is more efficient (once the complexity system is there).

    template <class charT, class traits, class Allocator>
    std::istream& operator >>(std::istream& in, ProgramRepresentationIncludes<charT, traits, Allocator>& pr) {
      return (StreamExtractor_by_istream_iterator(in))(pr);
    }

    // ####################################################################################

    struct NonUniqueExtension: std::runtime_error {
      NonUniqueExtension(const std::string& m) : runtime_error(m) {}
    };
    struct NoExtension: std::runtime_error {
      NoExtension(const std::string& m) : runtime_error(m) {}
    };

    struct ThrowIfNonUnique {
      template <class AssociativePrefixContainer, typename Iterator, class String>
      static String new_header_file(const AssociativePrefixContainer&, Iterator, const String& header_file) {
        throw NonUniqueExtension("OKlib::Refactoring::new_header_file(const AssociativePrefixContainer&, Iterator, const String&):\n header file " + header_file + " has an ambiguous extension");
      } 
    };

    /*!
      \class Extend_include_directives
      \brief Functor for adding suitable prefixes to the include directives of an istream.

      The input is an istream and a form of an "associative prefix container". All include directives are extracted,
      checked whether they have a unique extension via the prefix container, if yes, they are extended (in
      the representation of the input, not in the input itself), if not, a policy-controlled alternative action
      takes place.
      \todo Write a second operator(), taking a sequence (range) as input, which
      is converted by a StreamHandlingPolicy to an internal stream object, which
      is used by the current operator(), and then we pass the pr-object
      to the policy.
      \todo Update the design, create a concept.
      \todo Update the above explanation.
    */

    template <class UniquenessPolicy = ThrowIfNonUnique>
    class Extend_include_directives {
    public:
      typedef OKlib::SearchDataStructures::AssociativePrefixContainer<boost::filesystem::path> APC;
      typedef ProgramRepresentationIncludes<>::container_type container_type;
      typedef ProgramRepresentationIncludes<>::iterator iterator;
      typedef ProgramRepresentationIncludes<>::value_type value_type;
      typedef ProgramRepresentationIncludes<>::string_type string_type;
      typedef IncludeDirective<string_type> id_type;

      ProgramRepresentationIncludes<> pr;
      const APC& prefix_container;
      const typename APC::const_iterator& end_prefix_container;

      Extend_include_directives (const APC& prefix_container) : prefix_container(prefix_container), end_prefix_container(prefix_container.end()) {}

      void operator() (std::istream& input) {
        
        input >> pr;
        container_type& id_w_c_container(pr.include_directives_with_context);
        const iterator& end(id_w_c_container.end());
        for (iterator i=id_w_c_container.begin(); i!=end; ++i) {
          id_type& id(i -> first);
          const string_type& header_file(id.header_file());
          typedef APC::checked_iterator_type checked_iterator_type;
          const checked_iterator_type& extension(prefix_container.first_extension_uniqueness_checked(header_file));
          if (extension.first == end_prefix_container)
            throw NoExtension("OKlib::Refactoring::Extend_include_directives<UniquenessPolicy>::operator ()(std::istream& input):\n header file " + header_file + " has no extension");
          const string_type new_header_file((extension.second) ? extension.first -> string() : UniquenessPolicy::new_header_file(id_w_c_container, extension.first, header_file));
          id.header_file() = new_header_file;
        }
      }
    };

    // ####################################################################################

    /*!
      \class Extend_include_directives_Two_ranges
    */

    template <class Range, class Istream, class UniquenessPolicy = ThrowIfNonUnique>
    class Extend_include_directives_Two_ranges {
    public:

      typedef OKlib::SearchDataStructures::AssociativePrefixContainer<Range> APC;
      typedef typename boost::range_const_iterator<Range>::type iterator;
 
      const Range& ref_range;
      const Range& work_range;
      APC prefix_container;
     
      typedef Extend_include_directives<UniquenessPolicy> extend_include_directive_type;

      Extend_include_directives_Two_ranges(const Range& ref_range, const Range& work_range) : ref_range(ref_range), work_range(work_range) {

        prefix_container(ref_range);
        
        extend_include_directive_type eid(prefix_container);
        
        for(iterator begin(boost::begin(work_range)); begin!=boost::end(work_range); ++begin) {
          Istream working_stream(*begin);
          eid(working_stream);
          working_stream << eid.pr;

        }
      }

    };


    // ####################################################################################

    /*!
      \class Extend_include_directives_Two_directories
      \brief Extend all include directives in files below some working directories with pathes relative to a working directory.

      Given a reference directory and a working directory, in all suitable files below the working
      directory suitable include directives including a file given via PATH are replaced by the similar
      include directive now including PREFIX/PATH, if, using the reference directory as base, this new
      path uniquely determines an existing file.
      \todo Make prefix_container a data-member (initialisation should happen via appropriate constructors!).
      \todo Likely it is better to have the constructor taking two *iterator ranges* instead of taking the paths
      to the reference and the working directory (this design is more general). The "Two directories" in the
      name should then also be replaced by something suitable (and the two-directories construction
      should go to a helper-class/function).
      \todo Update the explanation.
    */

    template <class Path = boost::filesystem::path, class UniquenessPolicy = ThrowIfNonUnique>
    class Extend_include_directives_Two_directories {
    public:
       const Path& ref_dir;
       const Path& work_dir;

       Extend_include_directives_Two_directories(const Path& ref_dir,const Path& work_dir) : ref_dir(ref_dir), work_dir(work_dir) {

         Extend_include_directives_Two_ranges<Path,boost::filesystem::fstream,UniquenessPolicy>(ref_dir,work_dir);

       }

    };



  }

}

#endif

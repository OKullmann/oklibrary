// Oliver Kullmann, 28.8.2005 (Swansea)

/*!
  \file IncludeHandling_Tests.hpp
  \brief Tests regarding include statement handling.
*/

#ifndef INCLUDEHANDLINGTESTS_77665r

#define INCLUDEHANDLINGTESTS_77665r

#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>

#include <boost/spirit.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/tuple/tuple.hpp>

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"
#include "ParserBase_Tests.hpp"

namespace OKlib {

  namespace Refactoring {

    /*!
      \class Test_IncludeDirective
      \brief Testing classes representing one include directive.
      \todo Test all functionality.
    */

    template <template <class String> class Include_Directive>
    class Test_IncludeDirective : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_IncludeDirective test_type;
      Test_IncludeDirective() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        {
          typedef Include_Directive<std::string> IncludeDirective;
	  int spaces_after_hash = 0;
	  int spaces_after_include = 1;
          std::string header = "iostream";
	  Include_forms include_form = system_header;
          IncludeDirective include_directive(header,spaces_after_hash,spaces_after_include,include_form);
          // \todo Test output streaming of IncludeDirective
        }
      }
    };

    // ------------------------------------------------------------------

    /*!
      \class Test_IncludeParsingGrammar
      \brief Testing grammars for parsing source code units in order to extract include directives.
      \todo Using Boost.assignment library.
      \todo More systematic testing.
    */

    template <class IncludeParsingGrammar>
    class Test_IncludeParsingGrammar : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_IncludeParsingGrammar test_type;
      Test_IncludeParsingGrammar() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        typedef typename IncludeParsingGrammar::program_representation_type pr_t;
        typedef typename pr_t::string_type string_type;
        typedef typename pr_t::value_type v_t;
        typedef IncludeDirective<string_type> d_t;
        typedef boost::tuple<string_type, OKlib::Parser::Matching_possibilities, pr_t> el_t;
        typedef std::vector<el_t> test_vector_type;
        test_vector_type test_vector;
        using OKlib::Parser::match_full;
        using OKlib::Parser::match_not_full;

        using namespace boost::assign;
        test_vector +=
          el_t("", match_full, pr_t()),
          el_t("\"", match_full, pr_t()("\"")),
          el_t("\\*", match_full, pr_t()("\\*")),
          el_t("#include <abc>", match_full, pr_t()(v_t(d_t("abc", 0, 1, system_header), ""))),
          el_t("zzz\n#include <abc>xxx", match_full, pr_t()("zzz\n")(v_t(d_t("abc", 0, 1, system_header), "xxx"))),
          el_t("#include \"abc\"", match_full, pr_t()(v_t(d_t("abc", 0, 1, source_code_header), "")));

        typedef typename test_vector_type::const_iterator iterator;
        const iterator& end(test_vector.end());
        for (iterator i = test_vector.begin(); i != end; ++i) {

          pr_t pr;
          IncludeParsingGrammar g(pr);

          const el_t& el(*i);
          const std::string& el_s(el.template get<0>());
          const OKlib::Parser::Matching_possibilities& el_m(el.template get<1>());
          const pr_t& el_p(el.template get<2>());

          switch (el_m) {
          case match_full :
            if (not parse(el_s.c_str(), g).full)
              OKLIB_THROW("String \"" + el_s + "\" was not accepted");
            break;
          case match_not_full :
            if (parse(el_s.c_str(), g).full)
              OKLIB_THROW("String \"" + el_s + "\" was accepted");
            break;
          }

          OKLIB_TEST_EQUAL(pr, el_p);
        }
      }
    };

    /*!
      \class Test_ProgramRepresentationIncludes
      \brief Testing classes for representing the include-directives within a program..
      \todo Test all functionality.
    */

    template <template <class charT = char, class traits = std::char_traits<charT>, class Allocator = std::allocator<charT> > class Program_Representation_Includes>
    class Test_ProgramRepresentationIncludes : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ProgramRepresentationIncludes test_type;
      Test_ProgramRepresentationIncludes() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        { // ostream tests
          typedef Program_Representation_Includes<> pr_type;
          typedef std::string string_type;
          typedef IncludeDirective<string_type> id_type;
          typedef id_type::size_type size_type;
          typedef std::vector<std::pair<id_type, string_type> > container_type;
          typedef typename container_type::value_type value_type;

          typedef boost::tuple<string_type, string_type, size_type, size_type, Include_forms,string_type,string_type> el_t;

          typedef std::vector<el_t> test_vector_type;
          test_vector_type test_vector;

          using namespace boost::assign;

          test_vector +=
            el_t("","iostream",0,1,system_header,"","#include <iostream>"),
            el_t("","header.hpp",0,1,source_code_header,"","#include \"header.hpp\""),
            el_t("This is the preface\n","iostream",0,1,system_header,"This is some code after the include directive","This is the preface\n#include <iostream>This is some code after the include directive");

          typedef typename test_vector_type::const_iterator iterator;
          const iterator& end(test_vector.end());

          for (iterator i = test_vector.begin(); i != end; ++i) {
            pr_type program;
            const el_t& el(*i);
            string_type preface(el.template get<0>());
            string_type header(el.template get<1>());
            size_type spaces_after_hash(el.template get<2>());
            size_type spaces_after_include(el.template get<3>());
            Include_forms include_form(el.template get<4>());
            string_type following_text(el.template get<5>());
            id_type include(header,spaces_after_hash,spaces_after_include,include_form);
            value_type include_with_context(std::make_pair(include,following_text));

            program(preface); program(include_with_context);

            std::ostringstream output;
            output << program;
            std::string output_string = output.str();

            std::string expected_output(el.template get<6>());

            if (not (output_string==expected_output))
              OKLIB_THROW("output != expected output");
          }

        }
      }
    };

  }

}

#endif

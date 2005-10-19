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

  }

}

#endif

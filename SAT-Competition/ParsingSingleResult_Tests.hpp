// Oliver Kullmann, 13.5.2005 (Swansea)

#ifndef PARSINGSINGLERESULTTESTS_k33wQQl

#define PARSINGSINGLERESULTTESTS_k33wQQl

#include <sstream>
#include <string>
#include <cassert> // #####################

#include <boost/spirit/core.hpp>

#include "TestBaseClass.hpp"

#include "SingleResult.hpp"
#include "ParsingSingleResult.hpp"

namespace OKlib {

  namespace SATCompetition {

    class Test_ParserResultElement_SuperSeries : ::OKlib::TestSystem::TestBase {
      void perform_test_trivial() {
        SuperSeries s;
        ParserResultElement<SuperSeries> p(s);
        std::stringstream str;
        const std::string test1 = "abc123ABC";
        str << test1;
        assert(boost::spirit::parse(str.str().c_str(), p.parser()).full);
        assert(s.name() == test1);
      }
    };

  }

}

#endif

// Oliver Kullmann, 13.5.2005 (Swansea)

#ifndef PARSINGSINGLERESULTTESTS_k33wQQl

#define PARSINGSINGLERESULTTESTS_k33wQQl

#include <sstream>
#include <string>

#include <boost/spirit/core.hpp>

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

#include "SingleResult.hpp"
#include "ParsingSingleResult.hpp"

namespace OKlib {

  namespace SATCompetition {

    class Test_ParserResultElement_SuperSeries : ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_SuperSeries test_type;
      Test_ParserResultElement_SuperSeries() {
	insert(this);
      }
    private :
      void perform_test_trivial() {
        SuperSeries s;
        ParserResultElement<SuperSeries> p(s);
        std::stringstream str;
        const std::string test1 = "abc123ABC";
        str << test1;
	if (not boost::spirit::parse(str.str().c_str(), p.parser()).full)
	  throw ::OKlib::TestSystem::TestException("String " + test1 + " was not accepted.").add(OKLIB_TESTDESCRIPTION);
	// ToDo: Using module Messages
        if(s.name() != test1)
	  throw ::OKlib::TestSystem::TestException("Resulting name is " + s.name() + ", and not " + test1).add(OKLIB_TESTDESCRIPTION);
      }
    };

  }

}

#endif

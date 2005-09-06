// Oliver Kullmann, 28.8.2005 (Swansea)

#ifndef INCLUDEHANDLINGTESTOBJECTS_plmJJk

#define INCLUDEHANDLINGTESTOBJECTS_plmJJk

#include "IncludeHandling.hpp"
#include "IncludeHandling_Tests.hpp"

namespace OKlib {

  namespace Refactoring {

    const Include_forms a = system_header;
    const std::string::size_type b = 5;
    const std::string::size_type c = 0;
    const std::string d = "Hello";
    IncludeDirective<std::string> e(d,c,b,a);
    const Include_forms f = source_code_header;
    const std::string::size_type g = 7;
    const std::string::size_type h = 1;
    const std::string i = "Goodbye";
    IncludeDirective<std::string> j(i,h,g,f);
   
  }

}

#endif

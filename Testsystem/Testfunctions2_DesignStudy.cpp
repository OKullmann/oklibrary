// Oliver Kullmann, 8.1.2006 (Swansea)

#include "TestBaseClass_DesignStudy.hpp"

#include "Testfunctions2_DesignStudy.hpp"

namespace {

  OKlib::TestSystem::RunTest test1(new OKlib::TestSystem::Test2);
  OKlib::TestSystem::RunTest test3(new OKlib::TestSystem::Test3);

}

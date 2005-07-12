// Oliver Kullmann, 11.4.2005 (Swansea)

// Temporary solution; to be displaced by the automatic system

#include <iostream>

#include "TestBaseClass.hpp"

#include "Backup_Testobjects.hpp"
#include "Command_Caller_Testobjects.hpp"
#include "Info_Holder_Testobjects.hpp"
#include "Path_Loader_Testobjects.hpp"

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
}


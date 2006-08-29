// Oliver Kullmann, 11.4.2005 (Swansea)

// Temporary solution; to be displaced by the automatic system

#include <iostream>

#include <Transitional/TestSystem/TestBaseClass.hpp>

#include <Transitional/Backup/Backup_Testobjects.hpp>
#include <Transitional/Backup/Command_Caller_Testobjects.hpp>
#include <Transitional/Backup/Info_Holder_Testobjects.hpp>
#include <Transitional/Backup/Path_Loader_Testobjects.hpp>

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
}


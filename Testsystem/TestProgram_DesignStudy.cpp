// Oliver Kullmann, 8.1.2006

// ToDo: Synchronisation of streams (and copying of output) (yet we get undefined behaviour, wen for example two file names are identical).


#include <iostream>
#include <cstdlib>
#include <memory>
#include <fstream>
#include <string>

#include "IOStreamSinks.hpp"

#include "TestBaseClass_DesignStudy.hpp"

std::ostream& stream(const std::string& parameter, std::ofstream& fstream) {
  static OKlib::GeneralInputOutput::NullStream null_stream;
  if (parameter == "std::cerr")
    return std::cerr;
  else if (parameter == "std::cout")
    return std::cout;
  else if (parameter == "NULL")
    return null_stream;
  else {
    fstream.open(parameter.c_str());
    return fstream;
  }
}

int main(const int argc, const char* const argv[]) {
  if (argc != 5) {
    std::cerr << "Exactly four parameter expected (the test level, the error stream, the message stream, the log stream).\n";
    return EXIT_FAILURE;
  }

  std::auto_ptr<const ::OKlib::TestSystem::TestLevel> level(::OKlib::TestSystem::test_level(argv[1]));
  const std::string error_s(argv[2]);
  const std::string messages_s(argv[3]);
  const std::string log_s(argv[4]);
  
  std::ofstream error_f;
  std::ostream& error = ::stream(error_s, error_f);
  if (not error) {
    std::cerr << "Cannot open error stream " << error_s << "\n";
    return EXIT_FAILURE;
  }
  std::ofstream messages_f;
  std::ostream& messages = ::stream(messages_s, messages_f);
  if (not messages) {
    std::cerr << "Cannot open messages stream " << messages_s << "\n";
    return EXIT_FAILURE;
  }
  std::ofstream log_f;
  std::ostream& log = ::stream(log_s, log_f);
  if (not log) {
    std::cerr << "Cannot open log stream " << log_s << "\n";
    return EXIT_FAILURE;
  }

  OKlib::TestSystem::RunTest::run_tests(error, messages, log, level.get());
}

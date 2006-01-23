// Oliver Kullmann, 8.1.2006

/*!
  \file TestProgram_DesignStudy.cpp
  \brief Study for the one test program, which is linked appropriately with
  the .o-files for the testobjects.
  \todo Using module Messages.
*/

#include <iostream>
#include <cstdlib>
#include <memory>
#include <string>
#include <set>
#include <vector>
#include <functional>
#include <cassert>

#include <boost/algorithm/string/split.hpp>
#include <boost/range/iterator_range.hpp>

#include "Multiplexer.hpp"

#include "TestBaseClass_DesignStudy.hpp"

struct is_separator : std::unary_function<char, bool> {
  const char sep;
  is_separator(const char sep) : sep(sep) {}
  bool operator ()(const char c) const { return c == sep; }
};

int main(const int argc, const char* const argv[]) {

  const unsigned int number_streams = 3;
  const unsigned int additional_leading_parameters = 1;

  if (argc != 1 + additional_leading_parameters + number_streams) {
    std::cerr << "Exactly four parameter expected (the test level, the error stream, the message stream, the log stream).\nStream descriptors: cerr, cout, NULL, ofstream-app=, ofstream-w=, ostringstream=; joining streams with \'|\'.";
    return EXIT_FAILURE;
  }

  std::auto_ptr<const ::OKlib::TestSystem::TestLevel> level(::OKlib::TestSystem::test_level(argv[1]));

  ::OKlib::GeneralInputOutput::OStreamMultiplexer multiplexer;
  {
    const char separator = '|';
    typedef std::set< ::OKlib::GeneralInputOutput::OStreamDescriptor> descriptor_set_t;
    descriptor_set_t array_descriptors[number_streams];
    for (unsigned int i = 0; i < number_streams; ++i) {
      std::vector<std::string> tokens;
      const std::string parameter(argv[1+additional_leading_parameters+i]);
      if (not parameter.empty())
        boost::algorithm::split(tokens, argv[1+additional_leading_parameters+i], is_separator(separator));
      array_descriptors[i].insert(tokens.begin(), tokens.end());
    }
    multiplexer.assign(boost::make_iterator_range(array_descriptors, array_descriptors+number_streams));
  }

  assert(multiplexer.fostream_vector.size() == number_streams);
  OKlib::TestSystem::RunTest::run_tests(multiplexer.fostream_vector[0], multiplexer.fostream_vector[1], multiplexer.fostream_vector[2], level.get());
}

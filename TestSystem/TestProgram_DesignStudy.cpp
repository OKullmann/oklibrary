// Oliver Kullmann, 8.1.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TestSystem/TestProgram_DesignStudy.cpp
  \brief The one test program, which is linked appropriately with the .o-files for the testobjects.

  Reads from the command line the test levels and the output streams, prepares the output streams,
  and then runs the tests.
*/

#include <iostream>
#include <cstdlib>
#include <string>
#include <set>
#include <vector>
#include <functional>
#include <cassert>

#include <boost/algorithm/string/split.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/lexical_cast.hpp>

#include <OKlib/Programming/InputOutput/Multiplexer.hpp>
#include <OKlib/Programming/Messages/MessagesMain.hpp>
#include <OKlib/Programming/Messages/LanguageNames.hpp>

#include <OKlib/TestSystem/RunTest_DesignStudy.hpp>

struct is_separator : std::unary_function<char, bool> {
  const char sep;
  is_separator(const char sep) : sep(sep) {}
  bool operator ()(const char c) const { return c == sep; }
};

int main(const int argc, const char* const argv[]) {

  // Checking the input:

  const unsigned int number_streams = 3;
  const unsigned int additional_leading_parameters = 1;
  const unsigned int number_parameters =  1 + additional_leading_parameters + 3 * number_streams;

  if ((unsigned int)argc != number_parameters) {
    std::cerr << "Exactly ten parameter expected (the test level (\'e\', \'f\' or anything else), the error stream, the message stream, the log stream, for every stream the expliciteness level code, for every stream the language code).\nStream descriptors: cerr, cout, NULL, ofstream-app=, ofstream-w=, ostringstream=; joining streams with \'|\'.\n";
    return EXIT_FAILURE;
  }

  // Reading the test level:

  unsigned int current_parameter = 1;

  ::OKlib::TestSystem::TestLevel& level((argv[current_parameter][0] == 'f') ? ::OKlib::TestSystem::test_level(::OKlib::TestSystem::Full()) : (argv[current_parameter][0] == 'e') ? ::OKlib::TestSystem::test_level(::OKlib::TestSystem::Extensive()) : ::OKlib::TestSystem::test_level(::OKlib::TestSystem::Basic()));
  ++current_parameter;

  // Preparing the output streams:

  ::OKlib::GeneralInputOutput::OStreamMultiplexer multiplexer;
  {
    const char separator = '|';
    typedef std::set< ::OKlib::GeneralInputOutput::OStreamDescriptor> descriptor_set_t;
    descriptor_set_t array_descriptors[number_streams];
    for (unsigned int i = 0; i < number_streams; ++i, ++current_parameter) {
      std::vector<std::string> tokens;
      const std::string parameter(argv[current_parameter]);
      if (not parameter.empty())
        boost::algorithm::split(tokens, argv[current_parameter], is_separator(separator));
      array_descriptors[i].insert(tokens.begin(), tokens.end());
    }
    multiplexer.assign(boost::make_iterator_range(array_descriptors, array_descriptors+number_streams));
  }

  assert(multiplexer.fostream_vector.size() == number_streams);

  for (unsigned int i = 0; i < number_streams; ++i, ++current_parameter) {
    ::OKlib::Messages::MessagesBase::set(multiplexer.fostream_vector[i], ::OKlib::Messages::Strata(boost::lexical_cast<unsigned int>(argv[current_parameter])));
  }
  
  for (unsigned int i = 0; i < number_streams; ++i, ++current_parameter) {
    ::OKlib::Messages::MessagesBase::set(multiplexer.fostream_vector[i], ::OKlib::Messages::LanguageName()(argv[current_parameter]));
  }

  assert(current_parameter == number_parameters);

  // Running the tests:

  return OKlib::TestSystem::RunTest::run_tests(multiplexer.fostream_vector[0], multiplexer.fostream_vector[1], multiplexer.fostream_vector[2], level);
}

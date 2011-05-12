// Oliver Kullmann, 12.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/DistributedSolving/PreprocessSplitting.cpp
  \brief Application for preparing the list of splitting instances

  <ul>
   <li> Takes exactly one argument N. </li>
   <li> Runs then through files "1", "2", ..., "N" in the given directory.
   </li>
   <li> These files are assumed to be partial assignments, in correct
   Dimacs format (without any other content). </li>
   <li> Determines the pairs (i,n(i)), i=1,...,N, where n(i) is the number of
   variables of the partial assignment given by file "i". </li>
   <li> Stably sorts the pairs (i,n(i)) according to decreasing n(i). </li>
   <li> Prints "c i n(i)", one i per row, in this order, to standard output,
   where c is a counter (starting with 1). </li>
  </ul>

  \todo Better error messages
  <ul>
   <li> Use Messages. </li>
   <li> Don't hardcode the value of uint_type. </li>
  </ul>

  \todo Write application tests

*/

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <cassert>
#include <algorithm>

#include <boost/lexical_cast.hpp>


namespace {

  enum { errcode_parameter = 1, errcode_value = 2, errcode_file = 3 };

  const std::string program = "PreprocessSplitting";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.0.5";

  typedef unsigned int uint_type;
  typedef std::pair<uint_type, uint_type> pair_t;
  struct Comp {
    bool operator()(const pair_t& lhs, const pair_t& rhs) const {
      return lhs.second > rhs.second;
    }
  };

}

int main(const int argc, const char* const argv[]) {
  if (argc != 2) {
    std::cerr << err << "Exactly one argument is needed, the number N"
      " of splitting cases.\n";
    return errcode_parameter;
  }

  try {
  
    const uint_type N = boost::lexical_cast<uint_type>(argv[1]);

    typedef std::vector<pair_t> vector_t;
    vector_t statistics;
    statistics.reserve(N);
    for (uint_type i = 1; i <= N; ++i) {
      const std::string filename = boost::lexical_cast<std::string>(i);
      std::ifstream file(filename.c_str());
      if (not file) {
        std::cerr << err << "Can not open file \"" << filename << "\".\n";
        return errcode_file;
      }
      uint_type count = 0;
      for (std::string x; file >> x; ++count);
      assert(count >= 2);
      statistics.push_back(pair_t(i,count-2));
    }
    std::stable_sort(statistics.begin(), statistics.end(), Comp());
    typedef vector_t::const_iterator iterator;
    const iterator send = statistics.end();
    for (struct {iterator i; uint_type c;} l = {statistics.begin(), 0}; l.i != send; ++l.i)
      std::cout << ++l.c << " " << l.i->first << " " << l.i->second << "\n";
  }
  catch(const boost::bad_lexical_cast&) {
    std::cerr << err << "Parameter \"" << argv[1] << "\" does not represent"
    " a valid value of unsigned int.\n";
    return errcode_value;
  }

}

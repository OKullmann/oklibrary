// Oliver Kullmann, 12.5.2011 (Swansea)
/* Copyright 2011, 2020 Oliver Kullmann
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
   <li> Additionally also a file "decisions" in the current directory is
   used. </li>
   <li> Determines the triples (i,n_i,d_i), i=1,...,N, where n_i is the
   number of variables of the partial assignment given by file "i", while
   d_i is the number of decision variables given by line i of file decisions.
   </li>
   <li> Stably sorts the triples (i,n_i,d_i) according to decreasing n_i.
   </li>
   <li> Prints "c i n_i d_i", one i per row, in this order, to standard output,
   where c is a counter (starting with 1). </li>
  </ul>

  \todo Better error messages
  <ul>
   <li> Use Messages. </li>
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
#include <stdexcept>

#include <cstdint>


namespace {

  enum {
    errcode_parameter = 1,
    errcode_value = 2,
    errcode_decfile = 3,
    errcode_file = 4,
    errcode_reading = 5
  };

  const std::string program = "PreprocessSplitting";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.0.7";

  typedef std::uint64_t uint_type;

  struct Description {
    uint_type index, n, d;
    Description(const uint_type index, const uint_type n, const uint_type d) : index(index), n(n), d(d) {}
    friend std::ostream& operator <<(std::ostream& out, const Description& d) {
      out << d.index << " " << d.n << " " << d.d;
      return out;
    }
  };

  struct Comp {
    bool operator()(const Description& lhs, const Description& rhs) const {
      return lhs.n > rhs.n;
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
  
    const uint_type N = std::stoull(argv[1]);

    const std::string dec_filename = "decisions";
    std::ifstream Decisions(dec_filename.c_str());
    if (not Decisions) {
      std::cerr << err << "Can not open file \"" << dec_filename << "\".\n";
      return errcode_decfile;
    }

    typedef std::vector<Description> vector_t;
    vector_t statistics;
    statistics.reserve(N);
    for (uint_type i = 1; i <= N; ++i) {
      const std::string filename = std::to_string(i);
      std::ifstream file(filename.c_str());
      if (not file) {
        std::cerr << err << "Can not open file \"" << filename << "\".\n";
        return errcode_file;
      }
      uint_type count = 0;
      for (std::string x; file >> x; ++count);
      assert(count >= 2);
      uint_type number_decisions;
      Decisions >> number_decisions;
      Decisions.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
      if (not Decisions) {
        std::cerr << err << "Reading error with \"" << dec_filename << "\".\n";
        return errcode_reading;
      }
      statistics.push_back(Description(i,count-2,number_decisions));
    }
    std::stable_sort(statistics.begin(), statistics.end(), Comp());
    typedef vector_t::const_iterator iterator;
    const iterator send = statistics.end();
    for (struct {iterator i; uint_type c;} l = {statistics.begin(), 0}; l.i != send; ++l.i)
      std::cout << ++l.c << " " << *l.i << "\n";
  }
  catch(const std::exception&) {
    std::cerr << err << "Parameter \"" << argv[1] << "\" does not represent"
    " a valid value of 64-bit unsigned int.\n";
    return errcode_value;
  }

}

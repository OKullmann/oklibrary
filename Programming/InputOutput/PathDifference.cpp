// Oliver Kullmann, 9.9.2007 (Swansea)
/* Copyright 2007, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/InputOutput/PathDifference.cpp
  \brief Application for computing the relative path from one location to another

  Two input parameters are required, two absolute paths A and B,
  while the output R is the relative path leading from B to A
  (so in a sense R = A - B, since R + B = A).

  Further details:
  <ul>
   <li> For B only the directory part is of relevance, so that for example
   B = /a/f.txt and B = /a/ are equivalent. </li>
   <li> On the other side, for A the reached leaf is determinative, and
   so A = /x/ and A = /x are equivalent. </li>
  </ul>

  Errors are:
  <ul>
   <li> A == B </li>
   <li> A or B are not absolute paths. </li>
  </ul>


  \todo How to use boost::filesystem better?
  <ul>
   <li> It seems that paths cannot be constructed from their elements,
   only from strings?? </li>
  </ul>


  \todo The functionality of this applications should go into its own
  component (so that this application just becomes a wrapper).


  \todo Document it precisely.


  \todo Write extensive tests.

*/

#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <cassert>

#include <boost/filesystem/path.hpp>

#include <OKlib/Programming/Refactoring/BoostPathCorrected.hpp>
#include <OKlib/Programming/Sequences/Comparisons.hpp>

namespace {
  const std::string program = "PathDifference";
  const std::string err = "ERROR[" + program + "]: ";
  const std::string version = "0.1.1";
}

int main(const int argc, const char* const argv[]) {
  if (argc != 3) {
    std::cerr << err << "Two input parameters are required, the target path "
      "and the source path (both as absolute paths).\n"
      "However the number of actual arguments was " << argc-1 << ".";
    if (argc > 1) {
      std::cerr << " The arguments were:\n";
      for (int i = 1; i < argc; ++i)
        std::cerr << i << ": " << argv[i] << "\n";
    }
    std::cerr << "\n";
    return EXIT_FAILURE;
  }

  const std::string a(argv[1]);
  const std::string b(argv[2]);

  if (a == b) {
    std::cerr << err << "The two input paths are identical, namely = \"" << a << "\".\n";
    return EXIT_FAILURE;
  }

  const boost::filesystem::path pa(a);
  boost::filesystem::path pb(b);

  if (not pa.has_root_directory()) {
    std::cerr << err << "The first path = \"" << a << "\" is not an absolute path.\n";
    return EXIT_FAILURE;
  }
  if (not pb.has_root_directory()) {
    std::cerr << err << "The second path = \"" << b << "\" is not an absolute path.\n";
    return EXIT_FAILURE;
  }

  assert(not pb.empty());
  if (pb.has_parent_path()) pb.remove_filename();
  assert(not pb.empty());

  typedef boost::filesystem::path::iterator iterator;
  typedef boost::range_size<boost::filesystem::path>::type size_type;
  iterator ca, cb;
  {size_type cs;
   std::tr1::tie(ca, cb, cs) = OKlib::Programming::Sequences::common_part(pa, pb);
  }

  const std::string one_level_up("..");
  const std::string sep("/");
  const size_type numbers_up = std::distance(cb, pb.end());
  std::string r;
  for (size_type i = 0; i < numbers_up; ++i) r += one_level_up + sep;
  {const iterator aend = pa.end();
   for (iterator i = ca; i != aend; ++i) r += i->string() + sep;
  }
  assert(r.size() > 0);
  std::cout << r.substr(0, r.size()-1) << "\n";
}


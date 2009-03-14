// Oliver Kullmann, 14.3.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/LinInequal.cpp
  \brief Application for encoding linear inequalities as CNF
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <exception>

#include <boost/lexical_cast.hpp>

#include <OKlib/Satisfiability/Transformers/Generators/LinInequal.hpp>

#include <OKlib/General/StringHandling.hpp>

int main(const int argc, const char* const argv[]) {

  unsigned int L, B;
  std::cin >> L >> B;
  {
    typedef std::vector<std::string> st_vec;
    {
      st_vec X; X.reserve(L);
      for (unsigned int i = 1; i <= L; ++i)
        X.push_back(LinInequal::P("V",i));
      LinInequal::AddVar(X.begin(), X.end(), "S", "H", std::cout);
      std::copy(X.begin(), X.end(),
                std::ostream_iterator<std::string>(std::cout, "0 \n"));
    }
    {
      const unsigned int blL = LinInequal::bin_length(L);
      st_vec S; S.reserve(blL);
      for (unsigned int i = 1; i <= blL; ++i)
        S.push_back(LinInequal::P("S", i));
      LinInequal::CompVar(S.begin(), S.end(), B, std::cout);
    }
  }

  {
    LinInequal::ui_vec Cap;
    Cap.reserve(argc-2);
    for (unsigned int i = 1; i < argc-1; ++i)
      Cap.push_back(StringHandling::fromString<unsigned int>(argv[i]));
    const unsigned int level =
      StringHandling::fromString<unsigned int>(argv[argc-1]);
    LinInequal::Assignment(Cap, std::cin, level, std::cout);
  }

}


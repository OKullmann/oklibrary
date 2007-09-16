// Oliver Kullmann, 27.11.2004 (Swansea)
/* Copyright 2004 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <cstdlib>
#include <string>

#include <boost/date_time/gregorian/gregorian.hpp>

int main(const int argc, const char* const argv[]) {
  if (argc != 3) {
    std::cerr << "Exactly two arguments are needed, the two dates for which the difference in number of days is to be computed.\n";
    return EXIT_FAILURE;
  }
  const boost::gregorian::date date1(boost::gregorian::from_uk_string(argv[1])), date2(boost::gregorian::from_uk_string(argv[2]));
  std::cout << (date2 - date1).days() << "\n";
}

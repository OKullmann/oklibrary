// Oliver Kullmann, 6.3.2005 (Swansea)

#include <iostream>
#include <cstdlib>
#include <string>

#include "boost/date_time/gregorian/gregorian.hpp"

int main(const int argc, const char* const argv[]) {
  if ((argc-1) % 2 == 1) {
    std::cerr << "The number of arguments must be even and at least two! (The arguments are taken in pairs, and the sum of the lengths of the (closed) intervals they describe is output.)\n";
    return EXIT_FAILURE;
  }
  {
    const int list_length = (argc - 1) / 2;
    const char* const * argp = argv;
    typedef long int Count;
    Count sum = 0;
    for (int i = 0; i < list_length; ++i) {
      const boost::gregorian::date date1(boost::gregorian::from_uk_string(*++argp)), date2(boost::gregorian::from_uk_string(*++argp));
      sum += (date2 - date1).days();
    }
    sum += list_length;
    std::cout << sum << "\n";
  }
}

// Oliver Kullmann, 27.11.2004 (Swansea)

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

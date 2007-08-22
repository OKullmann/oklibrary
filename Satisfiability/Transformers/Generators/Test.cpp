// Oliver Kullmann, 6.10.2002 (Swansea)

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <exception>

#include <boost/lexical_cast.hpp>

#include "LinInequal.hpp"
#include "FaronsPuzzle.hpp"

// #include "StringHandling.hpp"
#include "ErrorHandling.hpp"

// typedef vector<string> st_vec;

int main(const int argc, const char* const argv[]) {

//   unsigned int L, B;
//   cin >> L >> B;
//   st_vec X; X.reserve(L);
//   for (unsigned int i = 1; i <= L; ++i)
//     X.push_back(P("V",i));
//   AddVar(X.begin(), X.end(), "S", "H", cout);
//   copy(X.begin(), X.end(), ostream_iterator<string>(cout, "0 \n"));
//   const unsigned int blL = bin_length(L);
//   st_vec S; S.reserve(blL);
//   for (unsigned int i = 1; i <= blL; ++i)
//     S.push_back(P("S", i));
//   CompVar(S.begin(), S.end(), B, cout);

//   ui_vec Cap;
//   Cap.reserve(argc-2);
//   for (unsigned int i = 1; i < argc-1; ++i)
//     Cap.push_back(fromString<unsigned int>(argv[i]));
//   const unsigned int level = fromString<unsigned int>(argv[argc-1]);
//   Assignment(Cap, cin, level, cout);

  if (argc <= 2) {
    std::cerr << "Two arguments are needed: The dimension and the score.\n";
    return 1;
  }
  try {
    using namespace FaronsPuzzle;
    const unsigned int d = boost::lexical_cast<unsigned int>(argv[1]);
    const unsigned int k = boost::lexical_cast<unsigned int>(argv[2]);
    const Versions version = (argc >= 4) ? static_cast<Versions>(boost::lexical_cast<int>(argv[3])) : basic;

    switch (version) {
    case basic :
      Test_score(d, k, std::cout)();
      break;
    case simple_symmetry_breaking :
      Test_score_simple_symmetry_breaking(d, k, std::cout)();
      break;
    case extended_symmetry_breaking :
      Test_score_extended_symmetry_breaking(d, k, std::cout)();
      break;
    }
  }
  catch (const std::exception& e) {
    std::cerr << ErrorHandling::Error2string(e);
  }
}


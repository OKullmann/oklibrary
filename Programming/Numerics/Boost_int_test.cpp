/* Example from
   https://www.boost.org/doc/libs/1_75_0/libs/multiprecision/doc/html/boost_multiprecision/tut/ints/cpp_int.html

  Does not compile:

  /usr/include/boost/multiprecision/cpp_int.hpp:172:8: error: no class template named -F¡rebind¢ in ¡class std::allocator<long long unsigned int>¢-A

  Perhaps just using fixed precision, we don't run into problems?

*/

#include <iostream>

#include <boost/multiprecision/cpp_int.hpp>

int main() {
  using namespace boost::multiprecision;

  {int128_t v = 1;
   for (unsigned i = 1; i <= 20; ++i) v *= i;
   std::cout << v << std::endl;
  }
  {uint1024_t v = 1;
   for (unsigned i = 1; i <= 100; ++i) v *= i;
   std::cout << v << std::endl;
  }
/*
  // Repeat at arbitrary precision:
  cpp_int u = 1;
  for(unsigned i = 1; i <= 100; ++i) u *= i;
  // prints 93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000 (i.e. 100!)
  std::cout << u << std::endl;
*/
}

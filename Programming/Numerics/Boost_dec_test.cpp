/* Obtained from

  https://www.boost.org/doc/libs/1_75_0/libs/multiprecision/doc/html/boost_multiprecision/tut/floats/cpp_dec_float.html

  Does not compile:

  /usr/include/boost/math/special_functions/detail/bernoulli_details.hpp:252:16: error: -F¡struct boost::math::detail::fixed_vector<boost::multiprecision::number<boost::multiprecision::backends::cpp_dec_float<100> > >¢ has no member named ¡destroy¢-A

  This seems to be due to the use of boost::math::tgamma -- since we don't
  need this, perhaps we won't run into problems?

*/

#include <iostream>

#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/math/special_functions/gamma.hpp>

int main() {
  using namespace boost::multiprecision;

  // Operations at fixed precision and full numeric_limits support:
  const cpp_dec_float_100 b = 2;
  std::cout << std::numeric_limits<cpp_dec_float_100>::digits << std::endl;
  // Note that digits10 is the same as digits, since we're base 10! :
  std::cout << std::numeric_limits<cpp_dec_float_100>::digits10 << std::endl;
  // We can use any C++ std lib function, lets print all the digits as well:
  std::cout << std::setprecision(std::numeric_limits<cpp_dec_float_100>::max_digits10)
            << log(b) << std::endl;
  typedef number<cpp_dec_float<1000>> cpp_dec_float_1000;
  const cpp_dec_float_1000 c = 2;
  std::cout << std::setprecision(std::numeric_limits<cpp_dec_float_1000>::max_digits10)
            << log(c) << std::endl;
/*
  // We can also use any function from Boost.Math:
  std::cout << boost::math::tgamma(b) << std::endl;
  // These even work when the argument is an expression template:
  std::cout << boost::math::tgamma(b * b) << std::endl;
  // And since we have an extended exponent range we can generate some really large
  // numbers here (4.0238726007709377354370243e+2564):
  std::cout << boost::math::tgamma(cpp_dec_float_100(1000)) << std::endl;
*/
}

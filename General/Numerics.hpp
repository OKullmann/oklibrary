// Oliver Kullmann, 27.7.2004 (Turin)

#ifndef NUMERICS_j134815cawq
#define NUMERICS_j134815cawq

#include <cmath>
#include <utility>
#include <cassert>

namespace Numerics {

  // TO IMPROVE: integration with Algorithms::power_natural (OK, 6.11.2004)
  template <typename base_type, typename exponent_type = int>
  struct Integer_exponentiation {
    base_type operator() (const base_type base, const exponent_type exp, base_type multiplier = base_type(1)) const {
      // returns multiplier * base^(floor(exp))
      // INEFFICIENT!
      if (exp >= exponent_type(0))
	for (exponent_type i = exponent_type(1); i <= exp; ++i)
	  multiplier *= base;
      else
	for (exponent_type i = exponent_type(1); i <= -exp; ++i)
	  multiplier /= base;
      return multiplier;
    }
  };
  template <typename base_type, typename exponent_type>
  inline base_type integer_exponentiation(const base_type base, const exponent_type exp, const base_type multiplier = base_type(1)) {
    return Integer_exponentiation<base_type, exponent_type>()(base, exp, multiplier);
  }

  template <typename Float, typename Return = Float, typename Int_basis = int, typename Int_position = int>
  struct Round {
    // TO DO:
    // 1. Two different facilities:
    //    (a) Round: from Float to Return (where Return can be a floating point or an integer type)
    //    (b) RoundPlaces: from Float to Float.
    // 2. Usage of std::round and std::lround, std::llround etc for Round.
    // RoundPlaces uses Round.
    typedef Float value_type;
    typedef Return return_type;
    typedef Int_basis basis_type;
    typedef Int_position position_type;

    const basis_type basis;
    Round(const basis_type basis = 10) : basis(basis) {}
    return_type operator() (value_type x) const {
      using std::floor;
      const value_type f = floor(x);
      const value_type result = (x >= 0) ?  ((x >= f + 0.5) ? f+1 : f) : ((x > f + 0.5) ? f+1 : f);
      const return_type result2 = return_type(result);
      assert(result == result2);
      return result2;
    }
    
    return_type operator() (value_type x, const position_type position) {
      assert(position >= 0);
      const value_type right_shifted = integer_exponentiation<value_type>(basis, position, x);
      const value_type rounded_right_shifted = operator ()(right_shifted);
      return return_type(integer_exponentiation<value_type>(basis, - position, rounded_right_shifted));
    }
  };
  template <typename Float>
  inline Float round(Float x) {
    return Round<Float>()(x);
  }
  template <typename Float, typename Int>
  inline Int round(Float x, Int position) {
    return Round<Float, Int>()(x, position);
  }
  


  template <typename Nat>
  struct Nat_1_2_diag {
    // Implementation von en_d^2 of "BuchAlgebra"
    typedef std::pair<Nat, Nat> pair_type;
    typedef Nat number_type;
    pair_type operator() (const number_type n) const {
      assert(n >= 0);
      const number_type h = 8 * n + 1;
      const long double sqr = std::sqrt((long double)(h)); // ???????????????
      Round<long double, number_type> r;
      const number_type rsqr = r(sqr);
      const number_type sum = (rsqr * rsqr == h) ? (rsqr - 1) / 2 : number_type((sqr - 1) / 2);
      const number_type max = (sum * (sum + 3)) / 2;
      const number_type x = max - n;
      return std::make_pair(x, sum - x);
    }
    number_type operator() (const pair_type& p) const {
      assert(p.first >= 0);
      assert(p.second >= 0);
      const number_type sum = p.first + p.second;
      return (sum * (sum+1)) / 2 + p.second;
    }
  };

  template <typename Nat>
  inline typename Nat::pair_type nat_1_2_diag(const Nat n) {
    return Nat_1_2_diag<Nat>()(n);
  }


  template <typename Nat>
  class Graph_edges {
    // Implementation of eng_n from "BuchAlgebra"
    const Nat n;
    const Nat n2, n22;
  public :
    typedef Nat number_type;
    typedef std::pair<Nat, Nat> pair_type;
    const number_type N;
    Graph_edges(const number_type n) : n(n), n2(2 * n - 1), n22(n2 * n2), N((n * (n-1)) / 2) {
      assert(n >= 0);
    }
    pair_type operator() (const Nat k) const {
      assert(k >= 1);
      assert(k <=  N);
      const number_type h = n22 - 8 * k;
      const long double sqr = std::sqrt((long double)(h)); // ???????????????
      Round<long double, number_type> r;
      const number_type rsqr = r(sqr);
      const number_type v = (rsqr * rsqr == h) ? (n2 - rsqr) / 2 : number_type(std::ceil(((n2 - sqr) / 2)));
      const number_type w = ((v * (v+1)) / 2 + k) - n * (v - 1);
      return std::make_pair(v,w);
    }
    number_type operator() (const pair_type& p) const {
      return (p.first - 1) * n - ((p.first + 1) * p.first) / 2 + p.second;
    }
  };
}

#endif

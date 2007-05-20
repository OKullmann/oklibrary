// Oliver Kullmann, 27.7.2004 (Turin)

/*!
  \file General/Numerics_Tests.hpp
  \brief Old tests for numeric operations (still using asserts)
  \deprecated Move to the new test system.
*/

#ifndef NUMERICS_TESTS_jjvvZZaqw3377
#define NUMERICS_TESTS_jjvvZZaqw3377

#include <cassert>
#include <limits>

#include <boost/test/floating_point_comparison.hpp>

namespace Numerics_Tests {

  template <template <typename base_type, typename exponent_type = int> class Integer_exponentiation>
  struct test_integer_exponentiation {
    test_integer_exponentiation() {
#ifndef NDEBUG
      Integer_exponentiation<double> ie;
#endif
      assert(ie(2,0) == 1);
      assert(ie(2,1) == 2);
      assert(ie(2,2) == 4);
      assert(ie(2,2,3) == 12);
      assert(ie(2,-1) == 0.5);
      assert(ie(2,-2) == 0.25);
      assert(ie(2,-2,4) == 1);
      // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    }
  };

  /*!
    \class test_round
    \brief %Test function for rounding operation

    \todo Check the usage of boost::test_tools::close_at_tolerance.
    \todo %Test of different floating point types and test of different return types
    \todo %Test of characteristic properties (x real, k integer):
    <ol>
     <li> r(x) is integral </li>
     <li> |r(x) - x| <= 1/2 </li>
     <li> r(-x) = - r(x) </li>
     <li> r(x + k) = r(x) + k </li>
     <li> r(1/2) = 1. </li>
    </ol>
  */

  template <template <typename Float, typename Return = Float, typename Int_basis = int, typename Int_position = int> class Round>
  struct test_round {
    test_round() {
      Round<double> r;
      assert(r(1/2.0) == 1);
      assert(r(- 1/2.0) == -1);
      for (int i = -100; i < 101; ++i) {
	assert(r(i) == i);
	assert(r(i,0) == r(i));
	assert(r(i + 0.1) == i);
	assert(r(i + 0.1,0) == r(i + 0.1));
	assert(r(i - 0.1) == i);
	assert(r(i - 0.1,0) == r(i - 0.1));
	assert(r(i + 0.500001) == i+1);
	assert(r(i - 0.500001) == i-1);
	if (i > 0) {
	  assert(r(i + 1/2.0) == i+1);
	  assert(r(i - 1/2.0) == i);
	}
	else if (i < 0) {
	  assert(r(i + 1/2.0) == i);
	  assert(r(i - 1/2.0) == i - 1);
	}
	// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
      }
      boost::test_tools::close_at_tolerance<double> cat(boost::test_tools::fraction_tolerance(12 * std::numeric_limits<double>::epsilon()));
      assert(cat(r(1.750001, 1), 1.8));
      assert(cat(r(1.749999, 1), 1.7));
      assert(cat(r(-1.750001, 1), -1.8));
      assert(cat(r(-1.749999, 1), -1.7));
      // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    }
  };

  template <template <typename Nat> class Nat_1_2>
  struct test_nat_1_2 {
    test_nat_1_2() {
      typedef Nat_1_2<unsigned int> Code;
      typedef typename Code::pair_type Pair;
      typedef typename Code::number_type Nat;
#ifndef NDEBUG
      Code code;
#endif
      assert(code(0) == Pair(0,0));
      assert(code(1) == Pair(1,0));
      assert(code(2) == Pair(0,1));
      assert(code(3) == Pair(2,0));
      assert(code(4) == Pair(1,1));
      assert(code(5) == Pair(0,2));
      assert(code(6) == Pair(3,0));
      assert(code(7) == Pair(2,1));
      assert(code(8) == Pair(1,2));
      assert(code(9) == Pair(0,3));
      const Nat max_test = 1000;
      for (Nat i = 0; i <= max_test; ++i)
        assert(code(code(i)) == i);
    }
  };

  template <template <typename Nat> class Graph_edges>
  struct test_graph_edges {
    test_graph_edges() {
      typedef Graph_edges<unsigned int> Edges;
      typedef typename Edges::pair_type Pair;
      typedef typename Edges::number_type Nat;
      {
        const Edges edges(0);
        assert(edges.N == 0);
      }
      {
        const Edges edges(1);
        assert(edges.N == 0);
      }
      {
        const Edges edges(2);
        assert(edges.N == 1);
        assert(edges(1) == Pair(1,2));
        assert(edges(Pair(1,2)) == 1);
      }
      {
        const Edges edges(3);
        assert(edges.N == 3);
        assert(edges(1) == Pair(1,2));
        assert(edges(2) == Pair(1,3));
        assert(edges(3) == Pair(2,3));
        for (Nat i = 1; i <= edges.N; ++i)
          assert(edges(edges(i)) == i);
      }
      {
        const Nat max_number_vertices = 50;
        for (Nat n = 4; n <= max_number_vertices; ++n) {
          const Edges edges(n);
          for (Nat i = 1; i <= edges.N; ++i)
            assert(edges(edges(i)) == i);
        }
      }
    }
  };

}

#endif

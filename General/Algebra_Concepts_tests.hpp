// Oliver Kullmann, 7.11.2004 (Swansea)
/* Copyright 2004 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef ALGEBRACONCEPTSTESTSWAECHTER_jjdn55Ra

#define ALGEBRACONCEPTSTESTSWAECHTER_jjdn55Ra

#include <cassert>

#include <OKlib/General/NumberTheory_Models.hpp>

namespace Algebra_Concepts_tests {

  // OK, 7.11.2004
  // TO IMPROVE: to be completed
  template <template <unsigned long int modulus, typename Int> class Z_mod_n>
  struct Test_Z_mod_n {
    Test_Z_mod_n() {
      {
	typedef int Int;
	const Int m = 2;
	typedef Z_mod_n<m, Int> Z_mod_m;

	assert(Z_mod_m::null() == Z_mod_m(0));
	assert(Z_mod_m::unit() == Z_mod_m(1));
	assert(Z_mod_m::unit() == Z_mod_m(-1));

	for (Int i = 0; i < m; ++i) {
	  Z_mod_m i_m(i);
	  assert(-i_m == i_m);
	  for (Int j = 0; j < m; ++j) {
	    Z_mod_m j_m(j);
	    assert(i_m + j_m == Z_mod_m((i + j) % m));
	    assert(i_m * j_m == Z_mod_m((i * j) % m));
	    assert(i_m - j_m == ((j) ? Z_mod_m(Int(not i)) : i_m));
	    if (j == 1)
	      assert(i_m / j_m == i_m);
	  }
	}
	{
	  Int count_inv = 0;
	  for (Int i = 0; i < m; ++i)
	    if (Z_mod_m(i).invert())
	      ++count_inv;
	  assert(count_inv == NumberTheory::euler_phi_brute_force(m));
	}
      }
      {
	typedef int Int;
	const Int m = 3;
	typedef Z_mod_n<m, Int> Z_mod_m;

	assert(Z_mod_m::null() == Z_mod_m(0));
	assert(Z_mod_m::unit() == Z_mod_m(1));
	assert(Z_mod_m::unit() == Z_mod_m(-2));
	assert(Z_mod_m(2) == Z_mod_m(-1));
	assert(Z_mod_m(3) == Z_mod_m(0));
	assert(Z_mod_m(4) == Z_mod_m(1));

	assert(Z_mod_m(2) / Z_mod_m(2) == Z_mod_m(1));

	const Int iterations = 3 * m;
	for (Int i = 0; i < iterations ; ++i) {
	  const Z_mod_m i_m(i);
	  assert(-i_m == Z_mod_m(m - i));
	  for (Int j = 0; j < iterations; ++j) {
	    const Z_mod_m j_m(j);
	    assert(i_m + j_m == Z_mod_m((i + j) % m));
	    assert(i_m + j_m == Z_mod_m(i + j));
	    assert(i_m * j_m == Z_mod_m((i * j) % m));
	    assert(i_m * j_m == Z_mod_m(i * j));
	    assert(i_m - j_m == Z_mod_m(i-j));
	    if (j % m)
	      assert(i_m / j_m == i_m * j_m);
	  }
	}
	{
	  Int count_inv = 0;
	  for (Int i = 0; i < m; ++i)
	    if (Z_mod_m(i).invert())
	      ++count_inv;
	  assert(count_inv == NumberTheory::euler_phi_brute_force(m));
	}
      }
      {
	typedef int Int;
	const Int m = 60;
	typedef Z_mod_n<m, Int> Z_mod_m;

	assert(Z_mod_m::null() == Z_mod_m(0));
	assert(Z_mod_m::unit() == Z_mod_m(1));
	assert(Z_mod_m::unit() == Z_mod_m(-59));
	assert(Z_mod_m(2) == Z_mod_m(-58));
	assert(Z_mod_m(3) == Z_mod_m(-57));
	assert(Z_mod_m(4) == Z_mod_m(-56));

	assert(Z_mod_m(2) * Z_mod_m(30) == Z_mod_m(0));
	assert(Z_mod_m(11) / Z_mod_m(49) == Z_mod_m(-1));
	{
	  const Int iterations = 3 * m;
	  for (Int i = 0; i < iterations ; ++i) {
	    const Z_mod_m i_m(i);
	    assert(-i_m == Z_mod_m(m - i));
	    for (Int j = 0; j < iterations; ++j) {
	      const Z_mod_m j_m(j);
	      assert(i_m + j_m == Z_mod_m((i + j) % m));
	      assert(i_m + j_m == Z_mod_m(i + j));
	      assert(i_m * j_m == Z_mod_m((i * j) % m));
	      assert(i_m * j_m == Z_mod_m(i * j));
	      assert(i_m - j_m == Z_mod_m(i-j));
	    }
	  }
	}
	{
	  Int count_inv = 0;
	  for (Int i = 0; i < m; ++i)
	    if (Z_mod_m(i).invert())
	      ++count_inv;
	  assert(count_inv == NumberTheory::euler_phi_brute_force(m));
	}
      }
    }
  };

}

#endif

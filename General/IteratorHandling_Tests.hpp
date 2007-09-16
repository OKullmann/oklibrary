// Oliver Kullmann, 11.9.2004 (Swansea)
/* Copyright 2004 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file IteratorHandling_Tests.hpp
  \brief Tests for the tools for handling iterators.
  \todo Testing of the iterator adaptors for sequences.
*/

#ifndef ITERATORHANDLING_TESTS_kkakxQWop83R

#define ITERATORHANDLING_TESTS_kkakxQWop83R

#include <cassert>

namespace IteratorHandling_Tests {

  template <template <typename Num, typename Int> class Arithmetical_progression>
  struct test_arithmetical_progression {
    test_arithmetical_progression() {
      {
	// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	typedef Arithmetical_progression<double, int> AP;
	typedef typename AP::iterator iterator;
	AP ap(4, -2, 5);
	assert(iterator() == iterator());
	assert(not(iterator() != iterator()));
	assert(iterator() != ap.begin());
	iterator i = ap.begin();
	assert(i == ap.begin());
	assert(*i++ == 4);
	assert(*i++ == 2);
	assert(*i++ == 0);
	assert(*i++ == -2);
	assert(*i++ == -4);
	assert(*i++ == -6);
	assert(i == ap.end());
	i -= 6;
	assert(i == ap.begin());
	i += 2;
	assert(*i-- == 0);
	assert(i == ++ap.begin());
	// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
      }
    }
  };

}

#endif

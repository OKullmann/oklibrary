// Oliver Kullmann, 19.2.2001 (Toronto)
/* Copyright 2001 - 2007, 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKsolver/SAT2002/Speicher.c
  \brief Implementations of auxiliary functions regarding memory allocation
*/

                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Angelegt: 19.2.2001 */

/* Einheit: Speicher */

#include <stdlib.h>
#include <stdio.h>

#include "OK.h"
#include "Ausgaben.h"
#include "Speicher.h"


void* xmalloc(const size_t size) {
  void* const value = malloc(size);
  if (value == 0) {
    fprintf(stderr, "%s\n", Meldung(5));
    exit(1);
  }
  return value;
}


void* xrealloc(void* const ptr, const size_t size) {
  void* const value = realloc(ptr, size);
  if (value == 0) {
    fprintf(stderr, "%s\n", Meldung(5));
    exit(1);
  }
  return value;
}

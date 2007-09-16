// Oliver Kullmann, 19.2.2001 (Toronto)
/* Copyright 2001 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKsolver/SAT2002/Speicher.h
  \brief Header file for auxiliary functions regarding memory allocation
*/

                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Angelegt: 19.2.2001 */

#ifndef GELESENSPEICHER_kKnqpqd
#define GELESENSPEICHER_kKnqpqd

#include <stdlib.h>

extern void *xmalloc(size_t size);

extern void *xrealloc(void *ptr, size_t size);

#endif

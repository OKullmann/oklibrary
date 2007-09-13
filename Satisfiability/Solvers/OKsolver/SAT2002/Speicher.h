// Oliver Kullmann, 19.2.2001 (Toronto)

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

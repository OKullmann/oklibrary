// Oliver Kullmann, 19.2.2001 (Toronto)

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


void *xmalloc(size_t size)
{
  register void *value = malloc(size);
  if (value == 0)
    {
      fprintf(stderr, "%s\n", Meldung(5));
      exit(1);
    }
  return value;
}


void *xrealloc(void *ptr, size_t size)
{
  register void *value = realloc(ptr, size);
  if (value == 0)
    {
      fprintf(stderr, "%s\n", Meldung(5));
      exit(1);
    }
  return value;
}



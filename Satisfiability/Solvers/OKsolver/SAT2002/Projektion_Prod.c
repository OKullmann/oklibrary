// Oliver Kullmann, 6.2.2001 (Toronto)

/*!
  \file OKsolver/SAT2002/Projektion_Prod.c
  \brief The standard projection function, the product
*/

/* Einheit: Projektion_Prod */

#include "OK.h"
#include "Abstand.h"
#include "Projektion.h"

const char *Projektionsname = "Projektion_Prod.c";

__inline__ double Projektion( void )
{
  return d0 * d1;
}

__inline__ unsigned int Projektion2 ( void )
{
  return d0S * d1S;
}

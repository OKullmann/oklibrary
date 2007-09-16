// Oliver Kullmann, 6.2.2001 (Toronto)
/* Copyright 2001 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

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

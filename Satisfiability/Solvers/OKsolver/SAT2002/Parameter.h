// Oliver Kullmann, 8.2.2001 (Toronto)
/* Copyright 2001 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKsolver/SAT2002/Parameter.h
  \brief Header file for parameters needed for tree pruning
*/

#ifndef GELESENPARAMETER_jJbcd4
#define GELESENPARAMETER_jJbcd4

#include "VarLitKlm.h"

//! the number of bits in a word
#define BITS (8 * sizeof (Wort))

//! number of words for variable-sets
extern unsigned int ANZWORTE;
//! number of bits in variable-sets
extern unsigned GroesseVarMenge;
/* extern unsigned int AnzahlAnker; */
/* extern unsigned int PositionAnker; */

#endif

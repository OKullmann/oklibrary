// Oliver Kullmann, 4.2.2001 (Toronto)
/* Copyright 2001 - 2007, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Solvers/OKsolver/SAT2002/Einlesen.h
  \brief Header file for input/output functions
*/

                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* angelegt: 4.2.2001 */


#ifndef GELESENEINLESEN_jbGf35T
#define GELESENEINLESEN_jbGf35T

#include <stdio.h>


typedef enum {Norm = 0, Sat = 1, Unsat = 2, Fehler = 3} TEIN;

extern TEIN Einlesen(FILE *fp, unsigned long int G);

extern void AufraeumenEinlesen(void);

/*!
  \brief Function to output a satisfying assignment
*/
extern void AusgabeBelegung(FILE *fp);

extern void setzenStandard(void);

#endif

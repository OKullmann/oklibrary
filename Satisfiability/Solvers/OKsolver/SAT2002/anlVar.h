// Oliver Kullmann, 4.2.2001 (Toronto)
/* Copyright 2001 - 2007, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKsolver/SAT2002/anlVar.h
  \brief Header file for functions creating the data structures
*/

                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* angelegt: 4.2.2001 */

#ifndef GELESENANLVAR_vqfsew9
#define GELESENANLVAR_vqfsew9

#include <stdlib.h>

#include "OK.h"
#include "VarLitKlm.h"
/* #include "Klauselmenge.h" */

extern void InitParameter();

extern size_t BedarfVarLitKlmV();
extern size_t BedarfBelegungV();
extern size_t BedarfRekursionV();
extern size_t BedarfFilterV();
extern size_t BedarfReduktionsV();
extern size_t BedarfAbstandsV();
extern size_t BedarfBaumResV();

extern void* VarLitKlmV(void* Z);
extern void* BelegungV(void*);
extern void* RekursionV(void*);
extern void* FilterV(void*);
extern void* ReduktionsV(void*);
extern void* AbstandsV(void*);
extern void* BaumResV(void*);

#endif

// Oliver Kullmann, 25.1.2002 (Swansea)
/* Copyright 2002 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

// Name: VLiteralGeneratorAES.hpp

// Copyright Oliver Kullmann, 25.1.2002
// O.Kullmann@Swansea.ac.uk

/*
   -------------------------------------------------------------------------
   Copyright (c) 2002, Dr Oliver Kullmann <O.Kullmann@Swansea.ac.uk>, Swansea, UK.
   All rights reserved.
   
   TERMS

   Redistribution and use in source and binary forms, with or without 
   modification, are permitted subject to the following conditions:

   1. Redistributions of source code must retain the above copyright 
      notice, this list of conditions and the following disclaimer. 

   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the 
      documentation and/or other materials provided with the distribution. 

   3. The copyright holder's name must not be used to endorse or promote 
      any products derived from this software without his specific prior 
      written permission. 

   This software is provided 'as is' with no express or implied warranties 
   of correctness or fitness for purpose.
   -------------------------------------------------------------------------
*/


#ifndef VLITERALGENERATORWAECHTER

namespace VLGAES {

#include <climits>

#if UINT_MAX == 4294967295

  typedef unsigned int UWort32;

#  elif ULONG_MAX == 4294967295

  typedef unsigned long int UWort32;

#  else

#    error "Es kann kein 32-Bit-Integer-Typ gefunden werden!"

#  endif

  typedef UWort32 Wort64[2];
  // Position 0 steht fuer die niedrigwertigen Teil

  typedef struct {
    UWort32 V; // Variable
    UWort32 W; // Wert
  } VLiterale;

  VLiterale VLiteral(const Wort64 Schluessel, const UWort32 FormelNummer, const UWort32 Variablenzahl, const UWort32 Klausellaenge, const UWort32 Domaingroesse, const Wort64 Literalzaehler);

}

#endif

#define VLITERALGENERATORWAECHTER


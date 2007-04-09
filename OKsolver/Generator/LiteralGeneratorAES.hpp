// Oliver Kullmann, 16.1.2002 (Swansea)

// Name: LiteralGeneratorAES.hpp

// Copyright Oliver Kullmann, 16.1.2002
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

#ifndef LITERALGENERATORAESWAECHTER

#include <climits>

namespace LGAES {

#if UINT_MAX == 4294967295

  typedef int SWort32;
  typedef unsigned int UWort32;

#  elif ULONG_MAX == 4294967295

  typedef long int SWort32;
  typedef unsigned long int UWort32;

#  else

#    error "Es kann kein 32-Bit-Integer-Typ gefunden werden!"

#  endif

  typedef UWort32 Wort64[2];

  SWort32 Literal(const Wort64 Schluessel, const Wort64 FormelNummer, const SWort32 Variablenzahl, const SWort32 Klausellaenge, const Wort64 Literalzaehler);

}

#endif

#define LITERALGENERATORAESWAECHTER


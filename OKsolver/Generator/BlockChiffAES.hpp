// Oliver Kullmann, 15.10.2001 (Swansea)

// Name: BlockChiffAES.hpp

// Copyright Oliver Kullmann, 15.10.2001
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

// Namensaenderung 15.1.2002

// Nun wird rein auf Byte-Ebene gearbeitet.


#ifndef BLOCKCHIFFAESWAECHTER


#define ANZAHLBYTESBLOCK 16

// Die beiden folgenden Tests sind noetig, um das Funktionen der
// aes-Implementation zu garantieren:

#  include <climits>

#  if CHAR_BIT != 8

#    error "ACHTUNG: Ein Byte hat mehr als 8 Bits!"

#  endif

#  if ULONG_MAX != 4294967295

// #    error "ACHTUNG: Ein unsigned long hat mehr als 32 Bits!"
// ??????????????????????????????????????????????????

#  endif


namespace BCAES {

typedef unsigned char Byte;
typedef Byte Wort_128[ANZAHLBYTESBLOCK];

void BlockChiffreAES(const Wort_128 Eingabe, const Wort_128 Schluessel, Wort_128 Ausgabe);

}

#endif

#define BLOCKCHIFFAESWAECHTER



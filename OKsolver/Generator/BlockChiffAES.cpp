// Oliver Kullmann, 15.10.2001 (Swansea)

// Name: BlockChiffAES.cpp

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

// Umbenannt: 15.1.2002

// Wenn TESTMODUS definiert ist, so wird ein Programm erzeugt,
// wobei zwischen MODUSDIREKT (direkte Eingabe), MODUSVK (der
// variable Schluessel-Test) und MODUSVT (der variable Text-Test)
// unterschieden wird.

#ifdef TESTMODUSBCAES

#include <cstdio>

#  ifdef MODUSDIREKT
#include <cstdlib>
#  endif

#endif

#include "BlockChiffAES.hpp"
#include "aescpp.h"

using namespace std;

namespace BCAES {

void BlockChiffreAES(const Wort_128 Eingabe, const Wort_128 Schluessel, Wort_128 Ausgabe)
{
  AESencrypt V(Schluessel);
  V.encrypt(Eingabe, Ausgabe);
}

}


#ifdef TESTMODUSBCAES


void BlockAusgabe( const BCAES::Wort_128 Block) {
  int i;
  for (i = 0; i < ANZAHLBYTESBLOCK; i++)
    printf("%3x", Block[i]);
  printf("\n");
}


void VarKeyKAT( void )
  /* Der "Variable Key Known Answer Test" */
{
  BCAES::Wort_128 Eingabe = {};
  BCAES::Wort_128 Ausgabe;
  BCAES::Wort_128 Schluessel = {};

  int i, j;

  for (i = 0; i < ANZAHLBYTESBLOCK; i++) {
    BCAES::Byte Wert = 0x80;
    for (j = 0; j < 8; j++) {
      Schluessel[i] = Wert;
      BCAES::BlockChiffreAES(Eingabe, Schluessel, Ausgabe);
      printf("%d\n", i * 8 + j + 1);
      BlockAusgabe(Schluessel);
      BlockAusgabe(Ausgabe);
      printf("\n");
      Wert >>= 1;
    }
    Schluessel[i] = 0x00;
  }
}


void VarTextKAT( void )
  /* Der "Variable Text Known Answer Test" */
{
  BCAES::Wort_128 Eingabe = {};
  BCAES::Wort_128 Ausgabe;
  BCAES::Wort_128 Schluessel = {};

  int i, j;

  for (i = 0; i < ANZAHLBYTESBLOCK; i++) {
    BCAES::Byte Wert = 0x80;
    for (j = 0; j < 8; j++) {
      Eingabe[i] = Wert;
      BCAES::BlockChiffreAES(Eingabe, Schluessel, Ausgabe);
      printf("%d\n", i * 8 + j + 1);
      BlockAusgabe(Eingabe);
      BlockAusgabe(Ausgabe);
      printf("\n");
      Wert >>= 1;
    }
    Eingabe[i] = 0x00;
  }
}



#endif
  


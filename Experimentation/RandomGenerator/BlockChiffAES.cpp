// Oliver Kullmann, 15.10.2001 (Swansea)
/* Copyright 2001 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/RandomGenerator/BlockChiffAES.cpp
  \brief Implementations regarding the AES chiffre
*/


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
  


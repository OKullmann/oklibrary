// Oliver Kullmann, 25.1.2002 (Swansea)
/* Copyright 2002 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/RandomGenerator/VLiteralGeneratorAES.cpp
  \brief Implementations regarding the basic generator for non-boolean literals
*/


#include "flint.h"

#include "BlockChiffAES.hpp"
#include "VLiteralGeneratorAES.hpp"

#ifdef TESTENLITERALGEN
#  include <cstdio>
#  include <iostream>
#  include <cstdlib>
using namespace std;
#endif

namespace {

  using namespace VLGAES;

  inline void UWortzuBytes(UWort32 W, BCAES::Byte * const Z) {
// Es sei b_{31}, ..., b_{0} die Binaerdarstellung von W (mit b_{31} als
// dem hoechstwertigen Bit). Es werden dann die vier Binaerzaehlen 
// b_{31}, ..., b_{24}
// b_{23}, ..., b_{16}
// b_{15}, ..., b_{8}
// b_{7}, ..., b_{0}
// in *Z, ..., *(Z+3) abgespeichert.
    BCAES::Byte *H = Z+4; // eine Position nach der letzten ist erlaubt
    do {
      H--;
      *H = W;
      W >>= 8;
    } while (H > Z);
  }

  inline UWort32 CLintzuUWort32(const CLINT x) {
    if (x[0] == 0)
      return 0;
    else if (x[0] == 1)
      return x[1];
    else
      return x[1] + BASE * x[2]; // BASE = 2^16;
  }
}

 
namespace VLGAES {

  VLiterale VLiteral(const Wort64 Schluessel, const UWort32 FormelNummer, const UWort32 Variablenzahl, const UWort32 Klausellaenge, const UWort32 Domaingroesse, const Wort64 Literalzaehler) {

// Voraussetzung: Variablenzahl >= 1, 0 <= Klausellaenge <= Variablenzahl, Domaingroesse >= 1;

    BCAES::Wort_128 AESEingabe, AESSchluessel, AESAusgabe;

    UWortzuBytes(Schluessel[0], AESSchluessel);
    UWortzuBytes(Schluessel[1], AESSchluessel+4);
    UWortzuBytes(Domaingroesse-2, AESSchluessel+8);
    UWortzuBytes(FormelNummer, AESSchluessel+12);

    UWortzuBytes(Variablenzahl, AESEingabe);
    UWortzuBytes(Klausellaenge, AESEingabe+4);
    UWortzuBytes(Literalzaehler[0], AESEingabe+8);
    UWortzuBytes(Literalzaehler[1], AESEingabe+12);

    BCAES::BlockChiffreAES(AESEingabe, AESSchluessel, AESAusgabe);

#ifdef TESTENLITERALGEN
    {
      for (int i = 0; i < 16; i++)
	printf("%x", AESAusgabe[i]);
      printf("\n");
    }
#endif

    VLiterale l; CLINT P;
    CLINT A; byte2clint_l(A, AESAusgabe, 16);
    CLINT B; ul2clint_l(B, Variablenzahl);
    CLINT C; ul2clint_l(C, Domaingroesse);
    mul_l(B, C, C);
    mod_l(A, C, A);
    div_l(A, B, B, C);
    l.W = CLintzuUWort32(B);
    l.V = CLintzuUWort32(C) + 1;
    return l;
  }

}


// Oliver Kullmann, 16.1.2002 (Swansea)
/* Copyright 2002 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/RandomGenerator/LiteralGeneratorAES.cpp
  \brief Implementations regarding the basic literal generator
*/


#include "BlockChiffAES.hpp"
#include "LiteralGeneratorAES.hpp"

#ifdef TESTENLITERALGEN
#  include <cstdio>
#  include <iostream>
#  include <cstdlib>
#endif

using namespace std;

namespace {

  using namespace LGAES;

  inline void UWortzuBytes(UWort32 W, BCAES::Byte * const Z) {
// Es sei b_{31}, ..., b_{0} die Binaerdarstellung von W (mit b_{31} als
// dem hoechstwertigen Bit). Es werden dann die vier Binaerzaehlen 
// b_{31}, ..., b_{24}
// b_{23}, ..., b_{16}
// b_{15}, ..., b_{8}
// b_{7}, ..., b_{0}
// in *Z, ..., *(Z+3) abgespeichert.
// Ist Argument W mit Vorzeichen versehen (der Wert, da eingesetzt wurde),
// so muss man W >= 0 voraussetzen, um Implementations-Unabhaengigkeit
// zu garantieren.
    BCAES::Byte *H = Z+4; // eine Position nach der letzten ist erlaubt
    do {
      H--;
      *H = W;
      W >>= 8;
    } while (H > Z);
  }

  inline SWort32 ModAdd(const SWort32 a, const SWort32 b, const UWort32 m) {
// Voraussetzungen: - floor(m/2) <= a, b <= ceil(m/2) - 1
//                  m >= 1
// Das Ergebnis z erfuellt
//    z = a + b mod m
//    - floor(m/2) <= z <= ceil(m/2) - 1

    if ( ((a <= 0) && (b >= 0)) || ((a >= 0) && (b <= 0)) )
      return a + b;

    if ((a < 0) && (b < 0)) {
      UWort32 a1 = -a;
      UWort32 b1 = -b;
      if (a1 + b1 <= m / 2)
	return a + b;
      return m - (a1 + b1);
    }

    {
      UWort32 a1 = a;
      UWort32 b1 = b;
      UWort32 m2 = (m % 2 == 0) ? m / 2 - 1 : m / 2;
      if (a1 + b1 <= m2)
	return a + b;
      return - SWort32(m - (a1 + b1));
    }
  }

  inline unsigned char EinzelBit(const unsigned int a, const unsigned int i) {
// Ausgabe ist Bit i von a, wobei vom niedrigst-wertigen Bit begonnen wird
// (das Bit 0 ist).

    return ((a & (1u << i)) > 0);
  }
}

namespace LGAES {

  SWort32 Literal(const Wort64 Schluessel, const Wort64 FormelNummer, const SWort32 Variablenzahl, const SWort32 Klausellaenge, const Wort64 Literalzaehler) {

// Voraussetzung: Variablenzahl >= 1, 0 <= Klausellaenge <= Variablenzahl

    BCAES::Wort_128 AESEingabe, AESSchluessel, AESAusgabe;

    UWortzuBytes(Schluessel[0], AESSchluessel);
    UWortzuBytes(Schluessel[1], AESSchluessel+4);
    UWortzuBytes(FormelNummer[0], AESSchluessel+8);
    UWortzuBytes(FormelNummer[1], AESSchluessel+12);

    UWortzuBytes(Variablenzahl, AESEingabe);
    UWortzuBytes(Klausellaenge, AESEingabe+4);
    UWortzuBytes(Literalzaehler[0], AESEingabe+8);
    UWortzuBytes(Literalzaehler[1], AESEingabe+12);
#ifdef TESTENLITERALGEN
    {
      for (int i = 0; i < 16; i++)
	printf("%x", AESSchluessel[i]);
      printf("\n");
      for (int i = 0; i < 16; i++)
	printf("%x", AESEingabe[i]);
      printf("\n");
      
    }
#endif
    BCAES::BlockChiffreAES(AESEingabe, AESSchluessel, AESAusgabe);

#ifdef TESTENLITERALGEN
    {
      for (int i = 0; i < 16; i++)
	printf("%x", AESAusgabe[i]);
      printf("\n");
    }
#endif

    int i, j;
    SWort32 Wertigkeit = (Variablenzahl >= 2) ? 1 : -1, Res = 0;
    UWort32 Modulus = 2 * UWort32(Variablenzahl);
    for (i = 15; i >= 0; i--) {
      BCAES::Byte a = AESAusgabe[i];
      for (j = 0; j < 8; j++) {
	if (EinzelBit(a, j))
	  Res = ModAdd(Res, Wertigkeit, Modulus);
	Wertigkeit = ModAdd(Wertigkeit, Wertigkeit, Modulus);
      }
    }
    if (Res >= 0)
      Res++;
    else
      Res = -Variablenzahl - 1 - Res;
    return Res;
  }

}


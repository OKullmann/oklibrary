// Oliver Kullmann, 25.1.2002 (Swansea)
/* Copyright 2002 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/RandomGenerator/VEinKlmGeneratorAES.cpp
  \brief Implementations regarding the generator for single non-boolean clause-sets
*/


#include <cstdio>
#include <algorithm>

#include "VEinKlmGeneratorAES.hpp"
#include "VLiteralGeneratorAES.hpp"

using namespace std;

namespace {

  char *Selbst = "VEinKlmGeneratorAES";

  const char *Klauselanfang = "(";
  const char *Klauselende = ")\n";
  const char *Trenner = ",";
  const char *Literalanfang = "(";
  const char *Literalende = ")";
  const char *Werttrenner = ",";
  bool XML = false;
  const char * const Einrueckung = "    ";
}

namespace VEKLMAES {

  using namespace VLGAES;

  int VEinfKlm(const Wort64 Schluessel, const UWort32 Nummer, const UWort32 n, const UWort32 d, const UWort32 p, const UWort32 c) {

    if (p > n) {
      fprintf(stderr, "FEHLER[%s]: Die Klauselnlaenge darf hoechstens so gross wie die Variablenzahl %ld sein, sie ist aber %ld!\n", Selbst, n, p);
      return 1;
    }

    Wort64 Literalzaehler = {};
    UWort32 *Gezogen = new UWort32[p];
    if (Gezogen == 0) {
      fprintf(stderr, "FEHLER[%s]: Es konnte kein dynamisches Feld der Groesse %d erzeugt werden!", Selbst, p);
      return 1;
    }
    for (unsigned int i = 0; i < c; i++) {
      if (XML)
	printf("%s<g_clause>", Einrueckung);
      else
	printf("%s", Klauselanfang);
      for (unsigned int j = 0; j < p; j++) {
	if (j != 0 && ! XML)
	  printf("%s", Trenner);
	VLiterale l = VLGAES::VLiteral(Schluessel, Nummer, n-j, p, d, Literalzaehler);
	UWort32 v = l.V;
	UWort32 * const Ende = Gezogen + j, *k0 = Gezogen, *k = k0;
	while (k0 < Ende) {
	  k = upper_bound(k0, Ende, v);
	  if ((k < Ende) && (*k == v)) k++;
	  v += k - k0;
	  if ((k == Ende) || (v < *k)) break;
	  k0 = k;
	}
	for (UWort32 *k2 = Ende; k2 > k; k2--)
	  *k2 = *(k2-1);
	*k = v;
	if (XML)
	  printf("<g_literal var = \"%ld\" val = \"%ld\" />", v, l.W);
	else
	  printf("%s%ld%s%ld%s", Literalanfang, v, Werttrenner, l.W, Literalende);
	if (++(Literalzaehler[1]) == 0)
	  Literalzaehler[0]++;
      }
      if (XML)
	printf("</g_clause>\n");
      else
	printf("%s", Klauselende);
    }
    delete Gezogen;
  }


  void setzenParameter(const char *Anfang, const char *Ende, const char *Literaltrenner, const char *LAnfang, const char *LEnde, const char *WTrenner) {
    Klauselanfang = Anfang;
    Klauselende = Ende;
    Trenner =  Literaltrenner;
    Literalanfang = LAnfang;
    Literalende = LEnde;
    Werttrenner = WTrenner;
    XML = false;
  }

  void setzenXML() {
    XML = true;
  }

}



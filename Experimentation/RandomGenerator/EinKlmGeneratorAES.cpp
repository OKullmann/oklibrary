// Oliver Kullmann, 18.1.2002 (Swansea)
/* Copyright 2002 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/RandomGenerator/EinKlmGeneratorAES.cpp
  \brief Implementations regarding the generator for a single clause-set
*/


#include <cstdio>
#include <algorithm>

#include "EinKlmGeneratorAES.hpp"
#include "LiteralGeneratorAES.hpp"

using namespace std;

namespace {

  char *Selbst = "EinKlmGeneratorAES";

  const char *Klauselanfang = "(";
  const char *Klauselende = ")\n";
  const char *Trenner = ",";
  bool XML = false;
  const char * const Einrueckung = "    ";
}

namespace EKLMAES {

  using namespace LGAES;

  int EinfKlm(const Wort64 Schluessel, const Wort64 Nummer, const SWort32 n, const SWort32 p, const UWort32 c) {
    if (n < 0) {
      fprintf(stderr, "FEHLER[%s]: Die Variablenzahl darf nicht negativ sein, sie ist aber %ld!\n", Selbst, n);
      return 1;
    }
    if (p < 0) {
      fprintf(stderr, "FEHLER[%s]: Die Klauselnlaenge darf nicht negativ sein, sie ist aber %ld!\n", Selbst, p);
      return 1;
    }
    if (p > n) {
      fprintf(stderr, "FEHLER[%s]: Die Klauselnlaenge darf hoechstens so gross wie die Variablenzahl %ld sein, sie ist aber %ld!\n", Selbst, n, p);
      return 1;
    }

    Wort64 Literalzaehler = {};
    SWort32 *Gezogen = new SWort32[p];
    if (Gezogen == 0) {
      fprintf(stderr, "FEHLER[%s]: Es konnte kein dynamisches Feld der Groesse %d erzeugt werden!", Selbst, p);
      return 1;
    }
    for (unsigned int i = 0; i < c; i++) {
      if (XML)
	printf("%s<clause>", Einrueckung);
      else
	printf("%s", Klauselanfang);
      for (int j = 0; j < p; j++) {
	if (j != 0 && ! XML)
	  printf("%s", Trenner);
	SWort32 l = LGAES::Literal(Schluessel, Nummer, n-j, p, Literalzaehler);
	SWort32 v = abs(l);
	SWort32 * const Ende = Gezogen + j, *k0 = Gezogen, *k = k0;
	while (k0 < Ende) {
	  k = upper_bound(k0, Ende, v);
	  if ((k < Ende) && (*k == v)) k++;
	  v += k - k0;
	  if ((k == Ende) || (v < *k)) break;
	  k0 = k;
	}
	for (int *k2 = Ende; k2 > k; k2--)
	  *k2 = *(k2-1);
	*k = v;
	if (XML)
	  printf("<literal var = \"%ld\" s = \"%c\" />", v, (l > 0) ? 'p' : 'n');
	else
	  printf("%ld", (l > 0) ? v : - v );
	if (++(Literalzaehler[1]) == 0)
	  Literalzaehler[0]++;
      }
      if (XML)
	printf("</clause>\n");
      else
	printf("%s", Klauselende);
    }
    delete Gezogen;
  }


  void setzenParameter(const char *Anfang, const char *Ende, const char *Literaltrenner) {
    Klauselanfang = Anfang;
    Klauselende = Ende;
    Trenner =  Literaltrenner;
    XML = false;
  }

  void setzenXML() {
    XML = true;
  }

}



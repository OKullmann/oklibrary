// Oliver Kullmann, 24.2.2002 (Swansea)
/* Copyright 2002 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <vector>
#include <list>
#include <string>

#include <OKlib/General/EndlAuto.hpp>

namespace {

  char *Selbst = "EndlAuto";

  const int AnzahlSprachen = 2;
  KMZ::Sprachen Sprache = KMZ::Deutsch;

  const char * const Version = "1.0";
  const char * const Datum = "3.3.2002";
  const char * const Autor = "Oliver Kullmann (Swansea); O.Kullmann@Swansea.ac.uk";
  const char * const Uebersetzungsdatum = __DATE__ " " __TIME__; // Gnu-Uebersetzung

  const char * const Meldungen[][AnzahlSprachen] = {
    {"Eine Zustandsnummer in der Tabelle der Uebergaenge fuer den Fall, dass keine der Transitionsfunktion anspringt, ist zu gross.", // 0
     "One state number in the table of default transitions is too big."},
    {"Eine Zustandsnummer in der Tabelle der Transitionen ist zu gross.",
     "One state number in the table of transitions is too big."},
  };

  const std::string Praefix = std::string("[") + std::string(Selbst) + std::string("] ");

}

namespace EndlicherAutomat {

  Automat::Automat (const Transition Tabelle[], unsigned int AnzTrans, const Zustaende SonstUebergaenge[], const unsigned int AnzZustaende) {
    AnzahlZustaende = AnzZustaende;
    AnzahlTransitionen = AnzTrans;
    SonstigeUebergaenge = SonstUebergaenge;
    T = std::vector< std::list<Pfeil> >(AnzahlZustaende);

    for (const Zustaende * i = SonstigeUebergaenge; i != SonstigeUebergaenge + AnzahlZustaende; i++)
      if (*i >= AnzahlZustaende)
	throw Initialisierungsfehler(Praefix + std::string(Meldungen[0][Sprache]));

    for (unsigned int i = 0; i < AnzahlTransitionen; i++) {
      Transition t = Tabelle[i];
      if ((t.von >= AnzahlZustaende) || (t.nach >= AnzahlZustaende))
	throw Initialisierungsfehler(Praefix + std::string(Meldungen[1][Sprache]));
      T[t.von].push_back(Pfeil(t.nach, t.f));
    }
  }

  Zustaende Automat::Uebergang(const std::string Wort) {
    Zustaende Zustand = 0;
    for (std::string::const_iterator i = Wort.begin(); i < Wort.end(); i++) {
      char c = *i;
      std::list<Pfeil>::const_iterator j;
      for (j = T[Zustand].begin(); j != T[Zustand].end(); j++) {
	if ((j -> f)(c))
	  break;
      }
      if (j == T[Zustand].end())
	Zustand = SonstigeUebergaenge[Zustand];
      else
	Zustand = j -> z;
    }
    return Zustand;
  }

}

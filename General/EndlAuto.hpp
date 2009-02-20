// Oliver Kullmann, 24.2.2002 (Swansea)
/* Copyright 2002 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef WAECHTERENDLAUTO

#include <list>
#include <vector>
#include <string>

#include <OKlib/General/Kommandozeile.hpp>

namespace EndlicherAutomat {

  typedef unsigned int Zustaende;
  
  typedef bool (*TransTest) (char);

  struct Transition {
    Zustaende von, nach;
    TransTest f;
  };

  struct Initialisierungsfehler {
    Initialisierungsfehler(std::string B2) {B = B2;}
    std::string B;
  };

  class Automat {

  public :

    Automat(const Transition Tabelle[], unsigned int AnzTrans, const Zustaende SonstUebergaenge[], unsigned int AnzZustaende);

    Zustaende Uebergang(const std::string Wort);
    // Anfangszustand ist Zustand Nr. 0

  private :

    struct Pfeil {
      Pfeil(const Zustaende zn, const TransTest fn) {
	z = zn; f = fn;
      }
      Zustaende z;
      TransTest f;
    };

    unsigned int AnzahlTransitionen;
    unsigned int AnzahlZustaende;

    const Zustaende * SonstigeUebergaenge;

    std::vector< std::list<Pfeil> > T;

  };

}

#endif

#define WAECHTERENDLAUTO


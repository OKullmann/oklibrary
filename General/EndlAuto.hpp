// Oliver Kullmann, 24.2.2002 (Swansea)

#ifndef WAECHTERENDLAUTO

#include <list>
#include <vector>
#include <string>

#include "Kommandozeile.hpp"

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


// Oliver Kullmann, 24.2.2002 (Swansea)
/* Copyright 2002 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/RandomGenerator/DeskriptorVerarbeitung.cpp
  \brief Implementations regarding the experiment descriptors
*/


// If
// NODATABASE
// is defined, then the (PostgreSQL) database is not used.

#include <istream>
#include <ostream>

#include <string>
#include <numeric>
#include <deque>
#include <map>
#include <list>
#include <cctype>
#include <cstdio> // wg. der Schnittstelle zur "Kommandozeile"

#include "DeskriptorVerarbeitung.hpp"

#include "EndlAuto.hpp"
#include "Kommandozeile.hpp"
#include "Tools.hpp"

#ifndef NODATABASE
#include "DatabaseHandler01.hpp"
#endif

#include "flintpp.h"
// ACHTUNG: wegen (Makro-)Unvertraeglichkeit dies zuletzt!

// -----------------------------------------------

namespace {

  using namespace std;

  using namespace Deskriptorverarbeitung::Fehler;

  // zusaetzliche Fehler

  struct internerFehler : DeskriptorVerarbeitung {
    string B;
    internerFehler(string B2) : B(B2) { }
    void Ausgabe() const;
  };


  char *Selbst = "interprete_descriptor";

  const int AnzahlSprachen = 2;
  KMZ::Sprachen Sprache = KMZ::Englisch;

  const char * const Version = "1.13";
  const char * const Datum = "18.5.2003";
  const char * const Autor = "Oliver Kullmann (Swansea); O.Kullmann@Swansea.ac.uk";
  const char * const Uebersetzungsdatum = __DATE__ " " __TIME__; // Gnu-Uebersetzung


  const char * const Meldungen[][AnzahlSprachen] = {
    {"Syntaxfehler: Die folgende Eingabe stellt keine Zahl dar:", // 0
     "Syntax error: The following input is not a number:"},
    {"Syntaxfehler: Jede Aktion muss durch ein \";\" beendet werden.", // 1
     "Syntax error: Each action must be concluded with a \";\"."},
    {"Syntaxfehler: Es fehlt \"=\".", // 2
     "Syntax error: The symbol \"=\" is missing,"},
    {"Syntaxfehler: Unerkannte Anweisung:", // 3
     "Syntax error: Unknown command:"},
    {"Interner Fehler: ", // 4
     "Internal error: "},
    {"Syntaxfehler: Der Experimentdeskriptor muss N, n, l sowie cp oder dp setzen.", // 5
     "Syntax error: The experiment descriptor must specify N, n, l and cp or dp."},
    {"Interner Fehler: Eine Aktion ist unbestimmt!", // 6
     "Internal error: One action is undertermined!"},
    {"Syntaxfehler: Im Experimentdeskriptor darf keine der Setzungen fuer s0, s1, nr0, nr1, N, n und sat mehrmals vorkommen.", // 7
     "Syntax error: The experiment descriptor may not contain a specification for s0, s1, nr0, nr1, N, n or sat more than once."},
    {"Interner Fehler: Addition von Zahlen unterschiedlichen Typs!", // 8
     "Internal error: Addition of numbers of different types!"},
    {"Syntaxfehler: Nur fuer die Setzung von dp sind nicht-ganzzahlige Werte erlaubt, nicht jedoch fuer", // 9
     "Syntax error: Only for the specification of dp non-integer values are possible, but not for"},
    {"Ein Nenner darf nicht gleich Null sein!", // 10
     "A denominator must not be equal to zero!"},
    {"Interner Fehler: Vergleich von Zahlen unterschiedlichen Typs!", // 11
     "Internal error: Comparison of numbers of different types!"},
    {"Eine Schrittweite darf nicht Null sein, wenn der Anfangswert echt kleiner als der Endwert ist im Falle einer Aufwaertsbewegung, bzw. echt groesser im Falle einer Abwaertsbewegung.", // 12
     "The step width may not be zero, if the start value is strictly less than the end value in case of \"to\" resp. if the start value is strictly bigger than the end value in case of \"downto\"."},
    {"Interner Fehler: Das this-Objekt ist unbestimmt!", // 13
     "Internal error: The this-object is not determined!"},
    {"Interner Fehler: Das Zahl-Objekt ist nicht im Zustand \"Ganzzahl\"!", // 14
     "Internal error: The Zahl object is not in state \"Ganzzahl\"!"},
    {"Vor dem ersten Auftreten einer Setzung dp oder cp muss die Klausellaenge l spezifiziert werden.", // 15
     "Before the first occurrence of setting dp or cp the clause length l must have been set."},
    {"Vor dem ersten Auftreten einer Setzung dp muss die Variablenanzahl n spezifiziert werden.", // 16
     "Before the first occurrence of setting dp the number n of variables must have been set."},
    {"Interner Fehler: Kein Ueberschreiben von Objekten vom Typ Zahl moeglich!", // 17
     "Internal error: Overwriting of objects of type Zahl is not possible!"},
    {"Bearbeitungsfehler: Schluessel zu gross:", // 18
     "Processing error: seed too big:"},
    {"Bearbeitungsfehler: Eine zu grosse Formelnummer ist erreicht worden:", // 19
     "Processing error: the formula number reached is too big:"},
    {"Bearbeitungsfehler: Der SAT-Parameter muss eine Zahl von 0 bis 3 ist, ist aber gleich", // 20
     "Processing error: the SAT parameter must be a number from 0 to 3, but that number is"},
    {"", // 21
     ""},
    {"Bearbeitungsfehler: Variablenzahl zu gross:", // 22
     "Processing error: the number of variables is too big:"},
    {"Bearbeitungsfehler: Variablenzahl ist Null.", // 23
     "Processing error: the number of variables is zero."},
    {"Bearbeitungsfehler: Klausellaenge ist Null.", // 24
     "Processing error: clause length is zero."},
    {"Bearbeitungsfehler: Die Klausellaenge darf hoechstens so gross sein wie die Anzahl der Variablen, sie ist aber gleich", // 25
     "Processing error: the clause length must not be greater than the number of variables, but it is"},
    {"", // 26
     ""},
    {"Bearbeitungsfehler: Die Klauselnanzahl ist zu gross:", // 27
     "Processing error: the clause length is too big:"},
    {"Bearbeitungsfehler: Die Domaingroesse muss mindestens gleich 2 sein, sie ist aber gleich", // 28
     "Processing error: the domain size must be at least two, but it is"},
    {"Bearbeitungsfehler: Die Domaingroesse ist zu gross:", // 29
     "Processing error: the domain size is too big:"},
    {"%s, Version %s vom %s; Autor  %s; Uebersetzungsdatum %s\n", // 30
     "%s, Version %s from %s; author %s; compilation date %s\n"},
    {"Programmname, Versionsnummer, Fertigstellungsdatum, Autor (mit e-Post-Adresse), Uebersetzungsdatum", // 31
     "Name of program, version number, date of completion, author (with e-mail-address), compilation date"},
    {"Normalerweise wird das DIMACS-Format benutzt (mittels der Option \"-D\" fuer OKgenerator). Wird mittels \"f=\" eine Zeichenkette eingelesen, dann wird diese anstelle von \"-D\" an OKgenerator uebergeben. (Die Syntax jener Zeichenkette wird nicht ueberprueft.)", // 32
     "The default output format is the DIMACS format (created by using the option \"-D\" for OKgenerator). If by \"f=\" a string is given, then this string replaces the option \"-D\". (The syntax of that string is not checked.)"},
    {"FEHLER", // 33
     "ERROR"},
    {"Dateifehler:", // 34
     "File error:"},
    {"Der Eingabestrom ist in einem schlechten Zustand.", // 35
     "The input stream is in a bad condition."},
    {"Der Ausgabestrom ist in einem schlechten Zustand.", // 36
     "The output stream is in a bad condition."},
    {"Lesefehler vom Eingabestrom.", // 37
     "Reading error from the input stream."},
    {"Der aktuelle Schluessel konnte nicht ermittelt werden.", // 38
     "The current key could not be determined."},
    {"Der Wertebereich des Schluessels ist erschoepft(!).", // 39
     "No more possible key values(!)."},
    {"Datenbankfehler:", // 40
     "Database error:"},
    {"Ohne Datenbankzugriff muss Schluessel oder Formelnummer spezifiziert werden.", // 41
     "Without having access to a database, either seed or formula number (or both) must be specified."},
    {"Der Name des Generators (evtl. inklusive Pfad). Voreingestellt ist \"OKgenerator\".", // 42
     "The name of the generator (if necessary, inclusive path). Default is \"OKgenerator\"."},
  };

  string Generatorname; // global variable for function Verarbeitung
  string special_generatorname;
  bool special_generator = false;

#ifndef NODATABASE
  const string OKDatabaseName = "OKRandGen";
#endif

  bool spezFormat = false;
  const char * spezFormatzeichenkette;

  bool Einlesen = true;

  using namespace KMZ;

  bool AktionVersion(const GenArg * const A) {
    printf(Meldungen[30][Sprache], Selbst, Version, Datum, Autor, Uebersetzungsdatum);
    Einlesen = false;
    return true;
  }
  void EVersion() {
    cout << Meldungen[31][Sprache];
  }
  bool AktionFormat(const GenArg * const A) {
    spezFormat = true;
    spezFormatzeichenkette = A -> s;
  }
  void EFormat() {
    cout << Meldungen[32][Sprache];
  }
  bool AktionGenerator(const GenArg * const A) {
    special_generator = true;
    special_generatorname = A -> s;
  }
  void EGenerator() {
    cout << Meldungen[42][Sprache];
  }


  Argumentstruktur Optionen[] = {
    {"-v", "--version", KEin, 0, AktionVersion, EVersion},
    {"f", "--format", ZEin, 0, AktionFormat, EFormat},
    {"g", "--generator", ZEin, 0, AktionGenerator, EGenerator},
  };
  const int AnzahlOptionen = sizeof(Optionen) / sizeof(Optionen[0]);


  // -----------------------------------------------------

  // Zahlen

  inline string lint2string(const LINT& x) {
    return x.lint2str(10);
  }
  // ACHTUNG: Erst diese Umwandlung macht, das in einer Ausgabekette
  // zwei LINT's ausgegeben werden koennen (da lint2str nur *einen*
  // Puffer benutzt!).

  inline LINT Potenz10(unsigned int p) {
    return LINT(string("1") + string(p, '0'), 10);
  }
  
  
  class DezBruch {
  public :
    
    DezBruch (const string ZZ, const string ZS);
    DezBruch (const LINT a) {z = a; s = 0;}
    
    DezBruch& add (const DezBruch);
    string aus() const;
    
    LINT Hauptzahl () const { return z; }
    unsigned int Stellen () const { return s; }

    bool operator == (const DezBruch&) const;
    bool operator < (const DezBruch&) const;
    bool operator <= (const DezBruch&) const;
    
  private :
    
    LINT z; // Zahl
    unsigned int s; // Nachkommastellen
  };
  
  inline DezBruch::DezBruch(const string ZZ, const string ZS) {
    z = LINT(ZZ, 10) * Potenz10(ZS.size()) + LINT(ZS, 10);
    s = ZS.size();
  }
  
  inline DezBruch& DezBruch::add(const DezBruch D) {
    unsigned int m = max(s, D.s);
    z = z * Potenz10(m - s) + D.z * Potenz10(m - D.s);
    s = m;
  }
  
  inline string DezBruch::aus() const {
    string A(lint2string(z));
    if (s >= A.size())
      A = string(s + 1 - A.size(), '0') + A;
    A.insert(A.end() - s, 1, '.');
    return A;
  }

  inline bool DezBruch::operator == (const DezBruch& D) const {
    return z * Potenz10(D.s) == D.z * Potenz10(s);
  }

  inline bool DezBruch::operator < (const DezBruch& D) const {
    return z * Potenz10(D.s) < D.z * Potenz10(s);
  }
  
  inline bool DezBruch::operator <= (const DezBruch& D) const {
    return z * Potenz10(D.s) <= D.z * Potenz10(s);
  }
  
  
  class Bruch {
  public :
    
    Bruch (const string ZZ, const string ZN);
    Bruch (const LINT Z) {z = Z; n = 1;}
    Bruch (const DezBruch D);
    
    Bruch& add (const Bruch);
    string aus () const;

    LINT Zaehler() const {return z;}
    LINT Nenner() const {return n;}

    bool operator == (const Bruch&) const;
    bool operator < (const Bruch&) const;
    bool operator <= (const Bruch&) const;

  private :
    
    LINT z; // Zaehler
    LINT n; // Nenner
    void kuerzen ();
  };
  
  inline Bruch::Bruch(const string ZZ, const string ZN) {
    n = LINT(ZN.c_str());
    if (n == 0)
      throw NullNenner();
    z = LINT(ZZ.c_str());
    this -> kuerzen();
  }
  
  inline Bruch::Bruch(const DezBruch D) {
    z = D.Hauptzahl();
    n = Potenz10(D.Stellen());
    this -> kuerzen();
  }
  
  inline Bruch& Bruch::add(const Bruch B) {
    z = z * B.n + B.z * n;
    n *= B.n;
    this -> kuerzen();
  }
  
  inline string Bruch::aus () const {
    return lint2string(z) + string("/") + lint2string(n);
  }
  
  inline void Bruch::kuerzen() {
    LINT t = gcd(z, n);
    z /= t; n /= t;
  }

  inline bool Bruch::operator == (const Bruch& B) const {
    return z * B.n == B.z * n;
  }

  inline bool Bruch::operator < (const Bruch& B) const {
    return z * B.n < B.z * n;
  }
  
  inline bool Bruch::operator <= (const Bruch& B) const {
    return z * B.n <= B.z * n;
  }
  
  enum Zahltypen {Zunbestimmt, Ganzzahl, Dezimalzahl, Bruchzahl};
  // Reihenfolge: von groesser werdender Allgemeinheit
  // Unbestimmt: wird nicht benutzt

  class Zahl {

  public :

    Zahl();
    Zahl(unsigned int);
    Zahl(LINT);
    Zahl(string S);

    Zahl(const Zahl&);

    Zahl& operator = (const Zahl&);

    Zahltypen Typ () const {return t;}

    Zahl& Umwandlung(Zahltypen);

    Zahl& add (const Zahl); // setzt voraus, dass beide vom gleichen Typ sind
    string aus () const;

    bool operator == (const Zahl&) const;
    bool operator < (const Zahl&) const;
    bool operator <= (const Zahl&) const;
    // jeweils nur bei Typgleichheit

    bool ist_Null () const;

    LINT ganzerWert() const; // setzt t == Ganzzahl voraus
    LINT Produkt(LINT) const; // mit Rundung
    
  private :

    Zahltypen t;
    union {
      LINT *g;
      DezBruch *d;
      Bruch *b;
    };

    void Vereinfachung (); // testet, ob die Zahl nicht doch ganz ist
    // (wird bei der Eingabe angewendet)

  };

  inline Zahl::Zahl() {
    t = Zunbestimmt;
  }

  inline Zahl::Zahl(unsigned int n) {
    t = Ganzzahl;
    g = new LINT(n);
  }
  inline Zahl::Zahl(const LINT n) {
    t = Ganzzahl;
    g = new LINT(n);
  }

  inline Zahl::Zahl(const Zahl& z) {
    t = z.t;
    switch (t) {
    case Ganzzahl :
      g = new LINT(*(z.g)); break;
    case Dezimalzahl :
      d = new DezBruch(*(z.d)); break;
    case Bruchzahl :
      b = new Bruch(*(z.b)); break;
    }
  }

  inline Zahl& Zahl::operator = (const Zahl& z) {
    if (t != Zunbestimmt)
      throw internerFehler(Meldungen[17][Sprache]);
    t = z.t;
    switch (t) {
    case Ganzzahl :
      g = new LINT(*(z.g)); break;
    case Dezimalzahl :
      d = new DezBruch(*(z.d)); break;
    case Bruchzahl :
      b = new Bruch(*(z.b)); break;
    }
    return *this;
  }

  inline Zahl& Zahl::Umwandlung(Zahltypen z) {
    if (z >= t) {
      if (z == Dezimalzahl && t == Ganzzahl) {
	DezBruch *p = new DezBruch(*g); // (unnoetiges Kopieren)
	delete g;
	d = p;
	t = Dezimalzahl;
      }
      else if (z == Bruchzahl) {
	if (t == Ganzzahl) {
	  Bruch *p = new Bruch(*g); // (unnoetiges Kopieren)
	  delete g;
	  b = p;
	  t = Bruchzahl;
	}
	else if (t == Dezimalzahl) {
	  Bruch *p = new Bruch(*d); // (unnoetiges Kopieren)
	  delete d;
	  b = p;
	  t = Bruchzahl;
	}
      }
    }
    return *this;
  }

  inline Zahl& Zahl::add(const Zahl Z) {
    if (t != Z.t)
      throw internerFehler(Meldungen[8][Sprache]);
    switch (t) {
    case Ganzzahl :
      *g += *(Z.g);
      break;
    case Dezimalzahl :
      d -> add(*(Z.d));
      break;
    case Bruchzahl :
      b -> add(*(Z.b));
      break;
    }
  }

  inline string Zahl::aus() const {
    switch(t) {
    case Ganzzahl :
      return lint2string(*g);
    case Dezimalzahl :
      return d -> aus();
    case Bruchzahl :
      return b -> aus();
    case Zunbestimmt :
      return "";
    }
  }

  inline bool Zahl::operator == (const Zahl& Z) const {
    if (t != Z.t)
      throw internerFehler(Meldungen[11][Sprache]);
    switch (t) {
    case Ganzzahl :
      return *g == *(Z.g);
    case Dezimalzahl :
      return *d == *(Z.d);
    case Bruchzahl :
      return *b == *(Z.b);
    }
  }

  inline bool Zahl::operator < (const Zahl& Z) const {
    if (t != Z.t)
      throw internerFehler(Meldungen[11][Sprache]);
    switch (t) {
    case Ganzzahl :
      return *g < *(Z.g);
    case Dezimalzahl :
      return *d < *(Z.d);
    case Bruchzahl :
      return *b < *(Z.b);
    }
  }

  inline bool Zahl::operator <= (const Zahl& Z) const {
    if (t != Z.t)
      throw internerFehler(Meldungen[11][Sprache]);
    switch (t) {
    case Ganzzahl :
      return *g <= *(Z.g);
    case Dezimalzahl :
      return *d <= *(Z.d);
    case Bruchzahl :
      return *b <= *(Z.b);
    }
  }

  inline bool Zahl::ist_Null () const {
    return (t == Ganzzahl && *g == 0) || (t == Dezimalzahl && d -> Hauptzahl() == 0) || (t == Bruchzahl && b -> Zaehler() == 0);
  }

  inline LINT Zahl::ganzerWert() const {
    if (t != Ganzzahl)
      throw internerFehler(Meldungen[14][Sprache]);
    return *g;
  }

  inline LINT Zahl::Produkt(LINT Z) const {
    if (t == Zunbestimmt)
      throw internerFehler(Meldungen[13][Sprache]);
    if (t == Ganzzahl)
      return *g * Z;
    if (t == Dezimalzahl) {
      LINT P = (d -> Hauptzahl() * Z);
      LINT Pot = Potenz10(d -> Stellen());
      LINT q = P / Pot;
      if (2 * (P % Pot) >= Pot)
	q++;
      return q;
    }
    LINT P = (b -> Zaehler() * Z);
    LINT q = P / b -> Nenner();
    if (2 * (P % b -> Nenner()) >= b -> Nenner())
      q++;
    return q;
  }

  inline void Zahl::Vereinfachung() {
    if (t == Dezimalzahl && d -> Hauptzahl() % Potenz10(d -> Stellen()) == 0) {
      LINT *p = new LINT(d -> Hauptzahl());
      *p /= Potenz10(d -> Stellen());
      delete d;
      g = p;
      t = Ganzzahl;
    }
    else if (t == Bruchzahl && b -> Nenner() == 1) {
      LINT *p = new LINT(b -> Zaehler());
      delete b;
      g = p;
      t = Ganzzahl;
    }
  }
  
  // Definition des endlichen Automaten zur Zahldarstellung

  using namespace EndlicherAutomat;

  bool TransZiffer(const char c) {
    return isdigit(c);
  }
  bool TransPunkt(const char c) {
    return c == '.';
  }
  bool TransGeteilt(const char c) {
    return c == '/';
  }

  /* Bedeutung der Zustaende:

  0 : Start
  1 : Ganzzahl
  2 : Uebergangszustand zu Dezimalbruch
  3 : Dezimalbruch
  4 : Uebergangszustand zu Bruch
  5 : Bruch
  6 : Fehler

  */

  const unsigned int AnzTrans = 9;
  
  Transition Tabelle [AnzTrans] = {
    {0, 1, TransZiffer},
    {0, 2, TransPunkt},
    {1, 1, TransZiffer},
    {1, 3, TransPunkt},
    {1, 4, TransGeteilt},
    {2, 3, TransZiffer},
    {3, 3, TransZiffer},
    {4, 5, TransZiffer},
    {5, 5, TransZiffer},
  };

  const unsigned int AnzZustaende = 7;

  unsigned int SonstUebergaenge [AnzZustaende] = {6, 6, 6, 6, 6, 6, 6};

  inline Zahl::Zahl (string S) {

    static Automat A(Tabelle, AnzTrans, SonstUebergaenge, AnzZustaende);

    Zustaende Endzustand = A.Uebergang(S);
    switch (Endzustand) {
    case 1 :
      t = Ganzzahl;
      g = new LINT(S.c_str());
      break;
    case 3 : {
      t = Dezimalzahl;
      size_t p = S.find_first_of('.');
      string ZS = S.substr(p+1);
      S.erase(p);
      d = new DezBruch(S, ZS);
      break;
    }
    case 5 : {
      t = Bruchzahl;
      size_t p = S.find_first_of('/');
      string N = S.substr(p+1);
      S.erase(p);
      b = new Bruch(S, N);
      break;
    }
    default :
      throw Syntaxfehler(Meldungen[0][Sprache], S);
    }
    this -> Vereinfachung();
  }


  class Bereich {

  public :

    Bereich(unsigned int);
    Bereich(LINT);
    Bereich(string);

    enum Schrittyp {konstant, aufwaerts, abwaerts};
    Schrittyp s;

    Zahl A, E, S; // Anfang, Ende, Schritt
    // E und S nur, wenn s != konstant
  };

  inline Bereich::Bereich(const unsigned int n) {
    s = konstant;
    A = Zahl(n);
  }
  inline Bereich::Bereich(const LINT n) {
    s = konstant;
    A = Zahl(n);
  }

  inline Bereich::Bereich(string Z) {
    size_t i;
    if ((i = Z.find("downto")) != string::npos) { // downto
      s = abwaerts;
      A = Zahl(Z.substr(0, i));
      Z.erase(0,i+6);
      if ((i = Z.find("step")) != string::npos) {
	E = Zahl(Z.substr(0,i));
	Z.erase(0,i+4);
	S = Zahl(Z);
      }
      else {
	E = Zahl(Z);
	S = Zahl(1);
      }
    }
    else if ((i = Z.find("to")) != string::npos) { // to
      s = aufwaerts;
      A = Zahl(Z.substr(0, i));
      Z.erase(0,i+2);
      if ((i = Z.find("step")) != string::npos) {
	E = Zahl(Z.substr(0,i));
	Z.erase(0,i+4);
	S = Zahl(Z);
      }
      else {
      	E = Zahl(Z);
	S = Zahl(1);
      }
    }
    else {
      s = konstant;
      A = Zahl(Z);
    }
  }


  // ------------------------------------------------------

  // Aktionen

  enum Aktionstypen {As, Anr, AN, An, Asat, Al, Acp, Adp, Ads, Aunbestimmt};
  // AN, An, Al sowie Adp oder Acp muessen vorkommen im Experiment-Deskriptor
  // es wird As0 = 0, ... benutzt

    static const unsigned int AnzEchterAktionen = 9;
    static const char * const KommandoSymbole[AnzEchterAktionen] = {"s", "nr", "N", "n", "sat", "l", "cp", "dp", "ds"};

  struct Symbolverzeichnis {

    Symbolverzeichnis();

    map<string, Aktionstypen> V;

  };

  Symbolverzeichnis::Symbolverzeichnis() {
    for (unsigned int i = 0; i != AnzEchterAktionen; i++)
      V[KommandoSymbole[i]] = Aktionstypen(i);
  }

  const Symbolverzeichnis Symbole;

  class Aktion {

  public :

    Aktion();

    Aktion(Aktionstypen, unsigned int);
    Aktion(Aktionstypen, LINT);
    Aktion(string); // die Anweisung (ohne ";")

    Aktionstypen t;

    deque<Bereich> B;
  };

  inline Aktion::Aktion() {
    t = Aunbestimmt;
  }

  inline Aktion::Aktion(Aktionstypen t0, unsigned int n) {
    t = t0;
    B.push_back(Bereich(n));
  }
  inline Aktion::Aktion(Aktionstypen t0, LINT n) {
    t = t0;
    B.push_back(Bereich(n));
  }

  inline Aktion::Aktion(string Z) {

    size_t i = Z.find('=');
    if (i == string::npos)
      throw Syntaxfehler(Meldungen[2][Sprache], Z);
    map<string,Aktionstypen>::const_iterator p = Symbole.V.find(Z.substr(0,i));
    if (p == Symbole.V.end())
      throw Syntaxfehler(Meldungen[3][Sprache], Z);
    t = p -> second;
    Z.erase(0, i+1);

    while ((i = Z.find(',')) != string::npos) {
      B.push_back(Bereich(Z.substr(0,i)));
      Z.erase(0, i+1);
    }
    B.push_back(Bereich(Z));
  }

  // ------------------------------------------------------------

  // Die zentrale Klasse der Aktionslisten

  const LINT S31("2147483648");
  const LINT S32("4294967296");
  const LINT S64(S32 * S32);

  class Aktionsliste {

  public :

    Aktionsliste(string, std::ostream&);

    void Bearbeitung() const;

  private :

    void rBearbeitung(deque<Aktion>::const_iterator i) const;
    // rekursive Bearbeitung

    void Kommandoausgabe() const;
    // Rekursionsbasis von rBearbeitung;

    deque<Aktion> L;

    bool bcls; // wird falsch, falls ds gesetzt wird

    bool seed; // wahr, falls seed gesetzt wurde
    bool nr; // wahr, falls nr gesetzt wurde

    mutable LINT fP[AnzEchterAktionen]; // "feste Parameter"
    mutable map<LINT, list<LINT> > vP; // "variable Parameter"

    mutable ostream& Ausgabestrom;

    // Testen von Zahlen
    
    void Test_sat() const {
      // Meaning: fp[Asat] =
      // 0 : only unsat
      // 1 : only sat
      // 2 : both
      // 3 : none
      if (fP[Asat] > 3)
	throw Bearbeitungsfehler(Meldungen[20][Sprache], lint2string(fP[Asat]));
    }
    void Test_s() const {
      if (fP[As] >= S64)
	throw Bearbeitungsfehler(Meldungen[18][Sprache], lint2string(fP[As]));
    }
    void Test_nr() const {
      if ((bcls && fP[Anr] + fP[AN] - 1 >= S64) || (! bcls && fP[Anr] + fP[AN] - 1 >= S32))
	throw Bearbeitungsfehler(Meldungen[19][Sprache], lint2string(fP[Anr] + fP[AN] - 1));
    }
    void Test_n() const {
      if (fP[An] == 0)
	throw Bearbeitungsfehler(Meldungen[23][Sprache], "");
      else if ((bcls && fP[An] >= S31) || (! bcls && fP[An] >= S32))
	throw Bearbeitungsfehler(Meldungen[22][Sprache], lint2string(fP[An]));
    }
    void Test_l(const LINT& l) const {
      if (l == 0)
	throw Bearbeitungsfehler(Meldungen[24][Sprache], "");
      else if (l > fP[An])
	throw Bearbeitungsfehler(Meldungen[25][Sprache], lint2string(l));
    }
    void Test_c(const LINT& c) const {
      if (c >= S32)
	throw Bearbeitungsfehler(Meldungen[27][Sprache], lint2string(c));
    }
    void Test_ds() const {
      if (fP[Ads] < 2)
	throw Bearbeitungsfehler(Meldungen[28][Sprache], lint2string(fP[Ads]));
      else if (fP[Ads] >= S32)
	throw Bearbeitungsfehler(Meldungen[29][Sprache], lint2string(fP[Ads]));
    }

  };

  // Implementations for Aktionsliste ------------------


  inline Aktionsliste::Aktionsliste(string S, std::ostream& Ausgabe)
    : bcls(true), Ausgabestrom(Ausgabe) {

    // Einlesen

    while (! S.empty()) {
      size_t i = S.find(';');
      if (i == string::npos)
	throw Syntaxfehler(Meldungen[1][Sprache], "");
      L.push_back(Aktion(S.substr(0, i)));
      S.erase(0, i+1);
    }

    // Counting of actions, and checking types

    unsigned int Anzahl[AnzEchterAktionen] = {};

    for (deque<Aktion>::iterator i = L.begin(); i != L.end(); i++) {

      Aktionstypen t = i -> t;

      if (t == Aunbestimmt)
	throw internerFehler(Meldungen[6][Sprache]);

      if ((t == Adp || t == Acp) && Anzahl[Al] == 0)
	throw l_unspezifiziert();
      if (t == Adp && Anzahl[An] == 0)
	throw n_unspezifiziert();

      Anzahl[t]++;

      if (t == Ads)
	bcls = false;

      for (deque<Bereich>::iterator j = i -> B.begin(); j != i -> B.end(); j++) {
	if (t == Adp) { // Angleichung der Zahltypen
	  if (j -> s != Bereich::konstant) {
	    Zahltypen MT = max(j -> A.Typ(), j -> E.Typ());
	    MT = max(MT, j -> S.Typ());
	    j -> A.Umwandlung(MT);
	    j -> E.Umwandlung(MT);
	    j -> S.Umwandlung(MT);
	  }
	}
	else // nur ganze Zahlen
	  if (j -> A.Typ() != Ganzzahl || j -> E.Typ() > Ganzzahl || j -> S.Typ() > Ganzzahl)
	    throw Syntaxfehler(Meldungen[9][Sprache], KommandoSymbole[t]);

	// pruefen auf Null-Schrittweite
	if (j -> s == Bereich::aufwaerts && j -> A < j -> E && j -> S.ist_Null())
	  throw NullSchrittweite();
	else if (j -> s == Bereich::abwaerts && j -> E < j -> A && j -> S.ist_Null())
	  throw NullSchrittweite();
      }
    } // end of counting

    if (Anzahl[An] == 0 || Anzahl[AN] == 0 || (Anzahl[Acp] == 0 && Anzahl[Adp] == 0))
      throw Syntaxfehler(Meldungen[5][Sprache], "");
      
    if (Anzahl[An] >= 2 || Anzahl[AN] >= 2 || Anzahl[As] >= 2 || Anzahl[Anr] >= 2 || Anzahl[Asat] >= 2)
      throw Syntaxfehler(Meldungen[7][Sprache], "");

    // Hinzufuegung der voreingestellten Werte (falls noetig)
    seed = Anzahl[As] != 0;
    nr = Anzahl[Anr] != 0;
    if (! seed && ! nr) { // Anfrage an die Datenbank zur Bestimmung des Schluessels
#ifdef NODATABASE
      throw seed_and_nr_unspecified();
#else
      DatabaseHandler01::DatabaseHandler DH(OKDatabaseName);
      DatabaseHandler01::TableHandler bcls_current_key(DH, "bcls_current_key");
      DatabaseHandler01::AttributeObject key0(bcls_current_key, "key0");
      DatabaseHandler01::AttributeObject key1(bcls_current_key, "key1");
      DatabaseHandler01::AttributeObject active(bcls_current_key, "active");
      active << "true";
       if (bcls_current_key.completion() != 1)
	throw DBFehler(Meldungen[38][Sprache]);
      unsigned int u_value_key0, u_value_key1;
      {
	int value_key0, value_key1;
	Tools::fromString(key0.getValue(), value_key0);
	Tools::fromString(key1.getValue(), value_key1);
	u_value_key0 = Tools::int4_to_uint4(value_key0);
	u_value_key1 = Tools::int4_to_uint4(value_key1);
      }
      L.push_front(Aktion(As, LINT(u_value_key1) + LINT(u_value_key0) * S32));
      L.push_front(Aktion(Anr, 0));
      // Aktualisierung des Schluessels
      DatabaseHandler01::Command command(DH);
      if (u_value_key1 < Tools::pow_2_31m1)
	++u_value_key1;
      else if (u_value_key0 < Tools::pow_2_31m1) {
	u_value_key1 = 0;
	 ++u_value_key0;
      }
      else
	throw DBFehler(Meldungen[39][Sprache]);
      ostringstream update;
      update << "update bcls_current_key set";
      update << " key0 = " << Tools::uint4_to_int4(u_value_key0);
      update << ", key1 = " << Tools::uint4_to_int4(u_value_key1);
      update << ";";
      command.issue(update.str());
#endif
    } // end of database access
    else {
      if (! seed)
	L.push_front(Aktion(As, 0));
      if (! nr)
	L.push_front(Aktion(Anr, 0));
    }
    if (Anzahl[Asat] == 0) {
      L.push_front(Aktion(Asat, 2));
    }
    
  }

  void Aktionsliste::Bearbeitung() const {
    rBearbeitung(L.begin());
  }

  void Aktionsliste::rBearbeitung(const deque<Aktion>::const_iterator i) const {
    if (i == L.end()) {
      this -> Kommandoausgabe();
      return;
    }
    
    Aktionstypen t = i -> t;
    if (t == Acp || t == Adp) {
      LINT aktl = fP[Al];
      map<LINT, list<LINT> >::iterator k = vP.find(aktl);
      if (k == vP.end())
	k = vP.insert(vP.begin(), make_pair(aktl, list<LINT>()));
      for (deque<Bereich>::const_iterator j = i -> B.begin(); j != i -> B.end(); j++) {
	if (j -> s == Bereich::konstant) {
	  if (t == Acp)
	    k -> second.push_back(j -> A.ganzerWert());
	  else
	    k -> second.push_back(j -> A.Produkt(fP[An]));
	  this -> rBearbeitung(i + 1);
	  // k = vP.find(aktl);
	  k -> second.pop_back();
	}
	else {
	  for (Zahl m(j -> A); m <= j -> E; m.add(j -> S)) {
	    if (t == Acp)
	      k -> second.push_back(m.ganzerWert());
	    else
	      k -> second.push_back(m.Produkt(fP[An]));
	    this -> rBearbeitung(i + 1);
	    // k = vP.find(aktl);
	    k -> second.pop_back();
	  }
	}
      }
    }
    else
      for (deque<Bereich>::const_iterator j = i -> B.begin(); j != i -> B.end(); j++)
	if (j -> s == Bereich::konstant) {
	  fP[t] = j -> A.ganzerWert();
	  this -> rBearbeitung(i + 1);
	}
	else
	  for (Zahl k(j -> A); k <= j -> E; k.add(j -> S)) {
	    fP[t] = k.ganzerWert();
	    this -> rBearbeitung(i + 1);
	  }
  }


  void Aktionsliste::Kommandoausgabe() const {

    const char* const Formatzeichenkette = "-D";

    Test_sat();
    Test_nr();

    string Ausgabe0 = Generatorname;

    Test_s();
    LINT s0(fP[As] / S32), s1(fP[As] % S32);
    Ausgabe0 += " s0=" + lint2string(s0) + " s1=" + lint2string(s1);

    Test_n();
    Ausgabe0 += " n=" + lint2string(fP[An]);

    bool eineKlausel = false;
    for (map<LINT, list<LINT> >::iterator k = vP.begin(); k != vP.end(); k++)
      if (! k -> second.empty()) {
	Test_l(k -> first);
	LINT c(accumulate(k->second.begin(), k -> second.end(), LINT(0)));
	if (c == 0)
	  continue;
	else {
	  eineKlausel = true;
	  Test_c(c);
	  Ausgabe0 += " l=" + lint2string(k->first) + " cp=" + lint2string(c);
	}
      }
    if (! eineKlausel)
      return;

    if (! bcls) {
      Test_ds();
      Ausgabe0 += " -g ds=" + lint2string(fP[Ads]);
    }

    Ausgabe0 += " ";
    if (spezFormat)
      Ausgabe0 += spezFormatzeichenkette;
    else
      Ausgabe0 += Formatzeichenkette;

    for (LINT i = fP[Anr]; i < fP[Anr] + fP[AN]; i++) {
      string Spec_nr;
      if (bcls) {
	LINT nr0(i / S32), nr1(i % S32);
	Spec_nr = " nr0=" + lint2string(nr0) + " nr1=" + lint2string(nr1);
      }
      else {
	LINT nr1(i % S32);
	Spec_nr = " nr1=" + lint2string(nr1);
      }
      string Spec_sat;
      if (fP[Asat] == 0)
	Spec_sat = "|0";
      else if (fP[Asat] == 1)
	Spec_sat = "|1";
      else if (fP[Asat] == 3)
	Spec_sat = "|count_only";
      Ausgabestrom << Ausgabe0 << Spec_nr << " -o" << Spec_sat << "\n";
    }
  }

}

// Die Hauptfunktion ---------------------------------

namespace Deskriptorverarbeitung {

  void Verarbeitung(std::istream& Ein, std::ostream& Aus, const std::string& generator) {

    if (Ein.bad())
      throw Eingabestrom();
    if (Aus.bad())
      throw Ausgabestrom();

    Generatorname = generator;
    
    string Eingabe;
    // Einlesen in "Eingabe", wobei Leerzeichen und Kommentare ignoriert werden
    // Achtung: "n = 88 9;" wird somit "n=889;"
    string Komponente;
    while (Ein >> Komponente) {
      if (Komponente[0] == '#')
	Ein.ignore(numeric_limits<streamsize>::max(), '\n');
      else
	Eingabe += Komponente;
    }
    if (Ein.bad())
      throw Lesefehler();

    // Bestimmung der Aktionsliste

    Aktionsliste Liste(Eingabe, Aus);

    Liste.Bearbeitung();

  }

}

// Fehler-Ausgaben --------------------------------------

#include <iostream>

namespace Deskriptorverarbeitung {
  namespace Fehler {

    void DeskriptorVerarbeitung::Ausgabe() const {
      std::cerr << Meldungen[33][Sprache] << "[" << Selbst << "] ";
    }

    void Syntaxfehler::Ausgabe() const {
      DeskriptorVerarbeitung::Ausgabe();
      std::cerr << M;
      if (! A.empty())
	std::cerr << " " << A << ".";
      std::cerr << "\n";
    }

    void DBFehler::Ausgabe() const {
      DeskriptorVerarbeitung::Ausgabe();
      std::cerr << M << "\n";
    }

    void Bearbeitungsfehler::Ausgabe() const {
      DeskriptorVerarbeitung::Ausgabe();
      std::cerr << M;
      if (! A.empty())
	std::cerr << " " << A << ".";
      std::cerr << "\n";
    }

    void NullNenner::Ausgabe() const {
      DeskriptorVerarbeitung::Ausgabe();
      std::cerr << Meldungen[10][Sprache] << "\n";
    }

    void NullSchrittweite::Ausgabe() const {
      DeskriptorVerarbeitung::Ausgabe();
      std::cerr << Meldungen[12][Sprache] << "\n";
    }

    void l_unspezifiziert::Ausgabe() const {
      DeskriptorVerarbeitung::Ausgabe();
      std::cerr << Meldungen[15][Sprache] << "\n";
    }

    void n_unspezifiziert::Ausgabe() const {
      DeskriptorVerarbeitung::Ausgabe();
      std::cerr << Meldungen[16][Sprache] << "\n";
    }

    void Dateifehler::Ausgabe() const {
      DeskriptorVerarbeitung::Ausgabe();
      std::cerr << Meldungen[34][Sprache] << "\n";
    }

    void Eingabestrom::Ausgabe() const {
      Dateifehler::Ausgabe();
      std::cerr << Meldungen[35][Sprache] << "\n";
    }
    void Ausgabestrom::Ausgabe() const {
      Dateifehler::Ausgabe();
      std::cerr << Meldungen[36][Sprache] << "\n";
    }
    void Lesefehler::Ausgabe() const {
      Dateifehler::Ausgabe();
      std::cerr << Meldungen[37][Sprache] << "\n";
    }

    void seed_and_nr_unspecified::Ausgabe() const {
      DeskriptorVerarbeitung::Ausgabe();
      std::cerr << Meldungen[41][Sprache] << "\n";
    }

    void ::internerFehler::Ausgabe() const {
      DeskriptorVerarbeitung::Ausgabe();
      std::cerr << B << "\n";
    }

  }
}

// Das Hauptprogramm ----------------------------------------------

#ifdef SELBSTSTAENDIG

int main(const int argc, const char * const argv[]) {

  if (argc != 1) {
    if (! KMZ::ArgumentBearbeitung(Selbst, Optionen, AnzahlOptionen, argv + 1, argc - 1, Sprache))
      return 1;
    if (argc != 2 || ! Einlesen)
      return 0;
  }

  try {
    if (special_generator)
      Deskriptorverarbeitung::Verarbeitung(std::cin, std::cout, special_generatorname);
    else
      Deskriptorverarbeitung::Verarbeitung(std::cin, std::cout);
    // Einlesen in "Eingabe", wobei Leerzeichen und Kommentare ignoriert werden
    // Achtung: "n = 88 9;" wird somit "n=889;"
  }
  catch (Deskriptorverarbeitung::Fehler::DeskriptorVerarbeitung& f) {
    f.Ausgabe();
    return 1;
  }
  catch (EndlicherAutomat::Initialisierungsfehler& f) {
    std::cerr << Meldungen[4][Sprache] << f.B << "\n";
    return 1;
  }
#ifndef NODATABASE
  catch (DatabaseHandler01::Error::General& f) {
    std::cerr << Meldungen[33][Sprache] << "[" << Selbst << "] ";
    std::cerr << Meldungen[40][Sprache] << "\n";
    std::cerr << f.what() << endl;
    return 1;
  }
#endif
}

#endif
  


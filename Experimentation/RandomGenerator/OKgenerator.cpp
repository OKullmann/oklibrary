// Oliver Kullmann, 19.1.2002 (Swansea)
/* Copyright 2002 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/RandomGenerator/OKgenerator.cpp
  \brief The OKgenerator
*/


#include <cstdio>
#include <cstdlib>
#include <cctype>

#include <deque>
#include <string>

#include "LiteralGeneratorAES.hpp"
#include "EinKlmGeneratorAES.hpp"
#include "VEinKlmGeneratorAES.hpp"
#include "Kommandozeile.hpp"
#include "Tools.hpp"

#include "flintpp.h"

using namespace std;

namespace {

  char *Selbst = "OKgenerator";

  const int AnzahlSprachen = 2;

  const char * const Meldungen[][AnzahlSprachen] = {
    {"Der maximale Wert hier ist 4294967295!\n", // 0
     "The maximal value here is 4294967295!\n"},
    {"Der maximale Wert hier ist 2147483647!\n", // 1
     "The maximal value here is 2147483647!\n"},
    {"Kein negativer Wert erlaubt!\n", // 2
     "No negative values possible!\n"},
    {"%s, Version %s vom %s; Autor  %s; Uebersetzungsdatum %s\n", // 3
     "%s, Version %s from %s; author %s; compilation date %s\n"},
    {"", // 4
     ""},
    {"Die Klauselnlaenge darf hoechstens so gross wie die Variablenzahl sein!\n", // 5
     "The clause-length must not be greater than the number of variables!\n"},
    {"Programmname, Versionsnummer, Fertigstellungsdatum, Autor (mit e-Post-Adresse), Uebersetzungsdatum", // 6
     "name of program, version number, date of completion, author (with e-mail-address), compilation date"},
    {"Die erste Zeile einer Klauselmenge (mit \"-o\" ausgegeben) ist eine Kommentarzeile mit den Parametern. Z.B. fuer normale Formeln:\n \% OKgenerator((0,1), (0,5), 300; 3, 100; 4, 30)\nStandardised density: 3 -> .333, 4 -> .1\n Hier ist \"%\" das Kommentarzeichen, der Schluessel ist (0, 1) (als Paar von 32-Bit-Zahlen), die Nummer der Formel ist (0, 5), die Variablenzahl ist 300, und es gibt 100 Klauseln der Laenge 3 und 30 Klauseln der Laenge 4.\nFuer verallgemeinerte Formeln hat man demgegenueber\n \% OKVRandGen((0,1), 5, 300, 7; 3, 100; 4, 30)\nStandardised density: 3 -> .333, 4 -> .1\n, wobei nun die Domaingroesse gleich 7 ist.\n", // 7
     "The first line of a clause-set output with \"-o\" is a comment containing the parameters. For example in the case of ordinary formulas:\n\% OKgenerator((0,1), (0,5), 300; 3, 100; 4, 30)\nStandardised density: 3 -> .333, 4 -> .1\n Here \"%\" is the comment symbol, the key is (0, 1) (as pair of 32-bit-numbers), the number of the formula is (0, 5), the number of variables is 300, and there are 100 clauses of length 3 and 30 clauses of length 4.\nFor generalised formulas an example is\n \% OKVRandGen((0,1), 5, 300, 7; 3, 100; 4, 30)\nStandardised density: 3 -> .333, 4 -> .1\n, where now 7 is the domain size.\n"},
    {"Die Ausgabe von Zufallsformeln geschieht ohne erklaerenden Kommentar.\n", // 8
     "No comment for the output of random formulas.\n"},
    {"Stellt auf das DIMACS-Format um:\n Kein Klauselanfangssymbol, Literale werden durch Leerzeichen getrennt, das Klauselende wird durch ein Leerzeichen, gefolgt von einer Null und einem Zeilenende-Zeichen markiert, Kommentare werden durch \"c\" eingeleitet, und nach dem initialen Kommentarblock wird eine spezielle Zeile \"p cnf n c\" ausgegeben, wobei n eine obere Schranke fuer die vorkommenden Variablennummern ist (hier gleich der eingestellten Variablenzahl) und c die Gesamtanzahl der Klauseln. Falls dieses Format schon aktiviert wurde, dann wird nun die Ausgabe der speziellen DIMACS-Parameter-Zeile (beginnend mit \"p\") unterbunden. Fuer verallgemeinerte Formeln ist die spezielle Paramterzeile von der From  \"p gcnf n c d\", wobei d die Domaingroesse ist.\n", // 9
     "The DIMACS format is used in the following for output:\n No clause-begin-symbol, literals are separated by spaces, the clause-end is marked by a space, followed by 0 and the clause-end-symbol, comment lines start with \"c\", and after the initial comment block a special line \"p cnf n c\" is printed, where n is an upper bound for the occurring variable indices (here we use just the number of variables from the parameter set), and c ist the total number of clauses. In case this format has already been activated, then the output of the special DIMACS parameter line is cancelled. For generalised formulas the special paramter line is \"p gcnf n c d\", where d is the domain size.\n"},
    {"Der Stapel mit Paaren (Klausellaenge, Klauselanzahl) wird geloescht.\n", // 10
     "The stack with pairs (clause-length, clause-number) is cleared.\n"},
    {"Das letzte Paar (Klausellaenge, Klauselanzahl) wird vom Stapel genommen.\n", // 11
     "The last pair (clause-length, clause-number) is eliminated from the stack.\n"},
    {"Die Ausgabe einer Zufalls-Formel schliesst mit einem zusaetzlichen Zeilenendezeichen ab.\n", // 12
     "The output of a random formula concludes with an additional end-of-line symbol.\n"},
    {"Ausgabe der Pseudo-Zufallsformel, bestimmt durch den aktuellen Schluessel, die aktuelle Formelnummer, die aktuelle Variablenzahl, die Domaingroesse (im Falle von verallgemeinerten Klauselmengen) und die Paare (Klausellaenge, Klauselanzahl) auf dem Stapel (aber in ihrer natuerlichen Reihenfolge).\n", // 13
     "Output of the random formula, using the current seed (key), formula number, number of variables, the domain size (in case of generalised formulas) and the pairs (clause-length, clause-number) from the stack (but now we have first in, first out).\n"},
    {"Eingabe des ersten Teils s0 des Schluessel (s0, s1), wobei s0, s1 Zahlen von 0 bis 2^32-1 = 4294967295 sind. Der Schluessel wird hierbei als Binaerzahl mit fuehrendem hoechstwertigem Bit interpretiert (somit ist der Schluessel gleich 2^32 * s0 + s1). Die Eingabe von Zahlen kann dezimal (Standard), oktal (mit fuehrender Null) oder hexadezimal (mit fuehrendem \"0x\" oder \"0X\") erfolgen. Eine negative Zahl -k wird als 2^32-k interpretiert.\n", // 14
     "Input of the low-order part s0 of the key (s0, s1), where s0, s1 are integers from 0 to 2^32-1 = 4294967295, and the key is interpreted as Big-Endian binary number (leading high-order bit). Numbers can be represented in the decimal system (default), in the octal system (with leading zero) or in the hexadezimal system (with leading \"0x\" oder \"0X\"). Negative numbers -k are interpretad as 2^32-k.\n"},
    {"Eingabe des zweiten Teils s1 des Schluessel (s0, s1). Siehe --seed0.\n", // 15
     "Input of the high-order part s1 of the key (s0, s1). See --seed0.\n"},
    {"Eingabe des ersten Teils m0 der Formelnummer (m0, m1), wobei m0, m1 Zahlen von 0 bis 2^32-1 = 4294967295 sind. Der Formelnumer wird hierbei als Binaerzahl mit fuehrendem hoechstwertigem Bit interpretiert (somit ist die Formelnummer gleich 2^32 * m0 + m1). Die Eingabe von Zahlen kann dezimal (Standard), oktal (mit fuehrender Null) oder hexadezimal (mit fuehrendem \"0x\" oder \"0X\") erfolgen. Eine negative Zahl -k wird als 2^32-k interpretiert.\n", // 16
     "Input of the low-order part m0 of the formula number (m0, m1), where m0, m1 are integers from 0 to 2^32-1 = 4294967295, and the formula number is interpreted as Big-Endian binary number (leading high-order bit). Numbers can be represented in the decimal system (default), in the octal system (with leading zero) or in the hexadezimal system (with leading \"0x\" oder \"0X\"). Negative numbers -k are interpretad as 2^32-k.\n"},
    {"Eingabe des zweiten Teils m1 der Formelnummer (m0, m1). Siehe --nr0.\n", // 17
     "Input of the high-order part m1 of the key (m0, m1). See --nr0.\n"},
    {"Eingabe der Anzahl der Variablen, eine Zahl von 0 bis 2^31-1 = 2147483647 (oder von 0 bis 2^32 - 1 = 4294967295 im Falle von verallgemeinerten Klauselmengen). Die Eingabe kann dezimal (Standard), oktal (mit fuehrender Null) oder hexadezimal (mit fuehrendem \"0x\" oder \"0X\") erfolgen.\n", // 18
     "Input of the number of variables, a number from 0 to 2^31-1 = 2147483647 (or from 0 to 2^32 - 1 = 4294967295 in the case of generalised formulas). Numbers can be represented in the decimal system (default), in the octal system (with leading zero) or in the hexadezimal system (with leading \"0x\" or \"0X\").\n"},
    {"Eingabe einer Klauselaenge, eine Zahl von 0 bis 2^31-1 = 2147483647 (oder von 0 bis 2^32 - 1 = 4294967295 im Falle von verallgemeinerten Klauselmengen). Die Eingabe kann dezimal (Standard), oktal (mit fuehrender Null) oder hexadezimal (mit fuehrendem \"0x\" oder \"0X\") erfolgen.\n", // 19
     "Input of a clause-length, a number from 0 to 2^31-1 = 2147483647 (or from 0 to 2^32 - 1 = 4294967295 in the case of generalised formulas). Numbers can be represented in the decimal system (default), in the octal system (with leading zero) or in the hexadezimal system (with leading \"0x\" or \"0X\").\n"},
    {"Eingabe einer Klauselnanzahl, eine Zahl von 0 bis 2^32 - 1 = 4294967295. Die Eingabe kann dezimal (Standard), oktal (mit fuehrender Null) oder hexadezimal (mit fuehrendem \"0x\" oder \"0X\") erfolgen. Das aktuelle Paar (Klausellaenge, Klauselnanzahl) wird auf den Stapel gesetzt.\n", // 20
     "Input of the number of clauses (a number from 0 to 2^32-1 = 4294967295) for the current clause-size . Numbers can be represented in the decimal system (default), in the octal system (with leading zero) or in the hexadezimal system (with leading \"0x\" oder \"0X\"). The current pair (clause-length, clause-number) is pushed on the stack.\n"},
    {"Eingabe einer Zeichenkette, das zwischen die Literale gesetzt wird. Zeilenende wird durch \\n und ein Tabulatorzeichen durch \\t dargestellt, wobei man die Zeichenkette in \' ... \' einschliessen muss. So setzt z.B.\n  -sep=\' \n & \t\'\n ein Trennsymbol, dass aus Leerzeichen, Zeilenende, Leerzeichen, kaufmaennischem Und, Leerzeichen und Tabulatorzeichen besteht.", // 21
     "Input of a string to separate the literals. End-of-line is represented by the escape symbol \\n, and the tabulator symbol by \\t. For example\n  -sep=\' \n & \t\'\n sets a separating string consisting of space, end-of-line, space, ampersand, space, tabulator symbol."},
    {"Eingabe einer Zeichenkette, die an den Anfang einer Klausel gesetzt wird. Vergleiche --sep (moechte man kein Klauselanfangssymbol, so setze man -cb=\'\').\n", // 22
     "Input of a string to be placed at the begin of a clause. Compare --sep (in case you do not want a begin-of-clause symbol, use -cb=\'\').\n"},
    {"Eingabe einer Zeichenkette, die an das Ende einer Klausel gesetzt wird. Vergleiche --sep.\n", // 23
     "Input of a string to be placed at the end of a clause. Compare --sep.\n"},
    {"Die Eingabe fuer eine Sprache muss eine Zahl von 0 bis Anzahl angebotener Sprachen - 1 sein.\n", // 24
     "A language is specified by a number from 0 to number of implemented languages - 1.\n"},
    {"Die Spezifikation der Ausgabesprache geschieht durch Eingabe einer Zahl von 0 bis", // 25
     "Specification of language for messages by a number from 0 to"},
    {"wobei 0 fuer Deutsch, 1 fuer Englisch steht.\n", // 26
     "where 0 stands for German, 1 for English.\n"},
    {"Umschalter zwischen booleschen Formeln (Literale sind positive oder negative Variablen) und verallgemeinerten Formeln (Literale sind Paare (v, e), wobei v die Variable ist und 1 <= e <= D). Standardmaessig sind boolesche Formeln aktiv.\n", // 27
     "Switch between boolean formulas (literals are positive or negative variables) and generalised formulas (literals are pairs (v, e), where v is the variable und 1 <= e <= D). Default are boolean formulas.\n"},
    {"Setzen der Domaingroesse 1 <= D <= 2^32 - 1 = 42949672951 fuer verallgemeinerte Formeln (standardmaessig ist D = 2, was boolesche Formeln entspricht).\n", // 28
     "Setting of domain size 1 <= D <= 2^32 - 1 = 42949672951 for generalised formulas (default is D = 2, which in effect yields boolean formulas).\n"},
    {"Die Domaingroesse muss mindestens 1 betragen.\n", // 29
     "Domain size must be at least 1.\n"},
    {"Warnung: Sie befinden sich im Verallgemeinerte-Formeln-Modus, und hier hat die Setzung von --formula_number_0 keine Auswirkung auf die erzeugte Klauselmenge.\n" // 30
     "Warning: You are in the mode for generation of generalised formulas, and thus setting --formula_number_0 has no effect on the generated formula.\n"},
    {"Einleitungs-Symbol fuer die Literale von verallgemeinerten Formeln. Standard ist \"(\".\n", // 31
     "Begin symbol for literals of generalised formulas. Default is \"(\".\n"},
    {"Ende-Symbol fuer die Literale von verallgemeinerten Formeln. Standard ist \")\".\n", // 32
     "End symbol for literals of generalised formulas. Default is \")\".\n"},
    {"Symbol zur Trennung von Wert und Variable fuer die Literale von verallgemeinerten Formeln. Standard ist \",\".\n", // 33
     "Symbol for separating variable and values for literals of generalised formulas. Default is \",\".\n"},
    {"Eingabe einer Dichte d, entweder in der Form p/q fuer natuerliche Zahlen p, q mit q <> 0 oder als nicht-negativer Dezimalbruch (getrennt durch den Dezimalpunkt). Das aktuelle Paar (Klausellaenge, r(d * n)) wird auf den Stapel geschoben, wobei n die aktuelle Variablenzahl ist, und r(x) die Rundung der (nicht-negativen) rationalen Zahl x auf die naechste ganze Zahl. Es ist ein Fehler, wenn r(d * n) groesser als 2^32 - 1 = 4294967295 ist.\n", // 34
     "Input of density d, either as fraction p/q for natural numbers p, q with q <> 0 or as non-negative decimal fraction. The current pair (clause length, r(d * n)) is pushed onto the stack, where n ist the current number of variables, and r(x) is rounding of the (non-negative) rational number x to the nearest integer. It is an error if r(d * n) is larger than 2^32 - 1 = 4294967295.\n"},
    {"Die Eingabe einer Dichte muss entweder von der Form p/q (z.B. \"5/8\") oder von der Form Ziffern.Ziffern (z.B. \"9.7\") sein, es kommt aber weder \"/\" noch \".\" in der Eingabe vor.\n", // 35
     "Densities must be given either as fractions p/q (e.g. \"5/8\") or as decimal fractions (e.g. \"9.7\"), but neither \"/\" nor \".\" occurs in the argument.\n"},
    {"Im Vor- und Nachkommateil eines Dezimalbruches duerfen nur Ziffern vorkommen.\n", // 36
     "Only decimal digits are allowed in decimal fractions.\n"},
    {"Zur Angabe von Zaehler und Nenner sind nur Dezimalziffern erlaubt.\n", // 37
     "For the specification of nominator and denominator only decimal digits are allowed.\n"},
    {"Zaehler und Nenner muessen mindestens eine Ziffer enthalten.\n", // 38
     "Nominator and denominator must not be empty.\n"},
    {"Das Produkt von Dichte und aktueller Variablenzahl ist groesser als 2^32 - 1 = 4294967295.\n", // 39
     "The product of density and current number of variables is larger than 2^32 - 1 = 4294967295.\n"},
    {"Der Nenner darf nicht gleich Null sein!\n", // 40
     "The nominator is zero!\n"},
    {"Es muss mindestens eine Ziffer im Vor- oder Nachkommateil eines Dezimalbruches geben.\n", // 41
     "There must be a least one digit in a decimal fraction.\n"},
    {"Ausgabe im folgenden ohne Klauseln (d.h., nur der Kommentarteil wird ausgegeben (falls ueberhaupt); falls das xml-Ausgabeformat benutzt wird, so wird nur das Element \"instance_info\" ausgegeben).\n", // 42
     "Output in the sequel without clauses, i.e., only the comment (if at all) is output (in case of the xml format only the \"instance_info\" element).\n"},
    {"Die Klausellaenge muss mindestens 1 sein.\n", // 43
     "The clause length must be at least 1.\n"},
    {"Umschalter fuer das xml-Ausgabeformat.\n", // 44
     "Switch for the xml-format.\n"},
  };

  const char * const Version = "1.33";
  const char * const Datum = "27.8.2004";
  const char * const Autor = "Oliver Kullmann (Swansea); O.Kullmann@Swansea.ac.uk";
  const char * const Programm = "OKgenerator";
  const char * const Uebersetzungsdatum = __DATE__ " " __TIME__; // Gnu-Uebersetzung
  
  const char *Separator = ",";
  const char *Klauselanfang = "(";
  const char *Klauselende = ")\n";
  const char *Kommentarbeginn = "%";
  const char *Literalanfang = "(";
  const char *Literalende = ")";
  const char *Werttrenner = ",";

  KMZ::Sprachen Sprache = KMZ::Englisch;
  bool Kommentierung = true;
  bool DimacsZeile = false;
  bool AusgabeFormel = true;
  bool XmlFormat = false;

  bool VerallgKlm =  false;

  using namespace LGAES;

  const SWort32 MaxSWort32 = 2147483647; // 2^31-1
  const UWort32 MaxUWort32 = 4294967295U; // 2^32-1

  Wort64 Schluessel = {}, Nummer = {};
  UWort32 n = 0, p = 0; // die Eingabefunktionen fangen negative Werte ab
  UWort32 c = 0, D = 2;

}


namespace {

  using namespace LGAES;

  typedef struct {
    UWort32 p;
    UWort32 c;
  } PC;

  deque<PC> ListePC;

  typedef unsigned char Ziffer;

  deque<Ziffer> GrosseDezZahl;
  // Voraussetzung: alle Elemente liegen in 0, ..., 9

  void addiere(unsigned long int a) {
    // addiert a zu GrosseDezZahl

    Ziffer u = 0; // Uebertrag
    deque<Ziffer>::iterator i = GrosseDezZahl.begin();
    while ((a != 0) || (u != 0)) {
      if (i == GrosseDezZahl.end())
	i = GrosseDezZahl.insert(i, 0);
      Ziffer r = a % 10;
      a /= 10;
      unsigned char s = *i + r + u;
      if (s >= 10) {
	*i = s - 10;
	u = 1;
      }
      else {
	*i = s;
	u = 0;
      }
      i++;
    }
  }

  void ZahlAusgeben() {
    // gibt GrosseDezZahl aus
    deque<Ziffer>::iterator i = GrosseDezZahl.end();
    if (i == GrosseDezZahl.begin())
      printf("0");
    else
      do
	printf("%d", *(--i));
      while (i > GrosseDezZahl.begin());
  }
    
}


namespace {

  bool cBerechnung(const LINT Z, const LINT N, UWort32& c) {
    // c = r(Z/N * n), r ist Rundung
    LINT q = (Z * n) / N;
    LINT r = (Z * n) % N;
    if (2 * r >= N)
      q++;
    if (q > MaxUWort32)
      return false;
    unsigned char *A;
    int Laenge;
    A = lint2byte(q, &Laenge);
    unsigned char *i = A + Laenge; unsigned int p = 1;
    for (c = 0; i > A; p *= 256)
      c += *(--i) * p;
    return true;
  }

  string NormierteDichte(UWort32 c) { // normierter Quotient c / n
    /*
    LINT Z = c / n, N = 1, r = c % n;
    int i = 0; // zaehlt die Nachkommastellen
    UWort32 c2;

    cBerechnung(Z, N, c2);
    while (c2 != c) { // immer c2 <= c
      r *= 10;
      Z = 10 * Z + (r / n);
      N *= 10; i++;
      r %= n;
      cBerechnung(Z, N, c2);
    }
    string Ergebnis = string(lint2str(Z, 10));
    if (i >= Ergebnis.size())
      Ergebnis = string(i + 1 - Ergebnis.size(), '0') + Ergebnis;
    Ergebnis.insert(Ergebnis.end() - i, 1, '.');
    return Ergebnis;
    */
    return Tools::standardised_quotient(c,n);
  }

}


namespace {

  using namespace KMZ;

  bool AktionVersion(const GenArg * const A) {
    printf(Meldungen[3][Sprache], Programm, Version, Datum, Autor, Uebersetzungsdatum);
    return true;
  }
  bool AktionHilfe(const GenArg * const A) {
    printf("%s", Meldungen[4][Sprache]);
    return true;
  }
  bool AktionMitKommentar(const GenArg * const A) {
    Kommentierung = true;
    return true;
  }
  bool AktionOhneKommentar(const GenArg * const A) {
    Kommentierung = false;
    return true;
  }
  bool AktionDimacs(const GenArg * const A) {
    if (! DimacsZeile) {
      Separator = " ";
      Klauselanfang = "";
      Klauselende = " 0\n";
      Kommentarbeginn = "c";
      Kommentierung = true;
      DimacsZeile = true;
    }
    else
      DimacsZeile = false;
    return true;
  }
  bool AktionReset(const GenArg * const A) {
    ListePC.clear();
    return true;
  }
  bool AktionElim(const GenArg * const A) {
    if (! ListePC.empty())
      ListePC.pop_back();
    return true;
  }
  bool TestSchluessel(const GenArg * const A, const char ** const Text) {
    if (A -> uli <= MaxUWort32)
      return true;
    *Text = Meldungen[0][Sprache];
    return false;
  }
  bool AktionSchluessel0(const GenArg * const A) {
    if (VerallgKlm)
      printf("%s", Meldungen[30][Sprache]);
    Schluessel[0] = A -> uli;
    return true;
  }
  bool AktionSchluessel1(const GenArg * const A) {
    Schluessel[1] = A -> uli;
    return true;
  }
  bool AktionNummer0(const GenArg * const A) {
    Nummer[0] = A -> uli;
    return true;
  }
  bool AktionNummer1(const GenArg * const A) {
    Nummer[1] = A -> uli;
    return true;
  }
  bool TestP(const GenArg * const A, const char ** const Text) {
    if (VerallgKlm) {
      if (A -> uli > MaxUWort32) {
	*Text = Meldungen[0][Sprache];
	return false;
      }
    }
    else if (A -> uli > MaxSWort32) {
      *Text = Meldungen[1][Sprache];
      return false;
    }
    else if (A -> uli > n) {
      *Text = Meldungen[5][Sprache];
      return false;
    }
    else if (A -> uli == 0) {
      *Text = Meldungen[43][Sprache];
      return false;
    }
    return true;
  }
  bool AktionP(const GenArg * const A) {
    p = A -> uli;
    return true;
  }
  bool AktionC(const GenArg * const A) {
    c = A -> uli;
    PC temp = {p, c};
    ListePC.push_back(temp);
    return true;
  }
  bool TestN(const GenArg * const A, const char ** const Text) {
    if (VerallgKlm) {
      if (A -> uli > MaxUWort32) {
	*Text = Meldungen[0][Sprache];
	return false;
      }
    }
    else
      if (A -> uli > MaxSWort32) {
	*Text = Meldungen[1][Sprache];
	return false;
      }
    return true;
   }
  bool AktionN(const GenArg * const A) {
    n = A -> uli;
    return true;
  }
  bool AktionAusgabe(const GenArg * const A) {
    if (AusgabeFormel)
      if (VerallgKlm)
	if (XmlFormat)
	  VEKLMAES::setzenXML();
	else
	  VEKLMAES::setzenParameter(Klauselanfang, Klauselende, Separator, Literalanfang, Literalende, Werttrenner);
      else
	if (XmlFormat)
	  EKLMAES::setzenXML();
	else
	  EKLMAES::setzenParameter(Klauselanfang, Klauselende, Separator);
    
    if (XmlFormat && AusgabeFormel)
      printf("<SAT-instance>\n");

    if (Kommentierung) {

      if (XmlFormat) {
	printf("<instance_info>\n");
	printf("  <instance-type>\n");
	printf("    <generated>\n");
	printf("      <random>\n");
	if (VerallgKlm)
	  printf("        <OKgenerator seed_0 = \"%lu\" seed_1 = \"%lu\" number_1 = \"%lu\" variables = \"%lu\" domain_size = \"%lu\" >\n", Schluessel[0], Schluessel[1], Nummer[1], n, D);
	else
	  printf("        <OKgenerator seed_0 = \"%lu\" seed_1 = \"%lu\" number_0 = \"%lu\" number_1 = \"%lu\" variables = \"%lu\" >\n", Schluessel[0], Schluessel[1], Nummer[0], Nummer[1], n);
	printf("          <g_numbers_of_clauses>\n");
      }
      else
	if (VerallgKlm)
	  printf("%s OKGRandGen( (%lu, %lu), %lu, %lu, %lu; ", Kommentarbeginn, Schluessel[0], Schluessel[1], Nummer[1], n, D);
	else
	  printf("%s OKgenerator( (%lu, %lu), (%lu, %lu), %ld; ", Kommentarbeginn, Schluessel[0], Schluessel[1], Nummer[0], Nummer[1], n);

      if (XmlFormat) {
	for (deque<PC>::iterator i = ListePC.begin(); i < ListePC.end(); i++)
	  printf("            <g_number length = \"%ld\" count = \"%lu\" st-density = \"%s\" />\n", i -> p, i -> c, NormierteDichte(i -> c).c_str());
	printf("          </g_numbers_of_clauses>\n");
	printf("        </OKgenerator>\n");
	printf("      </random>\n");
	printf("    </generated>\n");
	printf("  </instance-type>\n");
	printf("</instance_info>\n");
      }
      else {
	for (deque<PC>::iterator i = ListePC.begin(); i < ListePC.end(); i++)
	  printf("%ld, %lu; ", i -> p, i -> c);
	printf(")\n");
	printf("%s Standardised relative densities:", Kommentarbeginn);
	for (deque<PC>::iterator i = ListePC.begin(); i != ListePC.end(); i++) {
	  if (i != ListePC.begin())
	    printf(",");
	  printf(" %ld -> %s", i -> p, NormierteDichte(i -> c).c_str());
	}
	printf("\n");
      }
    } // (Kommentierung)

    if (!XmlFormat && DimacsZeile) {
      GrosseDezZahl.clear();
      for (deque<PC>::iterator i = ListePC.begin(); i < ListePC.end(); i++)
	addiere(i -> c);
      printf("p ");
      if (VerallgKlm)
	printf("gcnf ");
      else
	printf("cnf ");
      printf("%ld ", n);
      ZahlAusgeben();
      if (VerallgKlm)
	printf(" %ld", D);
      printf("\n");
    }

    if (AusgabeFormel) {
      if (VerallgKlm) {
	if (XmlFormat) {
	  printf("<instance_cls>\n");
	  printf("  <g_clause-set>\n");
	  printf("    <g_variables_as_numbers maximal_index = \"%ld\" >\n", n);
	}
	for (deque<PC>::iterator i = ListePC.begin(); i < ListePC.end(); i++)
	  VEKLMAES::VEinfKlm(Schluessel, Nummer[1], n, D, i -> p, i -> c);
	if (XmlFormat) {
	  printf("  </g_clause-set>\n");
	  printf("</instance_cls>\n");
	}
      }
      else {
	if (XmlFormat) {
	  printf("<instance_cls>\n");
	  printf("  <clause-set>\n");
	  printf("    <variables_as_numbers maximal_index = \"%ld\" >\n", n);
	}
	for (deque<PC>::iterator i = ListePC.begin(); i < ListePC.end(); i++)
	  EKLMAES::EinfKlm(Schluessel, Nummer, n, i -> p, i -> c);
	if (XmlFormat) {
	  printf("  </clause-set>\n");
	  printf("</instance_cls>\n");
	}
      }
    }

    if (XmlFormat&& AusgabeFormel)
      printf("</SAT-instance>\n");

    return true;
  }
  bool AktionSeparator(const GenArg * const A) {
    Separator = A -> s;
    return true;
  }
  bool AktionKlauselanfang(const GenArg * const A) {
    Klauselanfang = A -> s;
    return true;
  }
  bool AktionKlauselende(const GenArg * const A) {
    Klauselende = A -> s;
    return true;
  }
  bool AktionNL(const GenArg * const A) {
    printf("\n");
    return true;
  }
  bool TestSprache(const GenArg * const A, const char ** const Text) {
    if ((A -> li >= 0) && (A -> li < AnzahlSprachen))
      return true;
    *Text = Meldungen[24][Sprache];
    return false;
  }
  bool AktionSprache(const GenArg * const A) {
    Sprache = (KMZ::Sprachen) A -> li;
    return true;
  }
  bool AktionVerallg(const GenArg * const A) {
    VerallgKlm = !VerallgKlm;
    return true;
  }
  bool TestDomain(const GenArg * const A, const char ** const Text) {
    if (A -> uli > MaxUWort32) {
      *Text = Meldungen[0][Sprache];
      return false;
    }
    if (A -> uli == 0) {
      *Text = Meldungen[29][Sprache];
      return false;
    }
    return true;
  }
  bool AktionDomain(const GenArg * const A) {
    D = A -> uli;
  }
  bool AktionLiteralanfang(const GenArg * const A) {
    Literalanfang = A -> s;
  }
  bool AktionLiteralende(const GenArg * const A) {
    Literalende = A -> s;
  }
  bool AktionWerttrenner(const GenArg * const A) {
    Werttrenner = A -> s;
  }
  bool nurZiffern(const char * T) { // Hilfsfunktion
    return (strspn(T, "0123456789") == strlen(T));
  }
  bool TestDichte(const GenArg * const A, const char ** const Text) {
    const char * p;
    LINT Z, N;

    if ((p = strchr(A -> s, '/')) == 0) { // Dezimalzahl
      if ((p = strchr(A -> s, '.')) == 0) {
	*Text = Meldungen[35][Sprache];
	return false;
      }
      const string T1s = string(A -> s, p - (A -> s));
      const char * const T1 = T1s.c_str();
      if (! (nurZiffern(T1) && nurZiffern(p+1))) {
	*Text = Meldungen[36][Sprache];
	return false;
      }
      if (strlen(T1) == 0 && strlen(p+1) == 0) {
	*Text = Meldungen[41][Sprache];
	return false;
      }
      Z = LINT(string(T1) + string(p+1), 10);
      N = LINT(string("1") + string(strlen(p+1), '0'), 10);
     }
    else { // Bruch
      const char * const T1 = string(A -> s, p - (A -> s)).c_str();
      if (! (nurZiffern(T1) && nurZiffern(p+1))) {
	*Text = Meldungen[37][Sprache];
	return false;
      }
      if ((p == A -> s) || (*(p+1) == 0)) {
	*Text = Meldungen[38][Sprache];
	return false;
      }
      Z = LINT(T1, 10);
      N = LINT(p+1, 10);
      if (N == 0) {
	*Text = Meldungen[40][Sprache];
	return false;
      }
    }
    
    if (! cBerechnung(Z, N, c)) {
      *Text = Meldungen[39][Sprache];
      return false;
    }
    return true;
  }
  bool AktionDichte(const GenArg * const A) {
    PC temp = {p, c};
    ListePC.push_back(temp);
    return true;
  }
  bool AktionOhneFormel(const GenArg * const A) {
    AusgabeFormel = ! AusgabeFormel;
    return true;
  }
  bool AktionXml(const GenArg * const A) {
    XmlFormat = ! XmlFormat;
    return true;
  }


  void EVersion() {
    printf("%s", Meldungen[6][Sprache]);
  }
  void EKommentar() {
    printf("%s", Meldungen[7][Sprache]);
  }
  void EOhneKommentar() {
    printf("%s", Meldungen[8][Sprache]);
  }
  void EDIMACS() {
    printf("%s", Meldungen[9][Sprache]);
  }
  void EReset() {
    printf("%s", Meldungen[10][Sprache]);
  }
  void EElim() {
    printf("%s", Meldungen[11][Sprache]);
  }
  void ENL() {
    printf("%s", Meldungen[12][Sprache]);
  }
  void EAusgabe() {
    printf("%s", Meldungen[13][Sprache]);
  }
  void ESchluessel0() {
    printf("%s", Meldungen[14][Sprache]);
  }
  void ESchluessel1() {
    printf("%s", Meldungen[15][Sprache]);
  }
  void ENummer0() {
    printf("%s", Meldungen[16][Sprache]);
  }
  void ENummer1() {
    printf("%s", Meldungen[17][Sprache]);
  }
  void EN() {
    printf("%s", Meldungen[18][Sprache]);
  }
  void EP() {
    printf("%s", Meldungen[19][Sprache]);
  }
  void EC() {
    printf("%s", Meldungen[20][Sprache]);
  }
  void ESeparator() {
    printf("%s", Meldungen[21][Sprache]);
  }
  void EKlauselanfang() {
    printf("%s", Meldungen[22][Sprache]);
  }
  void EKlauselende() {
    printf("%s", Meldungen[23][Sprache]);
  }
  void ESprache() {
    printf("%s %d, %s", Meldungen[25][Sprache], AnzahlSprachen-1, Meldungen[26][Sprache]);
  }
  void EVerallg() {
    printf("%s", Meldungen[27][Sprache]);
  }
  void EDomain() {
    printf("%s", Meldungen[28][Sprache]);
  }
  void ELiteralanfang() {
    printf("%s", Meldungen[31][Sprache]);
  }
  void ELiteralende() {
    printf("%s", Meldungen[32][Sprache]);
  }
  void EWerttrenner() {
    printf("%s", Meldungen[33][Sprache]);
  }
  void EDichte() {
    printf("%s", Meldungen[34][Sprache]);
  }
  void EOhneFormel() {
    printf("%s", Meldungen[42][Sprache]);
  }
  void EXml() {
     printf("%s", Meldungen[44][Sprache]);
  }

  const int AnzahlOptionen = 27;
  Argumentstruktur Optionen[AnzahlOptionen] = {
    {"-v", "--version", KEin, 0, AktionVersion, EVersion},
    {"-uc", "--use_comment", KEin, 0, AktionMitKommentar, EKommentar},
    {"-nc", "--no_comment", KEin, 0, AktionOhneKommentar, EOhneKommentar},
    {"-D", "--DIMACS", KEin, 0, AktionDimacs, EDIMACS},
    {"-r", "--reset_stack", KEin, 0, AktionReset, EReset},
    {"-pop", "--pop", KEin, 0, AktionElim, EElim},
    {"-nl", "--print_new_line", KEin, 0, AktionNL, ENL},
    {"-o", "--output", KEin, 0, AktionAusgabe, EAusgabe},
    {"s0", "--seed_0", ULIEin, TestSchluessel, AktionSchluessel0, ESchluessel0},
    {"s1", "--seed_1", ULIEin, TestSchluessel, AktionSchluessel1, ESchluessel1},
    {"nr0", "--formula_number_0", ULIEin, TestSchluessel, AktionNummer0, ENummer0},
    {"nr1", "--formula_number_1", ULIEin, TestSchluessel, AktionNummer1, ENummer1},
    {"lb", "--literal_begin", ZEin, 0, AktionLiteralanfang, ELiteralanfang},
    {"le", "--literal_end", ZEin, 0, AktionLiteralende, ELiteralende},
    {"vs", "--value_separator", ZEin, 0, AktionWerttrenner, EWerttrenner},
    {"n", "--number_of_variables", ULIEin, TestN, AktionN, EN},
    {"l", "--clause_length", ULIEin, TestP, AktionP, EP},
    {"cp", "--set_#_clauses_and_push", ULIEin, TestSchluessel, AktionC, EC},
    {"sep", "--separator", ZEin, 0, AktionSeparator, ESeparator},
    {"cb", "--clause_begin", ZEin, 0, AktionKlauselanfang, EKlauselanfang},
    {"ce", "--clause_end", ZEin, 0, AktionKlauselende, EKlauselende},
    {"-l", "--language", LIEin, TestSprache, AktionSprache, ESprache},
    {"-g", "--generalized_formulas", KEin, 0, AktionVerallg, EVerallg},
    {"dp", "--set_density_and_push", ZEin, TestDichte, AktionDichte, EDichte},
    {"ds", "--domain_size", ULIEin, TestDomain, AktionDomain, EDomain},
    {"-nf", "--no_formula", KEin, 0, AktionOhneFormel, EOhneFormel},
    {"-X", "--XML", KEin, 0, AktionXml, EXml},
  };
}



int main(const int argc, const char * const argv []) {

  return ! KMZ::ArgumentBearbeitung(Selbst, Optionen, AnzahlOptionen, argv + 1, argc - 1, Sprache);
}

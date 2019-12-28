// Oliver Kullmann, 19.1.2001 (Toronto)
/* Copyright 2001 - 2007, 2008, 2011, 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKsolver/SAT2002/Einlesen.c
  \brief Implementations of input/output functions
*/

                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* angelegt 19.1.2001, UofT */

/* 13.2.2001: Berechnung der MAX-Werte in der Prozedur "Schranken" */

/* Einheit: Einlesen */

/* Einlesen der Eingabe */


#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <string.h> /* fuer C++ (memset; 14.8.2001) */
#include <assert.h>


#include "OK.h"
#include "VarLitKlm.h"
#include "anlVar.h"
#include "Parameter.h"
#include "Einlesen.h"
#include "Ausgaben.h"
#include "Speicher.h"
#include "Belegung.h"

extern bool nurVorreduktion;
extern bool Schranken;

static unsigned int maxn, maxl, maxk, maxlk;
/* maxn ist die maximale Anzahl von Variablen, */
/* maxlk die maximale Summe von Literalanzahl und Klauselanzahl */
static bool unsichermaxn;
static bool unsichermaxl;
static bool unsichermaxk;
static unsigned int M;  /* Groesse der Hashtabelle */
/* fuer die Hashtabelle, falls Schranken gesetzt: */
static const unsigned int Lastfaktor = 4;


static char *Eingabesymbole;  /* zeigt immer auf den aktuellen Symbolanfang */
static char *freiSymbole;  /* zeigt auf den ersten freien Platz */
static char *Eingabesymbole0 = NULL;

/* Zeiger auf dynamisch erzeugten Speicherplatz muesse NULL sein, */
/* wenn sie nicht auf solchen zeigen. */

static unsigned int *Hashtabelle = NULL;

static int *LitTab = NULL;
/* eingelesene Multiklauselmenge, Literal fuer Literal, wobei */
/* Klauselnenden durch NullLiteral angezeigt werden. */

static int *aktKlauselAnfang;
static int *aktfreies;  /* bzgl. LitTab */

static bool leereKlausel;
static bool Tautologie; /* ist die aktuelle Klausel tautologisch? */
static bool globalTautologie; /* gibt es insgesamt eine Tautologie? */
static bool EinerKlausel;

static unsigned int aktp, aktp0;

static void *Z0 = NULL; /* Zeiger auf lokale Variablen */

static void *Basis0 = NULL;
/* der Hauptteil der dynamisch erzeugten Variablen (fuer's Hauptprogramm) */


/* der Typ des Variablensymbols in der Eingabe: */

typedef char *VarSym;

/* Variablen koennen in der Eingabe beliebige Nummern haben, so ist z.B. */
/*    F = (123,-4,5)(-5,8,123) */
/* erlaubt. In VarTab[i] finden wir zur Variablen i mit 1 <= i <= N die urspruengliche */
/* Nummer, wobei sich i auf das Vorkommen bezieht: */
/*    VarTab[1] = 123, */
/*    VarTab[2] = 4 */
/*    VarTab[3] = 5, */
/*    VarTab[4] = 8. */
/* VarTab wird nur im Falle, dass eine erfuellende Belegung ausgegeben werden soll, */
/* als globale Variable gefuehrt. */

static VarSym *VarTab = NULL;  /* VarTab[v] fuer Variable v */

static unsigned int *VarTab1 = NULL;
static unsigned *VarTab2 = NULL;

static int *Pfad0 = NULL;



/* --------------------------------------------------- */


/* Syntaxerkennung */

bool Beginn[CHAR_MAX+1];
bool Ende[CHAR_MAX+1];
bool Trenner[CHAR_MAX+1];
bool Negator[CHAR_MAX+1];
bool Kommentar[CHAR_MAX+1];

void setzenStandard() {
  assert(Standard >= 1);
  assert(Standard <= 4);
  switch (Standard) {
  case 1 :
    for (int i = 0; i <= CHAR_MAX; ++i)
      Negator[i] = Kommentar[i] = false;
    Negator['-'] = true;
    Kommentar['c'] = Kommentar['p'] = true;
    break;
  case 2 :
    for (int i = 0; i <= CHAR_MAX; ++i)
      Beginn[i] = Ende[i] = Trenner[i] = Negator[i] = Kommentar[i] = false;
    Beginn['('] = true;
    Ende[')'] = true;
    Trenner[','] = true;
    Negator['-'] = true;
    Kommentar['%'] = true;
    break;
  case 3 :
    for (int i = 0; i <= CHAR_MAX; ++i)
      Beginn[i] = Ende[i] = Negator[i] = Kommentar[i] = false;
    Beginn['('] = true;
    Ende[')'] = true;
    Negator['-'] = true;
    Kommentar['%'] = true;
    break;
  case 4 :
    for (int i = 0; i <= CHAR_MAX; ++i)
      Negator[i] = Kommentar[i] = false;
    Negator['-'] = true;
    Kommentar['%'] = true;
    break;
  }
}


__inline__ bool Kommzeichen(const char c) {
  return Kommentar[(int) c];
}
__inline__ bool Sepzeichen(const char c) {
  return Trenner[(int) c];
}
__inline__ bool Negzeichen(const char c) {
  return Negator[(int) c];
}
__inline__ bool Variablenanfang(const char c) {
  if (Standard == 1)
    return (isalnum(c) && (! Kommentar[(int) c]) && (c != '0'));
  else
    return isalnum(c);
}
__inline__ bool Klauselbeginn(const char c) {
  return Beginn[(int) c];
}
__inline__ bool Klauselende(const char c) {
  return Ende[(int) c];
}

/* --------------------------------------------------- */


__inline__ bool uebernehmenLiteral(const int l) {
/* gibt false zurueck im Fehlerfalle */
  int *Lauf;

  L0++; aktp0++;
  if (Tautologie) return true;
  for (Lauf = aktKlauselAnfang; Lauf != aktfreies; Lauf++)
    if (*Lauf == -l) {
	Tautologie = globalTautologie = true;
	break;
    }
    else if (*Lauf == l) break;
  if (Tautologie) {
    aktfreies = aktKlauselAnfang;
    L -= aktp;
    return true;
  }
  else if (Lauf == aktfreies) {
    if (unsichermaxl && L >= maxl)
    return false;
    aktp++;
    L++;
    *(aktfreies++) = l;
  }
  return true;
}

__inline__ bool uebernehmenKlausel() {
  K0++;
  if (aktp0 > P0)
    P0 = aktp0;
  if (! Tautologie) {
    if (unsichermaxk && K >= maxk)
    return false;
    K++;
    if (aktp > P)
    P = aktp;
    if (aktp == 1)
    EinerKlausel = true;
    *(aktfreies++) = 0;
    aktKlauselAnfang = aktfreies;
  }
  return true;
}


/* --------------------------------------------------- */

/* Hashverwaltung */

unsigned int Hash(const char* Name) {
  unsigned int h = 0, a = 127;
  for (; *Name != '\0'; Name++) h = (a * h + (unsigned int) *Name) % M;
  return h;
}

unsigned int eintragen() {
/* Berechnet die Variablennummer 1, ... zum in Eingabesymbole */
/* abgespeicherten Symbol. */
/* Gibt 0 zurueck, falls Eintragen in die Hashtabelle */
/* nicht moeglich war. */
  unsigned int i; unsigned int v;

  i = Hash(Eingabesymbole);
  while ((v = *(Hashtabelle + i)) != 0) {
    if (strcmp(Eingabesymbole, *(VarTab + v)) == 0) {
      freiSymbole = Eingabesymbole;
	return v;
    }
    else
	i = (i + 1) % M;
    }
  if (unsichermaxn && (N0 >= maxn)) return 0;
  N0++;
  *(VarTab + N0) = Eingabesymbole;
  *(Hashtabelle + i) = N0;
  Eingabesymbole = freiSymbole;
  return N0;
}


/* --------------------------------------------------- */

void Fehlerumgebung(FILE* const fp, char c) {
  long Position;
  long Zeile, p, letzteZeile;

  Position = ftell(fp);
  fprintf(stderr, "%s %2ld, ", Meldung(11), Position);
  if (isgraph(c)) fprintf(stderr, "%c\n", c);
  else fprintf(stderr, "\\%03d\n", c);
  rewind(fp);
  Zeile = 1; letzteZeile = 0;
  for (p = 1; p < Position; p++) {
    c = fgetc(fp);
    if (c == '\n') {
	Zeile++;
	letzteZeile = p;
    }
  }
  fprintf(stderr, "%s %2ld; %s %2ld\n", Meldung(12), Zeile, Meldung(23), Position - letzteZeile);
  return;
}


/* --------------------------------------------------- */



TEIN Einlesen(FILE* const fp, const unsigned int G) {
  char c; unsigned int v; VZ e;

  P = P0 = N = N0 = K = K0 = L = L0 = 0;

  if (G == 0) return Sat;
  {unsigned int trivSchranke;
   trivSchranke = (unsigned int) ceil(G / 2.0); /* laesst Spiel von +1 zu */
   if (Schranken && (MAXN < trivSchranke)) {
     maxn = MAXN;
     M = Lastfaktor * maxn; /* Groesse der Hashtabelle */
     unsichermaxn = true;
   }
   else {
     maxn = trivSchranke;
     unsichermaxn = false;
     M = trivSchranke;
     if (M % 127 == 0)
     M++;
   }
   if (Schranken && (MAXL < trivSchranke)) {
     maxl = MAXL;
     unsichermaxl = true;
   }
   else {
     maxl = trivSchranke;
     unsichermaxl = false;
    }
   if (Schranken && (MAXK < trivSchranke)) {
     maxk = MAXK;
     unsichermaxk = true;
   }
   else {
     maxk = trivSchranke;
     unsichermaxk = false;
   }
   maxlk = maxl + maxk;
  }

  Hashtabelle = (unsigned int *) xmalloc(M * sizeof(unsigned int));
  aktKlauselAnfang = aktfreies = LitTab = (int *) xmalloc(maxlk * sizeof(int));
  freiSymbole = Eingabesymbole = Eingabesymbole0 = (char *) xmalloc(G * sizeof(char));
  VarTab = (VarSym *) xmalloc((maxn + 1) * sizeof(VarSym));
  memset((void *) Hashtabelle, 0, M * sizeof(unsigned int));

/*  1. Phase: Berechnung von LitTab und VarTab sowie von */
/*    P, K, L und P0, N0, K0, L0. */
/*  P0, N0, K0, L0 sind hierbei die urspuenglichen Groessen, */
/*    waehrend P, K, L die Streichung von tautologischen Klauseln  */
/*    und Mehrfachvorkommen von Literalen in Klauseln beruecksichtigen. */
/*  Format von LitTab: */
/*    Variablen sind von 1 ab durchnumeriert, das Komplement von v ist -v, */
/*    und Klauselenden werden durch 0 markiert. */

 leereKlausel = globalTautologie = Tautologie = EinerKlausel = false;
 aktp = aktp0 = 0;

 c = getc(fp);
 switch (Standard) {
 case 1 : goto S1;
 case 2 : goto S2;
 case 3 : goto S3;
 case 4 : goto S4;
 }

S1:

  ZustandS11 :

    if (c == EOF) goto Phase2;
    else if (isspace(c)) { c = getc(fp); goto ZustandS11; }
    else if (Kommzeichen(c)) { c = getc(fp); goto ZustandS12; }
    else if (Variablenanfang(c)) {
      *(freiSymbole++) = c;
      e = Pos; Tautologie = false;
      aktp0 = aktp = 0;
      c = getc(fp); goto ZustandS15;
    }
    else if (Negzeichen(c)) {
      e = Neg; Tautologie = false;
	aktp0 = aktp = 0;
	c = getc(fp); goto ZustandS14;
    }
    else {
	if (c == '0') fprintf(stderr, "%s\n", Meldung(50));
	else fprintf(stderr, "%s\n", Meldung(8));
      Fehlerumgebung(fp, c);
	return Fehler;
    }

  ZustandS12 :

    if (c == EOF) {
      fprintf(stderr, "%s\n", Meldung (9));
      return Fehler;
   }
   else if (c == '\n') { c = getc(fp); goto ZustandS11; }
   else { c = getc(fp); goto ZustandS12; }

  ZustandS14 :

    if (isspace(c)) { c = getc(fp); goto ZustandS14; }
   else if (Variablenanfang(c)) {
     *(freiSymbole++) = c;
     c = getc(fp); goto ZustandS15;
   }
   else {
     if (c == EOF) fprintf(stderr, "%s\n", Meldung(14));
     else {
       fprintf(stderr, "%s\n", Meldung(13));
	 Fehlerumgebung(fp, c);
     }
     return Fehler;
   }

  ZustandS15 :

    if (isspace(c)) {
      *(freiSymbole++) = '\0';
      v = eintragen();
      if (v == 0) {
	  fprintf(stderr, "%s %6d\n", Meldung(35), maxn);
	  return Fehler;
	}
      if (! uebernehmenLiteral((e == Pos) ? (int) v : - (int) v)) {
	  fprintf(stderr, "%s %7d\n", Meldung(38), maxl);
	  return Fehler;
	 }
       c = getc(fp); goto ZustandS16;
    }
    else if (isalnum(c)) {
      *(freiSymbole++) = c;
      c = getc(fp); goto ZustandS15;
    }
    else {
      if (c == EOF) fprintf(stderr, "%s\n", Meldung(14));
      else {
	  fprintf(stderr, "%s\n", Meldung(8));
	  Fehlerumgebung(fp, c);
	}
      return Fehler;
    }

  ZustandS16 :

    if (c == '0') {
      if (! uebernehmenKlausel()) {
	  fprintf(stderr, "%s %6d\n", Meldung(41), maxk);
	  return Fehler;
	}
      c = getc(fp); goto ZustandS17;
    }
    else if (isspace(c)) { c = getc(fp); goto ZustandS16; }
    else if (Variablenanfang(c)) {
      *(freiSymbole++) = c;
      e = Pos;
      c = getc(fp); goto ZustandS15;
    }
    else if (Negzeichen(c)) {
      e = Neg;
      c = getc(fp); goto ZustandS14;
    }
    else {
      if (c == EOF) fprintf(stderr, "%s\n", Meldung(14));
      else {
	  fprintf(stderr, "%s\n", Meldung(8));
	  Fehlerumgebung(fp, c);
	}
      return Fehler;
    }

  ZustandS17 :

    if (c == EOF) goto Phase2;
    else if (isspace(c)) {
      c = getc(fp); goto ZustandS11;
    }
    else {
      fprintf(stderr, "%s\n", Meldung(51));
      Fehlerumgebung(fp, c);
      return Fehler;
    }

/* ---------------------- */

S2:

  ZustandS21 :

    if (c == EOF) goto Phase2;
    else if (isspace(c)) { c = getc(fp); goto ZustandS21; }
    else if (Kommzeichen(c)) { c = getc(fp); goto ZustandS22; }
    else if (Klauselbeginn(c)) { c = getc(fp); goto ZustandS23; }
    else {
      fprintf(stderr, "%s\n", Meldung(8));
      Fehlerumgebung(fp, c);
      return Fehler;
    }

  ZustandS22 :

    if (c == EOF) {
      fprintf(stderr, "%s\n", Meldung (9));
      return Fehler;
    }
    else if (c == '\n') { c = getc(fp); goto ZustandS21; }
    else { c = getc(fp); goto ZustandS22; }

  ZustandS23 :

    if (Klauselende(c)) {
      leereKlausel = true; K0++; K++;
      c = getc(fp); goto ZustandS21;
    }
    else if (isspace(c)) { c = getc(fp); goto ZustandS23; }
    else if (isalnum(c)) {
      *(freiSymbole++) = c;
      e = Pos; Tautologie = false;
      aktp0 = aktp = 0;
      c = getc(fp); goto ZustandS25;
    }
    else if (Negzeichen(c)) {
      e = Neg; Tautologie = false;
      aktp0 = aktp = 0;
      c = getc(fp); goto ZustandS24;
    }
    else {
      if (c == EOF) fprintf(stderr, "%s\n", Meldung(10));
      else {
	  fprintf(stderr, "%s\n", Meldung(8));
	  Fehlerumgebung(fp, c);
	}
      return Fehler;
    }

  ZustandS24 :

    if (isspace(c)) { c = getc(fp); goto ZustandS24; }
    else if (isalnum(c)) {
      *(freiSymbole++) = c;
      c = getc(fp); goto ZustandS25;
    }
    else {
      if (c == EOF) fprintf(stderr, "%s\n", Meldung(14));
      else {
	  fprintf(stderr, "%s\n", Meldung(13));
	  Fehlerumgebung(fp, c);
	}
      return Fehler;
    }

  ZustandS25 :

    if (Klauselende(c)) {
      *(freiSymbole++) = '\0';
      v = eintragen();
      if (v == 0) {
	  fprintf(stderr, "%s %6d\n", Meldung(35), maxn);
	  return Fehler;
	}
      if (! uebernehmenLiteral((e == Pos) ? (int) v : - (int) v)) {
	  fprintf(stderr, "%s %7d\n", Meldung(38), maxl);
	  return Fehler;
	}
      if (! uebernehmenKlausel()) {
	  fprintf(stderr, "%s %6d\n", Meldung(41), maxk);
	  return Fehler;
	}
      c = getc(fp); goto ZustandS21;
    }
    else if (Sepzeichen(c)) {
      *(freiSymbole++) = '\0';
      v = eintragen();
      if (v == 0) {
	  fprintf(stderr, "%s %6d\n", Meldung(35), maxn);
	  return Fehler;
	}
      if (! uebernehmenLiteral((e == Pos) ? (int) v : - (int) v)) {
	  fprintf(stderr, "%s %7d\n", Meldung(38), maxl);
	  return Fehler;
	}
      c = getc(fp); goto ZustandS27;
    }
    else if (isspace(c)) {
      *(freiSymbole++) = '\0';
      v = eintragen();
      if (v == 0) {
	  fprintf(stderr, "%s %6d\n", Meldung(35), maxn);
	  return Fehler;
	}
      if (! uebernehmenLiteral((e == Pos) ? (int) v : - (int) v)) {
	  fprintf(stderr, "%s %7d\n", Meldung(38), maxl);
	  return Fehler;
	}
      c = getc(fp); goto ZustandS26;
    }
    else if (isalnum(c)) {
      *(freiSymbole++) = c;
      c = getc(fp); goto ZustandS25;
    }
    else {
      if (c == EOF) fprintf(stderr, "%s\n", Meldung(14));
      else {
	  fprintf(stderr, "%s\n", Meldung(8));
	  Fehlerumgebung(fp, c);
	}
      return Fehler;
    }

  ZustandS26 :

    if (Klauselende(c)) {
      if (! uebernehmenKlausel()) {
	  fprintf(stderr, "%s %6d\n", Meldung(41), maxk);
	  return Fehler;
	}
      c = getc(fp); goto ZustandS21;
    }
    else if (isspace(c)) { c = getc(fp); goto ZustandS26; }
    else if (Sepzeichen(c)) { c = getc(fp); goto ZustandS27; }
    else {
      if (c == EOF) fprintf(stderr, "%s\n", Meldung(14));
      else {
	  fprintf(stderr, "%s\n", Meldung(8));
	  Fehlerumgebung(fp, c);
	}
      return Fehler;
    }

  ZustandS27 :

    if (isspace(c)) { c = getc(fp); goto ZustandS27; }
    else if (isalnum(c)) {
      *(freiSymbole++) = c;
      e = Pos;
      c = getc(fp); goto ZustandS25;
    }
    else if (Negzeichen(c)) {
      e = Neg;
      c = getc(fp); goto ZustandS24;
    }
    else {
      if (c == EOF) fprintf(stderr, "%s\n", Meldung(15));
      else {
	  fprintf(stderr, "%s\n", Meldung(16));
	  Fehlerumgebung(fp, c);
	}
      return Fehler;
    }

/* ---------------------- */

S3:

 ZustandS31 :

   if (c == EOF)
     goto Phase2;
   else if (isspace(c))
     {
       c = getc(fp); goto ZustandS31;
     }
   else if (Kommzeichen(c))
     {
       c = getc(fp); goto ZustandS32;
     }
   else if (Klauselbeginn(c))
     {
       c = getc(fp); goto ZustandS33;
     }
   else
     {
       fprintf(stderr, "%s\n", Meldung(8));
       Fehlerumgebung(fp, c);
       return Fehler;
     }

 ZustandS32 :

   if (c == EOF)
     {
       fprintf(stderr, "%s\n", Meldung (9));
       return Fehler;
     }
   else if (c == '\n')
     {
       c = getc(fp); goto ZustandS31;
     }
   else
     {
       c = getc(fp); goto ZustandS32;
     }

 ZustandS33 :

   if (Klauselende(c))
     {
       leereKlausel = true; K0++; K++;
       c = getc(fp); goto ZustandS31;
     }
   else if (isspace(c))
     {
       c = getc(fp); goto ZustandS33;
     }
   else if (isalnum(c))
     {
       *(freiSymbole++) = c;
       e = Pos; Tautologie = false;
       aktp0 = aktp = 0;
       c = getc(fp); goto ZustandS35;
     }
   else if (Negzeichen(c))
     {
       e = Neg; Tautologie = false;
       aktp0 = aktp = 0;
       c = getc(fp); goto ZustandS34;
     }
   else
     {
       if (c == EOF)
	 fprintf(stderr, "%s\n", Meldung(10));
       else
	 {
	   fprintf(stderr, "%s\n", Meldung(8));
	   Fehlerumgebung(fp, c);
	 }
       return Fehler;
     }

 ZustandS34 :

   if (isspace(c))
     {
       c = getc(fp); goto ZustandS34;
     }
   else if (isalnum(c))
     {
       *(freiSymbole++) = c;
       c = getc(fp); goto ZustandS35;
     }
   else
     {
       if (c == EOF)
	 fprintf(stderr, "%s\n", Meldung(14));
       else
	 {
	   fprintf(stderr, "%s\n", Meldung(13));
	   Fehlerumgebung(fp, c);
	 }
       return Fehler;
     }

 ZustandS35 :

   if (Klauselende(c))
     {
       *(freiSymbole++) = '\0';
       v = eintragen();
       if (v == 0)
	 {
	   fprintf(stderr, "%s %6d\n", Meldung(35), maxn);
	   return Fehler;
	 }
       if (! uebernehmenLiteral((e == Pos) ? (int) v : - (int) v))
	 {
	   fprintf(stderr, "%s %7d\n", Meldung(38), maxl);
	   return Fehler;
	 }
       if (! uebernehmenKlausel())
	 {
	   fprintf(stderr, "%s %6d\n", Meldung(41), maxk);
	   return Fehler;
	 }
       c = getc(fp); goto ZustandS31;
     }
   else if (isspace(c))
     {
       *(freiSymbole++) = '\0';
       v = eintragen();
       if (v == 0)
	 {
	   fprintf(stderr, "%s %6d\n", Meldung(35), maxn);
	   return Fehler;
	 }
       if (! uebernehmenLiteral((e == Pos) ? (int) v : - (int) v))
	 {
	   fprintf(stderr, "%s %7d\n", Meldung(38), maxl);
	   return Fehler;
	 }
       c = getc(fp); goto ZustandS36;
     }
   else if (isalnum(c))
     {
       *(freiSymbole++) = c;
       c = getc(fp); goto ZustandS35;
     }
   else
     {
       if (c == EOF)
	 fprintf(stderr, "%s\n", Meldung(14));
       else
	 {
	   fprintf(stderr, "%s\n", Meldung(8));
	   Fehlerumgebung(fp, c);
	 }
       return Fehler;
     }

 ZustandS36 :

   if (Klauselende(c))
     {
       if (! uebernehmenKlausel())
	 {
	   fprintf(stderr, "%s %6d\n", Meldung(41), maxk);
	   return Fehler;
	 }
       c = getc(fp); goto ZustandS31;
     }
   else if (isspace(c))
     {
       c = getc(fp); goto ZustandS36;
     }
   else if (isalnum(c))
     {
       *(freiSymbole++) = c;
       e = Pos;
       c = getc(fp); goto ZustandS35;
     }
   else if (Negzeichen(c))
     {
       e = Neg;
       c = getc(fp); goto ZustandS34;
     }
   else
     {
       if (c == EOF)
	 fprintf(stderr, "%s\n", Meldung(14));
       else
	 {
	   fprintf(stderr, "%s\n", Meldung(8));
	   Fehlerumgebung(fp, c);
	 }
       return Fehler;
     }


/* ---------------------- */



S4:

 ZustandS41 :

   if (c == EOF)
     goto Phase2;
   else if (c == '\n')
     {
       leereKlausel = true; K0++; K++;
       c = getc(fp); goto ZustandS41;
     }
   else if (isspace(c))
     {
       c = getc(fp); goto ZustandS41;
     }
   else if (Kommzeichen(c))
     {
       c = getc(fp); goto ZustandS42;
     }
   else if (isalnum(c))
     {
       *(freiSymbole++) = c;
       e = Pos; Tautologie = false;
       aktp0 = aktp = 0;
       c = getc(fp); goto ZustandS45;
     }
   else if (Negzeichen(c))
     {
       e = Neg; Tautologie = false;
       aktp0 = aktp = 0;
       c = getc(fp); goto ZustandS44;
     }
   else
     {
       fprintf(stderr, "%s\n", Meldung(8));
       Fehlerumgebung(fp, c);
       return Fehler;
     }

 ZustandS42 :

   if (c == EOF)
     {
       fprintf(stderr, "%s\n", Meldung (9));
       return Fehler;
     }
   else if (c == '\n')
     {
       c = getc(fp); goto ZustandS41;
     }
   else
     {
       c = getc(fp); goto ZustandS42;
     }

 ZustandS44 :

   if (isspace(c))
     {
       c = getc(fp); goto ZustandS44;
     }
   else if (isalnum(c))
     {
       *(freiSymbole++) = c;
       c = getc(fp); goto ZustandS45;
     }
   else
     {
       if (c == EOF)
	 fprintf(stderr, "%s\n", Meldung(14));
       else
	 {
	   fprintf(stderr, "%s\n", Meldung(13));
	   Fehlerumgebung(fp, c);
	 }
       return Fehler;
     }

 ZustandS45 :

   if (c == '\n')
     {
       *(freiSymbole++) = '\0';
       v = eintragen();
       if (v == 0)
	 {
	   fprintf(stderr, "%s %6d\n", Meldung(35), maxn);
	   return Fehler;
	 }
       if (! uebernehmenLiteral((e == Pos) ? (int) v : - (int) v))
	 {
	   fprintf(stderr, "%s %7d\n", Meldung(38), maxl);
	   return Fehler;
	 }
       if (! uebernehmenKlausel())
	 {
	   fprintf(stderr, "%s %6d\n", Meldung(41), maxk);
	   return Fehler;
	 }
       c = getc(fp); goto ZustandS41;
     }
   else if (isspace(c))
     {
       *(freiSymbole++) = '\0';
       v = eintragen();
       if (v == 0)
	 {
	   fprintf(stderr, "%s %6d\n", Meldung(35), maxn);
	   return Fehler;
	 }
       if (! uebernehmenLiteral((e == Pos) ? (int) v : - (int) v))
	 {
	   fprintf(stderr, "%s %7d\n", Meldung(38), maxl);
	   return Fehler;
	 }
       c = getc(fp); goto ZustandS46;
     }
   else if (isalnum(c))
     {
       *(freiSymbole++) = c;
       c = getc(fp); goto ZustandS45;
     }
   else
     {
       if (c == EOF)
	 fprintf(stderr, "%s\n", Meldung(14));
       else
	 {
	   fprintf(stderr, "%s\n", Meldung(8));
	   Fehlerumgebung(fp, c);
	 }
       return Fehler;
     }

 ZustandS46 :

   if (c == '\n')
     {
       if (! uebernehmenKlausel())
	 {
	   fprintf(stderr, "%s %6d\n", Meldung(41), maxk);
	   return Fehler;
	 }
       c = getc(fp); goto ZustandS41;
     }
   else if (isspace(c))
     {
       c = getc(fp); goto ZustandS46;
     }
   else if (isalnum(c))
     {
       *(freiSymbole++) = c;
       e = Pos;
       c = getc(fp); goto ZustandS45;
     }
   else if (Negzeichen(c))
     {
       e = Neg;
       c = getc(fp); goto ZustandS44;
     }
   else
     {
       if (c == EOF)
	 fprintf(stderr, "%s\n", Meldung(14));
       else
	 {
	   fprintf(stderr, "%s\n", Meldung(8));
	   Fehlerumgebung(fp, c);
	 }
       return Fehler;
     }


/* ------------------------------------ */

/*  Nun sind P0, N0, K0, L0 und P, K, L berechnet, und */
/*    die Eingabe ist in LitTab abgespeichert (Klauseln */
/*    getrennt durch 0). */

Phase2:

 free(Hashtabelle); Hashtabelle = NULL;
 if (! Belegung)
   {
     free(VarTab); VarTab =  NULL;
     free(Eingabesymbole0); Eingabesymbole0 = NULL;
   }

 if (K == 0)
   {
     N = 0;
     return Sat;
   }

 N = N0;
 if (globalTautologie)
   /* es muss N neu berechnet werden */
   /* und die neuen Variablennummern neu bestimmt werden */
   {
     unsigned int m, i;
     int l; unsigned int v0, v; VZ e;
     N = 0;
     Hashtabelle = (unsigned int *) xmalloc((N0 + 1) * sizeof(unsigned int));
     memset(Hashtabelle, 0, (N0 + 1) * sizeof(unsigned int));
     if (Belegung)
       VarTab1 = (unsigned int *) xmalloc((N0 + 1) * sizeof(unsigned int));
     for (m = i = 0; m < L; m++, i++)
       {
	 l = LitTab[i];
	 if (l == 0)
	   l = LitTab[++i];
	 if (l > 0)
	   {
	     v0 = l; e = Pos;
	   }
	 else
	   {
	     v0 = -l; e = Neg;
	   }
	 v = *(Hashtabelle + v0);
	 if (v == 0)
	   {
	     N++;
	     v = *(Hashtabelle + v0) = N;
	     if (Belegung)
	       VarTab1[N] = v0;
	   }
	 LitTab[i] = (e == Pos) ? (int) v : - (int) v;
       }
     free(Hashtabelle); Hashtabelle = NULL;
   }

 if (leereKlausel)
   return Unsat;


 if (EinerKlausel)
   {
     typedef struct {
       int *Anfang;
       int *Ende;
       bool erfuellt;
     } Klauselinfo;
     typedef struct Vorkommen {
       struct Vorkommen *naechstes;
       Klauselinfo *Klausel;
     } Randvorkommen;

     unsigned int AnzahlAnker = 2 * N + 1;
     unsigned int PositionAnker = N;

     Randvorkommen *Anfangsvorkommen; Randvorkommen *freiesAnfvork;
     Randvorkommen *Endvorkommen; Randvorkommen *freiesEndvork;
     Klauselinfo *BeginnKlauseln; Klauselinfo *freieKlausel;
     bool *belegt;
     int *EinerKlauseln; int *sz;

     void *Z; unsigned int k;
     int *i;
     Klauselinfo *Klausel;
     unsigned int aktp;

     Z0 = xmalloc( 2 * (AnzahlAnker + K) * sizeof(Randvorkommen) +
		   K * sizeof(Klauselinfo) +
		   AnzahlAnker * sizeof(bool) +
		   N * sizeof(LIT));
     Anfangsvorkommen = (Randvorkommen *) Z0; Z = (void *) (Anfangsvorkommen + AnzahlAnker); Anfangsvorkommen += PositionAnker;
     freiesAnfvork = (Randvorkommen *) Z; Z = (void *) (freiesAnfvork + K);
     Endvorkommen = (Randvorkommen *) Z; Z = (void *) (Endvorkommen + AnzahlAnker); Endvorkommen += PositionAnker;
     freiesEndvork = (Randvorkommen *) Z; Z = (void *) (freiesEndvork + K);
     BeginnKlauseln = freieKlausel = (Klauselinfo *) Z; Z = (void *) (BeginnKlauseln + K);
     belegt = (bool *) Z; Z = (void *) (belegt + AnzahlAnker);
     belegt += PositionAnker;
     EinerKlauseln = (int *) Z; sz = EinerKlauseln;

     if (Belegung)
       Pfad0 = (int *) xmalloc(N * sizeof(LIT));

     for (v = 1; v <= N; v++)
       {
	 (Anfangsvorkommen + v) -> naechstes = NULL;
	 (Anfangsvorkommen - (int) v) -> naechstes = NULL;
	 (Endvorkommen + v) -> naechstes = NULL;
	 (Endvorkommen - (int) v) -> naechstes = NULL;
	 *(belegt + v) = false;
	 *(belegt - (int) v) = false;
       }

     i = LitTab;
     for (k = 0; k < K; ++k) {
       const int erstesLit = *i; int letztesLit;

       int* const i0 = i;
       do
         ++i;
       while (*i != 0);
       if (i - i0 == 1) {
         if (belegt[erstesLit])
           return Unsat;
         if (! belegt[-erstesLit]) {
           belegt[-erstesLit] = true;
           *(sz++) = -erstesLit;
         }
       }
       else {
         freiesAnfvork -> naechstes = Anfangsvorkommen[erstesLit].naechstes;
         Anfangsvorkommen[erstesLit].naechstes = freiesAnfvork;
         freiesAnfvork -> Klausel = freieKlausel;
         letztesLit = *(i-1);
         freiesEndvork -> naechstes = Endvorkommen[letztesLit].naechstes;
         Endvorkommen[letztesLit].naechstes = freiesEndvork;
         freiesEndvork -> Klausel =  freieKlausel;
         freieKlausel -> erfuellt = false;
         freieKlausel -> Anfang = i0;
         freieKlausel -> Ende = i - 1;
         freiesAnfvork++; freiesEndvork++; freieKlausel++;
       }
       ++i;
     }

     do {
       int l;
       Randvorkommen *x; Randvorkommen *xn;

       l = *(--sz);
       if (Belegung)
         Pfad0[InitEinerRed++] = l;
       else
         InitEinerRed++;

       for (x = Anfangsvorkommen[l].naechstes; x != NULL; x = xn) {
         xn = x -> naechstes;
         Klausel = x -> Klausel;
         if (Klausel -> erfuellt)
           continue;
         aktp = 0;
#ifndef NDEBUG
         int* i0 = 0;
#else
         int* i0;
#endif
         for (i = (Klausel -> Anfang) + 1; i != (Klausel -> Ende) + 1; i++) {
           if (belegt[-*i]) {
             Klausel -> erfuellt = true;
             break;
           }
           if (! belegt[*i]) {
             if (++aktp == 1)
               i0 = i;
             else
               break;
           }
         }
         if (Klausel -> erfuellt)
           continue;
         if (aktp == 0)
           return Unsat;
         if (aktp == 1) {
           assert(i0);
           belegt[-*i0] = true;
           *(sz++) = -*i0;
           Klausel -> erfuellt = true;
         }
         else {
           assert(i0);
           const int a = *i0;
           x -> naechstes = Anfangsvorkommen[a].naechstes;
           Anfangsvorkommen[a].naechstes = x;
           Klausel -> Anfang = i0;
         }
       }

       for (x = Endvorkommen[l].naechstes; x != NULL; x = xn) {
         xn = x -> naechstes;
         Klausel = x -> Klausel;
         if (Klausel -> erfuellt)
           continue;
         aktp = 0;
#ifndef NDEBUG
         int* i0 = 0;
#else
         int* i0;
#endif
         for (i = (Klausel -> Ende) - 1; i != (Klausel -> Anfang) - 1; i--) {
           if (belegt[-*i]) {
             Klausel -> erfuellt = true;
             break;
           }
           if (! belegt[*i]) {
             if (++aktp == 1)
               i0 = i;
             else
               break;
           }
         }
         if (Klausel -> erfuellt)
           continue;
         if (aktp == 0)
           return Unsat;
         if (aktp == 1) {
           assert(i0);
           belegt[-*i0] = true;
           *(sz++) = -*i0;
           Klausel -> erfuellt = true;
         }
         else {
           assert(i0);
           const int a = *i0;
           x -> naechstes = Endvorkommen[a].naechstes;
           Endvorkommen[a].naechstes = x;
           Klausel -> Ende = i0;
         }
       }
     }
     while (sz != EinerKlauseln);

     Hashtabelle = (unsigned int *) xmalloc((N + 1) * sizeof(unsigned int));
     memset(Hashtabelle, 0, (N + 1) * sizeof(unsigned int));
     if (Belegung)
       VarTab2 = (unsigned int *) xmalloc((N - InitEinerRed + 1) * sizeof(unsigned int));
     P = N = K = L = 0;
     int* i0 = LitTab;
     assert(i0);
     for (Klausel = BeginnKlauseln; Klausel != freieKlausel; Klausel++) {
       if (Klausel -> erfuellt)
         continue;
       for (i = Klausel -> Anfang; i != (Klausel -> Ende) + 1; i++)
         if (belegt[-*i]) {
           Klausel -> erfuellt = true;
           break;
         }
       if (Klausel -> erfuellt)
         continue;
       aktp = 0;
       for (i = Klausel -> Anfang; i != (Klausel -> Ende) + 1; i++)
         if (! belegt[*i]) {
	       unsigned int v0, v; int l; VZ e;
	       aktp++; l = *i;
	       if (l > 0) {
                 v0 = l; e = Pos;
               }
	       else {
                 v0 = -l; e = Neg;
               }
	       v = *(Hashtabelle + v0);
	       if (v == 0) {
		   ++N;
		   v = *(Hashtabelle + v0) = N;
		   if (Belegung)
		     VarTab2[N] = v0;
               }
	       *(i0++) = (e == Pos) ? (int) v : - (int) v; L++;
         }
       if (aktp > P)
         P = aktp;
       K++;
       *(i0++) = 0;
     }
     free(Hashtabelle); Hashtabelle = NULL;
     free(Z0); Z0 = NULL;

     if (K == 0)
       return Sat;
   }


 /*   Erzeugung aller Vektoren, deren Laenge von */
 /*     P, N, K oder L abhaengt */

 if (nurVorreduktion)
   return Norm;

 InitParameter();
 {
   void *Basis;

   Basis0 = Basis = xmalloc(BedarfVarLitKlmV() + BedarfBelegungV() + BedarfFilterV() + BedarfReduktionsV() + BedarfAbstandsV()
#ifdef BAUMRES
			    + BedarfBaumResV()
#endif
			    );
#ifdef BAUMRES
   Basis = BaumResV(Basis);
/* vor VarLitKlm, da Adresse von aktrelV benoetigt wird */
#endif
   Basis = VarLitKlmV(Basis);
   Basis = BelegungV(Basis);
   Basis = FilterV(Basis);
   Basis = ReduktionsV(Basis);
   Basis = AbstandsV(Basis);
 }

/* Zweite Stufe */

/*   Nun wird die Klauselmenge hergestellt, LaenK, aktAnzK belegt, */
/*   und aktP, aktN werden initialisiert. */

  /* Durchlaufen der Literalvorkommen in LitTab */
  /* Belegen der Literalvorkommen und von LaenK, aktAnzK */

 {
   unsigned int k;
   int *j;
   j = LitTab;
   for (k = 0; k < K; k++)
     {
       Klauselanfangen();
       while (*j != 0)
	 {
	   Literaleintragen(*j);
	   j++;
	 }
       Klauselbeenden();
       j++;
     }
 }

 if (Belegung)
   {
     unsigned int v, vi;
     for (v = 1; v <= N; v++)
       {
	 vi = v;
	 if (VarTab2 != NULL)
	   vi = VarTab2[vi];
	 if (VarTab1 != NULL)
	   vi = VarTab1[vi];
	 Symboleintragen(v, VarTab[vi]);
       }
   }

 free(LitTab); LitTab = NULL;
 free(VarTab2); VarTab2 = NULL;
 if (! Belegung)
   {
     free(VarTab1); VarTab1 = NULL;
     free(VarTab); VarTab = NULL;
   }

 aktP = P; /* Initialisieren von aktP */
 aktN = N; /* Initialisieren von aktN */

 init2Klauseln = aktAnzK[2];

 extern enum Ausgabeformat Format;
 if (Format == XML_Format)
   for (unsigned int i = 0; i <= P - 2; InitAnzK[i+2] = aktAnzK[i+2], i++);
 // i wird von 0 gestartet, um Korrektheit auch im Falle von
 // P = std::numeric_limits<unsigned int>::max() zu garantieren

 return Norm;

}

/* ------------------------------------------------------------- */


void AufraeumenEinlesen() {
  free(Hashtabelle); Hashtabelle = NULL;
  free(LitTab); LitTab = NULL;
  free(Eingabesymbole0); Eingabesymbole0 = NULL;
  free(VarTab); VarTab = NULL;
  free(VarTab1); VarTab1 = NULL;
  free(VarTab2); VarTab2 = NULL;
  free(Pfad0); Pfad0 = NULL;
  free(Basis0); Basis0 = NULL;
  free(Z0); Z0 = NULL;
}

/* ------------------------------------------------------------- */


__inline__ static char* Symbol1(unsigned int v) {
  if (VarTab1 != NULL) v = VarTab1[v];
  return VarTab[v];
}


void AusgabeBelegung(FILE* const fp) {
  assert(fp);
  extern enum Ausgabeformat Format;
  const char* Einrueckung = 0;

  if (Format == Dimacs_Format) fprintf(fp, "v");
  else {
    assert(Format == XML_Format);
    extern bool Dateiausgabe;
    Einrueckung = (Dateiausgabe) ? "" : " ";
    fprintf(fp, "%s<solution>\n", Einrueckung);
  }

  if (EinerKlausel)
    for (unsigned int i = 0; i < InitEinerRed; ++i) {
      assert(Pfad0[i] > INT_MIN);
      const unsigned int v = abs(Pfad0[i]);
      const VZ e = (Pfad0[i] > 0) ? Pos : Neg;
      if (Format == Dimacs_Format)
        if (e == Neg) fprintf(fp, " %s", Symbol1(v));
        else fprintf(fp, " -%s", Symbol1(v));
      else {
        assert(Einrueckung);
        fprintf(fp, "%s  <value var = \"%s\"> %d </value>\n", Einrueckung, Symbol1(v), e);
      }
    }
  {
    const Pfadinfo* const Z = Tiefe;
    for (Tiefe = Pfad; Tiefe < Z; ++Tiefe) {
      const LIT l = PfadLit(); const VAR v = Var(l);
      const VZ e = (l == Literal(v, Pos)) ? Pos : Neg;
      if (Format == Dimacs_Format)
        if (e == Neg) fprintf(fp, " %s", Symbol(v));
        else fprintf(fp, " -%s", Symbol(v));
      else {
        assert(Einrueckung);
        fprintf(fp, "%s  <value var = \"%s\"> %d </value>\n", Einrueckung, Symbol(v), e);
      }
    }
    Tiefe = (Pfadinfo*) Z;
  }

  if (Format == Dimacs_Format) fprintf(fp, " 0\n");
  else {
    assert(Einrueckung);
    fprintf(fp, "%s</solution>\n", Einrueckung);
    extern bool Dateiausgabe;
    if (! Dateiausgabe) fprintf(fp, "</SAT-Solver.output>\n");
  }
}

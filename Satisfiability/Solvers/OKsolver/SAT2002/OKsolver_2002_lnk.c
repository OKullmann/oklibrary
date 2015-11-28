// Oliver Kullmann, 5.3.1998 (Frankfurt)
/* Copyright 1998 - 2007, 2008, 2009, 2011, 2015 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKsolver/SAT2002/OKsolver_2002_lnk.c
  \brief The main compilation unit for the OKsolver_2002
*/

                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */


/* Hauptprogramm */

/* Einheit: OK */


/* 21.8.1998: Ersetzen von "ErfK" durch "NeuK" */

/* 20.9.1998: Bisher wurde beim Finden einer Autarkie oder einer */
/* verallg. 1-Klauseln-Elim. ein neuer "Single-"Knoten erzeugt. */
/* Nun wird diese Belegung direkt ausgefuehrt, und die Auswahl */
/* einer Verzweigungs-Variablen neu begonnen. */

/* 13.11.1998: Resolutions-Baum-Verwaltung */

/* 31.3.2000: InitAbstand2() im Falle, dass DYNAMISCH gesetzt wurde */

/* 27.4.2000: Einlesen von OKVar.h */

/* 24.7.2000: Die Variablen LitTab und KlTab aus der Prozedure Einlesen */
/* nun mit "calloc" dynamisch erzeugt, und mit "free" dann geloescht. */

/* 25.7.2000: Ersetzen der drei calloc-Aufrufe durch einen malloc-Aufruf. */

/* 27.7.2000: Elimination der Rekursion (mittels Indices) */

/* 29.1.2001: Nun wurde die Elimination der Rekursion dynamisch durchgefuehrt, */
/* d.h., es wird nur soviel Speicherplatz fuer die lokalen Variablen angefordert, */
/* wie auch wirklich benoetigt wird. */

/* 29.1.2001: Die Speicherung der zweiten Belegung nun korrekt durchgefuehrt */
/* (mittels Dynamisierung). */

/* 30.1.2001: Elimination von OKVar.h */

/* 18.2.2001: Liste von Argumenten (mit Optionen) */

/* 20.2.2001: Ausgabe der Gesamtzeit. */

/* 27.2.2001: Realisierung der 1-Klauseln-Elimination fuer die Eingabe */

/* 28.2.2001: Freisetzen von Speicher und Datenstroemen wird nun ueber den */
/* NULL-Wert geregelt. */
/* 28.2.2001: Ein Syntax-Fehler fuehrt nun nicht mehr zum Abbruch. */
/* 28.2.2001: Senden des Signals SIGUSR1 bewirkt Ausdruck der Statistik */
/* (aber nicht in die Datei), und SIGUSR2 bewirkt Abbruch der Bearbeitung */
/* der aktuellen Formel und Ausdruck der Statistik (diesmal in die Datei, */
/* falls gewuenscht). */

/* 3.3.2001: Dynamisierung von relVarM; die Stapelzeiger nun wirklich als */
/* Zeiger; H und EK im Reduktionsmodul, EinerKl und Huelle im Filtermodul, */
/* sowie relVarM, relVarK und Pfad zusammengefasst; LITTAB als Uebersetzer- */
/* Option bewirkt, dass die Variablenmengen der Eingabeklauseln mit weniger */
/* Speicherplatz abgespeichert werden. */

/* 6.3.2001: Feld 14 (Redplus) aus der Statistikausgabe gestrichen. */


/* ------------------------------------------------------------- */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>
#include <assert.h>
#include <stddef.h>

#include <sys/stat.h>
#include <unistd.h>

#include "OK.h"
#include "BaumRes.h"
#include "VarLitKlm.h"
#include "Filter.h"
#include "Belegung.h"
#include "Abstand.h"
#include "Projektion.h"
#include "Einlesen.h"
#include "Parameter.h"
#include "Ausgaben.h"
#include "Speicher.h"
#include "Reduktion.h"

#ifdef LOKALLERNEN
#include "lokalesLernen.h"
#endif

#ifdef SYSTIME
#include <sys/times.h>
#endif

/* ------------------------------------------------------------- */

const bool internal = false; // keine Entwicklungsversion

unsigned int Sprache = SPRACHE;
unsigned int Standard = STANDARD;

#ifndef BELEGUNG
bool Belegung = false;
#else
bool Belegung = true;
#endif

/* Schalter: */

bool Dateiausgabe = false;
bool Monitor = false;
bool nurVorreduktion = false;
bool Schranken = false;
unsigned int Zeitschranke = 0;
bool randomisiert = false;
//! using the solver in splitting-only mode
bool splitting_only = false;
//! for the splitting-only mode, whether n is to be used instead of depth
bool splitting_n = true;
//! whether the splittings are stored in a directory or a single file
bool splitting_file = false;
//! true iff a splitting-only computation is aborted
bool splitting_abortion = false;

// Setzen des voreingestellen Ausgabeformates

#ifdef DIMACSAUS
#  ifdef XMLAUS
#    error "Keine Kombination von DIMACS- und XML-Format moeglich!"
#  endif
bool spezRueckgabe = true; // Kodierung des Ergebnisses im Rueckgabewert
enum Ausgabeformat Format = Dimacs_Format;
#else
bool spezRueckgabe = false;
Ausgabeformat Format = XML_Format;
#endif
// am 10.3.2002 fuer den SAT-Wettbewerb hinzugefuegt
// falls DIMACS_Format gesetzt ist, so wird die Ausgabe einer Belegung
// als Liste von wahr gemachten Literalen ausgegeben (in einer Zeile),
// und die Kommentarzeile wird in zwei Zeilen aufgespalten:
// der SAT-Wert (0, 1 oder 2) ergibt
// s UNSATISFIABLE (SATISFIABLE, UNKNOWN)
// und die restlichen Zahlen werden in eine Zeile beginnend mit "c "
// geschrieben.
// (Fuer den Wettbewerb muss sowohl BELEGUNG als auch DIMACSAUS
// gesetzt werden.)

unsigned int MAXN = 30000;
unsigned int MAXL = 400000;
unsigned int MAXK = 150000;

/* ------------------------------------------------------------- */

/* Einheiten pro Sekunde */
#ifndef SYSTIME
  static const long EPS = CLOCKS_PER_SEC;
#else
  static long EPS;
#endif

#ifdef SYSTIME
  struct tms SysZeit;
  struct tms* Zeiger = &SysZeit;
#endif

clock_t akkVerbrauch = 0; /* akkumulierter Verbrauch */

/* ------------------------------------------------------------- */

/* Die Darstellung der Eingabe */

unsigned int P, P0, N, N0, K, K0, L, L0, aktN, aktP;

/* Hilfsvariablen fuer Reduktion und Filter */

unsigned int Runde;

/* ------------------------------------------------------------- */

/* Statistik */

StatisticsCount Knoten, SingleKnoten, VerSingleKnoten, QuasiSingleKnoten, PureL, Autarkien, V1KlRed, FastAutarkien, InitEinerRed, neue2Klauseln, maxneue2K;

StatisticsCount_short Suchbaumtiefe, Ueberschreitung2, init2Klauseln;

typedef long double Probability;
//! the proportion of the search space of 2^N assignments where unsatisfiability has been established, and that by visiting the branches
static Probability proportion_searched;
//! the additional proportion due to single nodes
static Probability proportion_single;

static clock_t Verbrauch;

static const char* aktName;
static const char* Wurzel = NULL;

//! the depth of the current node
static unsigned int Rekursionstiefe;

enum Ergebniswerte {SAT = 1, UNSAT = 0, Unbestimmt = 2};

static enum Ergebniswerte s = Unbestimmt; /* Ergebniswert */

#ifdef ALLSAT
  nsat_t number_satisfying_assignments;
#endif

/* ------------------------------------------------------------- */

/* Lokale Datenstrukturen und Variablen */

/* Der Stapel, der die Belegungen fuer den jeweils zweiten Zweig enthaelt */

static StapeleintragFZ zweiteBel = NULL;

static unsigned int Zeiger2;
static unsigned int Groesse2;

/* Zur Simulation der Rekursion */

enum Spruenge { SAT1, SAT2 };

struct Sammlung {
  unsigned int P2, N2, altZeiger2;
  enum Spruenge Ruecksprung;
  unsigned int* AnzK2;
#ifndef LOKALLERNEN
  Pfadinfo* altTiefe;
#else
  Marken* Marke;
#endif
#ifdef OUTPUTTREEDATAXML
  // clock_t start_run_time;
  StatisticsCount number_2_reductions_at_new_node;
#endif
  struct Sammlung* davor;
  struct Sammlung* danach;
};

static struct Sammlung* SatVar;
static struct Sammlung* SatVar0 = NULL;
/* speichert den initialen Wert von SatVar */

/* ------------------------------------------------------------- */

/* Monitoring the search */

//! the (precise) depth of the monitoring nodes
static unsigned int Beobachtungsniveau = 6;

//! array containing the constants 2^(Beobachtungsniveau-1), ..., 2^0
static unsigned int* Zweiglast = NULL;
/*!
  \brief Constant with value 2^Beobachtungsniveau

  <ul>
   <li> There are exactly 2^Beobachtungsniveau many monitoring nodes (all at
   depth (exactly) Beobachtungsniveau). </li>
   <li> If nodes don't get expanded in the search tree up to depth
   Beobachtungsniveau (due to earlier decisions, or due to a single node),
   then they are "virtually" expanded. </li>
  </ul>
*/
static unsigned int Gesamtlast;

//! in case of splitting_only the number of sub-problems yet encountered
static unsigned int splitting_cases;
//! name of directory resp. file with splitting cases
static const char* splitting_store;
//! the file for storing the splittings (if splitting_file == true)
static FILE* fpsplit = NULL;
//! file-name for splitting-decisions
static char* splitting_decisions;
//! the file for the splitting-decisions
static FILE* fpsplitdec = NULL;

/*!
  \brief Helper array with Beobachtungsniveau many elements

  For depth 0 <= d < Beobachtungsniveau the number of monitoring nodes yet
  solved which lie above solved nodes at depth d.
*/
static unsigned int* beobachtet = NULL;
//! (total) count of monitoring nodes yet solved (search completed iff totalbeobachtet is equal to Gesamtlast)
static unsigned int totalbeobachtet;

//! output file for monitoring-results
static FILE* fpmo = NULL;

/*!
  \brief Given the current count of monitoring nodes, output the monitoring
  statistics to stdout, and to the file if file-output is set.

  Details:
  <ul>
   <li> This function is only called when monitoring is activated. </li>
   <li> Called whenever a branch for a node with depth < Beobachtungsniveau
   has been solved. </li>
   <li> Output only happens when at least one new monitoring node has been
   completed. </li>
   <li> Has the task of updating variable totalbeobachtet. </li>
   <li> The output for the number of nodes is the total number of nodes
   solved after the last call. </li>
   <li> So the numbers of nodes add up to the total number of nodes in the
   whole search tree. </li>
  </ul>
*/
__inline__ static void Monitorausgabe(const unsigned int count_monitor_nodes) {
  static StatisticsCount old_nodes = 0;
  static StatisticsCount old_single_nodes = 0;
  static StatisticsCount old_autarkies = 0;
  static StatisticsCount old_2reductions = 0;
  static clock_t old_total_time = 0; /* in ticks */
  if (count_monitor_nodes > totalbeobachtet) {
    totalbeobachtet = count_monitor_nodes;
#ifndef SYSTIME
    const clock_t total_time = clock() - akkVerbrauch;
#else
    times(Zeiger);
    const clock_t total_time = SysZeit.tms_utime - akkVerbrauch;
#endif
    const StatisticsCount new_nodes = Knoten - old_nodes;
    const double average_nodes = (double) Knoten / count_monitor_nodes;
    const double predicted_nodes = Gesamtlast * average_nodes;
    const double new_time = (double) (total_time - old_total_time) / EPS; /* in sec */
    const double average_time = ((double) total_time / EPS) / count_monitor_nodes;
    {
      const double predicted_remaining_time =
        (Gesamtlast - count_monitor_nodes) * average_time;
      double time_ = round(predicted_remaining_time);
      const double sec = fmod(time_, 60);
      time_ = (time_ - sec) / 60;
      const double min = fmod(time_, 60);
      time_ = (time_ - min) / 60;
      const double hours = fmod(time_, 24);
      time_ = (time_ - hours) / 24;
      const double days = fmod(time_, 365);
      const double years = (time_ - days) / 365;
      printf(
             "%6d:%7ld %9.2f %9.2E %8.2fs %8.2fs %5.0fy%4.0fd%3.0fh%3.0fm%3.0fs %5ld %5ld %4d\n",
             count_monitor_nodes,
             new_nodes,
             average_nodes,
             predicted_nodes,
             new_time,
             average_time,
             years, days, hours, min, sec,
             SingleKnoten - old_single_nodes,
             Autarkien - old_autarkies,
             Suchbaumtiefe
             );
    }
    if (Dateiausgabe) {
      const double average_new_2reductions =
        (new_nodes == 0) ? 0 : (double) (V1KlRed-old_2reductions)/new_nodes;
      assert((new_nodes != 0) || (V1KlRed == old_2reductions));
      fprintf(fpmo,
              "%9d %6ld %9.3f %9.3f %9.3f %6ld %5ld %5d %8.2f\n",
              count_monitor_nodes,
              new_nodes,
              average_nodes,
              new_time,
              average_time,
              SingleKnoten - old_single_nodes,
              Autarkien - old_autarkies,
              Suchbaumtiefe,
              average_new_2reductions
              );
      old_2reductions = V1KlRed;
    }
    fflush(NULL);
    old_nodes = Knoten;
    old_single_nodes = SingleKnoten;
    old_autarkies = Autarkien;
    old_total_time = total_time;
  }
}

/*!
  \brief In monitoring mode, output branching literal x (with the given
  depth in the search tree).

  Currently output actually only happens if output of a satisfying assignment
  is enabled.
*/
__inline__ static void Verzweigungsliteralausgabe(const LIT x, const unsigned int Tiefe) {
  const VAR v = Var(x);
  const VZ e = (x == Literal(v, Pos)) ? Pos : Neg;
  if (Belegung) fprintf(fpmo, "# %-6d %7s %d\n", Tiefe, Symbol(v), e);
  fflush(NULL);
}

/*!
  \brief Output of decision levels

  Output first the number of decision on the current path, and then
  the indices (starting with 0) of the decision variables in the
  current partial assignment.
*/
__inline__ static void output_decision_levels(FILE* const fp) {
  assert(fp);
  fprintf(fp, "%d ", Rekursionstiefe);
  for (struct {const struct Sammlung* i; ptrdiff_t c;} l = {SatVar0, 0}; l.c !=  (ptrdiff_t) Rekursionstiefe; l.i=l.i->danach, ++l.c)
    fprintf(fp, "%ld ", l.i->altTiefe - Pfad);
  fprintf(fp, "\n");
}


/* ------------------------------------------------------------- */

typedef enum { gleich = 0, groesser = 1, kleiner = 2} VERGL;

/* Zur Bestimmung, ob einer Gleitpunktzahl "wirklich" groesser ist als eine andere: */

//! determines whether a > b, a == b, a < b, up to rounding errors
static VERGL Vergleich(const float a, const float b) {
  const float h = b * 4 * FLT_EPSILON;
  if (a > b + h) return groesser;
  else if (a < b - h) return kleiner;
  else return gleich;
}

/* ------------------------------------------------------------- */

/* Prozeduren zur Speicherverwaltung */

static struct Sammlung* neuerKnoten() {
  struct Sammlung* s;
  s = (struct Sammlung*) xmalloc(sizeof(struct Sammlung) + (P + 1) * sizeof(unsigned int));
  s -> AnzK2 = (unsigned int*)(s + 1);
  s -> danach = NULL;
  return s;
}

/* ------------------------------------------------------------- */

/* Randomisierung */

static long unsigned int Schluessel = 1;
static long unsigned int randx;
static float Verhaeltnis = 0.2;

__inline__ static void srand_S() { randx = Schluessel; }

__inline__ static int rand_S() {
  return(((randx = randx * 1103515245L + 12345)>>16) & 0x7fff);
}

__inline__ static float Verschmierung(const double x) {
  return (rand_S() / ((float) 0x7fff) * Verhaeltnis + 1) * x;
}

/* ------------------------------------------------------------- */

/* Initialisierung */

void InitSat() {
  Groesse2 = N;
  zweiteBel = (StapeleintragFZ) xmalloc(Groesse2 * sizeof(StapeleintragF));
  SatVar0 = SatVar = neuerKnoten();
  SatVar -> davor = NULL;
  
  Runde = 0; Zeiger2 = 0;
  Rekursionstiefe = 0;
  proportion_searched = 0;
  proportion_single = 0;
  if (splitting_only) splitting_cases = 0;

  if (Monitor && (! nurVorreduktion)) {
    totalbeobachtet = 0;
    Zweiglast = (unsigned int*) xmalloc(Beobachtungsniveau * sizeof(unsigned int));
    {unsigned int p = 1;
     for (unsigned int* Z = Zweiglast+Beobachtungsniveau; Z != Zweiglast; p *= 2)
       *(--Z) = p;
     Gesamtlast = p;
    }
    beobachtet = (unsigned int*) xmalloc(Beobachtungsniveau * sizeof(unsigned int));
    beobachtet[0] = 0;
  }
}

static void AufraeumenSat() {
  struct Sammlung* Z; struct Sammlung* Z0;
  
  Knoten = SingleKnoten = VerSingleKnoten = QuasiSingleKnoten = PureL = Autarkien = V1KlRed = Suchbaumtiefe = Ueberschreitung2 = FastAutarkien = InitEinerRed = neue2Klauseln = maxneue2K = init2Klauseln = 0;
  proportion_searched = proportion_single = 0;
  Tiefe = NULL;

  free(zweiteBel); zweiteBel = NULL;

  Z0 = SatVar0;
  while (Z0 != NULL) {
    Z = Z0 -> danach;
    free(Z0);
    Z0 = Z;
  }
  SatVar0 = NULL;

  free(Zweiglast); Zweiglast = NULL;
  free(beobachtet); beobachtet = NULL;
}


/* ------------------------------------------------------------- */

#ifdef OUTPUTTREEDATAXML

static char* NameTreeDataFile = NULL;
static FILE* TreeDataFile = NULL;

void BeginTreeElement() {
  fprintf(TreeDataFile, "<t l=\"%ld\">", V1KlRed - SatVar -> number_2_reductions_at_new_node);
}
void EndTreeElement() {
  fprintf(TreeDataFile, "</t>\n");
}
void FinaliseSATPath() {
  do {
    EndTreeElement();
    SatVar = SatVar -> davor;
  } while (SatVar != NULL);
}

#endif

/*!
  \brief The SAT decision procedure

  <ul>
   <li> Recursion is simulated (the main variable is SatVar). </li>
   <li> Assumes input has been read. </li>
   <li> Returns a decision. </li>
  </ul>
*/

static enum Ergebniswerte SATEntscheidung() {
  VZ optZweig;

#ifdef ALLSAT
  assert(! Belegung);
  mpz_init(number_satisfying_assignments);
#endif

Anfang:

  ++Knoten;

#ifdef OUTPUTTREEDATAXML
  SatVar -> number_2_reductions_at_new_node = V1KlRed;
#endif  

  /* Reduktionen, die von Autarkien nicht affiziert werden */
  /* (zumindest verallgemeinerte 1-Klauseln-Elimination) */

alleReduktionen:

  switch ( Reduktion1() ) {

  case 1:
#ifdef OUTPUTTREEDATAXML
    BeginTreeElement();
    FinaliseSATPath();
#endif  
    return SAT;

  case 2:
    assert(proportion_searched >= 0);
    assert(Vergleich(proportion_searched+proportion_single,1) != groesser);
    proportion_searched += exp2l(- (int) Rekursionstiefe);
#ifdef OUTPUTTREEDATAXML
    BeginTreeElement();
#endif
    {const enum Spruenge r = SatVar -> Ruecksprung;
     SatVar = SatVar -> davor;
     if (SatVar == NULL) {
#ifdef OUTPUTTREEDATAXML
       EndTreeElement();
#endif
       return UNSAT;
     }
     assert(Rekursionstiefe >= 1);
     --Rekursionstiefe;
     switch (r) {
     case SAT1 : goto nachSAT1;
     case SAT2 : goto nachSAT2;
     }
    }
  }

 Schleife:

#ifdef DYNAMISCH
  InitAbstand2();
#endif

/* Zwar werden so die Autarkien nicht in die dynamische Gewichtsberechnung 
   einbezogen, dieser Effekt scheint mir jedoch gering, waehrend ihr Einbezug
   grossen Aufwand mit sich braechte. So wird also nur Reduktion1() 
   beruecksichtigt.
*/

  /* Reduktionen, die von Autarkien affiziert werden */
  /* (zumindest pure Literale): */

  switch ( Reduktion2() ) {
    
  case 1:
#ifdef OUTPUTTREEDATAXML
    BeginTreeElement();
    FinaliseSATPath();
#endif
    return SAT;

  case 2:
    assert(proportion_searched >= 0);
    assert(Vergleich(proportion_searched+proportion_single,1) != groesser);
    proportion_searched += exp2l(- (int) Rekursionstiefe);
#ifdef OUTPUTTREEDATAXML
    BeginTreeElement();
#endif
    {const enum Spruenge r = SatVar -> Ruecksprung;
     SatVar = SatVar -> davor;
     if (SatVar == NULL) {
#ifdef OUTPUTTREEDATAXML
       EndTreeElement();
#endif
       return UNSAT;
     }
     --Rekursionstiefe;
     switch (r) {
     case SAT1 : goto nachSAT1;
     case SAT2 : goto nachSAT2;
     }
    }
  }

  {
    float opta = 0;
    unsigned int optaS = 0;
/*  Schleife ueber alle Variablen, die jeweils dem Filter vorgelegt werden:
    Findet dieser eine Entscheidung oder eine Einer-Verzweigung, so wird eine 
    solche Variable ausgesucht, und die Schleife abgebrochen.
    Andernfalls wird mittels "Abstand" die (Zweier-)Verzweigung bewertet, und
    sie ersetzt, falls besser, die alte, bisher beste Verzweigung.
    (Die Zweigauswahl wird von "Abstand" mitberechnet.) 
*/
    for (VAR v = ersteVar(); echteVar(v); v = naechsteVar(v)) {
      Filter(v);
      if (erfuellt) {
        if (Belegung) { /* Durchfuehrung der Belegung (zur Ausgabe) */
          const unsigned int DN = DeltaN[Zweig][Schalter];
          for (struct {unsigned int i; StapeleintragFZ Z;} l = {0,Huelle[Zweig][Schalter]}; l.i < DN; ++l.i, ++l.Z) {
#ifndef BAUMRES
            belege(*l.Z);
#else
            belege(l.Z -> l);
#endif
          }
        }
#ifdef OUTPUTTREEDATAXML
        BeginTreeElement();
        FinaliseSATPath();
#endif  
        return SAT;
      }
      
      if (reduziert) goto alleReduktionen;
      
      if (Wahl) {
        if (Single) {  /* (zur Zeit) der (nicht-erfuellende) Autarkiefall */
          /* Durchfuehrung der Belegung: */
          const unsigned int DN = DeltaN[Zweig][Schalter];
#ifdef LOKALLERNEN
          eintragenTiefe();
#endif
          for (struct {unsigned int i; StapeleintragFZ Z;} l = {0,Huelle[Zweig][Schalter]}; l.i < DN; ++l.i, ++l.Z) {
#ifndef BAUMRES
            belege(*l.Z);
#else
            belege(l.Z -> l);
#endif
          }
          /* Falls BAUMRES gesetzt ist: */
          /* Da der Standard-Filter nur Autarkien hier liefern kann, */
          /* die nie zur Erzeugung der leeren Klausel beitragen koennen, */
          /* muss hier in relVar nichts eingetragen werden. */
          aktP = LaP[Zweig][Schalter];
          aktN -= DeltaN[Zweig][Schalter];
          memcpy((void*)(aktAnzK + 2), (void*)(LaAnzK[Zweig][Schalter] + 2), (aktP - 1) * sizeof(unsigned int));
          goto Schleife;
        }
        else {
          ++QuasiSingleKnoten;
          Schalter = ! Schalter;
          optZweig = Zweig;
          break;
        }
      }
      {
        Abstand();
        const float a = (randomisiert) ? Verschmierung(Projektion()) : Projektion();
        switch (Vergleich(a, opta)) {
        case gleich :
          if (Projektion2() <= optaS) break;
        case groesser :
          opta = a; optaS = Projektion2();
          Schalter = ! Schalter;
          optZweig = Zweig;
          break;
        case kleiner :
          break;
        }
      }
    }
  }
  
  /* Now the branching variable has been determined. */

  // handling of splitting-only:
  if (splitting_only &&
      ((! splitting_n && Rekursionstiefe == Beobachtungsniveau) ||
       (splitting_n && N - aktN >= Beobachtungsniveau))) {
    ++splitting_cases;
    output_decision_levels(fpsplitdec);
    if (splitting_file) AusgabeBelegung(fpsplit);
    else {
      assert(splitting_cases <= 1073741824U);
      char buf[10+1];
      snprintf(buf,10+1,"%u",splitting_cases);
      char* const name_sc = (char*) xmalloc(strlen(splitting_store)+1+strlen(buf)+1);
      strcpy(name_sc,splitting_store); strcat(name_sc,"/"), strcat(name_sc,buf);
      FILE* const file_sc = fopen(name_sc, "w");
      if (file_sc == NULL) { fprintf(stderr, "%s\n", Meldung(59)); exit(1); }
      AusgabeBelegung(file_sc);
      fclose(file_sc);
    }
    const enum Spruenge r = SatVar -> Ruecksprung;
    SatVar = SatVar -> davor;
    if (SatVar == NULL) {
      assert(Rekursionstiefe == 0);
#ifdef OUTPUTTREEDATAXML
      EndTreeElement();
#endif
      assert(splitting_cases > 0);
      return Unbestimmt;
    }
    assert(Rekursionstiefe >= 1);
    --Rekursionstiefe;
#ifdef BAUMRES
    printf("NOT IMPLEMENTED: combination of -S with BAUMRES!\n");
    exit(1);
#endif
#ifdef LOKALLERNEN
    printf("NOT IMPLEMENTED: combination of -S with LOKALLERNEN!\n");
    exit(1);
#endif
    switch (r) {
    case SAT1 : goto nachSAT1;
    case SAT2 : goto nachSAT2;
    }
  }
  assert(! splitting_only || Rekursionstiefe < Beobachtungsniveau);

  /* Now branching. */

#ifdef OUTPUTTREEDATAXML
  BeginTreeElement();
#endif
#ifndef LOKALLERNEN
  SatVar -> altTiefe = Tiefe;
#else
  SatVar -> Marke = Markierung();
#endif

  /* First optZweig: */

  const unsigned int DN = DeltaN[optZweig][! Schalter];
  const unsigned int DN2 = DeltaN[! optZweig][! Schalter];
#ifdef LOKALLERNEN
  eintragenTiefe();
#endif
#ifndef BAUMRES
  for (struct {unsigned int i; StapeleintragFZ Z;} l = {0,Huelle[optZweig][! Schalter]}; l.i < DN; ++l.i, ++l.Z)
    belege(*l.Z);
#else
  {
   StapeleintragFZ Z = Huelle[optZweig][! Schalter];
   belege((Z++) -> l);
   for (unsigned int i = 1; i < DN; ++i, ++Z) belege_VK(Z -> l, Z -> k);
  }
#endif
  aktP = LaP[optZweig][! Schalter];
  SatVar -> P2 = LaP[! optZweig][! Schalter];
  memcpy((void*)(aktAnzK + 2), (void*)(LaAnzK[optZweig][! Schalter] + 2), (aktP - 1) * sizeof(unsigned int));
  memcpy((void*)((SatVar -> AnzK2) + 2), (void*)(LaAnzK[! optZweig][! Schalter] + 2), ((SatVar -> P2) - 1) * sizeof(unsigned int));
  SatVar -> N2 = aktN - DN2;
  aktN -= DN;
  SatVar -> altZeiger2 = Zeiger2;

/* Ist noch genug Speicher fuer die zweite Belegung?!: */

  if (Zeiger2 + DN2 >= Groesse2) {
    ++Ueberschreitung2;
    Groesse2 += N;
    zweiteBel = (StapeleintragFZ) xrealloc(zweiteBel, Groesse2 * sizeof(StapeleintragF));
  }
  memcpy((void*)(zweiteBel + Zeiger2), (void*)(Huelle[! optZweig][! Schalter]), DN2 * sizeof(StapeleintragF));
  Zeiger2 += DN2;
  
  if (SatVar -> danach == NULL) {
    struct Sammlung* Z;
    ++Suchbaumtiefe;
    SatVar -> danach = (Z = neuerKnoten());
    Z -> davor = SatVar;
    SatVar = Z;
  }
  else
    SatVar = SatVar -> danach;
  SatVar -> Ruecksprung = SAT1;
  ++Rekursionstiefe;
  if (Monitor)
    if (Rekursionstiefe < Beobachtungsniveau) {
      beobachtet[Rekursionstiefe] = beobachtet[Rekursionstiefe-1];
      if (Dateiausgabe) {
        assert(Rekursionstiefe >= 1);
#ifndef BAUMRES
	  Verzweigungsliteralausgabe(*Huelle[optZweig][! Schalter], Rekursionstiefe - 1);
#else
        Verzweigungsliteralausgabe(Huelle[optZweig][! Schalter] -> l, Rekursionstiefe - 1);
#endif
      }
    }
  goto Anfang; // Branching (recursion simulated)

  nachSAT1 :

#ifdef OUTPUTTREEDATAXML
    EndTreeElement();
#endif

    if (Monitor)
      if (Rekursionstiefe < Beobachtungsniveau) {
        beobachtet[Rekursionstiefe] += Zweiglast[Rekursionstiefe];
        Monitorausgabe(beobachtet[Rekursionstiefe]);
      }

  /* rueckgaengig machen: */

#ifdef LOKALLERNEN
  Rueckgaengigmachung(SatVar -> Marke);
#else
# ifndef BAUMRES
  do {
    --Tiefe;
    rebelege(PfadLit());
  }
  while (Tiefe > SatVar -> altTiefe);
# else
  while (--Tiefe > SatVar -> altTiefe)
    rebelege(PfadLit());
  if (rebelege_Verz(PfadLit()))
    aktV_eintragen_relV();
  else {
    Zeiger2 = SatVar -> altZeiger2;
    ++SingleKnoten;
    assert(proportion_single >= 0);
    assert(Vergleich(proportion_searched+proportion_single,1) != groesser);
    proportion_single += exp2l(- (int) (Rekursionstiefe) - 1);
    {const enum Spruenge r = SatVar -> Ruecksprung;
     SatVar = SatVar -> davor;
     if (SatVar == NULL) {
#  ifdef OUTPUTTREEDATAXML
       EndTreeElement();
#  endif
       return UNSAT;
     }
     assert(Rekursionstiefe >= 1);
     --Rekursionstiefe;
     switch (r) {
     case SAT1 : goto nachSAT1;
     case SAT2 : goto nachSAT2;
     }
    }
  }
# endif
#endif

  /* nun der zweite Zweig: */

#ifdef LOKALLERNEN
  eintragenTiefe();
#endif
#ifndef BAUMRES
  for (struct {unsigned i; StapeleintragFZ Z;} l = {SatVar->altZeiger2, zweiteBel + (SatVar -> altZeiger2)}; l.i < Zeiger2; ++l.i, ++l.Z)
    belege(*l.Z);
#else
  {
   StapeleintragFZ Z = zweiteBel + (SatVar -> altZeiger2);
   belege((Z++) -> l);
   for (unsigned int i = SatVar -> altZeiger2 + 1; i < Zeiger2; ++i, ++Z)
    belege_VK(Z -> l, Z -> k);
  }
#endif

  Zeiger2 = SatVar -> altZeiger2;
  aktP = SatVar -> P2;
  aktN = SatVar -> N2;
  memcpy((void*)(aktAnzK + 2), (void*)((SatVar -> AnzK2) + 2), (aktP - 1) * sizeof(unsigned int));

  if (SatVar -> danach == NULL) {
    struct Sammlung* Z;
    ++Suchbaumtiefe;
    SatVar -> danach = (Z = neuerKnoten());
    Z -> davor = SatVar;
    SatVar = Z;
  }
  else
    SatVar = SatVar -> danach;
  SatVar -> Ruecksprung = SAT2;
  ++Rekursionstiefe;
  if (Monitor)
    if (Rekursionstiefe < Beobachtungsniveau)
      beobachtet[Rekursionstiefe] = beobachtet[Rekursionstiefe-1];
  goto Anfang;

  nachSAT2 :

#ifdef OUTPUTTREEDATAXML
    EndTreeElement();
#endif

    if (Monitor)
      if (Rekursionstiefe < Beobachtungsniveau) {
        beobachtet[Rekursionstiefe] += Zweiglast[Rekursionstiefe];
        Monitorausgabe(beobachtet[Rekursionstiefe]);
      }

#ifdef LOKALLERNEN
  Rueckgaengigmachung(SatVar -> Marke);
#else
# ifndef BAUMRES
  do { --Tiefe; rebelege(PfadLit()); }
  while (Tiefe > SatVar -> altTiefe);
# else
  while (--Tiefe > SatVar -> altTiefe) rebelege(PfadLit());
  if (rebelege_Verz(PfadLit())) relVMhinzufuegen();
  else ++VerSingleKnoten;
# endif
#endif
  {const enum Spruenge r = SatVar -> Ruecksprung;
   SatVar = SatVar -> davor;
   if (SatVar == NULL) {
#ifdef OUTPUTTREEDATAXML
     EndTreeElement();
#endif
     return UNSAT;
   }
   assert(Rekursionstiefe >= 1);
   --Rekursionstiefe;
   switch (r) {
   case SAT1 : goto nachSAT1;
   case SAT2 : goto nachSAT2;
   default :
     assert(0);
   }
  }
}

/* ------------------------------------------------------------- */

/*!
  \brief Output of statistics

  Either to stdout or to file.
*/

void Statistikzeile(FILE* const fp) {
  if (Format == Dimacs_Format) {
    fprintf(fp, "s ");
    switch (s) {
    case SAT :
      fprintf(fp, "SATISFIABLE"); break;
    case UNSAT :
      fprintf(fp, "UNSATISFIABLE"); break;
    case Unbestimmt :
      fprintf(fp, "UNKNOWN"); break;
    }
    fprintf(fp, "\n");
    fprintf(fp,
            "c sat_status                            %d\n"
            "c initial_maximal_clause_length         %u\n"
            "c initial_number_of_variables           %u\n"
            "c initial_number_of_clauses             %u\n"
            "c initial_number_of_literal_occurrences %u\n"
            "c number_of_initial_unit-eliminations   %lu\n"
            "c reddiff_maximal_clause_length         %u\n"
            "c reddiff_number_of_variables           %u\n"
            "c reddiff_number_of_clauses             %u\n"
            "c reddiff_number_of_literal_occurrences %u\n"
            "c number_of_2-clauses_after_reduction   %u\n"
            "c running_time(sec)                     %1.1f\n"
            "c number_of_nodes                       %lu\n"
            "c number_of_single_nodes                %lu\n"
            "c number_of_quasi_single_nodes          %lu\n"
            "c number_of_2-reductions                %lu\n"
            "c number_of_pure_literals               %lu\n"
            "c number_of_autarkies                   %lu\n"
            "c number_of_missed_single_nodes         %lu\n"
            "c max_tree_depth                        %u\n"
            "c proportion_searched                   %1.6e\n"
            "c proportion_single                     %1.6e\n"
            "c total_proportion                      %1.16g\n"
            "c number_of_table_enlargements          %u\n"
            "c number_of_1-autarkies                 %lu\n"
            "c number_of_new_2-clauses               %lu\n"
            "c maximal_number_of_added_2-clauses     %lu\n"
            "c file_name                             %s\n",
	    s, P0, N0, K0, L0,
            InitEinerRed, P0 - P, N0 - N, K0 - K, L0 - L, init2Klauseln,
            (double) Verbrauch / EPS,
	    Knoten, SingleKnoten, QuasiSingleKnoten, V1KlRed, PureL,
	    Autarkien, VerSingleKnoten, Suchbaumtiefe,
          (double) proportion_searched, (double) proportion_single, (double)(proportion_searched+proportion_single),
	    Ueberschreitung2, 
	    FastAutarkien, neue2Klauseln, maxneue2K,
	    aktName);
          if (splitting_only)
            fprintf(fp,
            "c splitting_directory                   %s\n"
            "c splitting_cases                       %u\n",
            splitting_store, splitting_cases);
  }
  else {
    fprintf(fp, "<SAT-solver.output timestamp = \"%ld\" >\n", time(0));
    
    fprintf(fp, "  <result value = \"");
    switch (s) {
    case SAT : fprintf(fp, "satisfiable"); break;
    case UNSAT : fprintf(fp, "unsatisfiable"); break;
    case Unbestimmt : fprintf(fp, "unknown"); break;
    }
    fprintf(fp, "\" />\n");

    fprintf(fp, "  <processing-description>\n");
    fprintf(fp, "    <basic>\n");
    fprintf(fp, "      <running-time type = \"core\" sec = \"%.2f\" />\n", (double) Verbrauch / EPS);
    fprintf(fp, "      <tree-depth type = \"maximal\" count = \"%d\" />\n", Suchbaumtiefe);
    fprintf(fp, "      <autarky-reductions where = \"search-tree\" level = \"pure\" count = \"%ld\" />\n", PureL);
    fprintf(fp, "      <autarky-reductions where = \"search-tree\" level = \"non-pure\" count = \"%ld\" />\n", Autarkien);
    fprintf(fp, "      <number_of_table_enlargements count = \"%d\" />\n", Ueberschreitung2);
    fprintf(fp, "      <nodes type = \"node\" count = \"%ld\" />\n", Knoten);
    fprintf(fp, "      <nodes type = \"single\" count = \"%ld\" />\n", SingleKnoten);
    fprintf(fp, "      <nodes type = \"quasi-single\" count = \"%ld\" />\n", QuasiSingleKnoten);
    fprintf(fp, "      <nodes type = \"missed_single\" count = \"%ld\" />\n", VerSingleKnoten);
    fprintf(fp, "      <nodes type = \"splitting_cases\" count = \"%u\" />\n", splitting_cases);

    fprintf(fp, "    </basic>\n");
    fprintf(fp, "    <extended>\n");
    fprintf(fp, "      <generalised_unit_reductions where = \"initial\" level = \"1\" count = \"%ld\" />\n", InitEinerRed);
    fprintf(fp, "      <generalised_unit_reductions where = \"search-tree\" level = \"2\" count = \"%ld\" />\n", V1KlRed);
    fprintf(fp, "      <generalised_autarky-reductions where = \"search-tree\" level = \"1\" specifier = \"total\" count = \"%ld\" />\n", FastAutarkien);
    fprintf(fp, "      <numbers_of_new_clauses list_of_clause-lengths = \"complete\" type = \"local\" specifier = \"maximum\" >\n");
    fprintf(fp, "        <nc_number length = \"2\" count = \"%ld\" />\n", maxneue2K);
    fprintf(fp, "      </numbers_of_new_clauses>\n");
    fprintf(fp, "      <numbers_of_new_clauses list_of_clause-lengths = \"complete\" type = \"local\" specifier = \"total\" >\n");
    fprintf(fp, "        <nc_number length = \"2\" count = \"%ld\" />\n", neue2Klauseln);
    fprintf(fp, "      </numbers_of_new_clauses>\n");
    fprintf(fp, "    </extended>\n");
    fprintf(fp, "  </processing-description>\n");

    if (! splitting_only)
    fprintf(fp, "  <instance_specs system-name = \"%s\" >\n", aktName);
    else
    fprintf(fp, "  <instance_specs system-name = \"%s\" splitting-dir = \"%s\" >\n", aktName, splitting_store);

    fprintf(fp, "    <measures>\n");
    fprintf(fp, "      <reduction> <none/> </reduction>\n");
    fprintf(fp, "      <number_of_variables specifier = \"exact\" count = \"%d\" />\n", N0);
    fprintf(fp, "      <clause-length specifier = \"exact\" type = \"maximal\" value = \"%d\" />\n", P0);
    fprintf(fp, "      <total_number_of_clauses specifier = \"exact\" count = \"%d\" />\n", K0);
    fprintf(fp, "      <number_of_literal_occurrences specifier = \"exact\" count = \"%d\" />\n", L0);
    fprintf(fp, "    </measures>\n");

    fprintf(fp, "    <measures>\n");
    fprintf(fp, "      <reduction> <tautological_clauses/> <multiple_literal_occurences_in_clauses/> <unit_clause_elimination/> </reduction>\n");

    if (Knoten == 0) { // nur Vorreduktion
      if (s == UNSAT) { // Vorreduktion erzeugte die leere Klausel
	fprintf(fp, "      <numbers_of_clauses list_of_clause-lengths = \"incomplete\" >\n");
	fprintf(fp, "        <number length = \"0\" specifier = \"lower_bound\" count = \"1\" />\n");
	fprintf(fp, "      </numbers_of_clauses>\n");
      }
      else if (s == SAT) { // Vorreduktion erfuellte die Klauselmenge
	fprintf(fp, "      <total_number_of_clauses specifier = \"exact\" count = \"0\" />\n");
      }
      else { // keine Entscheidung
	fprintf(fp, "      <number_of_variables specifier = \"exact\" count = \"%d\" />\n", N);
	fprintf(fp, "      <clause-length type = \"maximal\" specifier = \"exact\" count = \"%d\" />\n", P);
	fprintf(fp, "      <total_number_of_clauses specifier = \"exact\" count = \"%d\" />\n", K);
	fprintf(fp, "      <number_of_literal_occurrences specifier = \"exact\"count = \"%d\" />\n", L);
      }
    }
    else { // nun wurde der Suchbaum aufgebaut
      fprintf(fp, "      <number_of_variables specifier = \"exact\" count = \"%d\" />\n", N);
      fprintf(fp, "      <clause-length type = \"maximal\" specifier = \"exact\" value = \"%d\" />\n", P);
      fprintf(fp, "      <total_number_of_clauses specifier = \"exact\" count = \"%d\" />\n", K);
      fprintf(fp, "      <numbers_of_clauses list_of_clause-lengths = \"complete\" >\n");
      for (unsigned int i = 0; i <= P - 2; ++i)
	if (InitAnzK[i+2] != 0)
	  fprintf(fp, "        <number length = \"%d\" count = \"%d\" />\n", i+2, InitAnzK[i+2]);
      fprintf(fp, "      </numbers_of_clauses>\n");
      fprintf(fp, "      <number_of_literal_occurrences specifier = \"exact\" count = \"%d\" />\n", L);
    }
    fprintf(fp, "    </measures>\n");

    fprintf(fp, "  </instance_specs>\n");

   if (! Belegung || s != SAT || Dateiausgabe)
      fprintf(fp, "</SAT-solver.output>\n");
  }
  return;
}

const char* BasisName(const char* const name) {
  const char* const basis = strrchr(aktName, '/');
  if (basis == NULL) return name;
  else return basis + 1;
}


/* ------------------------------------------------------------- */

static FILE* fpaus = NULL; /* fuer die Ausgabe der Ergebnisse */

static void Zustandsanzeige (const int sig) {
#ifndef SYSTIME
  Verbrauch = clock() - akkVerbrauch;
#else
  times(Zeiger);
  Verbrauch = SysZeit.tms_utime - akkVerbrauch;
#endif
  Statistikzeile(stdout);
  if (Dateiausgabe) Statistikzeile(fpaus);
  fflush(NULL);
  signal(SIGUSR1, Zustandsanzeige);
}

jmp_buf Ausgabepunkt;

static void Abbruch (const int sig) {
  signal(SIGINT, Abbruch);
  signal(SIGALRM, Abbruch);
  if (splitting_only) splitting_abortion = true;
  longjmp(Ausgabepunkt, 1);
}

static FILE* fp = NULL; /* die aktuelle Eingabedatei */
static unsigned int Groesse;
static FILE* fppa = NULL; /* fuer die Ausgabe einer erfuellenden Belegung */

static char* NameBel = NULL; char* NameMon = NULL;


int main(const int argc, const char* const argv[]) {
  const char* const Ausgabedatei = "OKs" VERSIONSNUMMER1 "_" VERSIONSNUMMER2 "_" OPTIONENKENNUNG5 OPTIONENKENNUNG6 OPTIONENKENNUNG7 OPTIONENKENNUNG1 OPTIONENKENNUNG2 OPTIONENKENNUNG3 OPTIONENKENNUNG4".res";
  const char* const Version = VERSIONSNUMMER1 "." VERSIONSNUMMER2;

#ifdef SYSTIME
  EPS = sysconf(_SC_CLK_TCK);
#endif

  signal(SIGUSR1, Zustandsanzeige);
  signal(SIGINT, Abbruch);
  signal(SIGALRM, Abbruch);
  if (setjmp(Ausgabepunkt)) goto Ausgabe;

  if (Konstantenfehler()) {
    fprintf(stderr, "%s\n", Meldung(0));
    return 1;
  }

  if (argc == 1) {
    fprintf(stderr, "%s\n", Meldung(3));
    return 0;
  }

  setzenStandard();

  for (int Argument = 1; Argument < argc; ++Argument) {
    if (strcmp("--version", argv[Argument]) == 0) {
      printf("%s %s; %s %s\n%s: %s, %s\n", Meldung(24), DATUM, Meldung(2), Version, Meldung(6), __DATE__, __TIME__);
      printf("%s", Meldung(44));
#ifdef NBAUMRES
      printf(" NBAUMRES");
#endif
#ifdef LITTAB
      printf(" LITTAB");
#endif
#ifdef SYSTIME
      printf(" SYSTIME");
#endif
#ifdef DYNAMISCH
      printf(" DYNAMISCH");
#endif
#ifdef DYNAMISCH
      printf(" FAKTOR=%.2f", FAKTOR);
#endif
#ifdef LOKALLERNEN
      printf(" LOKALLERNEN");
#endif
#ifdef NL2RED
      printf(" NL2RED");
#endif
#ifdef FASTAUTARKIE
      printf(" FASTAUTARKIE");
#endif
#ifdef KEININLINE
      printf(" KEININLINE");
#endif
#ifdef OUTPUTTREEDATAXML
      printf(" OUTPUTTREEDATAXML");
#endif
      printf(" STANDARD=%1d", STANDARD);

      printf("\n");
      printf("%s %s\n", Meldung(45), Abstandsname);
      printf("%s %s\n", Meldung(46), Projektionsname);
    }
    else if (strcmp("--author", argv[Argument]) == 0)
      printf("%s\n", Meldung(25));
    else if (strcmp("--help", argv[Argument]) == 0)
      printf("%s\n", Meldung(26));
    else if (strcmp("-O", argv[Argument]) == 0)
      Belegung = ! Belegung;
    else if (strcmp("-F", argv[Argument]) == 0)
      Dateiausgabe = ! Dateiausgabe;
    else if (strcmp("-M", argv[Argument]) == 0)
      Monitor = ! Monitor;      
    else if (strcmp("-P", argv[Argument]) == 0)
      nurVorreduktion = ! nurVorreduktion;      
    else if (strcmp("-R", argv[Argument]) == 0)
      spezRueckgabe = ! spezRueckgabe;
    else if (strcmp("-B", argv[Argument]) == 0)
      Schranken = ! Schranken;
    else if (strcmp("--info", argv[Argument]) == 0)
      printf("%s\n%s\n", Meldung(47), Meldung(48));
    else if (strcmp("-RA", argv[Argument]) == 0)
      randomisiert = ! randomisiert;
    else if (strcmp("-SD", argv[Argument]) == 0)
      splitting_n = ! splitting_n;
    else if (strcmp("-SF", argv[Argument]) == 0)
      splitting_file = ! splitting_file;
    else if (strcmp("-DO", argv[Argument]) == 0) {
      Format = Dimacs_Format;
      spezRueckgabe = true;
    }
    else if (strcmp("-XO", argv[Argument]) == 0) {
      Format = XML_Format;
      spezRueckgabe = false;
    }
    else if (strcmp("--specification", argv[Argument]) == 0) {
      printf("<SAT-solver.specification>\n");
      printf("  <solver-type mode = \"deterministic\">\n");
      printf("    <complete/>\n");
      printf("  </solver-type>\n");
      printf("  <solver-name base-name = \"%s\" version = \"%s\" url = \"%s\" />\n", "OKsolver", Version, "http://cs.swan.ac.uk/~csoliver/OKsolver");
      printf("  <solver-author first_name = \"Oliver\" last_name = \"Kullmann\" country = \"United Kingdom\" e-mail = \"O.Kullmann@Swansea.ac.uk\" www = \"http://cs.swan.ac.uk/~csoliver/\" />\n");
      printf("  <programming_language name = \"C\" />\n");
      printf("  <compilation_time timestamp = \"%s %s\" />\n", __DATE__, __TIME__);
      printf("  <url for = \"executable\" value = \"");
      if (argv[0][0] == '/') // absoluter Pfadname
        printf("%s\" />\n", argv[0]);
      else // relativer Pfadname
        printf("%s/%s\" />\n", getenv("PWD"), argv[0]);
      printf("  <options string = \"%s\" />\n", OPTIONENKENNUNG5 OPTIONENKENNUNG6 OPTIONENKENNUNG7 OPTIONENKENNUNG1 OPTIONENKENNUNG2 OPTIONENKENNUNG3 OPTIONENKENNUNG4);
      if (internal) printf("  <internal/>\n");
      printf("</SAT-solver.specification>\n");
    }
    else if (strncmp("--language=", argv[Argument], 11) == 0) {
      int Nummer;
      if (sscanf(argv[Argument] + 11, "%d", &Nummer) != 1) {
        fprintf(stderr, "%s\n", Meldung(18));
        return 1;
      }
      if ((Nummer < 0) || (Nummer >= ANZSPRACHEN)) {
        fprintf(stderr, "%s %2d\n", Meldung(19), ANZSPRACHEN - 1);
        return 1;
      }
      Sprache = Nummer;
    }
    else if (strncmp("--standard=", argv[Argument], 11) == 0) {
      int Nummer;
      if (sscanf(argv[Argument] + 11, "%d", &Nummer) != 1) {
        fprintf(stderr, "%s\n", Meldung(20));
        return 1;
      }
      if ((Nummer <= 0) || (Nummer > ANZSTANDARDS)) {
        fprintf(stderr, "%s %2d\n", Meldung(21), ANZSTANDARDS);
        return 1;
      }
      Standard = Nummer;
      setzenStandard();
    }
    else if (strncmp("-D", argv[Argument], 2) == 0) {
      // -D fuer Beobachtungsniveau (depth)
      // Verzweigungsliterale werden (falls ueberhaupt Dateiausgabe gesetzt ist)
      // nur bis zu zwei Stufen unterhalb der Beobachtungsschicht ausgegeben,
      // so dass hierfuer als das Beobachtungsniveau mindestens zwei sein
      // sollte.
      int Nummer;
      if (sscanf(argv[Argument] + 2, "%d", &Nummer) != 1) {
        fprintf(stderr, "%s\n", Meldung(31));
        return 1;
      }
      if (Nummer < 0) {
        fprintf(stderr, "%s\n", Meldung(32));
        return 1;
      }
      Beobachtungsniveau = Nummer;
    }
    else if (strncmp("-MAXN=", argv[Argument], 6) == 0) {
      int maxn;
      if (sscanf(argv[Argument] + 6, "%d", &maxn) != 1) {
        fprintf(stderr, "%s\n", Meldung(33));
        return 1;
      }
      if (maxn < 0) {
        fprintf(stderr, "%s\n", Meldung(34));
        return 1;
      }
      MAXN = maxn;
    }
    else if (strncmp("-MAXL=", argv[Argument], 6) == 0) {
      int maxl;
      if (sscanf(argv[Argument] + 6, "%d", &maxl) != 1) {
        fprintf(stderr, "%s\n", Meldung(36));
        return 1;
      }
      if (maxl < 0) {
        fprintf(stderr, "%s\n", Meldung(37));
        return 1;
      }
      MAXL = maxl;
    }
    else if (strncmp("-MAXK=", argv[Argument], 6) == 0) {
      int maxk;
      if (sscanf(argv[Argument] + 6, "%d", &maxk) != 1) {
        fprintf(stderr, "%s\n", Meldung(39));
        return 1;
      }
      if (maxk < 0) {
        fprintf(stderr, "%s\n", Meldung(40));
        return 1;
      }
      MAXK = maxk;
    }
    else if (strncmp("--timeout=", argv[Argument], 10) == 0) {
      int t;
      if (sscanf(argv[Argument] + 10, "%d", &t) != 1) {
        fprintf(stderr, "%s\n", Meldung(42));
        return 1;
      }
      if (t < 0) {
        fprintf(stderr, "%s\n", Meldung(43));
        return 1;
      }
      Zeitschranke = t;
    }
    else if (strncmp("-seed=", argv[Argument], 6) == 0) {
      long unsigned int S;
      if (sscanf(argv[Argument] + 6, "%lud", &S) != 1) {
        fprintf(stderr, "%s\n", Meldung(52));
        return 1;
      }
      Schluessel = S;
    }
    else if (strncmp("-quot=", argv[Argument], 6) == 0) {
      double V;
      if (sscanf(argv[Argument] + 6, "%lf", &V) != 1)
        {
          fprintf(stderr, "%s\n", Meldung(53));
          return 1;
        }
      Verhaeltnis = V;
    }
    else if (strncmp("-S=", argv[Argument], 3) == 0) {
      splitting_only = true;
      Belegung = true;
      splitting_store = argv[Argument]+3;
      if (strlen(splitting_store) == 0) {
        fprintf(stderr, "%s\n", Meldung(57));
        return 1;
      }
    }
    else if (strncmp("-", argv[Argument], 1) == 0) {
      fprintf(stderr, "%s %s\n", Meldung(22), argv[Argument]);
      return 1;
    }
    else {
      if (splitting_only) {
        if (! Belegung) { fprintf(stderr, "%s\n", Meldung(58)); return 1; }
        if (splitting_file) {
          if ((fpsplit = fopen(splitting_store, "w")) == NULL) {
            fprintf(stderr, "%s %s\n", Meldung(60), splitting_store);
            return 1;
          }
        }
        splitting_decisions = (char*) xmalloc(strlen(splitting_store)+10+1);
        strcpy(splitting_decisions,splitting_store);
        if (splitting_file) strcat(splitting_decisions, "_");
        else strcat(splitting_decisions, "/");
        strcat(splitting_decisions, "decisions");
        if ((fpsplitdec = fopen(splitting_decisions, "w")) == NULL) {
          fprintf(stderr, "%s %s\n", Meldung(62), splitting_decisions);
          return 1;
        }
      }
      aktName = argv[Argument];
      s = Unbestimmt;
      alarm(Zeitschranke);
#ifndef SYSTIME
      akkVerbrauch = clock();
#else
      times(Zeiger);
      akkVerbrauch = SysZeit.tms_utime;
#endif
      if (randomisiert) srand_S();
      if ((fp = fopen(aktName, "r")) == NULL) {
        fprintf(stderr, "%s %s\n", Meldung(4), aktName);
        return 1;
      }
      {
        struct stat stbuf;
        if (stat(aktName, &stbuf) == -1) {
          fprintf(stderr, Meldung(7), aktName);
          return 1;
        }
        Groesse = stbuf.st_size;
      }
      
      if (Dateiausgabe) {
        if ((fpaus = fopen(Ausgabedatei, "a")) == NULL) {
          fprintf(stderr, "%s %s\n", Meldung(30), Ausgabedatei);
          return 1;
        }
        if (Belegung || (Monitor && (! nurVorreduktion))) {
          Wurzel = BasisName(aktName);
          if (Belegung) {
            NameBel = (char*) xmalloc((strlen(Wurzel) + 3 + 1) * sizeof(char));
            strcpy(NameBel, Wurzel); strcat(NameBel, ".pa");
          }
          if (Monitor && (! nurVorreduktion)) {
            NameMon = (char*) xmalloc((strlen(Wurzel) + 3 + 1) * sizeof(char));
            strcpy(NameMon, Wurzel); strcat(NameMon, ".mo");
            if ((fpmo = fopen(NameMon, "w")) == NULL) {
              fprintf(stderr, "%s %s\n", Meldung(29), NameMon);
              return 1;
            }
          }
        }     
      }
      
#ifdef OUTPUTTREEDATAXML
      {
        if (! Wurzel) Wurzel = BasisName(aktName);
        NameTreeDataFile = (char*) xmalloc((strlen(Wurzel) + 4 + 1));
        strcpy(NameTreeDataFile, Wurzel); strcat(NameTreeDataFile, ".xml");
        if ((TreeDataFile = fopen(NameTreeDataFile, "w")) == NULL) {
          fprintf(stderr, "%s %s\n", Meldung(54), NameTreeDataFile);
          return 1;
        }
        fprintf(TreeDataFile, "<?xml version=\"1.0\" standalone=\"yes\" ?>\n");
        fprintf(TreeDataFile, "<!DOCTYPE t [\n");
        fprintf(TreeDataFile, "  <!ELEMENT t (t?, t?)>\n");
        fprintf(TreeDataFile, "  <!ATTLIST t\n");
        fprintf(TreeDataFile, "    l NMTOKEN #REQUIRED>\n");
        fprintf(TreeDataFile, "]>\n\n");
        // If already in the preprocessing phase the formula is decided, then
        // no tree-xml-element is output, and thus the file with name
        // NameTreeDataFile does not contain correct xml.
      }
#endif
      
      switch (Einlesen(fp, Groesse)) {
      case Sat : s = SAT; break;
      case Unsat : s = UNSAT; break;
      case Fehler :
        fprintf(stdout, "%s %s.\n", Meldung(17), aktName);
        if (Dateiausgabe)
          fprintf(fpaus, "%s %s.\n", Meldung(17), aktName);
        goto Aufraeumen;
      case Norm :
        if (nurVorreduktion)
          break;
        InitVarLitKlm();
        InitSat();
#ifdef LOKALLERNEN
        InitlokalesLernen();
#endif
        if (Monitor) {
          printf("\n%s\n %s, %4d, %10d\n", Meldung(28), aktName, Beobachtungsniveau, Gesamtlast);
          printf("%s\n\n", Meldung(55));
          if (Dateiausgabe) {
            fprintf(fpmo, "# %s %4d %11d\n", aktName, Beobachtungsniveau, Gesamtlast);
            fprintf(fpmo, "%s\n", Meldung(56));
          }
        }
        
        s = SATEntscheidung();
        if (splitting_only && splitting_cases != 0) s = Unbestimmt;
      }
      
    Ausgabe :

      if (splitting_abortion) fprintf(stderr, "\n%s\n", Meldung(61));
#ifndef SYSTIME
      Verbrauch = clock() - akkVerbrauch;
#else
      times(Zeiger);
      Verbrauch = SysZeit.tms_utime - akkVerbrauch;
#endif
      if (Monitor) printf("\n");
      Statistikzeile(stdout);
      if (Dateiausgabe) Statistikzeile(fpaus);
      
      /* Achtung: Die Analyse der Ausgabe verlangt, dass das allererste */
      /* Zeichen die SAT-Zugehoerigkeit (d.h.: 0 oder 1) angibt. */
      
      if (Belegung && (s == SAT))
        if (! Dateiausgabe) AusgabeBelegung(stdout);
        else {
          if ((fppa = fopen(NameBel, "w")) == NULL) {
            fprintf(stderr, "%s %s\n", Meldung(27), NameBel);
            return 1;
          }
          AusgabeBelegung(fppa);
        }
    Aufraeumen :
      
      alarm(0);
      AufraeumenSat();
#ifdef BAUMRES
      AufraeumenBaumRes();
#endif
#ifdef LOKALLERNEN
      AufraeumenlokalesLernen();
#endif
      AufraeumenEinlesen(); /* zuletzt */
      free(NameBel); NameBel = NULL;
      free(NameMon); NameMon = NULL;
#ifdef OUTPUTTREEDATAXML
      fprintf(TreeDataFile, "\n");
      free(NameTreeDataFile); NameTreeDataFile = NULL;
      if (TreeDataFile != NULL) {
        fclose(TreeDataFile); TreeDataFile = NULL;
      }
#endif
      if (fp != NULL) { fclose(fp); fp = NULL; }
      if (fpmo != NULL) { fclose(fpmo); fpmo = NULL; }
      if (fpaus != NULL) { fclose(fpaus); fpaus = NULL; }
      if (fppa != NULL) { fclose(fppa); fppa = NULL; }
    }
  }

  if (splitting_abortion) return 1;
  if (spezRueckgabe)
    switch (s) {
    case SAT : return 10;
    case UNSAT : return 20;
    case Unbestimmt : return 0;
    }
  else return 0;
}

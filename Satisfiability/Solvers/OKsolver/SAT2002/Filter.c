// Oliver Kullmann, 5.3.1998 (Frankfurt)
/* Copyright 1998 - 2007, 2011, 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Solvers/OKsolver/SAT2002/Filter.c
  \brief %Implementation of the filter
*/

                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Der "Filter" */
/* Einheit: Filter */
/* 21.8.1998: Austausch von "ErfK" durch "NeuK" (neue Klauseln) */
/* 31.10.1998: Elimination des Feldes "ausgefuehrt" */
/* 13.11.1998: Resolutions-Baum-Verwaltung */
/* 14.8.2001: DK in DKF umbenannt, um den Namenskonflikt mit dem */
/* Reduktions-Modul zu loesen */

/* --------------------------------------------------------- */

#include <stdlib.h>

#include "OK.h"
#include "VarLitKlm.h"
#include "Parameter.h"
#include "Filter.h"
#ifdef FASTAUTARKIE
#include "lokalesLernen.h"
#endif

//! true iff a satisfying assignment was found
bool erfuellt;

//! true iff no further reduction needs to be attempted
bool reduziert;

//! check first branch v -> Zweig (because of autarky or 2-SAT)
VZ Zweig;

//! if Wahl==true, so choose v as branching variable
bool Wahl;

//! if Wahl==true, so Single==true if branch v -> Zweig alone is sufficient
bool Single;

/* Im Falle von "! erfuellt" und "! reduziert" werden von "Filter" die  */
/* folgenden Daten berechnet (evtl. nur fuer einen Zweig), wobei die  */
/* Hilfsvariable "Schalter" verwendet wird. */

unsigned int Schalter; /* Werte: 0, 1 */

/* Die lokale Variable Eps und die globale Variable Schalter gehoeren zu  */
/* der  ersten bzw. zweiten Komponente der folgenden 3-dimensionalen  */
/* Felder: */
/* Eps regelt den Zweig, waehrend "Schalter" zur aktuell von "Filter"  */
/* behandelten Variable gehoert --- "! Schalter" gibt die Daten zur bisher  */
/* besten Variablen (durch "Umschalten" werden Speicheroperationen  */
/* gespart).  */

//! the number of (formal) eliminated variables
unsigned int DeltaN[2][2];

//! the new maximal clause-length (after look-ahead_
unsigned int LaP[2][2];


StapeleintragFZ Huelle[2][2];
/* LIT Huelle [ 2 ] [ 2 ] [ MAXN ]; */
/* Huelle [Eps, Schalter, 0 .. (DeltaN[Eps, Schalter] - 1)] ist die 1-Klauseln-Huelle */
/* der Belegung v -> Eps */


int* DeltaK[2][2];
/* int DeltaK [ 2 ] [ 2 ] [ MAXP + 1 ]; */

/* DeltaK [Eps, Schalter, i] ist die Differenz der Anzahlen "alter" und "neuer" Klauseln */
/* der Laenge i bei der Anwendung der Huelle der Belegung v -> Eps */
/* (DeltaK ist negativ, falls sich diese Anzahl erhoeht hat). */


unsigned int* NeuK [2][2];
/* unsigned int NeuK [2][2][MAXP + 1]; */

/* NeuK [E, Schalter, i] ist die Anzahl der neuen Klauseln */
/* der Laenge i. */


unsigned int* LaAnzK [2][2];
/* unsigned int LaAnzK [2][2][MAXP + 1]; */
/* die "vorausgesehenen" neuen Klauselnzahlen */


extern unsigned int Runde;

/* --------------------------------------------------------------------------- */

#ifdef FASTAUTARKIE

static LIT* neueFA2Klauseln0;
static LIT* neueFA2Klausel;

unsigned int RundeFA;

#endif

/* --------------------------------------------------------------------------- */

/* Berechnung der 1-Klauseln-Huelle */

static VZ Eps;

/* Die folgenden Variablen sind die Felder DeltaN, Huelle, DeltaK und NeuK, */
/* wobei Eps und Schalter eingesetzt wurden. */
/* Im Falle von BAUMRES wird noch EinerKl verwendet. */

static unsigned int* DN;

static StapeleintragFZ HF;
/* "F" in "HF" wie "Filter" zur Unterscheidung von (nur) "H" in "Reduktion" */

static int* DKF;
/* ebenfalls das "F" in "DKF" zur Unterscheidung von "DK" in "Reduktion" */

static unsigned int* NK;


__inline__ static StapeleintragFZ La_belegeFil(const LIT x, StapeleintragFZ sp) {
/* Fuehrt x -> 0 durch (bzgl. letzteRK, letzteRL und LaLaenK), */
/* schreibt neu entstehende 1-Klauseln nach HF (mit Eintrag in letzteRL), */
/* wobei sp erhoeht wird, und aktualisiert DKF und NK. */
/* Vor.: Es entsteht nicht die leere Klausel. */
/* Im Falle von BAUMRES wird auch EK aktualisiert. */
/* Rueckgabewert ist der neue Wert von sp. */

  /* Durchlaufe alle x-Vorkommen und kuerze die aktiven Klauseln: */

  for (LITV y = erstesVork(x); echtesVork(y, x); y = naechstesVork(y)) {
    const KLN kn = KlnVk(y);
    if (RundeK(kn) != Runde) {
    /* Klausel von y noch nicht angefasst? */
      if (Laenge(kn) == 2) {  /* 2-Klausel? */
        const LIT klz = Komp(LitVk(naechstesVorkK(y)));
        if (RundeL(klz) != Runde) {
	  /* 1-Kl-Elim. nicht schon vorgemerkt? */
          setzenRundeL(klz);
#ifndef BAUMRES	
          *(sp++) = klz;
#else
          sp -> l = klz;
          (sp++) -> k = kn;
#endif
	  }
	}
      else { /* >= 3-Klausel */
        setzenRundeK(kn);
        const KLL p = Laenge(kn);
        ZuwLaLaenge(kn, p-1);
        --DKF[p-1];
        ++DKF[p];
        ++NK[p-1];
	}
    }
    else {  /* Klausel schon angefasst */
      const KLL p = LaLaenge(kn);
      if (p != 0) { /* Klausel noch nicht erfuellt? */
        if (p == 2) {  /* 2-Klausel? */
          LIT lz; // the second literal
          for (LITV z = naechstesVorkK(y); RundeL(lz=LitVk(z)) == Runde; z = naechstesVorkK(z));
          const LIT klz = Komp(lz);
          if (RundeL(klz) != Runde) {  /* 1-Kl-Elim. nicht schon vorgemerkt? */
            setzenRundeL(klz);
#ifndef BAUMRES
            *(sp++) = klz;
#else
            sp -> l = klz;
            (sp++) -> k = kn;
#endif
	    }
	  }
        else { /*  >= 3-Klausel */
          --DKF[ M1LaLaenge(kn) ];
          ++DKF[p];
          ++NK[p-1];
          --NK[p];
	  }
	}
    }
  }

  /* Durchlaufen der Komp(x)-Vorkommen und eliminiere alle noch aktiven Klauseln */
  const LIT kx = Komp(x);
  for (LITV y = erstesVork(kx); echtesVork(y, kx); y = naechstesVork(y)) {
    const KLN kn = KlnVk(y);
    if (RundeK(kn) != Runde) { /* Klausel nicht schon angefasst? */
      setzenRundeK(kn);
      ++DKF[Laenge(kn)];
      ZuwLaLaenge(kn, 0);
    }
    else {  /* Klausel schon angefasst */
      const KLL p = LaLaenge(kn);
      if (p != 0) { /* Klausel noch nicht erfuellt? */
        ++DKF[p];
        --NK[p];
        ZuwLaLaenge(kn, 0);
      }
    }
  }
  return sp;
}


static void La_HuelleFil(LIT x) {
/* Berechnet Huelle, DeltaN, DeltaK und NeuK. */
/* (Im Falle von BAUMRES auch EinerKl.) */
  StapeleintragFZ sp; /* Stapelzeiger auf das naechste freie Element in HF */
  StapeleintragFZ p;

  if (++Runde == 0)
    {
      NullsetzenRK(); NullsetzenRL();
      Runde = 1;
    }

  DN = & (DeltaN[Eps][Schalter]); HF = Huelle[Eps][Schalter];
  DKF = DeltaK[Eps][Schalter]; NK = NeuK[Eps][Schalter];
#ifndef BAUMRES
  HF[0] = x;
#else
  HF -> l = x;
#endif
  sp = HF + 1;

  setzenRundeL(x);

  p = HF;

  {
    unsigned int i;
    for (i = 2; i <= aktP; i++)
    DKF[i] = NK[i] = 0;
  }

  do
    {
      sp = La_belegeFil(x, sp);

      p++;

      if (sp > p)  /* noch 1-Kl. zu eliminieren? */
#ifndef BAUMRES
        x = *p;
#else
        x = p -> l;
#endif
      else
	{
	  *DN = p - HF;
	  return;
	}
    }
  while ( true );
}


/* ---------------------------------------------------------------------------------- */

__inline__ static unsigned char auswerten() {
/* Es werden die Daten von DKF und NK ausgewertet (bzgl. Autarkien und  */
/* 2-CLS), und LaAnzK, LaP berechnet. */
/* "auswerten" wird nur aufgerufen, wenn die leere Klausel nicht erzeugt  */
/* wurde. */

/* auswerten ==  */

/*   1 falls eine erfuellende Belegung vorliegt, */
/*   2 falls eine Autarkie vorliegt (die nicht erfuellend ist), */
/*   3 falls nur noch 2-Klauseln uebrig sind (und keine Autarkie vorliegt), */
/*   4 falls mind. eine >= 3-Kl. uebrig ist, und genau eine neue Klausel */
/*     erzeugt wurde, */
/*   0 sonst. */

  unsigned int i, j; unsigned int GAnzNK;
  unsigned int *AK; unsigned int *LP;

  GAnzNK = 0;
  AK = LaAnzK[Eps][Schalter]; LP = & (LaP[Eps][Schalter]);

  for (i = aktP; i > 1; i--) /* setzen von LaAnzK */
    AK[i] = aktAnzK[i] - DKF[i];

  for (i = aktP; i > 1; i--) /* berechne groesstes i, fuer dass i-Klauseln existieren */
    if (AK[i] != 0)
      break;

  if (i == 1) /* erfuellende Belegung gefunden */
    return 1;

  *LP = i; /* setzen von LaP */

  for (j = 2; j < aktP; j++) /* Berechnung der Gesamtzahl neuer Klauseln */
    GAnzNK += NK[j];

  if (GAnzNK == 0) /* Autarkie gefunden */
    return 2;
  else if (i == 2) /* neue Klauselmenge ist in 2-CLS */
    return 3;
  else if (GAnzNK == 1) /* Fast-Autarkie gefunden */
    return 4;
  else
    return 0;
}


/* ---------------------------------------------------------------------------------- */

size_t BedarfFilterV() {
  return 4 * N * sizeof(StapeleintragF) +
    4 * (P+1) * sizeof(int) +
    2 * 4 * (P+1) * sizeof(unsigned int)
#ifdef FASTAUTARKIE
    + (P - 1) * sizeof(LIT)
#endif
    ;
}

void* FilterV(void* Z) {
  unsigned char i, j;
  for (i = 0; i <= 1; i++)
    for (j = 0; j <= 1; j++)
      {
	Huelle[i][j] = (StapeleintragFZ) Z; Z = (void *) (Huelle[i][j] + N);
	DeltaK[i][j] = (int *) Z; Z = (void *) (DeltaK[i][j] + P + 1);
	NeuK[i][j] = (unsigned int *) Z; Z = (void *) (NeuK[i][j] + P + 1);
	LaAnzK[i][j] = (unsigned int *) Z; Z = (void *) (LaAnzK[i][j] + P + 1);
      }

#ifdef FASTAUTARKIE
  neueFA2Klauseln0 = (LIT *) Z; Z = (void *) (neueFA2Klauseln0 + (P - 1));
#endif

  return Z;
}

void InitFilter() {
#ifdef FASTAUTARKIE
  RundeFA = 0;
#endif
  return;
}

/* ---------------------------------------------------------------------------------- */

#ifdef FASTAUTARKIE

bool BehFastautarkie() { /* Behandlung einer Fast-Autarkie */
  LIT l, kl, a;
  unsigned int i;
  LITV x, nx;
  KLN nk;

  l = HF[0]; /* HF = Huelle[Eps][Schalter] */
  if (Fastautarkie(l))
    return false;

  /* finden der einen neuen Klausel nx */
  for (i = 0; i < *DN; i++)
    {
      a = HF[i];
      for (nx = erstesVork(a); echtesVork(nx, a); nx = naechstesVork(nx))
	{
	  nk = KlnVk(nx);
	  if (LaLaenge(nk) != 0)
	    goto gefunden;
	}
    }
 gefunden:

  kl = Komp(l);
  /* eintragen aller 2-Klauseln zu kl */
  if (++RundeFA == 0)
    {
      NullsetzenRLFA();
      RundeFA = 1;
    }
  for (x = erstesVork(kl); echtesVork(x, kl); x = naechstesVork(x))
    if (Laenge(KlnVk(x)) == 2)
      setzenRundeLFA(LitVk(naechstesVorkK(x)));

  /* herausfinden der neuen 2-Klauseln */
  neueFA2Klausel = neueFA2Klauseln0;
  for (x = naechstesVorkK(nx); x != nx; x = naechstesVorkK(x))
    {
      a = LitVk(x);
      if ((RundeL(a) != Runde) && (RundeLFA(Komp(a)) != RundeFA))
	*(neueFA2Klausel++) = Komp(a);
    }

  if (neueFA2Klausel == neueFA2Klauseln0)
    return false;
  /* eintragen der neuen 2-Klauseln */
  FastAutarkien++;
  eintragenNK();
  erzeugeFANK(kl, neueFA2Klauseln0, neueFA2Klausel);
  setzenFastautarkie(l, true);
  return true;
}

#endif

/* ---------------------------------------------------------------------------------- */



__inline__ void Filter(const VAR v) {
  Eps = Pos;
/* Belegungen werden durch Literale angegeben, die auf 0 gesetzt werden. */
/* Somit ist der erste Zweig v -> 0. */

  La_HuelleFil(Literal(v, Eps));

  switch (auswerten()) {  /* Auswertung des ersten Zweiges */

    case 1 :
      erfuellt = true;  /* aktuelle Klm ist erfuellbar */
      Zweig = Eps;      /* zur Rekonstruktion */
      ++Autarkien;      /* jede erfuellende Belegung ist Autarkie */
      return;

    case 2 :
      ++Autarkien;  /* eine (echte) Autarkie wurde gefunden */
      erfuellt = false;
      reduziert = false;
      Wahl = true;
      Single = true;
      Zweig = Eps;
      return;

    case 3 :  /* nur noch 2-Klauseln? */
      Eps = Neg;
      La_HuelleFil(Literal(v, Eps));
      switch (auswerten()) {
      /* Auswerten des zweiten Zweiges im Falle, dass der erste Zweig */
      /*  nur 2-Kl. zurueckliess */
        case 1 :
          erfuellt = true;  /* aktuelle Klm ist erfuellbar */
	    Zweig = Eps;      /* zur Rekonstruktion */
	    ++Autarkien;      /* jede erfuellende Belegung ist Autarkie */
          return;

        case 2 :
          ++Autarkien;  /* eine (echte) Autarkie wurde gefunden */
          erfuellt = false;
          reduziert = false;
          Wahl = true;
          Single = true;
          Zweig = Eps;
          return;

        case 3 :
        /* ein Quasi-Single-Knoten (mit 2-CNF in beiden(!) Zweigen) */
          erfuellt = false;
          reduziert = false;
          Wahl = true;
          Single = false;
          Zweig = Pos;  /* willkuerlich (da unerheblich) */
          return;

        default :  /* ein Quasi-Single-Knoten */
          erfuellt = false;
          reduziert = false;
          Wahl = true;
          Single = false;
          Zweig = (VZ) ! Eps;
          return;
        }

    case 4 :
#ifndef FASTAUTARKIE
      ++FastAutarkien; /* geht ueber in den Sonst-Fall */
      [[ fallthrough ]];
#else
      if ((reduziert = BehFastautarkie())) {
	  erfuellt = false;
	  return;
	}
#endif
    default :  /* erster Zweig hat nichts erbracht */
      Eps = Neg;
      La_HuelleFil(Literal(v, Eps));
      switch (auswerten()) { /* Auswertung des zweiten Zweiges */

        case 1 :
          erfuellt = true;  /* aktuelle Klm ist erfuellbar */
          Zweig = Eps;      /* zur Rekonstruktion */
          ++Autarkien;      /* jede erfuellende Belegung ist Autarkie */
          return;

        case 2 :
          ++Autarkien;  /* eine (echte) Autarkie wurde gefunden */
          erfuellt = false;
          reduziert = false;
          Wahl = true;
          Single = true;
          Zweig = Eps;
          return;

        case 3 :  /* ein Quasi-Single-Knoten */
          erfuellt = false;
          reduziert = false;
          Wahl = true;
          Single = false;
          Zweig = Eps;
          return;

	case 4 :
#ifndef FASTAUTARKIE
	  ++FastAutarkien; /* geht ueber in den Sonst-Fall */
        [[ fallthrough ]];
#else
	  if ((reduziert = BehFastautarkie())) {
          erfuellt = false;
	    return;
	  }
#endif
        default :  /* normale binaere Verzweigung */
          erfuellt = false;
          reduziert = false;
          Wahl = false;
          return;
        }
    }
}

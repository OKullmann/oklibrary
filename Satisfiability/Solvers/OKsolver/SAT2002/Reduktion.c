// Oliver Kullmann, 1.11.1998 (Frankfurt)
/* Copyright 1998 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKsolver/SAT2002/Reduktion.c
  \brief The standard reduction process
*/

                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Einheit: Reduktion */

/* Das "Reduktionsmodul" */


/* 1.11.1998 */


/* 4.5.1999 Zum Aufbau: */

/* Reduktion1 fuehrt F in r_2(F) ueber, waehrend Reduktion2 pure Literale eliminiert. */

/* Reduktion1 ruft La_Huelle ("look ahead" der 1-Klauseln-Huelle fuer x -> 0) */
/* und belege_Huelle (tatsaechliches Belegen einer berechneten 1-Klauseln-Huelle) */
/* auf. */

/* belege_Huelle ruft belege auf (und, falls BAUMRES gesetzt ist, belege_VK fuer */
/* nicht-Verzweigungsvariablen. */

/* La_Huelle ruft La_Belege fuer die einzelnen Elemente der Huelle. */


#include <stdlib.h>
#include <assert.h>

#include "OK.h"
#include "VarLitKlm.h"
#include "BaumRes.h"
#include "Belegung.h"
#include "Reduktion.h"
#ifdef LOKALLERNEN
#include "lokalesLernen.h"
#endif

extern unsigned int Runde;


/* --------------------------------------------------------- */


static Stapeleintrag *H;
static Stapeleintrag *sp;

/* *H, ..., *(sp-1) ist die Huelle des aktuellen Test-Literals x */
/* (also *H = x bzw. H -> l = x), wobei H -> k die zugehoerige */
/* Klausel ist, die nun zur 1-Klausel wurde. */

#ifdef LOKALLERNEN
#ifndef NL2RED
static Stapeleintrag *spneu;
#endif
#endif

static Stapeleintrag *H0;
static Stapeleintrag *H1; 
/* zum Speichern von Huellen (fuer die beiden Zweige) */


static int *DK;
/* DK[i] ist die Anzahl eliminierter i-Klauseln. */

static unsigned int Reduktionsrunde;
/* der Wert von Runde vor der letzten Reduktion */

 /* --------------------------------------------------------------------------------------- */

#ifndef BAUMRES
__inline__ static void neue1Klausel(LIT x)
#else
__inline__ static void neue1Klausel(LIT x, KLN kn)
#endif
/* wird aufgerufen, wenn eine 1-Klausel */
/* Komp(x) gefunden wurde */
{
  if (RundeL(x) != Runde)  /* 1-Kl-Elim. nicht schon vorgemerkt? */
    {
      setzenRundeL(x);
#ifndef BAUMRES
      *(sp++) = x;
#else
      sp -> l = x;
      (sp++) -> k = kn; /* die fuer die neue 1-Klausel verantwortliche Klausel */
#endif
    }
}


__inline__ static bool La_belege( LIT x )

/* Fuehrt x -> 0 durch (bzgl. letzteRK, letzteRL und LaLaenK), */
/* und schreibt neu entstehende 1-Klauseln nach H (mit Eintrag in letzteRL), */
/* wobei sp erhoeht wird. */
/* La_belege(x) == false, falls die leere Klausel entstand. */
/* Im Unterschied zur analogen Funktion in "Filter.c" brauchen hier die */
/* (non x)-Vorkommen nicht betrachtet zu werden (und Klauseln werden auch */
/* nicht als erfuellt markiert). */

/* Ist BAUMRES gesetzt, so werden im "Normalfall" parallel zu H in das Feld EK */
/* die Nummern der Klauseln eingetragen, die nun zu 1-Klauseln wurden. */
/* Im Falle dass die leere Klausel entsteht, stehen in aktrelV die (urspruenglichen) */
/* Variablen der nun leeren Klausel. */

{
  LITV y, z;
  LIT lz;
  KLN kn;

  /* Durchlaufe alle x-Vorkommen und kuerze die aktiven Klauseln */

  for (y = erstesVork(x); echtesVork(y, x); y = naechstesVork(y))

      if (RundeK( kn = KlnVk(y) ) != Runde)
      /* Klausel von y noch nicht angefasst? */
        {
          if (Laenge(kn) == 2)  /* 2-Klausel? */
            {
              if (RundeL(lz = LitVk(naechstesVorkK(y))) == Runde)  
	      /* Widerspruch? (Komplementaere 1-Kl-Elim. schon vorgemerkt) */
#ifndef BAUMRES
		return false;
#else
	      {
	        setzenKl(kn); 
		/* die (urspruengl.) Variablen in der (nun) leeren Klausel */
		return false;
	      }
#endif
#ifndef BAUMRES
	      neue1Klausel(Komp(lz));
#else
	      neue1Klausel(Komp(lz), kn);
#endif
            }
          else  /* >= 3-Klausel */
            {
              setzenRundeK(kn);
              ZuwLaLaenge(kn, Laenge(kn) - 1);
            }
        }
      else  /* Klausel schon angefasst */

          if (LaLaenge(kn) == 2)  /* 2-Klausel? */
            {
              for (z = naechstesVorkK(y); z!= y; z = naechstesVorkK(z))
	      /* Suche zweites Literal */
                 if (RundeL(lz = LitVk(z)) != Runde)
		 /* z nicht auf 0 gesetzt? */
                   break;
              if (z == y)
	      /* alle Literale in der Klausel auf 0 gesetzt */
#ifndef BAUMRES
                return false;
#else
	      {
		setzenKl(kn);
		/* die (urspruengl.) Variablen in der (nun) leeren Klausel */
		return false;
	      }
#endif
#ifndef BAUMRES
	      neue1Klausel(Komp(lz));
#else
	      neue1Klausel(Komp(lz), kn);
#endif
            }
          else  /* >= 3-Klausel */
            M1LaLaenge(kn);

  return true;
}



__inline__ static bool La_Huelle(LIT x)

/* Ruft La_Belege auf zuerst mit x, und dann mit den neu entstehenden  */
/* 1-Kl.-Elim. */
/* La_Huelle == false falls die leere Klausel entstand. */

/* Ist BAUMRES gesetzt, so werden im Falle der Erzeugung der leeren Klausel  */
/* (andernfalls geschieht ja nichts) in aktrelV die beteiligten Variablen  */
/* zurueckgegeben. */

/* Falls nicht die leere Klausel entstand: */
/* sp zeigt nach Beendigung auf das erste freie Element im Stapel. */
/* Ist LOKALLERNEN gesetzt, so zeigt spneu <= sp auf die */
/* erste neue "1-Klausel". */


{
  Stapeleintrag *p; /* zeigt auf die naechste "1-Klausel" */

  if (++Runde == 0)
    {
      NullsetzenRK(); NullsetzenRL();
      Reduktionsrunde = 0; 
/* (damit geht die Information zum aktuellen Reduktions-Durchlauf verloren) */
      Runde = 1;
    }

  p = H;
#ifndef BAUMRES
  H[0] = x;
#else
  H -> l = x;
#endif
  sp = H + 1; setzenRundeL(x);
  if (La_belege(x) == false)
    /* La_belege schreibt 1-Klauseln nach H unter Erhoehung */
    /* von sp; letzteRL wird dabei belegt */
#ifndef BAUMRES
    return false;
#else
    {
      while (--sp > H)
	if (enthalten(Var(sp -> l)))
	  hinzufuegenKl(sp -> k);
      return false;
    }
#endif
  p++;
#ifdef LOKALLERNEN
#ifndef NL2RED
  spneu = sp;
#endif
#endif
  while (p < sp)
    {
#ifndef BAUMRES
      x = *p;
#else
      x = p -> l;
#endif
      if (La_belege(x) == false)
#ifndef BAUMRES
	return false;
#else
        {
	  while (--sp > H)
	    if (enthalten(Var(sp -> l)))
	      hinzufuegenKl(sp -> k);
	  return false;
	}
#endif
      p++;
    }
  return true;
}


__inline__ static void belegeRed(LIT x)

/* fuehrt x -> 0 durch mit Aktualisierung von "Laenk" und "DK" */
/* sowie "Pfad" (inkl. "Tiefe") und "belegt"; */
/* Vorbedingungen:  Es entsteht nicht die leere Klausel. */

{
  LITV y, z;
  LIT kx;
  KLL p;
  VAR v;

  for (y = erstesVork(x); echtesVork(y, x); y = naechstesVork(y))
  {
    loeseLK(y);
    DK[ p = LaengeM1(KlnVk(y)) ]++;
    DK[p-1]--;
  }

  for (y = erstesVork(kx = Komp(x)); echtesVork(y, kx); y = naechstesVork(y))
  {
    for (z = naechstesVorkK(y); z != y; z = naechstesVorkK(z))
      loeseLv(z);

    DK[Laenge(KlnVk(y))]++;
  }

  LiteinPfad(x); Tiefe++;
  loeseV(v = Var(x));
  setzenbelegt(v, true);
}


#ifdef BAUMRES

__inline__ static void belege_VKRed(LIT x, KLN K)

/* fuehrt x -> 0 durch mit Aktualisierung von "Laenk" und "DK" */
/* sowie "Pfad" (inkl. "Tiefe") und "belegt"; */
/* Vorbedingung:  Es entsteht nicht die leere Klausel. */
/* Zusaetzlich wird noch EK aktualisiert. */


{
  LITV y, z;
  LIT kx;
  KLL p;
  VAR v;

  extern void loeseLK(LITV x);
  extern void loeseLv(LITV x);
  extern void loeseV(VAR x);

  for (y = erstesVork(x); echtesVork(y, x); y = naechstesVork(y))
  {
    loeseLK(y);
    DK[ p = LaengeM1(KlnVk(y)) ]++;
    DK[p-1]--;
  }

  for (y = erstesVork(kx = Komp(x)); echtesVork(y, kx); y = naechstesVork(y))
  {
    for (z = naechstesVorkK(y); z != y; z = naechstesVorkK(z))
      loeseLv(z);

    DK[Laenge(KlnVk(y))]++;
  }

  LiteinPfad(x);
  loeseV(v = Var(x));
  setzenbelegt(v, true);

  Kln_eintragen_relV(K); Tiefe++;
}

#endif


__inline__ static bool belege_Huelle( void )

/* Wendet eine bereits berechnete "Huelle" an auf die aktuelle Klm */
/* durch Aufrufen von "belegeRed". */

/* Belegt so die Literale aus H[0, ..., sp-1] mit 0 und aktualisiert */
/* LaenK, aktAnzK, aktP und aktN. */
/* belege_Huelle == true, falls die Klauselmenge erfuellt wird. */

{
  unsigned int i;
  Stapeleintrag *p;

  for (i = 0; i <= aktP; i++)
    DK[i] = 0;
#ifdef LOKALLERNEN
  eintragenTiefe();
#endif
#ifndef BAUMRES
  for (p = H; p < sp; p++)
    belegeRed(*p);
#else
  aktV_volleintragen_relV();
  belegeRed(H -> l);
  for (p = H + 1; p < sp; p++)
    belege_VKRed(p -> l, p -> k);
#endif
  for (i = aktP; i >= 2; i--)
    aktAnzK[i] -= DK[i];
  for (i = aktP; i >= 2; i--)
    if (aktAnzK[i] != 0)
      break;
  if (i == 1)
    return true;
  aktP = i;
  aktN  -= (sp - H);
  return false;
}


/* -------------------------------------------------------------------------------------- */

void berechne_Huelle(LIT x) {
  /* berechnet die Huelle von x und schreibt die Ergebnisse nach H */
  /*      (sp zeigt dann auf das erste freie Element) */
  /* Voraussetzung: Die leere Klausel entsteht nicht. */

  Stapeleintrag *p; /* zeigt auf die naechste "1-Klausel" */
  LITV z;
  KLN kn;

  if (++Runde == 0) {
    NullsetzenRK(); NullsetzenRL();
    Reduktionsrunde = 0; 
    /* (damit geht die Information zum aktuellen Reduktions-Durchlauf verloren) */
    Runde = 1;
  }

  p = H;
#ifndef BAUMRES
  H[0] = x;
#else
  H -> l = x;
#endif
  sp = H + 1; setzenRundeL(x);
  do {
    for (LITV y = erstesVork(x); echtesVork(y, x); y = naechstesVork(y))
      if (RundeK( kn = KlnVk(y) ) != Runde) {
        /* Klausel von y noch nicht angefasst? */
        if (Laenge(kn) == 2) {  /* 2-Klausel? */
          const LIT lz = LitVk(naechstesVorkK(y)); 
#ifndef BAUMRES
          neue1Klausel(Komp(lz));
#else
          neue1Klausel(Komp(lz), kn);
#endif
        }
        else { /* >= 3-Klausel */
          setzenRundeK(kn);
          ZuwLaLaenge(kn, Laenge(kn) - 1);
        }
      }
      else  /* Klausel schon angefasst */
	if (LaLaenge(kn) == 2) { /* 2-Klausel? */
#ifndef NDEBUG
          LIT lz = 0;
#else
          LIT lz;
#endif
          for (z = naechstesVorkK(y); z!= y; z = naechstesVorkK(z))
            /* Suche zweites Literal */
            if (RundeL(lz = LitVk(z)) != Runde)
              /* z nicht auf 0 gesetzt? */
              break;
#ifndef BAUMRES
          assert(lz);
          neue1Klausel(Komp(lz));
#else
          assert(lz);
          neue1Klausel(Komp(lz), kn);
#endif
        }
	else  /* >= 3-Klausel */
	  M1LaLaenge(kn);
    
    if (++p == sp)
      break;
#ifndef BAUMRES
    x = *p;
#else
    x = p -> l;
#endif
  }
  while (true);
}


/* -------------------------------------------------------------------------------------- */

size_t BedarfReduktionsV( void )
{
  return  2 * N * sizeof(Stapeleintrag)
    + (P+1) * sizeof(int);
}

void *ReduktionsV(void *Z)
{
  H0 = (Stapeleintrag *) Z; Z = (void *) (H0 + N);
  H1 = (Stapeleintrag *) Z; Z = (void *) (H1 + N);
  DK = (int *) Z; Z = (void *) (DK + P + 1);
  return Z;
}


/* -------------------------------------------------------------------------------------- */


__inline__ char Reduktion1( void ) {

/* Fuehrt die aktuelle Klm F in r_2(F) ueber, wobei die entsprechende Belegung */
/* in "Pfad" eingetragen wird. */

/* 0: normal */
/* 1: SAT, d.h. r_2(F) = {} */
/* 2: UNSAT, d.h. r_2(F) = { {} } */


  Stapeleintrag *sp0;
#ifdef LOKALLERNEN
#ifndef NL2RED
  Stapeleintrag *spneu0;
#endif
#endif
  if (aktAnzK[2] == 0) /* ohne 2-Klauseln keine Chance */
    return 0;

  bool r; /* wurde reduziert? */
  bool ersterZweig, zweiterZweig;

  Reduktionsrunde = Runde;
  do {
    r = false;
    for (VAR v = ersteVar(); echteVar(v); v = naechsteVar(v)) {
      if (r && belegt(v))
        continue; /* falls v von voriger Reduktion schon belegt wurde */
      const LIT v0 = Literal(v, Pos); H = H0; 
      if ((ersterZweig = (RundeL(v0) <= Reduktionsrunde)) && (La_Huelle(v0) == false)) {
        /* Huelle[ v -> 0 ] ergab die leere Klausel */
        ++V1KlRed; /* Anwendung einer Reduktion der 2. Stufe */
#ifdef BAUMRES
        aktV_speichern();
#endif
        const LIT v1 = Literal(v,Neg);
        if (RundeL(v1) <= Reduktionsrunde) {
          if (La_Huelle(v1) == false)
            /* auch Huelle[ v -> 1 ] ergab leere Klausel */
#ifndef BAUMRES
            return 2; /* UNSAT */
#else
          {
            hinzufuegenS();
            return 2; /* UNSAT */
          }
#endif
        }
        else
          berechne_Huelle(v1);
        if (belege_Huelle() == true)
          /* Huelle[ v -> 1 ] ist erfuellend */
          return 1; /* SAT */
        else {
          r = true;  /* es wurde reduziert */
          Reduktionsrunde = Runde;
        }
      }
      else { /* Zweig v -> 0 brachte nichts */
        const LIT v1 = Literal(v,Neg); H = H1;
        /* Umschalten auf v -> 1 */
        /* (die zu v -> 0 gehoerende Belegung ist in H0) */
        sp0 = sp;
#ifdef LOKALLERNEN
#ifndef NL2RED
        spneu0 = spneu;
#endif
#endif
        if ((zweiterZweig = (RundeL(v1) <= Reduktionsrunde)) && (La_Huelle(v1) == false)) {
          /* Huelle[ v -> 1 ] ergab die leere Klausel */
          ++V1KlRed; /* Anwendung einer Reduktion der 2. Stufe */
          if (ersterZweig) {
            H = H0; sp = sp0; /* wieder zurueckschalten */
          }
          else
            berechne_Huelle(v0);
          if (belege_Huelle() == true)
            /* Huelle[ v -> 0 ] ist erfuellend */
            return 1; /* SAT */
          else {
            r = true;  /* es wurde reduziert */
            Reduktionsrunde = Runde;
          }
        }
#ifdef LOKALLERNEN
#ifndef NL2RED
        else {
          /* Beide Zweige brachten keine direkten Reduktionen */
          if (ersterZweig && (spneu0 < sp0)) {
            eintragenNK();
            erzeugeNK(v0, spneu0, sp0);
            r = true;
            Reduktionsrunde = Runde;
          }
          if (zweiterZweig && (spneu < sp)) {
            eintragenNK();
            erzeugeNK(v1, spneu, sp);
            r = true;
            Reduktionsrunde = Runde;
          }
        }
#endif
#endif
      }
    }
  }
  while (r);
  
  return 0;
}


/* -------------------------------------------------------------------------------------- */


__inline__ char Reduktion2( void )

/* Elimination purer Literale */

/* 0: normal */
/* 1: SAT */

{
  unsigned int dn = 0;
  bool r;
  LIT l, kl;
  unsigned int i;
  VAR v;

  for (i = 0; i <= aktP; i++)
    DK[i] = 0;

  do
  {
    r = false;

    for (v = ersteVar(); echteVar(v); v = naechsteVar(v))
    {
      l = Literal(v,Pos); kl = Literal(v,Neg);

      if (! echtesVork(erstesVork(l), l))
      {
#ifdef LOKALLERNEN
	eintragenTiefe();
#endif
        belegeRed(l); dn++;
        r = true; PureL++;
      }
      else if (! echtesVork(erstesVork(kl), kl))
      {
#ifdef LOKALLERNEN
	eintragenTiefe();
#endif
        belegeRed(kl); dn++;
        r = true; PureL++;
      }
    }
  }
  while (r);

  if (dn == 0)
    return 0;

  for (i = aktP; i >= 2; i--)
    aktAnzK[i] -= DK[i];

  for (i = aktP; i >= 2; i--)
    if (aktAnzK[i] != 0)
      break;

  if (i == 1)
    return 1;

  aktP = i;
  aktN -= dn;

  return 0;
}

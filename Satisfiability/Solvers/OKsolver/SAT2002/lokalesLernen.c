// Oliver Kullmann, 4.3.2001 (Toronto)

/*!
  \file OKsolver/SAT2002/lokalesLernen.c
  \brief First implementation of local learning
*/

                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Angelegt: 4.3.2001 */

/* Einheit: lokalesLernen */

#ifndef LOKALESLERNEN_ajsje619Y5r
#define LOKALESLERNEN_ajsje619Y5r

# ifndef LOKALLERNEN

struct EmptyFile;

# else

#include <stdlib.h>

#include "OK.h"
#include "VarLitKlm.h"
#include "Speicher.h"
#include "lokalesLernen.h"


static NKL freieNK;
static NKL AnfangNK = NULL;

static Marken *freieMarke;
static Marken *Anfangsmarke = NULL;


void InitlokalesLernen(void)
{
  freieNK = AnfangNK = (NKL) xmalloc(sizeneuK);
  AnfangNK -> naechstes = NULL;
  AnfangNK -> vorheriges = NULL;
  /* die folgenden Setzungen werden als konstant angesehen */
  setzenKln(&(AnfangNK -> x1), &(AnfangNK -> Klausel));
  setzenKln(&(AnfangNK -> x2), &(AnfangNK -> Klausel));

  freieMarke = Anfangsmarke = (Marken *) xmalloc(sizeof(Marken));
  Anfangsmarke -> naechste = NULL;
  Anfangsmarke -> vorige = NULL;
  Anfangsmarke -> Tiefe = NULL;
  Anfangsmarke -> NK = NULL;
}


__inline__ static void naechsteNK(void)
{
  if (freieNK -> naechstes != NULL)
    freieNK = freieNK -> naechstes;
  else
    {
      NKL Z;
      maxneue2K++;

      Z = (NKL) xmalloc(sizeneuK);
      Z -> naechstes = NULL;
      Z -> vorheriges = freieNK;
      freieNK -> naechstes = Z;
      /* die folgenden Setzungen werden als konstant angesehen */
      setzenKln(&(Z -> x1), &(Z -> Klausel));
      setzenKln(&(Z -> x2), &(Z -> Klausel));

      freieNK = Z;
    }
}


__inline__ void erzeugeNK(LIT l, Stapeleintrag *spneu, Stapeleintrag *sp)
/* setzt voraus, dass mindestens eine neue Klausel eingetragen wird */
{
  LITV x0, x1, y1; unsigned int d;
  LIT komp;

  d = (sp - spneu);
  neue2Klauseln += d;
  aktAnzK[2] += d;

  x0 = NULL; x1 = erstesVork(l);
  setzenerstesV(l, &(freieNK -> x1));
  do
    {
      setzenlLv(&(freieNK -> x1), x0);

      setzennLK(&(freieNK -> x1), &(freieNK -> x2));
      setzennLK(&(freieNK -> x2), &(freieNK -> x1));
      setzenlLK(&(freieNK -> x1), &(freieNK -> x2));
      setzenlLK(&(freieNK -> x2), &(freieNK -> x1));

      setzenLit(&(freieNK -> x1), l);

      komp = Komp(*spneu);
      setzenLit(&(freieNK -> x2), komp);

      y1 = erstesVork(komp);
      setzenlLv(&(freieNK -> x2), NULL);
      setzennLv(&(freieNK -> x2), y1);
      setzenerstesV(komp, &(freieNK -> x2));
      setzenlLv(y1, &(freieNK -> x2));
      
      ZuwLaenge(&(freieNK -> Klausel), 2);
      setzen0RundeK(&(freieNK -> Klausel));
#ifdef FASTAUTARKIE
      freieNK -> l = NullLiteral;
#endif
#ifdef BAUMRES
#error "Noch nicht!"
#endif
      x0 = &(freieNK -> x1);
      naechsteNK();
      if (++spneu == sp)
	break;
      setzennLv(x0, &(freieNK -> x1));
    }
  while (true);
  setzennLv(x0, x1);
  setzenlLv(x1, x0);
}

#ifdef FASTAUTARKIE

__inline__ void erzeugeFANK(LIT l, LIT *a, LIT *b)
/* setzt voraus, dass mindestens eine neue Klausel eingetragen wird */
{
  LITV x0, x1, y1; unsigned int d;

  d = (b - a);
  neue2Klauseln += d;
  aktAnzK[2] += d;

  x0 = NULL; x1 = erstesVork(l);
  setzenerstesV(l, &(freieNK -> x1));
  do
    {
      setzenlLv(&(freieNK -> x1), x0);

      setzennLK(&(freieNK -> x1), &(freieNK -> x2));
      setzennLK(&(freieNK -> x2), &(freieNK -> x1));
      setzenlLK(&(freieNK -> x1), &(freieNK -> x2));
      setzenlLK(&(freieNK -> x2), &(freieNK -> x1));

      setzenLit(&(freieNK -> x1), l);

      setzenLit(&(freieNK -> x2), *a);

      y1 = erstesVork(*a);
      setzenlLv(&(freieNK -> x2), NULL);
      setzennLv(&(freieNK -> x2), y1);
      setzenerstesV(*a, &(freieNK -> x2));
      setzenlLv(y1, &(freieNK -> x2));
      
      ZuwLaenge(&(freieNK -> Klausel), 2);
      setzen0RundeK(&(freieNK -> Klausel));

      freieNK -> l = Komp(l);

#ifdef BAUMRES
#error "Noch nicht!"
#endif
      x0 = &(freieNK -> x1);
      naechsteNK();
      if (++a == b)
	break;
      setzennLv(x0, &(freieNK -> x1));
    }
  while (true);
  setzennLv(x0, x1);
  setzenlLv(x1, x0);
}

#endif


__inline__ static void neueMarke()
{
  if (freieMarke -> naechste == NULL)
    {
      Marken *Z;
      Z = (Marken *) xmalloc(sizeof(Marken));
      Z -> naechste = NULL;
      Z -> vorige = freieMarke;
      Z -> Tiefe = NULL;
      Z -> NK = NULL;
      freieMarke -> naechste = Z;
      freieMarke = Z;
    }
  else
    {
      freieMarke = freieMarke -> naechste;
      freieMarke -> Tiefe = NULL;
      freieMarke -> NK = NULL;
    }
}

__inline__ void eintragenTiefe(void)
{
  if (freieMarke -> NK != NULL)
    {
      neueMarke();
      freieMarke -> Tiefe = Tiefe;
    }
  else if (freieMarke -> Tiefe == NULL)
    freieMarke -> Tiefe = Tiefe;
}

void eintragenNK()
{
  if (freieMarke -> Tiefe != NULL)
    {
      neueMarke();
      freieMarke -> NK = freieNK;
    }
  else if (freieMarke -> NK == NULL)
    freieMarke -> NK = freieNK;
}


Marken *Markierung(void)
{
  if ((freieMarke -> Tiefe != NULL) || (freieMarke -> NK != NULL))
    neueMarke();
  return freieMarke;
}


void Rueckgaengigmachung(Marken *Zeiger)
{
  while (true)
    {
      if (freieMarke -> Tiefe != NULL)
	do
	  {
	    Tiefe--;
	    rebelege(PfadLit());
	  }
	while (Tiefe != freieMarke -> Tiefe);
      else if (freieMarke -> NK != NULL)
	do
	  {
	    freieNK = freieNK -> vorheriges;
	    loeseLv(&(freieNK -> x1));
	    loeseLv(&(freieNK -> x2));
	    aktAnzK[2]--;
#ifdef FASTAUTARKIE
	    if (freieNK -> l != NullLiteral)
	      setzenFastautarkie(freieNK -> l, false);
/* 	    setzt voraus, das diese Klauseln auf einmal eingetragen */
/* 	      werden */
#endif
	  }
	while (freieNK != freieMarke -> NK);
      if (freieMarke == Zeiger)
	break;
      freieMarke = freieMarke -> vorige;
    }
  freieMarke -> Tiefe = NULL;
  freieMarke -> NK = NULL;
}



void AufraeumenlokalesLernen(void)
{
  NKL Z; NKL Z0;
  Marken *Z1; Marken *Z10;

  Z0 = AnfangNK;
  while (Z0 != NULL)
    {
      Z = Z0 -> naechstes;
      free(Z0);
      Z0 = Z;
    }
  AnfangNK = NULL;

  Z10 = Anfangsmarke;
  while (Z10 != NULL)
    {
      Z1 = Z10 -> naechste;
      free(Z10);
      Z10 = Z1;
    }
  Anfangsmarke = NULL;
}

void NullsetzenRKNK(void)
{
  NKL Z;
  for (Z = AnfangNK; Z != NULL; Z = Z -> naechstes)
    setzen0RundeK(&(Z -> Klausel));
}


# endif

#endif


// Oliver Kullmann, 2.5.2000 (Toronto)
/* Copyright 2000 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKsolver/SAT2002/Abstand_nK.c
  \brief Standard distance function (weighted number of new clauses), choice of branch according to approximation of true probability
*/

/* Oliver Kullmann */
/* 2.5.2000 */

/* Einheit: Abstand_nK.c */

/* "Neue Klauseln mit Gewichten" */

/* Fuer Gewichte G3 - G7 (fuer neue Klauseln der Laenge 3 - 7) wird das optimale Gewicht  */
/* aus Experimenten mit Zufalls-k-KNFs nahe dem Schwellenwert (wobei k = i+1 fuer Gi)  */
/* gesetzt. */

/* Fuer Gewichte G8, ... wird eine Extrapolation der Gewichte G3 - G7 verwendet. */

#include <stdlib.h>
#include <math.h>

#include "OK.h"
#include "Filter.h"
#include "Abstand.h"

const char *Abstandsname = "Abstand_nK.c";

float d0, d1;
unsigned int d0S, d1S;


#define G3 0.2
#define G4 0.05
#define G5 0.01
#define G6 0.003

#define ALPHA 20.4514
#define BETA 0.218673

static float *Gewicht;


/* -------------------------------------------------------------------------- */

size_t BedarfAbstandsV( void )
{
  return P * sizeof(float);
}

void *AbstandsV(void *Z)
{
  unsigned int p;

  Gewicht = (float *) Z; Z = (void *) (Gewicht + P);

  for (p = 2; p < P; p++)
    switch (p)
      {
      case 2 : Gewicht[2] = 1; break;
      case 3 : Gewicht[3] = G3; break;
      case 4 : Gewicht[4] = G4; break;
      case 5 : Gewicht[5] = G5; break;
      case 6 : Gewicht[6] = G6; break;
      default : Gewicht[p] = ALPHA * pow(BETA, p);
      }
  
  return Z;
}

/* -------------------------------------------------------------------------- */


__inline__ void Abstand ( void )

/* d = \sum_{i=2}^{+\infty} Gewicht[i] * NeuK[i] */

/* NeuK[i] = Anzahl neuer Klauseln der Laenge i (= 0 fuer i >= aktP) */

/* Zweigauswahl: Minimiere \sum_{C \in F} 2^{-|C|}  */

{
  unsigned int i, gZ;
  int s;
  float gd;

  for (i = aktP - 1, gZ = 2, d0 = d1 = 0,
	 s = (DeltaK[Pos][Schalter])[aktP] - (DeltaK[Neg][Schalter])[aktP];
       i > 1; i--, gZ *= 2)
    {
      d0 += (gd = Gewicht[i]) * (NeuK[Pos][Schalter])[i];
      d1 += gd * (NeuK[Neg][Schalter])[i];
      
      s += gZ * ((DeltaK[Pos][Schalter])[i] - (DeltaK[Neg][Schalter])[i]);
    }
  
  if (s >= 0)
    Zweig = Pos;
  else
    Zweig = Neg;

  d0S = DeltaN[Pos][Schalter];
  d1S = DeltaN[Neg][Schalter];
}

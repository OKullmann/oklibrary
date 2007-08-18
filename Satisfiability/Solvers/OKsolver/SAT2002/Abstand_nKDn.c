                      /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Angelegt: Februar 2001 */

/* Einheit: Abstand_nKDn */

/* "Mischung zwischen Neue Klauseln mit Gewichten und Delta n" */

/* Fuer Gewichte G3 - G7 (fuer neue Klauseln der Laenge 3 - 7) wird das optimale Gewicht  */
/* aus Experimenten mit Zufalls-k-KNFs nahe dem Schwellenwert (wobei k = i+1 fuer Gi)  */
/* gesetzt. */

/* Fuer Gewichte G8, ... wird eine Extrapolation der Gewichte G3 - G7 verwendet. */

/* 9.3.2001: Nun auch die Gewichte zur Zweigauswahl als Gleitkomma-Zahlen */
/* abgespeichert (die vorige Version funktionierte korrekt nur fuer */
/* kleine Klausellaengen). */


#include <stdlib.h>
#include <math.h>

#include "OK.h"
#include "Filter.h"
#include "Abstand.h"

const char *Abstandsname = "Abstand_nKDn.c";

float d0, d1;
unsigned int d0S, d1S;

// #ifndef DYNAMISCH
// #error "DYNAMISCH muss definiert sein!"
// #endif

#ifndef FAKTOR
# define FAKTOR 1.0
#endif

static float q;

#define G3 0.2
#define G4 0.05
#define G5 0.01
#define G6 0.003

#define ALPHA 20.4514
#define BETA 0.218673

static float *Gewicht;

static float *GewichtZ;


/* -------------------------------------------------------------------------- */

size_t BedarfAbstandsV( void )
{
  return P * sizeof(float) +
    (P + 1) * sizeof(float);
}

void *AbstandsV(void *Z)
{
  unsigned int p;

  Gewicht = (float *) Z; Z = (void *) (Gewicht + P);
  GewichtZ = (float *) Z; Z = (void *) (GewichtZ + P + 1);

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
 
  GewichtZ[2] = 1;
  for (p = 3; p <= P; p++)
    GewichtZ[p] = GewichtZ[p-1] / 2;

  return Z;
}


void InitAbstand2 ( void )
{
  if (aktAnzK[2] >= FAKTOR * aktN)
    q = 1;
  else
    q = (float) aktAnzK[2] / (float) (FAKTOR * aktN);
}

/* -------------------------------------------------------------------------- */


void Abstand ( void )

/* d = \sum_{i=2}^{+\infty} Gewicht[i] * NeuK[i] */

/* NeuK[i] = Anzahl neuer Klauseln der Laenge i (= 0 fuer i >= aktP) */

/* Zweigauswahl: Minimiere \sum_{C \in F} 2^{-|C|}  */

{
  unsigned int i;
  float s;

  /* Es wird vorausgesetzt, dass P >= 3 und Gewicht[2] = GewichtZ[2] = 1. */

  d0 = (NeuK[Pos][Schalter])[2];
  d1 = (NeuK[Neg][Schalter])[2];

  s = (DeltaK[Pos][Schalter])[2] - (DeltaK[Neg][Schalter])[2];

  for (i = 3; i < aktP; i++)
    {
      d0 += Gewicht[i] * (NeuK[Pos][Schalter])[i];
      d1 += Gewicht[i] * (NeuK[Neg][Schalter])[i];

      s += GewichtZ[i] * ((DeltaK[Pos][Schalter])[i] - (DeltaK[Neg][Schalter])[i]);
    }

  s += GewichtZ[aktP] * ((DeltaK[Pos][Schalter])[aktP] - (DeltaK[Neg][Schalter])[aktP]);
  
  if (s >= 0)
    Zweig = Pos;
  else
    Zweig = Neg;

  d0S = DeltaN[Pos][Schalter];
  d1S = DeltaN[Neg][Schalter];

  d0 = q * d0S + (1 - q) * d0;
  d1 = q * d1S + (1 - q) * d1;
}

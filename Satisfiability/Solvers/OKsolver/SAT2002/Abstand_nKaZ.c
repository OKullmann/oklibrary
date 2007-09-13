// Oliver Kullmann, 23.2.2001 (Toronto)

/*!
  \file OKsolver/SAT2002/Abstand_nKaZ.c
  \brief Standard distance function (weighted number of new clauses), choice of branch according to maximal probability for random assignment
*/

                      /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Angelegt: 23.2.2001 */

/* Einheit: Abstand_nKaZ */

/* "Neue Klauseln mit Gewichten" */

/* Zweigauswahl: Maximiere die Wahrscheinlichkeit, dass eine feste */
/* Belegung die Klauselmenge, deren Klauseln zufaellig und unabhaengig */
/* voneinander gewaehlt werden (nur die Groessen und die Anzahlen sind */
/* gegeben) erfuellt. */

/* Fuer Gewichte G3 - G7 (fuer neue Klauseln der Laenge 3 - 7) wird das optimale  */
/* Gewicht aus Experimenten mit Zufalls-k-KNFs nahe dem Schwellenwert  */
/* (wobei k = i+1 fuer Gi) gesetzt. */

/* Fuer Gewichte G8, ... wird eine Extrapolation der Gewichte G3 - G7  */
/* verwendet. */


#include <stdlib.h>
#include <math.h>

#include "OK.h"
#include "Filter.h"
#include "Abstand.h"

const char *Abstandsname = "Abstand_nKaZ.c";

float d0, d1;
unsigned int d0S, d1S;


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
  const float c2 = log(3.0 / 4);

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
    GewichtZ[p] = log(1 - pow(2, - ((int) p))) / c2;
  
  return Z;
}


/* -------------------------------------------------------------------------- */


__inline__ void Abstand ( void )

/* d = \sum_{i=2}^{+\infty} Gewicht[i] * NeuK[i] */

/* NeuK[i] = Anzahl neuer Klauseln der Laenge i (= 0 fuer i >= aktP) */

/* Zweigauswahl: Minimiere \sum_{C \in F} log(1 - 2^(-|C|))  */

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
}

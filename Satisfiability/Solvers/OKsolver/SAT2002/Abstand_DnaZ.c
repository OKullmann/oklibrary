// Oliver Kullmann, 5.3.2001 (Toronto)

/*!
  \file OKsolver/SAT2002/Abstand_DnaZ.c
  \brief Distance function Delta(n), choice of branch according to maximal probability for random assignment
*/

                      /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Angelegt: 5.3.2001 */

/* Einheit: Abstand_DnaZ.c */

/* "Delta n" */

/* Zweigauswahl: Maximiere die Wahrscheinlichkeit, dass eine feste */
/* Belegung die Klauselmenge, deren Klauseln zufaellig und unabhaengig */
/* voneinander gewaehlt werden (nur die Groessen und die Anzahlen sind */
/* gegeben) erfuellt. */

#include <stdlib.h>
#include <math.h>

#include "OK.h"
#include "Filter.h"
#include "Abstand.h"

const char *Abstandsname = "Abstand_DnaZ.c";

float d0, d1;
unsigned int d0S, d1S;

static float *GewichtZ;


/* -------------------------------------------------------------------------- */

size_t BedarfAbstandsV( void )
{
  return (P + 1) * sizeof(float);
}

void *AbstandsV(void *Z)
{
  int p;
  const float c2 = log(3.0 / 4);

  GewichtZ = (float *) Z; Z = (void *) (GewichtZ + P + 1);

  GewichtZ[2] = 1;
  for (p = 3; p <= P; p++)
    GewichtZ[p] = log(1 - pow(2, -p)) / c2;
  
  return Z;
}


/* -------------------------------------------------------------------------- */


__inline__ void Abstand ( void )

/* Zweigauswahl: Minimiere \sum_{C \in F} log(1 - 2^(-|C|))  */

{
  unsigned int i;
  float s;

  /* Es wird vorausgesetzt, dass P >= 3 und Gewicht[2] = GewichtZ[2] = 1. */

  for (s = 0, i = 2; i <= aktP; i++)
    s += GewichtZ[i] * ((DeltaK[Pos][Schalter])[i] - (DeltaK[Neg][Schalter])[i]);
  
  if (s >= 0)
    Zweig = Pos;
  else
    Zweig = Neg;

  d0 = (float) DeltaN[Pos][Schalter];
  d1 = (float) DeltaN[Neg][Schalter];

  d0S = 1;
  d1S = 1;
}

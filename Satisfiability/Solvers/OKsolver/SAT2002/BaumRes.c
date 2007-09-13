// Oliver Kullmann, 16.3.2001 (Toronto)

/*!
  \file OKsolver/SAT2002/BaumRes.c
  \brief Implementations of the functions for tree pruning
*/

                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */

/* Angelegt: 16.3.2001 */

/* Einheit: BaumRes */

#include <stdlib.h>
#include <string.h> /* fuer C++ (memset; 14.8.2001) */

#include "OK.h"
#include "VarLitKlm.h"
#include "Parameter.h"
#include "Speicher.h"
#include "Belegung.h"


#ifdef BAUMRES

/* ------------------------------------------------------------------------- */


/* Variablen */

VarMenge aktrelV;
VarMenge aktrelV0;
/* Die Implementation verlangt, dass unmittelbar nach aktrelV die */
/* Variable aktrelV0 ansetzt. */


/* ------------------------------------------------------------------------- */


size_t BedarfBaumResV( void )
{
  return 2 * GroesseVarMenge;
}

void *BaumResV(void *Z)
{
  aktrelV = (VarMenge) Z; Z = (void *) (aktrelV + ANZWORTE);
  aktrelV0 = (VarMenge) Z; Z = (void *) (aktrelV0 + ANZWORTE);
  return Z;
}


void AufraeumenBaumRes( void )
{
  Pfadinfo *Z; unsigned int i;
  if (Pfad != NULL)
    {
      for (Z = Pfad, i = 0; i < N; Z++, i++)
	free(Z -> M);
      Pfad = NULL;
    }
}


/* ------------------------------------------------------------------------- */



__inline__ bool enthalten (VAR v)
{
  return ((*(VPosition(v)) & VMaske(v)) != 0UL);
}



__inline__ static void hinzufuegen (VarMenge M)
{
  ZWort Z;
  for (Z = aktrelV; Z != aktrelV0; Z++, M++)
      *Z |= *M;
}



__inline__ void hinzufuegenKl (KLN k)
{
#ifndef LITTAB
  hinzufuegen(VarK(k));
#else
  VarMaske *z; unsigned int i;
  for (z = ersteVK(k), i = UrLaenge(k); i > 0; z++, i--)
    *(z -> Position) |= (z -> Maske);
#endif
}



__inline__ void setzenKl (KLN k)
{
#ifndef LITTAB
  memcpy((void *) aktrelV, (void *)(VarK(k)), GroesseVarMenge);
#else
  memset((void *) aktrelV, 0, GroesseVarMenge);
  hinzufuegenKl(k);
#endif
}



__inline__ void relVhinzufuegen (void)
{
  KLN k;
  k = Tiefe -> k;
  if (k != NULL)
    hinzufuegenKl(k);
  else
    hinzufuegen(Tiefe -> M);
}

__inline__ void relVMhinzufuegen (void)
{
  hinzufuegen(Tiefe -> M);
}


/* die naechste Operation ist wohl die am haeufigsten benutzte */

__inline__ void Kln_eintragen_relV(KLN k)
{
  Tiefe -> k = k;
}



__inline__ void aktV_eintragen_relV(void)
{
  if (Tiefe -> M == NULL)
    Tiefe -> M = (VarMenge) xmalloc(GroesseVarMenge);
  memcpy((void *)(Tiefe -> M), (void *) aktrelV, GroesseVarMenge);
}
  

__inline__ void aktV_volleintragen_relV(void)
{
  Tiefe -> k = NULL;
  if (Tiefe -> M == NULL)
    Tiefe -> M = (VarMenge) xmalloc(GroesseVarMenge);
  memcpy((void *)(Tiefe -> M), (void *) aktrelV, GroesseVarMenge);
}


__inline__ void aktV_speichern( void )
{
  memcpy((void *) aktrelV0, (void *) aktrelV, GroesseVarMenge);
}

__inline__ void hinzufuegenS ( void )
{
  hinzufuegen(aktrelV0);
}


#endif



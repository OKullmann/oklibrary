// Oliver Kullmann, 16.3.2001 (Toronto)
/* Copyright 2001 - 2007, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKsolver/SAT2002/BaumRes.h
  \brief The interface for tree pruning
*/

                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Angelegt: 16.3.2001 */

/* ----------------------------------------------------------------------- */
/* Zur Beschneidung des Suchbaumes */

#ifndef GELESENBAUMRES_096t4r
#define GELESENBAUMRES_096t4r

#ifdef BAUMRES

#include "VarLitKlm.h"

/* ----------------------------------------------------------------------- */

extern bool enthalten (const VAR v);
/* Ist v in aktrelV enthalten? */


/* Bearbeiten von aktrelV: */

extern void hinzufuegenKl (const KLN K);
/* fuegt die Variablen in Klausel K zu aktrelV hinzu */

extern void setzenKl (const KLN K);
/* setzt aktrelV auf die Variablen in K */

extern void relVhinzufuegen ();
/* fuegt die Variablen der durch Zeiger markierten Klausel im */
/* aktuellen Pfad zu aktrelV hinzu */

extern void relVMhinzufuegen ();
/* wie relVhinzufuegen, nur ist hier bekannt, dass die hinzuzufuegende */
/* Variablenmenge nicht durch eine Eingabe-Klausel-Nummer dargestellt ist */


/* Bearbeiten des aktuellen Pfades: */

extern void Kln_eintragen_relV(const KLN K);
/* traegt die Klausel(nummer) K in den aktuellen Pfad ein */

extern void aktV_volleintragen_relV();
/* traegt aktrelV in den aktuellen Pfad ein */

extern void aktV_eintragen_relV();
/* wie aktV_volleintragen, nur wird davon ausgegangen, dass */
/* es hier nicht mehr noetig ist, die Darstellung auf den Modus */
/* "Variablen-Menge" zu setzen (vergleiche relVMhinzufuegen) */


/* Speichern von aktrelV: */

extern void aktV_speichern();
/* speichert aktrelV in aktrelV0 */

extern void hinzufuegenS();
/* fuegt aktrelV0 zu aktrelV hinzu */


extern void AufraeumenBaumRes();


#endif

#endif


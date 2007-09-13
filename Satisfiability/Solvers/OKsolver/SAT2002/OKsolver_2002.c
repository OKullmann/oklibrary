// Oliver Kullmann, 3.2.2001 (Toronto)

/*!
  \file Solvers/OKsolver/SAT2002/OKsolver_2002.c
  \brief Complete program of the old OKsolver where everything is included (for
  better inlining)

  No linking needed here.
*/

                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Angelegt: 3.2.2001 */


#define __include__ include

#include "Parameter.c"
#include "VarLitKlm.c"
#include "BaumRes.c"
#include "Belegung.c"
#include "lokalesLernen.c"
#include "Reduktion.c"
#include "Filter.c"
#include "Abstand_nKaZ.c"
#include "Projektion_Prod.c"
#include "Speicher.c"
#include "OKsolver_2002_lnk.c"
#include "Ausgaben.c"
#include "Einlesen.c"


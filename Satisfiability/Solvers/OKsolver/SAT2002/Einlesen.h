                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* angelegt: 4.2.2001 */


#ifndef GELESENEINLESEN

#include <stdio.h>


typedef enum {Norm = 0, Sat = 1, Unsat = 2, Fehler = 3} TEIN;

extern TEIN Einlesen(FILE *fp, unsigned int G);

extern void AufraeumenEinlesen(void);

extern void AusgabeBelegung(FILE *fp);

extern void setzenStandard(void);

#endif

#define GELESENEINLESEN = 1

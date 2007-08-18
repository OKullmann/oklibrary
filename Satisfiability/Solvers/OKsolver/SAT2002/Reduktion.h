                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Angelegt: 2.3.2001 */


#ifndef GELESENREDUKTION

extern char Reduktion1( void );

extern char Reduktion2( void );

/* der folgende Typ wird nur vom Modul "lokalesLernen" benoetigt: */

#ifndef BAUMRES
typedef LIT Stapeleintrag;
#else
typedef struct {
  LIT l;
  KLN k;
} Stapeleintrag;
#endif

#endif

#define GELESENREDUKTION = 1


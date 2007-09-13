// Oliver Kullmann, 2.3.2001 (Toronto)

/*!
  \file OKsolver/SAT2002/Reduktion.h
  \brief The interface for the reduction process
*/

                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Angelegt: 2.3.2001 */


#ifndef GELESENREDUKTION_4101Kj
#define GELESENREDUKTION_4101Kj

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

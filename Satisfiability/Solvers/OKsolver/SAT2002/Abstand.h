                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Angelegt: 6.2.2001 */

#ifndef GELESENABSTAND

/* -------------------------------------------------------------------------- */


extern float d0, d1;

extern unsigned int d0S, d1S; /* "sekundaer" */

extern void Abstand ( void );

#ifdef DYNAMISCH

extern void InitAbstand2( void );

#endif

extern const char *Abstandsname;

#endif

#define GELESENABSTAND 1


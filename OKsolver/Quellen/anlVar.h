                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* angelegt: 4.2.2001 */

#ifndef GELESENANLVAR

#include <stdlib.h>

#include "OK.h"
#include "VarLitKlm.h"
/* #include "Klauselmenge.h" */

extern void InitParameter( void );

extern size_t BedarfVarLitKlmV ( void );
extern size_t BedarfBelegungV ( void );
extern size_t BedarfRekursionV ( void );
extern size_t BedarfFilterV ( void );
extern size_t BedarfReduktionsV ( void );
extern size_t BedarfAbstandsV ( void );
extern size_t BedarfBaumResV ( void );

extern void *VarLitKlmV(void *Z);
extern void *BelegungV(void *);
extern void *RekursionV (void *);
extern void *FilterV (void *);
extern void *ReduktionsV (void *);
extern void *AbstandsV (void *);
extern void *BaumResV (void *);

#endif

#define GELESENANLVAR 1

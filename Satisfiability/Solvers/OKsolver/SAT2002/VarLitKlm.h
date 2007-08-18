                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Erzeugung: 14.3.2001 */


#ifndef GELESENVARLITKLM

#include "OK.h"

#ifdef BAUMRES
typedef unsigned long int Wort;
typedef Wort *ZWort;
typedef Wort *VarMenge;
typedef struct {
  ZWort Position;
  Wort Maske;
} VarMaske;
#endif

/* ------------------------------------------------- */


/* Die vier zentralen abstakten Datentypen VAR, LIT, KLN, LITV */

#ifdef LOKALLERNEN
typedef struct neuK *NKL;
#endif
typedef struct var *VAR;
typedef struct lit *LIT;
typedef struct litv *LITV;
typedef struct Klauselv *KLN;

struct Klauselv {
  KLL Laenge;
  KLL LaLaenge;
  /* LaLaenK[k] == 0 heisst, dass die Klausel schon erfuellt wurde. */
  unsigned int RundeK;
#ifdef BAUMRES
#ifndef LITTAB
  VarMenge VarM;
#else
  unsigned int UrLaenge;
  VarMaske *ersteV;
#endif
#endif
};

struct litv {
  struct litv *nLv;
  struct litv *lLv;
  struct litv *nLK;
  struct litv *lLK;
  struct lit *lit;
  struct Klauselv *kln;
};

#ifdef LOKALLERNEN

typedef struct neuK {
  struct neuK *naechstes;
  struct neuK *vorheriges;
  struct Klauselv Klausel;
  struct litv x1;
  struct litv x2;
#ifdef FASTAUTARKIE
  LIT l; /* falls l != NullLiteral, so ist dies das "Fast-Autarkie-Literal" */
#endif
};

const unsigned int sizeneuK = sizeof(neuK);

#endif

extern const LIT NullLiteral;


/* ------------------------------------------------- */

extern unsigned int *aktAnzK;
/* extern unsigned int aktAnzK [ MAXP + 1 ]; */
extern unsigned int *InitAnzK;

/* ------------------------------------------------- */

extern void InitVarLitKlm(void);

extern LIT Literal(VAR v, VZ e);
extern VAR Var(LIT l);
extern LIT Komp(LIT l);
#ifdef BAUMRES
extern ZWort VPosition(VAR v);
extern Wort VMaske(VAR v);
#endif
extern bool belegt(VAR v);
extern void setzenbelegt(VAR v, bool T);
extern char *Symbol(VAR v);

extern void loeseV(VAR v);
extern void bindeV(VAR v);

extern VAR ersteVar( void );
extern bool echteVar(VAR v);
extern VAR naechsteVar(VAR v);

unsigned int RundeL(LIT l);
void setzenRundeL(LIT l);
void NullsetzenRL( void );

#ifdef FASTAUTARKIE

bool Fastautarkie(LIT l);
void setzenFastautarkie(LIT l, bool T);

unsigned int RundeLFA(LIT l);
void setzenRundeLFA(LIT l);
void NullsetzenRLFA(void);

#endif

extern void loeseLv(LITV x);
extern void bindeLv(LITV x);
extern void loeseLK(LITV x);
extern void bindeLK(LITV x);

extern LITV erstesVork(LIT l);
extern bool echtesVork(LITV x, LIT l);
extern LITV naechstesVork(LITV x);

extern LITV naechstesVorkK(LITV x);

extern LIT LitVk(LITV x);
extern KLN KlnVk(LITV x);

extern KLL Laenge(KLN k);
extern void ZuwLaenge(KLN k, KLL m);
extern KLL LaengeM1(KLN k);
extern KLL LaengeP1(KLN k);
extern KLL LaLaenge(KLN k);
extern void ZuwLaLaenge(KLN k, KLL m);
extern KLL M1LaLaenge(KLN k);

extern unsigned int RundeK(KLN k);
extern void setzenRundeK(KLN k);
extern void NullsetzenRK( void );
#ifdef BAUMRES
#ifndef LITTAB
extern VarMenge VarK(KLN k);
#else
extern unsigned int UrLaenge(KLN k);
extern VarMaske *ersteVK(KLN k);
#endif
#endif

#ifdef LOKALLERNEN
extern void setzen0RundeK(KLN K);
#endif

extern void Klauselanfangen(void);
extern void Literaleintragen(int l);
extern void Klauselbeenden(void);
void Symboleintragen(unsigned int v, char *S);

void setzenerstesV(LIT l, LITV x);
void setzenLit(LITV x, LIT l);
void setzenKln(LITV x, KLN k);
void setzennLv(LITV x, LITV y);
void setzenlLv(LITV x, LITV y);
void setzennLK(LITV x, LITV y);
void setzenlLK(LITV x, LITV y);


#endif

#define GELESENVARLITKLM 1


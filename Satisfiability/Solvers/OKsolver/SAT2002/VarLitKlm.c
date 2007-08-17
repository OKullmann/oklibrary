                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Angelegt: 16.3.2001 */

/* Einheit: VarLitKlm */

#include <stdlib.h>
#include <string.h> /* fuer C++ (memset; 14.8.2001) */

#include "OK.h"
#include "Parameter.h"
#include "VarLitKlm.h"
#ifdef LOKALLERNEN
#include "lokalesLernen.h"
#endif


struct var {
  struct lit *pos;
  struct lit *neg;
  struct var *vor;
  struct var *nae;
  bool belegt;
  char *Symbol;
#ifdef BAUMRES
  ZWort Position;
  Wort Maske;
#endif
};

struct lit {
  struct lit *Komp;
  struct litv *erstes;
  struct var *Var;
  unsigned int Runde;
#ifdef FASTAUTARKIE
  bool schonFA;
  unsigned int RundeFA;
#endif
};


const LIT NullLiteral = NULL;

static VAR AnkerVar;
static LIT erstesLiteral;
static LITV F;
static KLN FK;

unsigned int *aktAnzK; /* unsigned int aktAnzK [ MAXP + 1 ]; */
unsigned int *InitAnzK; // zur Speicherung der initialen Klauselnanzahlen im Falle, dass Format = XLM_Format

#ifdef BAUMRES

#ifndef LITTAB
static ZWort VarKl;
#else
static VarMaske *MaskenKl;
#endif

#endif



/* -------------------------------------------------------------------------- */

__inline__ LIT Literal(VAR v, VZ e)
{
  switch (e) {
  case Pos : return v -> pos;
  case Neg : return v -> neg;
  }
}

__inline__ VAR Var(LIT l)
{
  return l -> Var;
}

__inline__ LIT Komp(LIT l)
{
  return l -> Komp;
}

#ifdef BAUMRES
__inline__ ZWort VPosition(VAR v)
{
  return v -> Position;
}

__inline__ Wort VMaske(VAR v)
{
  return v -> Maske;
}
#endif

bool belegt(VAR v)
{
  return v -> belegt;
}

void setzenbelegt(VAR v, bool T)
{
  v -> belegt = T;
}

char *Symbol(VAR v)
{
  return v -> Symbol;
}

/* ---------------------------------- */

__inline__ void loeseV(VAR v)
{
  (v -> vor -> nae = v -> nae) -> vor = v -> vor;
}

__inline__ void bindeV(VAR v)
{
  v -> nae -> vor = v -> vor -> nae = v;
}

/* ---------------------------------- */

__inline__ VAR ersteVar( void )
{
  return AnkerVar -> nae;
}

__inline__ bool echteVar(VAR v)
{
  return (v != AnkerVar);
}

__inline__ VAR naechsteVar(VAR v)
{
  return v -> nae;
}

/* ---------------------------------- */

__inline__ unsigned int RundeL(LIT l)
{
  return l -> Runde;
}

__inline__ void setzenRundeL(LIT l)
{
  extern unsigned int Runde;
  l -> Runde = Runde;
}

__inline__ void NullsetzenRL(void)
{
  LIT l; unsigned int i;
  for (l = erstesLiteral, i = 0; i < 2 * N; l++, i++)
    l -> Runde = 0;
}


/* ---------------------------------- */

#ifdef FASTAUTARKIE

__inline__ bool Fastautarkie(LIT l)
{
  return l -> schonFA;
}

__inline__ void setzenFastautarkie(LIT l, bool T)
{
  l -> schonFA = T;
}

__inline__ unsigned int RundeLFA(LIT l)
{
  return l -> RundeFA;
}

__inline__ void setzenRundeLFA(LIT l)
{
  extern unsigned int RundeFA;
  l -> RundeFA = RundeFA;
}

__inline__ void NullsetzenRLFA(void)
{
  LIT l; unsigned int i;
  for (l = erstesLiteral, i = 0; i < 2 * N; l++, i++)
    l -> RundeFA = 0;
}

#endif

/* ---------------------------------- */

__inline__ void loeseLv(LITV x)
{
  if (x -> lLv == NULL)
    x -> lit -> erstes = x -> nLv;
  else
    x -> lLv -> nLv = x -> nLv;
  if (x -> nLv != NULL)
    x -> nLv -> lLv = x -> lLv;
}

__inline__ void bindeLv(LITV x)
{
  if (x -> lLv == NULL)
    x -> lit -> erstes = x;
  else
    x -> lLv -> nLv = x;
  if (x -> nLv != NULL)
    x -> nLv -> lLv = x;
}

__inline__ void loeseLK(LITV x)
{
    ( ((x -> lLK) -> nLK) = (x -> nLK) ) -> lLK = x -> lLK;
}

__inline__ void bindeLK(LITV x)
{
   (x -> nLK) -> lLK = (x -> lLK) -> nLK = x;
}

/* ---------------------------------- */

__inline__ LITV erstesVork(LIT l)
{
  return l -> erstes;
}


__inline__ bool echtesVork(LITV x, LIT l)
{
  return (x != NULL);
}


__inline__ LITV naechstesVork(LITV x)
{
  return x -> nLv;
}

/* ---------------------------------- */

__inline__ LITV naechstesVorkK(LITV x)
{
  return x -> nLK;
}

/* ---------------------------------- */

__inline__ LIT LitVk(LITV x)
{
  return x -> lit;
}

__inline__ KLN KlnVk(LITV x)
{
  return x -> kln;
}

/* ---------------------------------- */

__inline__ KLL Laenge(KLN k)
{
  return k -> Laenge;
}

__inline__ void ZuwLaenge(KLN k, KLL m)
{
  k -> Laenge = m;
}

__inline__ KLL LaengeM1(KLN k)
{
  return (k -> Laenge)--;
}

__inline__ KLL LaengeP1(KLN k)
{
  return (k -> Laenge)++;
}

__inline__ KLL LaLaenge(KLN k)
{
  return k -> LaLaenge;
}

__inline__ void ZuwLaLaenge(KLN k, KLL m)
{
  k -> LaLaenge = m;
}

__inline__ KLL M1LaLaenge(KLN k)
{
  return --(k -> LaLaenge);
}

/* ---------------------------------- */

__inline__ unsigned int RundeK(KLN k)
{
  return k -> RundeK;
}

__inline__ void setzenRundeK(KLN k)
{
  extern unsigned int Runde;
  k -> RundeK = Runde;
}

__inline__ void NullsetzenRK( void )
{
  unsigned int k; KLN kn;
  for (kn = FK, k = 0; k < K; k++, kn++)
    kn -> RundeK = 0;
#ifdef LOKALLERNEN
  NullsetzenRKNK();
#endif
}

#ifdef LOKALLERNEN
__inline__ void setzen0RundeK(KLN k)
{
  k -> RundeK = 0;
}
#endif

/* ---------------------------------- */

#ifdef BAUMRES

#ifndef LITTAB

__inline__ VarMenge VarK(KLN k)
{
  return k -> VarM;
}

#else

__inline__ void ZuwUrLaenge(KLN k, unsigned int m)
{
  k -> UrLaenge = m;
}

__inline__ void ZuwersteVK(KLN k, VarMaske *p)
{
  k -> ersteV = p;
}


__inline__ unsigned int UrLaenge(KLN k)
{
  return k -> UrLaenge;
}

__inline__ VarMaske *ersteVK(KLN k)
{
  return k -> ersteV;
}


#endif

#endif

/* ---------------------------------- */

static LITV aktLitV;
static LITV aktLitV0;
static KLN aktKln;
#ifdef BAUMRES
#ifdef LITTAB
static VarMaske *aktMaske;
#endif
#endif
static unsigned int aktKlLaenge;

__inline__ void Klauselanfangen(void)
{
  aktKlLaenge = 0;
  aktLitV -> lLK = aktLitV -> nLK = aktLitV;
#ifdef BAUMRES
#ifdef LITTAB
  aktKln -> ersteV = aktMaske;
#endif
#endif
}

__inline__ void Literaleintragen(int l)
{
  unsigned int vi; VZ e; LIT a; VAR v;
  aktKlLaenge++;
  if (l > 0)
    {
      vi = l; e = Pos;
    }
  else
    {
      vi = -l; e = Neg;
    }
  v = AnkerVar + vi;
  a = Literal(v, e);
  aktLitV -> lit = a;
  aktLitV -> kln = aktKln;

  aktLitV -> lLv = NULL;
  aktLitV -> nLv = a -> erstes;
  if (a -> erstes != NULL)
    a -> erstes -> lLv = aktLitV;
  a -> erstes = aktLitV;

  if (aktKlLaenge > 1)
    {
      aktLitV -> nLK = aktLitV0 -> nLK;
      aktLitV -> lLK = aktLitV0;
      aktLitV0 -> nLK -> lLK = aktLitV0 -> nLK = aktLitV;
    }
  aktLitV0 = aktLitV++;
#ifdef BAUMRES
#ifdef LITTAB
  aktMaske -> Position = VPosition(v);
  aktMaske -> Maske = VMaske(v);
  aktMaske++;
#endif
#endif
  return;
}
  
__inline__ void Klauselbeenden(void)
{
  aktKln -> Laenge = aktKlLaenge;
#ifdef BAUMRES
#ifdef LITTAB
  aktKln -> UrLaenge = aktKlLaenge;
#endif
#endif
  aktAnzK[aktKlLaenge]++;
  aktKln++;
}

__inline__ void Symboleintragen(unsigned int v, char *S)
{
  (AnkerVar + v) -> Symbol  = S;
}


/* ---------------------------------- */

__inline__ void setzenerstesV(LIT l, LITV x)
{
  l -> erstes = x;
}

__inline__ void setzenLit(LITV x, LIT l)
{
  x -> lit = l;
}

__inline__ void setzenKln(LITV x, KLN k)
{
  x -> kln = k;
}

__inline__ void setzennLv(LITV x, LITV y)
{
  x -> nLv = y;
}

__inline__ void setzenlLv(LITV x, LITV y)
{
  x -> lLv = y;
}

__inline__ void setzennLK(LITV x, LITV y)
{
  x -> nLK = y;
}

__inline__ void setzenlLK(LITV x, LITV y)
{
  x -> lLK = y;
}

/* ---------------------------------- */

size_t BedarfVarLitKlmV( void )
{
  extern Ausgabeformat Format;
  return (N + 1) * sizeof(struct var) +
    (2 * N) * sizeof(struct lit) +
    L * sizeof(struct litv) +
    K * sizeof(struct Klauselv) +
    (P + 1) * sizeof(unsigned int) +
    ((Format == XML_Format) ? (P + 1) * sizeof(unsigned int) : 0)
#ifdef BAUMRES
#ifndef LITTAB
    + K * GroesseVarMenge
#else
    + L * sizeof(VarMaske)
#endif
#endif
    ;
 }

void *VarLitKlmV(void *Z)
{
  AnkerVar = (VAR) Z; Z = (void *) (AnkerVar + N + 1);
  erstesLiteral = (LIT) Z; Z = (void *) (erstesLiteral + 2 * N);
  aktLitV = F = (LITV) Z;  Z = (void *) (F + L);
  aktKln = FK = (KLN) Z; Z = (void *) (FK + K);
  aktAnzK = (unsigned int *) Z; Z = (void *) (aktAnzK + P + 1);
  extern Ausgabeformat Format;
  if (Format == XML_Format) {
    InitAnzK = (unsigned int *) Z; Z = (void *) (InitAnzK + P + 1);
  }
#ifdef BAUMRES
#ifndef LITTAB
  VarKl = (ZWort) Z; Z = (void *) (VarKl + K * ANZWORTE);
#else
  aktMaske = MaskenKl = (VarMaske *) Z; Z = (void *) (MaskenKl + L);
#endif
#endif

  {
    VAR v0, v;
    LIT l;
    unsigned int i;
    v0 = v = AnkerVar;
    l = erstesLiteral;
    for (i = 0; i < N; i++)
      {
	v++;
	v0 -> nae = v;
	v -> vor = v0;
#ifdef BAUMRES
	{
	  extern VarMenge aktrelV;
	  div_t q;
	  q  = div(i, BITS);
	  v -> Position = aktrelV + q.quot;
	  v -> Maske = 1UL << q.rem;
	}
#endif
	v -> pos = l;

	l -> erstes = NULL;
	l -> Var = v;
	l -> Runde = 0;
#ifdef FASTAUTARKIE
	l -> schonFA = false;
	l -> RundeFA = 0;
#endif
	v -> neg = l -> Komp = l + 1;
	l++;

	l -> erstes = NULL;
	l -> Var = v;
	l -> Runde = 0;
#ifdef FASTAUTARKIE
	l -> schonFA = false;
	l -> RundeFA = 0;
#endif
	l -> Komp = l - 1;
	l++;
	
	v0 = v;
      }
    v -> nae = AnkerVar;
    AnkerVar -> vor = v;
  }
  {
    unsigned int k; KLN kn;
    for (kn = FK, k = 0; k < K; k++, kn++)
      kn -> RundeK = 0;
  }
#ifdef BAUMRES
#ifndef LITTAB
  {
    unsigned int i; KLN k; ZWort M;
    memset((void *) VarKl, 0, K * GroesseVarMenge);
    for (i = 0, k = FK, M = VarKl; i < K; i++, k++, M += ANZWORTE)
      k -> VarM = M;
  }
#endif
#endif
  memset(aktAnzK, 0, (P+1) * sizeof(unsigned int));
  return Z;
}

void InitVarLitKlm(void)
{
#ifdef BAUMRES
#ifndef LITTAB
  {
    extern VarMenge aktrelV;
    LITV x; VAR v; VarMenge M; unsigned int i;
    for (x = F, i = 0; i < L; x++, i++)
    {
      v = Var(LitVk(x)); M = VarK(KlnVk(x));
      *(M + (VPosition(v) - aktrelV)) |= VMaske(v);
    }
  }
#endif
#endif
  return;
}


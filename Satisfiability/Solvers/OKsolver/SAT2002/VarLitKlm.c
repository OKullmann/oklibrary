// Oliver Kullmann, 16.3.2001 (Toronto)
/* Copyright 2001 - 2007, 2009, 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Solvers/OKsolver/SAT2002/VarLitKlm.c
  \brief Implementations related to variables, literals, and clauses

  Implementations of the types and functions
  representing the abstract data type for variables, literals,
  literal occurrences and clause-information-nodes; also
  initialisation functions.
*/

#include <cstdlib>
#include <string.h> /* fuer C++ (memset; 14.8.2001) */
#include <assert.h>
#include <limits.h>

#include "OK.h"
#include "Parameter.h"
#include "VarLitKlm.h"
#ifdef LOKALLERNEN
#include "lokalesLernen.h"
#endif


struct var {
  struct lit* pos;
  struct lit* neg;
  struct var* vor;
  struct var* nae;
  bool belegt;
  const char* Symbol;
#ifdef BAUMRES
  ZWort Position;
  Wort Maske;
#endif
};

struct lit {
  struct lit* Komp;
  struct litv* erstes;
  struct var* Var;
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

unsigned int* aktAnzK; /* unsigned int aktAnzK [ MAXP + 1 ]; */
unsigned int* InitAnzK; // zur Speicherung der initialen Klauselnanzahlen im Falle, dass Format = XLM_Format

#ifdef BAUMRES
# ifndef LITTAB
static ZWort VarKl;
# else
static VarMaske *MaskenKl;
# endif
#endif



/* -------------------------------------------------------------------------- */

__inline__ LIT Literal(const VAR v, const VZ e) {
  assert(v);
  switch (e) {
  case Pos : return v -> pos;
  case Neg : return v -> neg;
  default :
    assert(0);
  }
}

__inline__ VAR Var(const LIT l) {
  assert(l);
  return l -> Var;
}

__inline__ LIT Komp(const LIT l) {
  assert(l);
  return l -> Komp;
}

#ifdef BAUMRES
__inline__ ZWort VPosition(const VAR v) {
  assert(v);
  return v -> Position;
}

__inline__ Wort VMaske(const VAR v) {
  assert(v);
  return v -> Maske;
}
#endif

bool belegt(const VAR v) {
  assert(v);
  return v -> belegt;
}

void setzenbelegt(const VAR v, const bool T) {
  assert(v);
  v -> belegt = T;
}

const char* Symbol(const VAR v) {
  assert(v);
  assert(v -> Symbol);
  return v -> Symbol;
}

/* ---------------------------------- */

__inline__ void loeseV(const VAR v) {
  assert(v);
  assert(v -> vor);
  assert(v -> nae);
  (v -> vor -> nae = v -> nae) -> vor = v -> vor;
}

__inline__ void bindeV(const VAR v) {
  assert(v);
  assert(v -> vor);
  assert(v -> nae);
  v -> nae -> vor = v -> vor -> nae = v;
}

/* ---------------------------------- */

__inline__ VAR ersteVar( void ) {
  assert(AnkerVar);
  return AnkerVar -> nae;
}

__inline__ bool echteVar(const VAR v) {
  assert(v);
  return (v != AnkerVar);
}

__inline__ VAR naechsteVar(const VAR v) {
  assert(v);
  return v -> nae;
}

/* ---------------------------------- */

__inline__ unsigned int RundeL(const LIT l) {
  assert(l);
  return l -> Runde;
}

__inline__ void setzenRundeL(const LIT l) {
  extern unsigned int Runde;
  assert(l);
  l -> Runde = Runde;
}

__inline__ void NullsetzenRL(void)
{
  LIT l; unsigned int i;
  for (l = erstesLiteral, i = 0; i < 2 * N; l++, i++) {
    assert(l);
    l -> Runde = 0;
  }
}


/* ---------------------------------- */

#ifdef FASTAUTARKIE

__inline__ bool Fastautarkie(const LIT l) {
  assert(l);
  return l -> schonFA;
}

__inline__ void setzenFastautarkie(const LIT l, const bool T) {
  assert(l);
  l -> schonFA = T;
}

__inline__ unsigned int RundeLFA(const LIT l) {
  assert(l);
  return l -> RundeFA;
}

__inline__ void setzenRundeLFA(const LIT l) {
  extern unsigned int RundeFA;
  assert(l);
  l -> RundeFA = RundeFA;
}

__inline__ void NullsetzenRLFA(void) {
  LIT l = erstesLiteral;
  for (unsigned int i = 0; i < 2 * N; l++, i++) {
    assert(l);
    l -> RundeFA = 0;
  }
}

#endif

/* ---------------------------------- */

__inline__ void loeseLv(const LITV x) {
  assert(x);
  if (! (x -> lLv)) {
    assert(x -> lit);
    x -> lit -> erstes = x -> nLv;
  }
  else {
    assert(x -> lLv);
    x -> lLv -> nLv = x -> nLv;
  }
  if (x -> nLv)
    x -> nLv -> lLv = x -> lLv;
}

__inline__ void bindeLv(const LITV x) {
  assert(x);
  if (! (x -> lLv)) {
    assert(x -> lit);
    x -> lit -> erstes = x;
  }
  else {
    assert(x -> lLv);
    x -> lLv -> nLv = x;
  }
  if (x -> nLv)
    x -> nLv -> lLv = x;
}

__inline__ void loeseLK(const LITV x) {
  assert(x);
  assert(x -> nLK);
  assert(x -> lLK);
  ( ((x -> lLK) -> nLK) = (x -> nLK) ) -> lLK = x -> lLK;
}

__inline__ void bindeLK(const LITV x) {
  assert(x);
  assert(x -> lLK);
  assert(x -> nLK);
  (x -> nLK) -> lLK = (x -> lLK) -> nLK = x;
}

/* ---------------------------------- */

__inline__ LITV erstesVork(const LIT l) {
  assert(l);
  return l -> erstes;
}


__inline__ bool echtesVork(const LITV x, const LIT) {
  return (x != NULL);
}


__inline__ LITV naechstesVork(const LITV x) {
  assert(x);
  return x -> nLv;
}

/* ---------------------------------- */

__inline__ LITV naechstesVorkK(const LITV x) {
  assert(x);
  return x -> nLK;
}

/* ---------------------------------- */

__inline__ LIT LitVk(const LITV x) {
  assert(x);
  return x -> lit;
}

__inline__ KLN KlnVk(const LITV x) {
  assert(x);
  return x -> kln;
}

/* ---------------------------------- */

__inline__ KLL Laenge(const KLN k) {
  assert(k);
  return k -> Laenge;
}

__inline__ void ZuwLaenge(const KLN k, const KLL m) {
  assert(k);
  k -> Laenge = m;
}

__inline__ KLL LaengeM1(const KLN k) {
  assert(k);
  return (k -> Laenge)--;
}

__inline__ KLL LaengeP1(KLN k) {
  assert(k);
  return (k -> Laenge)++;
}

__inline__ KLL LaLaenge(const KLN k) {
  assert(k);
  return k -> LaLaenge;
}

__inline__ void ZuwLaLaenge(const KLN k, const KLL m) {
  assert(k);
  k -> LaLaenge = m;
}

__inline__ KLL M1LaLaenge(const KLN k) {
  assert(k);
  return --(k -> LaLaenge);
}

/* ---------------------------------- */

__inline__ unsigned int RundeK(const KLN k) {
  assert(k);
  return k -> RundeK;
}

__inline__ void setzenRundeK(const KLN k) {
  extern unsigned int Runde;
  assert(k);
  k -> RundeK = Runde;
}

__inline__ void NullsetzenRK( void ) {
  KLN kn = FK;
  for (unsigned int k = 0; k < K; ++k, ++kn) {
    assert(kn);
    kn -> RundeK = 0;
  }
#ifdef LOKALLERNEN
  NullsetzenRKNK();
#endif
}

#ifdef LOKALLERNEN
__inline__ void setzen0RundeK(const KLN k) {
  assert(k);
  k -> RundeK = 0;
}
#endif

/* ---------------------------------- */

#ifdef BAUMRES

# ifndef LITTAB

__inline__ VarMenge VarK(const KLN k) {
  assert(k);
  return k -> VarM;
}

#else

__inline__ void ZuwUrLaenge(const KLN k, const unsigned int m) {
  assert(k);
  k -> UrLaenge = m;
}

__inline__ void ZuwersteVK(const KLN k, VarMaske* const p) {
  assert(k);
  k -> ersteV = p;
}


__inline__ unsigned int UrLaenge(const KLN k) {
  assert(k);
  return k -> UrLaenge;
}

__inline__ VarMaske* ersteVK(const KLN k) {
  assert(k);
  return k -> ersteV;
}

# endif

#endif

/* ---------------------------------- */

static LITV aktLitV;
static LITV aktLitV0;
static KLN aktKln;
#ifdef BAUMRES
# ifdef LITTAB
static VarMaske* aktMaske;
# endif
#endif
static unsigned int aktKlLaenge;

__inline__ void Klauselanfangen(void) {
  aktKlLaenge = 0;
  assert(aktLitV);
  aktLitV -> lLK = aktLitV -> nLK = aktLitV;
#ifdef BAUMRES
# ifdef LITTAB
  assert(aktKln);
  aktKln -> ersteV = aktMaske;
# endif
#endif
}

/*!
  \brief Adding literal l to the current clause
*/
__inline__ void Literaleintragen(const int l) {
  ++aktKlLaenge;
  assert(l > INT_MIN);
  const VAR v = AnkerVar + abs(l);
  {
    const LIT a = Literal(v, (l > 0) ? Pos : Neg);
    assert(a);
    assert(aktLitV);
    aktLitV -> lit = a;
    aktLitV -> kln = aktKln;
    aktLitV -> lLv = NULL;
    aktLitV -> nLv = a -> erstes;
    if (a -> erstes)
      a -> erstes -> lLv = aktLitV;
    a -> erstes = aktLitV;
  }
  if (aktKlLaenge > 1) {
    // before: x <-> aktLitV0 <-> first
    // after: x <-> aktLitV0 <-> aktLitV <-> first
    assert(aktLitV0);
    assert(aktLitV == aktLitV0+1);
    aktLitV -> nLK = aktLitV0 -> nLK;
    aktLitV -> lLK = aktLitV0;
    aktLitV0 -> nLK = aktLitV;
    aktLitV -> nLK -> lLK = aktLitV;
  }
  aktLitV0 = aktLitV++;
#ifdef BAUMRES
# ifdef LITTAB
  assert(aktMaske);
  aktMaske -> Position = VPosition(v);
  aktMaske -> Maske = VMaske(v);
  ++aktMaske;
# endif
#endif
}
  
__inline__ void Klauselbeenden(void) {
  assert(aktKln);
  aktKln -> Laenge = aktKlLaenge;
#ifdef BAUMRES
# ifdef LITTAB
  aktKln -> UrLaenge = aktKlLaenge;
# endif
#endif
  // assert(aktKlLaenge <= MAXP); ???
  ++aktAnzK[aktKlLaenge];
  ++aktKln;
}

__inline__ void Symboleintragen(const unsigned int v, const char* const S) {
  assert(AnkerVar + v);
  (AnkerVar + v) -> Symbol  = S;
}


/* ---------------------------------- */

__inline__ void setzenerstesV(const LIT l, const LITV x) {
  assert(l);
  assert(x);
  l -> erstes = x;
}

__inline__ void setzenLit(const LITV x, const LIT l) {
  assert(x);
  assert(l);
  x -> lit = l;
}

__inline__ void setzenKln(const LITV x, const KLN k) {
  assert(x);
  assert(k);
  x -> kln = k;
}

__inline__ void setzennLv(const LITV x, const LITV y) {
  assert(x);
  assert(y);
  x -> nLv = y;
}

__inline__ void setzenlLv(const LITV x, const LITV y) {
  assert(x);
  assert(y);
  x -> lLv = y;
}

__inline__ void setzennLK(const LITV x, const LITV y) {
  assert(x);
  assert(y);
  x -> nLK = y;
}

__inline__ void setzenlLK(const LITV x, const LITV y) {
  assert(x);
  assert(y);
  x -> lLK = y;
}

/* ---------------------------------- */

size_t BedarfVarLitKlmV( void ) {
  extern enum Ausgabeformat Format;
  return (N + 1) * sizeof(struct var) +
    (2 * N) * sizeof(struct lit) +
    L * sizeof(struct litv) +
    K * sizeof(struct Klauselv) +
    (P + 1) * sizeof(unsigned int) +
    ((Format == XML_Format) ? (P + 1) * sizeof(unsigned int) : 0)
#ifdef BAUMRES
# ifndef LITTAB
    + K * GroesseVarMenge
# else
    + L * sizeof(VarMaske)
# endif
#endif
    ;
 }

/*!
  \brief Initialisation of the datastructure representing variables, literals and clauses-nodes
*/
void* VarLitKlmV(void* Z) {
  AnkerVar = (VAR) Z; Z = (void *) (AnkerVar + N + 1);
  erstesLiteral = (LIT) Z; Z = (void *) (erstesLiteral + 2 * N);
  aktLitV = F = (LITV) Z;  Z = (void *) (F + L);
  aktKln = FK = (KLN) Z; Z = (void *) (FK + K);
  aktAnzK = (unsigned int *) Z; Z = (void *) (aktAnzK + P + 1);
  extern enum Ausgabeformat Format;
  if (Format == XML_Format) {
    InitAnzK = (unsigned int *) Z; Z = (void *) (InitAnzK + P + 1);
  }
#ifdef BAUMRES
# ifndef LITTAB
  VarKl = (ZWort) Z; Z = (void *) (VarKl + K * ANZWORTE);
# else
  aktMaske = MaskenKl = (VarMaske *) Z; Z = (void *) (MaskenKl + L);
# endif
#endif

  { // Initialisation of variables and literals
    assert(AnkerVar);
    VAR v0 = AnkerVar, v = AnkerVar;
    assert(erstesLiteral);
    LIT l = erstesLiteral;
    for (unsigned int i = 0; i < N; ++i)
      {
	++v;
	v0 -> nae = v;
	v -> vor = v0;
        v -> belegt = false;
#ifdef BAUMRES
	{
	  const std::div_t q = std::div(int(i), int(BITS));
	  extern VarMenge aktrelV;
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
	++l;

	l -> erstes = NULL;
	l -> Var = v;
	l -> Runde = 0;
#ifdef FASTAUTARKIE
	l -> schonFA = false;
	l -> RundeFA = 0;
#endif
	l -> Komp = l - 1;
	++l;
	
	v0 = v;
      }
    v -> nae = AnkerVar;
    AnkerVar -> vor = v;
  }
  { // Initialisation of clause-nodes
    assert(FK);
    KLN kn = FK;
    for (unsigned int k = 0; k < K; ++k, ++kn)
      kn -> RundeK = 0;
  }
#ifdef BAUMRES
# ifndef LITTAB
  {
    memset((void *) VarKl, 0, K * GroesseVarMenge);
    assert(FK);
    KLN k = FK; ZWort M = VarKl;
    for (unsigned int i = 0; i < K; i++, k++, M += ANZWORTE)
      k -> VarM = M;
  }
# endif
#endif
  memset(aktAnzK, 0, (P+1) * sizeof(unsigned int));
  assert(Z);
  return Z;
}

void InitVarLitKlm(void) {
#ifdef BAUMRES
# ifndef LITTAB
  extern VarMenge aktrelV;
  LITV x = F;
  for (unsigned int i = 0; i < L; ++x, ++i) {
    const VAR v = Var(LitVk(x)); const VarMenge M = VarK(KlnVk(x));
    assert(M + (VPosition(v) - aktrelV));
    *(M + (VPosition(v) - aktrelV)) |= VMaske(v);
  }
# endif
#endif
  return;
}


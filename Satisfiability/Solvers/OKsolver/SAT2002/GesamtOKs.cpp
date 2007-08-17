                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Erzeugung: 8.2.2001 */

/* Einheit: Parameter */

#include "OK.h"
#include "Parameter.h"

/* ------------------------------------------------------------------------- */

#ifdef BAUMRES

unsigned int ANZWORTE;
/* Anzahl benoetigter Worte zur Darstellung von Mengen von Var. */

unsigned int GroesseVarMenge;

#endif


/* Ferner wird noch die Anzahl der Ankerplaetze benoetigt: */

/* unsigned int AnzahlAnker; */

/* Und die Position von F innerhalb der Liste der Ankerelemente: */

/* unsigned int PositionAnker; */




/* ------------------------------------------------------------------------- */

void InitParameter( void )
/* benoetigt nur N */
{
/*   AnzahlAnker = 2 * N + 1; */
/*   PositionAnker = N; */

#ifdef BAUMRES
  ANZWORTE = (N - 1) / ((int) BITS) + 1;
  GroesseVarMenge = ANZWORTE * sizeof(Wort);
#endif
  return;
}



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

                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */

/* Angelegt: 16.3.2001 */

/* Einheit: BaumRes */

#include <stdlib.h>
#include <string.h> /* fuer C++ (memset; 14.8.2001) */

#include "OK.h"
#include "VarLitKlm.h"
#include "Parameter.h"
#include "Speicher.h"
#include "Belegung.h"


#ifdef BAUMRES

/* ------------------------------------------------------------------------- */


/* Variablen */

VarMenge aktrelV;
VarMenge aktrelV0;
/* Die Implementation verlangt, dass unmittelbar nach aktrelV die */
/* Variable aktrelV0 ansetzt. */


/* ------------------------------------------------------------------------- */


size_t BedarfBaumResV( void )
{
  return 2 * GroesseVarMenge;
}

void *BaumResV(void *Z)
{
  aktrelV = (VarMenge) Z; Z = (void *) (aktrelV + ANZWORTE);
  aktrelV0 = (VarMenge) Z; Z = (void *) (aktrelV0 + ANZWORTE);
  return Z;
}


void AufraeumenBaumRes( void )
{
  Pfadinfo *Z; unsigned int i;
  if (Pfad != NULL)
    {
      for (Z = Pfad, i = 0; i < N; Z++, i++)
	free(Z -> M);
      Pfad = NULL;
    }
}


/* ------------------------------------------------------------------------- */



__inline__ bool enthalten (VAR v)
{
  return ((*(VPosition(v)) & VMaske(v)) != 0UL);
}



__inline__ static void hinzufuegen (VarMenge M)
{
  ZWort Z;
  for (Z = aktrelV; Z != aktrelV0; Z++, M++)
      *Z |= *M;
}



__inline__ void hinzufuegenKl (KLN k)
{
#ifndef LITTAB
  hinzufuegen(VarK(k));
#else
  VarMaske *z; unsigned int i;
  for (z = ersteVK(k), i = UrLaenge(k); i > 0; z++, i--)
    *(z -> Position) |= (z -> Maske);
#endif
}



__inline__ void setzenKl (KLN k)
{
#ifndef LITTAB
  memcpy((void *) aktrelV, (void *)(VarK(k)), GroesseVarMenge);
#else
  memset((void *) aktrelV, 0, GroesseVarMenge);
  hinzufuegenKl(k);
#endif
}



__inline__ void relVhinzufuegen (void)
{
  KLN k;
  k = Tiefe -> k;
  if (k != NULL)
    hinzufuegenKl(k);
  else
    hinzufuegen(Tiefe -> M);
}

__inline__ void relVMhinzufuegen (void)
{
  hinzufuegen(Tiefe -> M);
}


/* die naechste Operation ist wohl die am haeufigsten benutzte */

__inline__ void Kln_eintragen_relV(KLN k)
{
  Tiefe -> k = k;
}



__inline__ void aktV_eintragen_relV(void)
{
  if (Tiefe -> M == NULL)
    Tiefe -> M = (VarMenge) xmalloc(GroesseVarMenge);
  memcpy((void *)(Tiefe -> M), (void *) aktrelV, GroesseVarMenge);
}
  

__inline__ void aktV_volleintragen_relV(void)
{
  Tiefe -> k = NULL;
  if (Tiefe -> M == NULL)
    Tiefe -> M = (VarMenge) xmalloc(GroesseVarMenge);
  memcpy((void *)(Tiefe -> M), (void *) aktrelV, GroesseVarMenge);
}


__inline__ void aktV_speichern( void )
{
  memcpy((void *) aktrelV0, (void *) aktrelV, GroesseVarMenge);
}

__inline__ void hinzufuegenS ( void )
{
  hinzufuegen(aktrelV0);
}


#endif


                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Erzeugung: 3.2.2001 */

/* Einheit: Belegung */

#include <stdlib.h>

#include "OK.h"
#include "VarLitKlm.h"
#include "BaumRes.h"
#include "Belegung.h"

/* -------------------------------------------------------------------------------- */

Pfadinfo *Tiefe;

Pfadinfo *Pfad = NULL; 
/* "= NULL" ist zur Information fuer BaumRes, dass Pfad noch nicht angelegt */
/* wurde; */


/* -------------------------------------------------------------------------------- */

size_t BedarfBelegungV( void )
{
  return N * sizeof(Pfadinfo);
}

void *BelegungV(void *Z)
{
  Pfad = (Pfadinfo *) Z; Z = (void *) (Pfad + N);
  Tiefe = Pfad;
#ifdef BAUMRES
  {
    Pfadinfo *z; unsigned int i;
    for (i = 0, z = Pfad; i < N; i++, z++)
      z -> M = NULL;
  }
#endif
  return Z;
}

  
/* -------------------------------------------------------------------------------- */

__inline__ LIT PfadLit(void)
{
#ifndef BAUMRES
  return *Tiefe;
#else
  return Tiefe -> l;
#endif
}

__inline__ void LiteinPfad(LIT x)
{
#ifndef BAUMRES
  *Tiefe = x;
#else
  Tiefe -> l = x;
#endif
}

/* -------------------------------------------------------------------------------- */


/* Funktion zur Belegung einer Variablen */

__inline__ void belege(LIT x)

/* Fuehrt x -> 0 durch */
/* (mit Aktualisierung von "LaenK" und Eintrag in "Pfad" und "belegt"); */

/* Vorbedingungen:  Es entsteht nicht die leere Klausel. */


{
  LITV y, z;
  LIT kx;
  VAR v;

  /* entferne alle x-Vorkommen aus ihren Klauseln */

  for (y = erstesVork(x); echtesVork(y, x); y = naechstesVork(y))
    {
      loeseLK(y);
      LaengeM1(KlnVk(y));
    }

  /* fuer Klauseln C mit (non x) in C entferne alle anderen y in C aus */
  /* ihren Vorkommenslisten */

  for (y = erstesVork(kx = Komp(x)); echtesVork(y, kx); y = naechstesVork(y))
    for (z = naechstesVorkK(y); z != y; z = naechstesVorkK(z))
      loeseLv(z);

  LiteinPfad(x); Tiefe++;
  loeseV(v = Var(x));
  setzenbelegt(v, true);
}


#ifdef BAUMRES

__inline__ void belege_VK(LIT x, KLN K)

/* Hier wird noch die fuer die 1-Klauseln-Erzeugung relevante Klausel */
/* in relVar eingetragen. */
/* "belege_VK" wird nicht fuer die Verzweigungsvariable verwendet. */

{
  LITV y, z;
  LIT kx;
  VAR v;

  /* entferne alle x-Vorkommen aus ihren Klauseln */

  for (y = erstesVork(x); echtesVork(y, x); y = naechstesVork(y))
    {
      loeseLK(y);
      LaengeM1(KlnVk(y));
    }

  /* fuer Klauseln C mit (non x) in C entferne alle anderen y in C aus */
  /* ihren Vorkommenslisten */

  for (y = erstesVork(kx = Komp(x)); echtesVork(y, kx); y = naechstesVork(y))
    for (z = naechstesVorkK(y); z != y; z = naechstesVorkK(z))
      loeseLv(z);

  LiteinPfad(x);
  loeseV(v = Var(x));
  setzenbelegt(v, true);

  Kln_eintragen_relV(K);
  Tiefe++;
}

#endif


/* -------------------------------------------------------------------------------- */

/* Funktion zur Umkehrung einer Belegung */


__inline__ void rebelege(LIT x)

/* Macht x -> 0 wieder rueckgaengig */
/* ("belegt" wird wieder rueckgesetzt). */
/* Ist BAUMRES gesetzt, so wird aktrelV aktualisiert. */

{
  LITV y, z;
  LIT kx;
  VAR v;

  for (y = erstesVork(x); echtesVork(y, x); y = naechstesVork(y))
    {
      bindeLK(y);
      LaengeP1(KlnVk(y));
    }

  for (y = erstesVork(kx = Komp(x)); echtesVork(y, kx); y = naechstesVork(y))
    for (z = naechstesVorkK(y); z != y; z = naechstesVorkK(z))
      bindeLv(z);

  bindeV(v = Var(x));
  setzenbelegt(v, false);

#ifdef BAUMRES
  if (enthalten(v))
    relVhinzufuegen();
#endif

}


#ifdef BAUMRES

__inline__ bool rebelege_Verz(LIT x)

/* Macht x -> 0 wieder rueckgaengig fuer eine Verzweigungsvariable. */

/* rebelege_Verz = true falls verzweigt werden muss. */

{
  LITV y, z;
  LIT kx;
  VAR v;

  for (y = erstesVork(x); echtesVork(y, x); y = naechstesVork(y))
    {
      bindeLK(y);
      LaengeP1(KlnVk(y));
    }

  for (y = erstesVork(kx = Komp(x)); echtesVork(y, kx); y = naechstesVork(y))
    for (z = naechstesVorkK(y); z != y; z = naechstesVorkK(z))
      bindeLv(z);

  bindeV(v = Var(x));
  setzenbelegt(v, false);

  return enthalten(v);

}

#endif

                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Angelegt: 4.3.2001 */

/* Einheit: lokalesLernen */

#ifdef LOKALLERNEN

#include <stdlib.h>

#include "OK.h"
#include "VarLitKlm.h"
#include "Speicher.h"
#include "lokalesLernen.h"


static NKL freieNK;
static NKL AnfangNK = NULL;

static Marken *freieMarke;
static Marken *Anfangsmarke = NULL;


void InitlokalesLernen(void)
{
  freieNK = AnfangNK = (NKL) xmalloc(sizeneuK);
  AnfangNK -> naechstes = NULL;
  AnfangNK -> vorheriges = NULL;
  /* die folgenden Setzungen werden als konstant angesehen */
  setzenKln(&(AnfangNK -> x1), &(AnfangNK -> Klausel));
  setzenKln(&(AnfangNK -> x2), &(AnfangNK -> Klausel));

  freieMarke = Anfangsmarke = (Marken *) xmalloc(sizeof(Marken));
  Anfangsmarke -> naechste = NULL;
  Anfangsmarke -> vorige = NULL;
  Anfangsmarke -> Tiefe = NULL;
  Anfangsmarke -> NK = NULL;
}


__inline__ static void naechsteNK(void)
{
  if (freieNK -> naechstes != NULL)
    freieNK = freieNK -> naechstes;
  else
    {
      NKL Z;
      maxneue2K++;

      Z = (NKL) xmalloc(sizeneuK);
      Z -> naechstes = NULL;
      Z -> vorheriges = freieNK;
      freieNK -> naechstes = Z;
      /* die folgenden Setzungen werden als konstant angesehen */
      setzenKln(&(Z -> x1), &(Z -> Klausel));
      setzenKln(&(Z -> x2), &(Z -> Klausel));

      freieNK = Z;
    }
}


__inline__ void erzeugeNK(LIT l, Stapeleintrag *spneu, Stapeleintrag *sp)
/* setzt voraus, dass mindestens eine neue Klausel eingetragen wird */
{
  LITV x0, x1, y1; unsigned int d;
  LIT komp;

  d = (sp - spneu);
  neue2Klauseln += d;
  aktAnzK[2] += d;

  x0 = NULL; x1 = erstesVork(l);
  setzenerstesV(l, &(freieNK -> x1));
  do
    {
      setzenlLv(&(freieNK -> x1), x0);

      setzennLK(&(freieNK -> x1), &(freieNK -> x2));
      setzennLK(&(freieNK -> x2), &(freieNK -> x1));
      setzenlLK(&(freieNK -> x1), &(freieNK -> x2));
      setzenlLK(&(freieNK -> x2), &(freieNK -> x1));

      setzenLit(&(freieNK -> x1), l);

      komp = Komp(*spneu);
      setzenLit(&(freieNK -> x2), komp);

      y1 = erstesVork(komp);
      setzenlLv(&(freieNK -> x2), NULL);
      setzennLv(&(freieNK -> x2), y1);
      setzenerstesV(komp, &(freieNK -> x2));
      setzenlLv(y1, &(freieNK -> x2));
      
      ZuwLaenge(&(freieNK -> Klausel), 2);
      setzen0RundeK(&(freieNK -> Klausel));
#ifdef FASTAUTARKIE
      freieNK -> l = NullLiteral;
#endif
#ifdef BAUMRES
#error "Noch nicht!"
#endif
      x0 = &(freieNK -> x1);
      naechsteNK();
      if (++spneu == sp)
	break;
      setzennLv(x0, &(freieNK -> x1));
    }
  while (true);
  setzennLv(x0, x1);
  setzenlLv(x1, x0);
}

#ifdef FASTAUTARKIE

__inline__ void erzeugeFANK(LIT l, LIT *a, LIT *b)
/* setzt voraus, dass mindestens eine neue Klausel eingetragen wird */
{
  LITV x0, x1, y1; unsigned int d;

  d = (b - a);
  neue2Klauseln += d;
  aktAnzK[2] += d;

  x0 = NULL; x1 = erstesVork(l);
  setzenerstesV(l, &(freieNK -> x1));
  do
    {
      setzenlLv(&(freieNK -> x1), x0);

      setzennLK(&(freieNK -> x1), &(freieNK -> x2));
      setzennLK(&(freieNK -> x2), &(freieNK -> x1));
      setzenlLK(&(freieNK -> x1), &(freieNK -> x2));
      setzenlLK(&(freieNK -> x2), &(freieNK -> x1));

      setzenLit(&(freieNK -> x1), l);

      setzenLit(&(freieNK -> x2), *a);

      y1 = erstesVork(*a);
      setzenlLv(&(freieNK -> x2), NULL);
      setzennLv(&(freieNK -> x2), y1);
      setzenerstesV(*a, &(freieNK -> x2));
      setzenlLv(y1, &(freieNK -> x2));
      
      ZuwLaenge(&(freieNK -> Klausel), 2);
      setzen0RundeK(&(freieNK -> Klausel));

      freieNK -> l = Komp(l);

#ifdef BAUMRES
#error "Noch nicht!"
#endif
      x0 = &(freieNK -> x1);
      naechsteNK();
      if (++a == b)
	break;
      setzennLv(x0, &(freieNK -> x1));
    }
  while (true);
  setzennLv(x0, x1);
  setzenlLv(x1, x0);
}

#endif


__inline__ static void neueMarke()
{
  if (freieMarke -> naechste == NULL)
    {
      Marken *Z;
      Z = (Marken *) xmalloc(sizeof(Marken));
      Z -> naechste = NULL;
      Z -> vorige = freieMarke;
      Z -> Tiefe = NULL;
      Z -> NK = NULL;
      freieMarke -> naechste = Z;
      freieMarke = Z;
    }
  else
    {
      freieMarke = freieMarke -> naechste;
      freieMarke -> Tiefe = NULL;
      freieMarke -> NK = NULL;
    }
}

__inline__ void eintragenTiefe(void)
{
  if (freieMarke -> NK != NULL)
    {
      neueMarke();
      freieMarke -> Tiefe = Tiefe;
    }
  else if (freieMarke -> Tiefe == NULL)
    freieMarke -> Tiefe = Tiefe;
}

void eintragenNK()
{
  if (freieMarke -> Tiefe != NULL)
    {
      neueMarke();
      freieMarke -> NK = freieNK;
    }
  else if (freieMarke -> NK == NULL)
    freieMarke -> NK = freieNK;
}


Marken *Markierung(void)
{
  if ((freieMarke -> Tiefe != NULL) || (freieMarke -> NK != NULL))
    neueMarke();
  return freieMarke;
}


void Rueckgaengigmachung(Marken *Zeiger)
{
  while (true)
    {
      if (freieMarke -> Tiefe != NULL)
	do
	  {
	    Tiefe--;
	    rebelege(PfadLit());
	  }
	while (Tiefe != freieMarke -> Tiefe);
      else if (freieMarke -> NK != NULL)
	do
	  {
	    freieNK = freieNK -> vorheriges;
	    loeseLv(&(freieNK -> x1));
	    loeseLv(&(freieNK -> x2));
	    aktAnzK[2]--;
#ifdef FASTAUTARKIE
	    if (freieNK -> l != NullLiteral)
	      setzenFastautarkie(freieNK -> l, false);
/* 	    setzt voraus, das diese Klauseln auf einmal eingetragen */
/* 	      werden */
#endif
	  }
	while (freieNK != freieMarke -> NK);
      if (freieMarke == Zeiger)
	break;
      freieMarke = freieMarke -> vorige;
    }
  freieMarke -> Tiefe = NULL;
  freieMarke -> NK = NULL;
}



void AufraeumenlokalesLernen(void)
{
  NKL Z; NKL Z0;
  Marken *Z1; Marken *Z10;

  Z0 = AnfangNK;
  while (Z0 != NULL)
    {
      Z = Z0 -> naechstes;
      free(Z0);
      Z0 = Z;
    }
  AnfangNK = NULL;

  Z10 = Anfangsmarke;
  while (Z10 != NULL)
    {
      Z1 = Z10 -> naechste;
      free(Z10);
      Z10 = Z1;
    }
  Anfangsmarke = NULL;
}

void NullsetzenRKNK(void)
{
  NKL Z;
  for (Z = AnfangNK; Z != NULL; Z = Z -> naechstes)
    setzen0RundeK(&(Z -> Klausel));
}


#endif
                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Einheit: Reduktion */

/* Das "Reduktionsmodul" */


/* 1.11.1998 */


/* 4.5.1999 Zum Aufbau: */

/* Reduktion1 fuehrt F in r_2(F) ueber, waehrend Reduktion2 pure Literale eliminiert. */

/* Reduktion1 ruft La_Huelle ("look ahead" der 1-Klauseln-Huelle fuer x -> 0) */
/* und belege_Huelle (tatsaechliches Belegen einer berechneten 1-Klauseln-Huelle) */
/* auf. */

/* belege_Huelle ruft belege auf (und, falls BAUMRES gesetzt ist, belege_VK fuer */
/* nicht-Verzweigungsvariablen. */

/* La_Huelle ruft La_Belege fuer die einzelnen Elemente der Huelle. */


#include <stdlib.h>

#include "OK.h"
#include "VarLitKlm.h"
#include "BaumRes.h"
#include "Belegung.h"
#include "Reduktion.h"
#ifdef LOKALLERNEN
#include "lokalesLernen.h"
#endif

extern unsigned int Runde;


/* --------------------------------------------------------- */


static Stapeleintrag *H;
static Stapeleintrag *sp;

/* *H, ..., *(sp-1) ist die Huelle des aktuellen Test-Literals x */
/* (also *H = x bzw. H -> l = x), wobei H -> k die zugehoerige */
/* Klausel ist, die nun zur 1-Klausel wurde. */

#ifdef LOKALLERNEN
#ifndef NL2RED
static Stapeleintrag *spneu;
#endif
#endif

static Stapeleintrag *H0;
static Stapeleintrag *H1; 
/* zum Speichern von Huellen (fuer die beiden Zweige) */


static int *DK;
/* DK[i] ist die Anzahl eliminierter i-Klauseln. */

static unsigned int Reduktionsrunde;
/* der Wert von Runde vor der letzten Reduktion */

 /* --------------------------------------------------------------------------------------- */

#ifndef BAUMRES
__inline__ static void neue1Klausel(LIT x)
#else
__inline__ static void neue1Klausel(LIT x, KLN kn)
#endif
/* wird aufgerufen, wenn eine 1-Klausel */
/* Komp(x) gefunden wurde */
{
  if (RundeL(x) != Runde)  /* 1-Kl-Elim. nicht schon vorgemerkt? */
    {
      setzenRundeL(x);
#ifndef BAUMRES
      *(sp++) = x;
#else
      sp -> l = x;
      (sp++) -> k = kn; /* die fuer die neue 1-Klausel verantwortliche Klausel */
#endif
    }
}


__inline__ static bool La_belege( LIT x )

/* Fuehrt x -> 0 durch (bzgl. letzteRK, letzteRL und LaLaenK), */
/* und schreibt neu entstehende 1-Klauseln nach H (mit Eintrag in letzteRL), */
/* wobei sp erhoeht wird. */
/* La_belege(x) == false, falls die leere Klausel entstand. */
/* Im Unterschied zur analogen Funktion in "Filter.c" brauchen hier die */
/* (non x)-Vorkommen nicht betrachtet zu werden (und Klauseln werden auch */
/* nicht als erfuellt markiert). */

/* Ist BAUMRES gesetzt, so werden im "Normalfall" parallel zu H in das Feld EK */
/* die Nummern der Klauseln eingetragen, die nun zu 1-Klauseln wurden. */
/* Im Falle dass die leere Klausel entsteht, stehen in aktrelV die (urspruenglichen) */
/* Variablen der nun leeren Klausel. */

{
  LITV y, z;
  LIT lz;
  KLN kn;

  /* Durchlaufe alle x-Vorkommen und kuerze die aktiven Klauseln */

  for (y = erstesVork(x); echtesVork(y, x); y = naechstesVork(y))

      if (RundeK( kn = KlnVk(y) ) != Runde)
      /* Klausel von y noch nicht angefasst? */
        {
          if (Laenge(kn) == 2)  /* 2-Klausel? */
            {
              if (RundeL(lz = LitVk(naechstesVorkK(y))) == Runde)  
	      /* Widerspruch? (Komplementaere 1-Kl-Elim. schon vorgemerkt) */
#ifndef BAUMRES
		return false;
#else
	      {
	        setzenKl(kn); 
		/* die (urspruengl.) Variablen in der (nun) leeren Klausel */
		return false;
	      }
#endif
#ifndef BAUMRES
	      neue1Klausel(Komp(lz));
#else
	      neue1Klausel(Komp(lz), kn);
#endif
            }
          else  /* >= 3-Klausel */
            {
              setzenRundeK(kn);
              ZuwLaLaenge(kn, Laenge(kn) - 1);
            }
        }
      else  /* Klausel schon angefasst */

          if (LaLaenge(kn) == 2)  /* 2-Klausel? */
            {
              for (z = naechstesVorkK(y); z!= y; z = naechstesVorkK(z))
	      /* Suche zweites Literal */
                 if (RundeL(lz = LitVk(z)) != Runde)
		 /* z nicht auf 0 gesetzt? */
                   break;
              if (z == y)
	      /* alle Literale in der Klausel auf 0 gesetzt */
#ifndef BAUMRES
                return false;
#else
	      {
		setzenKl(kn);
		/* die (urspruengl.) Variablen in der (nun) leeren Klausel */
		return false;
	      }
#endif
#ifndef BAUMRES
	      neue1Klausel(Komp(lz));
#else
	      neue1Klausel(Komp(lz), kn);
#endif
            }
          else  /* >= 3-Klausel */
            M1LaLaenge(kn);

  return true;
}



__inline__ static bool La_Huelle(LIT x)

/* Ruft La_Belege auf zuerst mit x, und dann mit den neu entstehenden  */
/* 1-Kl.-Elim. */
/* La_Huelle == false falls die leere Klausel entstand. */

/* Ist BAUMRES gesetzt, so werden im Falle der Erzeugung der leeren Klausel  */
/* (andernfalls geschieht ja nichts) in aktrelV die beteiligten Variablen  */
/* zurueckgegeben. */

/* Falls nicht die leere Klausel entstand: */
/* sp zeigt nach Beendigung auf das erste freie Element im Stapel. */
/* Ist LOKALLERNEN gesetzt, so zeigt spneu <= sp auf die */
/* erste neue "1-Klausel". */


{
  Stapeleintrag *p; /* zeigt auf die naechste "1-Klausel" */

  if (++Runde == 0)
    {
      NullsetzenRK(); NullsetzenRL();
      Reduktionsrunde = 0; 
/* (damit geht die Information zum aktuellen Reduktions-Durchlauf verloren) */
      Runde = 1;
    }

  p = H;
#ifndef BAUMRES
  H[0] = x;
#else
  H -> l = x;
#endif
  sp = H + 1; setzenRundeL(x);
  if (La_belege(x) == false)
    /* La_belege schreibt 1-Klauseln nach H unter Erhoehung */
    /* von sp; letzteRL wird dabei belegt */
#ifndef BAUMRES
    return false;
#else
    {
      while (--sp > H)
	if (enthalten(Var(sp -> l)))
	  hinzufuegenKl(sp -> k);
      return false;
    }
#endif
  p++;
#ifdef LOKALLERNEN
#ifndef NL2RED
  spneu = sp;
#endif
#endif
  while (p < sp)
    {
#ifndef BAUMRES
      x = *p;
#else
      x = p -> l;
#endif
      if (La_belege(x) == false)
#ifndef BAUMRES
	return false;
#else
        {
	  while (--sp > H)
	    if (enthalten(Var(sp -> l)))
	      hinzufuegenKl(sp -> k);
	  return false;
	}
#endif
      p++;
    }
  return true;
}


__inline__ static void belegeRed(LIT x)

/* fuehrt x -> 0 durch mit Aktualisierung von "Laenk" und "DK" */
/* sowie "Pfad" (inkl. "Tiefe") und "belegt"; */
/* Vorbedingungen:  Es entsteht nicht die leere Klausel. */

{
  LITV y, z;
  LIT kx;
  KLL p;
  VAR v;

  for (y = erstesVork(x); echtesVork(y, x); y = naechstesVork(y))
  {
    loeseLK(y);
    DK[ p = LaengeM1(KlnVk(y)) ]++;
    DK[p-1]--;
  }

  for (y = erstesVork(kx = Komp(x)); echtesVork(y, kx); y = naechstesVork(y))
  {
    for (z = naechstesVorkK(y); z != y; z = naechstesVorkK(z))
      loeseLv(z);

    DK[Laenge(KlnVk(y))]++;
  }

  LiteinPfad(x); Tiefe++;
  loeseV(v = Var(x));
  setzenbelegt(v, true);
}


#ifdef BAUMRES

__inline__ static void belege_VKRed(LIT x, KLN K)

/* fuehrt x -> 0 durch mit Aktualisierung von "Laenk" und "DK" */
/* sowie "Pfad" (inkl. "Tiefe") und "belegt"; */
/* Vorbedingung:  Es entsteht nicht die leere Klausel. */
/* Zusaetzlich wird noch EK aktualisiert. */


{
  LITV y, z;
  LIT kx;
  KLL p;
  VAR v;

  extern void loeseLK(LITV x);
  extern void loeseLv(LITV x);
  extern void loeseV(VAR x);

  for (y = erstesVork(x); echtesVork(y, x); y = naechstesVork(y))
  {
    loeseLK(y);
    DK[ p = LaengeM1(KlnVk(y)) ]++;
    DK[p-1]--;
  }

  for (y = erstesVork(kx = Komp(x)); echtesVork(y, kx); y = naechstesVork(y))
  {
    for (z = naechstesVorkK(y); z != y; z = naechstesVorkK(z))
      loeseLv(z);

    DK[Laenge(KlnVk(y))]++;
  }

  LiteinPfad(x);
  loeseV(v = Var(x));
  setzenbelegt(v, true);

  Kln_eintragen_relV(K); Tiefe++;
}

#endif


__inline__ static bool belege_Huelle( void )

/* Wendet eine bereits berechnete "Huelle" an auf die aktuelle Klm */
/* durch Aufrufen von "belegeRed". */

/* Belegt so die Literale aus H[0, ..., sp-1] mit 0 und aktualisiert */
/* LaenK, aktAnzK, aktP und aktN. */
/* belege_Huelle == true, falls die Klauselmenge erfuellt wird. */

{
  unsigned int i;
  Stapeleintrag *p;

  for (i = 0; i <= aktP; i++)
    DK[i] = 0;
#ifdef LOKALLERNEN
  eintragenTiefe();
#endif
#ifndef BAUMRES
  for (p = H; p < sp; p++)
    belegeRed(*p);
#else
  aktV_volleintragen_relV();
  belegeRed(H -> l);
  for (p = H + 1; p < sp; p++)
    belege_VKRed(p -> l, p -> k);
#endif
  for (i = aktP; i >= 2; i--)
    aktAnzK[i] -= DK[i];
  for (i = aktP; i >= 2; i--)
    if (aktAnzK[i] != 0)
      break;
  if (i == 1)
    return true;
  aktP = i;
  aktN  -= (sp - H);
  return false;
}


/* -------------------------------------------------------------------------------------- */

void berechne_Huelle(LIT x)
/* berechnet die Huelle von x und schreibt die Ergebnisse nach H */
/*      (sp zeigt dann auf das erste freie Element) */
/* Voraussetzung: Die leere Klausel entsteht nicht. */
{
  Stapeleintrag *p; /* zeigt auf die naechste "1-Klausel" */
  LITV y, z;
  LIT lz;
  KLN kn;

  if (++Runde == 0)
    {
      NullsetzenRK(); NullsetzenRL();
      Reduktionsrunde = 0; 
/* (damit geht die Information zum aktuellen Reduktions-Durchlauf verloren) */
      Runde = 1;
    }

  p = H;
#ifndef BAUMRES
  H[0] = x;
#else
  H -> l = x;
#endif
  sp = H + 1; setzenRundeL(x);
  do
    {
      for (y = erstesVork(x); echtesVork(y, x); y = naechstesVork(y))
	if (RundeK( kn = KlnVk(y) ) != Runde)
	  /* Klausel von y noch nicht angefasst? */
	  {
          if (Laenge(kn) == 2)  /* 2-Klausel? */
            {
              lz = LitVk(naechstesVorkK(y)); 
#ifndef BAUMRES
	      neue1Klausel(Komp(lz));
#else
	      neue1Klausel(Komp(lz), kn);
#endif
            }
          else  /* >= 3-Klausel */
            {
              setzenRundeK(kn);
              ZuwLaLaenge(kn, Laenge(kn) - 1);
            }
        }
      else  /* Klausel schon angefasst */
	if (LaLaenge(kn) == 2)  /* 2-Klausel? */
	  {
	    for (z = naechstesVorkK(y); z!= y; z = naechstesVorkK(z))
	      /* Suche zweites Literal */
	      if (RundeL(lz = LitVk(z)) != Runde)
		/* z nicht auf 0 gesetzt? */
		break;
#ifndef BAUMRES
	    neue1Klausel(Komp(lz));
#else
	    neue1Klausel(Komp(lz), kn);
#endif
	  }
	else  /* >= 3-Klausel */
	  M1LaLaenge(kn);

      if (++p == sp)
	break;
#ifndef BAUMRES
      x = *p;
#else
      x = p -> l;
#endif
    }
  while (true);
}


/* -------------------------------------------------------------------------------------- */

size_t BedarfReduktionsV( void )
{
  return  2 * N * sizeof(Stapeleintrag)
    + (P+1) * sizeof(int);
}

void *ReduktionsV(void *Z)
{
  H0 = (Stapeleintrag *) Z; Z = (void *) (H0 + N);
  H1 = (Stapeleintrag *) Z; Z = (void *) (H1 + N);
  DK = (int *) Z; Z = (void *) (DK + P + 1);
  return Z;
}


/* -------------------------------------------------------------------------------------- */


__inline__ char Reduktion1( void )

/* Fuehrt die aktuelle Klm F in r_2(F) ueber, wobei die entsprechende Belegung */
/* in "Pfad" eingetragen wird. */

/* 0: normal */
/* 1: SAT, d.h. r_2(F) = {} */
/* 2: UNSAT, d.h. r_2(F) = { {} } */

{
  Stapeleintrag *sp0;
#ifdef LOKALLERNEN
#ifndef NL2RED
  Stapeleintrag *spneu0;
#endif
#endif
  bool r; /* wurde reduziert? */
  bool ersterZweig, zweiterZweig;
  VAR v; LIT v0, v1;

  if (aktAnzK[2] == 0) /* ohne 2-Klauseln keine Chance */
    {
      return 0;
    }

  Reduktionsrunde = Runde;
  do
    {
      r = false;
      for (v = ersteVar(); echteVar(v); v = naechsteVar(v))
	{
	  if (r && belegt(v))
	    continue; /* falls v von voriger Reduktion schon belegt wurde */
	  v0 = Literal(v, Pos); H = H0; 
	  if ((ersterZweig = (RundeL(v0) <= Reduktionsrunde)) && (La_Huelle(v0) == false))
	    /* Huelle[ v -> 0 ] ergab die leere Klausel */
	    {
	      V1KlRed++; /* Anwendung einer Reduktion der 2. Stufe */
#ifdef BAUMRES
	      aktV_speichern();
#endif
	      v1 = Literal(v,Neg);
	      if (RundeL(v1) <= Reduktionsrunde)
		{
		  if (La_Huelle(v1) == false)
		    /* auch Huelle[ v -> 1 ] ergab leere Klausel */
#ifndef BAUMRES
		    return 2; /* UNSAT */
#else
		    {
		      hinzufuegenS();
		      return 2; /* UNSAT */
		    }
#endif
		}
	      else
		berechne_Huelle(v1);
	      if (belege_Huelle() == true)
		/* Huelle[ v -> 1 ] ist erfuellend */
		return 1; /* SAT */
	      else
		{
		  r = true;  /* es wurde reduziert */
		  Reduktionsrunde = Runde;
		}
	    }
	  else  /* Zweig v -> 0 brachte nichts */
	    {
	      v1 = Literal(v,Neg); H = H1;
	      /* Umschalten auf v -> 1 */
	      /* (die zu v -> 0 gehoerende Belegung ist in H0) */
	      sp0 = sp;
#ifdef LOKALLERNEN
#ifndef NL2RED
	      spneu0 = spneu;
#endif
#endif
	      if ((zweiterZweig = (RundeL(v1) <= Reduktionsrunde)) && (La_Huelle(v1) == false))
		/* Huelle[ v -> 1 ] ergab die leere Klausel */
		{
		  V1KlRed++; /* Anwendung einer Reduktion der 2. Stufe */
		  if (ersterZweig)
		    {
		      H = H0; sp = sp0; /* wieder zurueckschalten */
		    }
		  else
		    berechne_Huelle(v0);
		  if (belege_Huelle() == true)
		    /* Huelle[ v -> 0 ] ist erfuellend */
		    return 1; /* SAT */
		  else
		    {
		      r = true;  /* es wurde reduziert */
		      Reduktionsrunde = Runde;
		    }
		}
#ifdef LOKALLERNEN
#ifndef NL2RED
	      else
		/* Beide Zweige brachten keine direkten Reduktionen */
		{
		  if (ersterZweig && (spneu0 < sp0))
		    {
		      eintragenNK();
		      erzeugeNK(v0, spneu0, sp0);
		      r = true;
		      Reduktionsrunde = Runde;
		    }
		  if (zweiterZweig && (spneu < sp))
		    {
		      eintragenNK();
		      erzeugeNK(v1, spneu, sp);
		      r = true;
		      Reduktionsrunde = Runde;
		    }
		}
#endif
#endif
	    }
	}
    }
  while (r);
  
  return 0;
}


/* -------------------------------------------------------------------------------------- */


__inline__ char Reduktion2( void )

/* Elimination purer Literale */

/* 0: normal */
/* 1: SAT */

{
  unsigned int dn = 0;
  bool r;
  LIT l, kl;
  unsigned int i;
  VAR v;

  for (i = 0; i <= aktP; i++)
    DK[i] = 0;

  do
  {
    r = false;

    for (v = ersteVar(); echteVar(v); v = naechsteVar(v))
    {
      l = Literal(v,Pos); kl = Literal(v,Neg);

      if (! echtesVork(erstesVork(l), l))
      {
#ifdef LOKALLERNEN
	eintragenTiefe();
#endif
        belegeRed(l); dn++;
        r = true; PureL++;
      }
      else if (! echtesVork(erstesVork(kl), kl))
      {
#ifdef LOKALLERNEN
	eintragenTiefe();
#endif
        belegeRed(kl); dn++;
        r = true; PureL++;
      }
    }
  }
  while (r);

  if (dn == 0)
    return 0;

  for (i = aktP; i >= 2; i--)
    aktAnzK[i] -= DK[i];

  for (i = aktP; i >= 2; i--)
    if (aktAnzK[i] != 0)
      break;

  if (i == 1)
    return 1;

  aktP = i;
  aktN -= dn;

  return 0;
}
                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */


/* Der "Filter" */

/* Einheit: Filter */


/* 21.8.1998: Austausch von "ErfK" durc "NeuK" (neue Klauseln) */

/* 31.10.1998: Elimination des Feldes "ausgefuehrt" */

/* 13.11.1998: Resolutions-Baum-Verwaltung */

/* 14.8.2001: DK in DKF umbenannt, um den Namenskonflikt mit dem */
/* Reduktions-Modul zu loesen */



/* --------------------------------------------------------- */

#include <stdlib.h>

#include "OK.h"
#include "VarLitKlm.h"
#include "Parameter.h"
#include "Filter.h"
#ifdef FASTAUTARKIE
#include "lokalesLernen.h"
#endif


bool erfuellt;
/* wurde eine erfuellende Belegung gefunden? */

bool reduziert;
/* muss wieder neu reduziert werden? */

VZ Zweig;
/* Teste zuerst v -> Zweig (wg. Autarkie oder 2-SAT). */

bool Wahl;
/* Ist Wahl == true, so ist v als Verzweigungsvariable zu waehlen. */

bool Single;
/* Gibt an, ob im Falle von "Wahl" nur "v -> Zweig" zu betrachten ist. */

/* Im Falle von "! erfuellt" und "! reduziert" werden von "Filter" die  */
/* folgenden Daten berechnet (evtl. nur fuer einen Zweig), wobei die  */
/* Hilfsvariable "Schalter" verwendet wird. */

unsigned int Schalter; /* Werte: 0, 1 */

/* Die lokale Variable Eps und die globale Variable Schalter gehoeren zu  */
/* der  ersten bzw. zweiten Komponente der folgenden 3-dimensionalen  */
/* Felder: */
/* Eps regelt den Zweig, waehrend "Schalter" zur aktuell von "Filter"  */
/* behandelten Variable gehoert --- "! Schalter" gibt die Daten zur bisher  */
/* besten Variablen (durch "Umschalten" werden Speicheroperationen  */
/* gespart).  */

unsigned int DeltaN [ 2 ] [ 2 ];
/* DeltaN ist die Anzahl (formal) eliminierter Variablen */

unsigned int LaP [ 2 ] [ 2 ]; /* die "vorausgesehene" neue maximale Klauselnlaenge */


StapeleintragFZ Huelle [ 2 ] [ 2 ];
/* LIT Huelle [ 2 ] [ 2 ] [ MAXN ]; */
/* Huelle [Eps, Schalter, 0 .. (DeltaN[Eps, Schalter] - 1)] ist die 1-Klauseln-Huelle */
/* der Belegung v -> Eps */


int *DeltaK [ 2 ] [ 2 ];
/* int DeltaK [ 2 ] [ 2 ] [ MAXP + 1 ]; */

/* DeltaK [Eps, Schalter, i] ist die Differenz der Anzahlen "alter" und "neuer" Klauseln */
/* der Laenge i bei der Anwendung der Huelle der Belegung v -> Eps */
/* (DeltaK ist negativ, falls sich diese Anzahl erhoeht hat). */


unsigned int *NeuK [ 2 ] [ 2 ];
/* unsigned int NeuK [ 2 ] [ 2 ] [ MAXP + 1 ]; */

/* NeuK [E, Schalter, i] ist die Anzahl der neuen Klauseln */
/* der Laenge i. */


unsigned int *LaAnzK [ 2 ] [ 2 ];
/* unsigned int LaAnzK [ 2 ] [ 2 ] [ MAXP + 1 ]; */
/* die "vorausgesehenen" neuen Klauselnzahlen */


extern unsigned int Runde;

/* --------------------------------------------------------------------------- */

#ifdef FASTAUTARKIE

static LIT *neueFA2Klauseln0;
static LIT *neueFA2Klausel;

unsigned int RundeFA;

#endif


/* --------------------------------------------------------------------------- */

/* Berechnung der 1-Klauseln-Huelle */

static VZ Eps;

/* Die folgenden Variablen sind die Felder DeltaN, Huelle, DeltaK und NeuK, */
/* wobei Eps und Schalter eingesetzt wurden. */
/* Im Falle von BAUMRES wird noch EinerKl verwendet. */

static unsigned int *DN;

static StapeleintragFZ HF;
/* "F" in "HF" wie "Filter" zur Unterscheidung von (nur) "H" in "Reduktion" */

static int *DKF;
/* ebenfalls das "F" in "DKF" zur Unterscheidung von "DK" in "Reduktion" */

static unsigned int *NK;


__inline__ static StapeleintragFZ La_belegeFil(LIT x, StapeleintragFZ sp)

/* Fuehrt x -> 0 durch (bzgl. letzteRK, letzteRL und LaLaenK), */
/* schreibt neu entstehende 1-Klauseln nach HF (mit Eintrag in letzteRL), */
/* wobei sp erhoeht wird, und aktualisiert DKF und NK. */
/* Vor.: Es entsteht nicht die leere Klausel. */
/* Im Falle von BAUMRES wird auch EK aktualisiert. */
/* Rueckgabewert ist der neue Wert von sp. */

{
  LITV y, z;
  LIT kx, lz, klz;
  KLN kn; KLL p;

  /* Durchlaufe alle x-Vorkommen und kuerze die aktiven Klauseln */

  for (y = erstesVork(x); echtesVork(y, x); y = naechstesVork(y))
    {
      if (RundeK( kn = KlnVk(y) ) != Runde)  /* Klausel von y noch nicht angefasst? */
	{
	  if (Laenge(kn) == 2)  /* 2-Klausel? */
	    {
	      if (RundeL(klz = Komp(LitVk(naechstesVorkK(y)))) != Runde)
		/* 1-Kl-Elim. nicht schon * vorgemerkt? */
		{
		  setzenRundeL(klz);	  
#ifndef BAUMRES	
		  *(sp++) = klz;	
#else
		  sp -> l = klz;
		  (sp++) -> k = kn;
#endif
		}
	    }
	  else  /* >= 3-Klausel */
	    {
	      setzenRundeK(kn);
	      p = Laenge(kn);
	      ZuwLaLaenge(kn, p-1);
	      DKF[p-1]--;
	      DKF[p]++;
	      NK[p-1]++;
	    }
	}
      else  /* Klausel schon angefasst */
	{
	  if ((p = LaLaenge(kn)) != 0)  /* Klausel noch nicht erfuellt? */
	    {    
	      if (p == 2)  /* 2-Klausel? */
		{
		  for (z = naechstesVorkK(y); ; z = naechstesVorkK(z))  /* Suche zweites Literal */
		    if (RundeL(lz = LitVk(z)) != Runde)  /* z nicht auf 0 gesetzt? */
		      break;
	      
		  if (RundeL(klz = Komp(lz)) != Runde)  /* 1-Kl-Elim. nicht schon vorgemerkt? */
		    {
		      setzenRundeL(klz);
#ifndef BAUMRES
		    
		      *(sp++) = klz;
		    
#else

		      sp -> l = klz;
		      (sp++) -> k = kn;

#endif

		    }
		}
	      else /*  >= 3-Klausel */
		{
		  DKF[ M1LaLaenge(kn) ]--;
		  DKF[p]++;
		  NK[p-1]++;
		  NK[p]--;
		}
	    }
	}
    }

  /* Durchlaufen der Komp(x)-Vorkommen und eliminiere alle noch aktiven Klauseln */

  for (y = erstesVork(kx = Komp(x)); echtesVork(y, kx); y = naechstesVork(y))

      if (RundeK( kn = KlnVk(y) ) != Runde)  /* Klausel nicht schon angefasst? */
        {
          setzenRundeK(kn);
          DKF[Laenge(kn)]++;
          ZuwLaLaenge(kn, 0);
        }
      else  /* Klausel schon angefasst */
        {
          if ( (p = LaLaenge(kn)) != 0)  /* Klausel noch nicht erfuellt? */
            {
              DKF[p]++;
              NK[p]--;
              ZuwLaLaenge(kn, 0);
            }
        }
  return sp;
}


static void La_HuelleFil(LIT x)

/* Berechnet Huelle, DeltaN, DeltaK und NeuK. */
/* (Im Falle von BAUMRES auch EinerKl.) */

{
  StapeleintragFZ sp; /* Stapelzeiger auf das naechste freie Element in HF */
  StapeleintragFZ p;

  if (++Runde == 0)
    {
      NullsetzenRK(); NullsetzenRL();
      Runde = 1;
    }

  DN = & (DeltaN[Eps][Schalter]); HF = Huelle[Eps][Schalter];
  DKF = DeltaK[Eps][Schalter]; NK = NeuK[Eps][Schalter];
#ifndef BAUMRES
  HF[0] = x;
#else
  HF -> l = x;
#endif
  sp = HF + 1;

  setzenRundeL(x);

  p = HF;

  {
    unsigned int i;
    for (i = 2; i <= aktP; i++)
    DKF[i] = NK[i] = 0;
  }

  do
    {
      sp = La_belegeFil(x, sp);

      p++;

      if (sp > p)  /* noch 1-Kl. zu eliminieren? */
#ifndef BAUMRES
        x = *p;
#else
        x = p -> l;
#endif
      else
	{
	  *DN = p - HF;
	  return;
	}
    }
  while ( true );
}


/* ---------------------------------------------------------------------------------- */

__inline__ static unsigned char auswerten( void )

/* Es werden die Daten von DKF und NK ausgewertet (bzgl. Autarkien und  */
/* 2-CLS), und LaAnzK, LaP berechnet. */
/* "auswerten" wird nur aufgerufen, wenn die leere Klausel nicht erzeugt  */
/* wurde. */

/* auswerten ==  */

/*   1 falls eine erfuellende Belegung vorliegt, */
/*   2 falls eine Autarkie vorliegt (die nicht erfuellend ist), */
/*   3 falls nur noch 2-Klauseln uebrig sind (und keine Autarkie vorliegt), */
/*   4 falls mind. eine >= 3-Kl. uebrig ist, und genau eine neue Klausel */
/*     erzeugt wurde, */
/*   0 sonst. */

{
  unsigned int i, j; unsigned int GAnzNK;
  unsigned int *AK; unsigned int *LP;

  GAnzNK = 0;
  AK = LaAnzK[Eps][Schalter]; LP = & (LaP[Eps][Schalter]);

  for (i = aktP; i > 1; i--) /* setzen von LaAnzK */
    AK[i] = aktAnzK[i] - DKF[i];

  for (i = aktP; i > 1; i--) /* berechne groesstes i, fuer dass i-Klauseln existieren */
    if (AK[i] != 0)
      break;

  if (i == 1) /* erfuellende Belegung gefunden */
    return 1;

  *LP = i; /* setzen von LaP */

  for (j = 2; j < aktP; j++) /* Berechnung der Gesamtzahl neuer Klauseln */
    GAnzNK += NK[j];

  if (GAnzNK == 0) /* Autarkie gefunden */
    return 2;
  else if (i == 2) /* neue Klauselmenge ist in 2-CLS */
    return 3;
  else if (GAnzNK == 1) /* Fast-Autarkie gefunden */
    return 4;
  else
    return 0;
}


/* ---------------------------------------------------------------------------------- */

size_t BedarfFilterV( void )
{
  return 4 * N * sizeof(StapeleintragF) +
    4 * (P+1) * sizeof(int) +
    2 * 4 * (P+1) * sizeof(unsigned int)
#ifdef FASTAUTARKIE
    + (P - 1) * sizeof(LIT)
#endif
    ;
}

void *FilterV(void *Z)
{
  unsigned char i, j;
  for (i = 0; i <= 1; i++)
    for (j = 0; j <= 1; j++)
      {
	Huelle[i][j] = (StapeleintragFZ) Z; Z = (void *) (Huelle[i][j] + N);
	DeltaK[i][j] = (int *) Z; Z = (void *) (DeltaK[i][j] + P + 1);
	NeuK[i][j] = (unsigned int *) Z; Z = (void *) (NeuK[i][j] + P + 1);
	LaAnzK[i][j] = (unsigned int *) Z; Z = (void *) (LaAnzK[i][j] + P + 1);
      }

#ifdef FASTAUTARKIE
  neueFA2Klauseln0 = (LIT *) Z; Z = (void *) (neueFA2Klauseln0 + (P - 1));
#endif
  
  return Z;
}

void InitFilter(void)
{
#ifdef FASTAUTARKIE
  RundeFA = 0;
#endif
  return;
}

/* ---------------------------------------------------------------------------------- */

#ifdef FASTAUTARKIE

bool BehFastautarkie() /* Behandlung einer Fast-Autarkie */
{
  LIT l, kl, a;
  unsigned int i;
  LITV x, nx;
  KLN nk;

  l = HF[0]; /* HF = Huelle[Eps][Schalter] */
  if (Fastautarkie(l))
    return false;

  /* finden der einen neuen Klausel nx */
  for (i = 0; i < *DN; i++)
    {
      a = HF[i];
      for (nx = erstesVork(a); echtesVork(nx, a); nx = naechstesVork(nx))
	{
	  nk = KlnVk(nx);
	  if (LaLaenge(nk) != 0)
	    goto gefunden;
	}
    }
 gefunden:

  kl = Komp(l);
  /* eintragen aller 2-Klauseln zu kl */
  if (++RundeFA == 0)
    {
      NullsetzenRLFA();
      RundeFA = 1;
    }
  for (x = erstesVork(kl); echtesVork(x, kl); x = naechstesVork(x))
    if (Laenge(KlnVk(x)) == 2)
      setzenRundeLFA(LitVk(naechstesVorkK(x)));

  /* herausfinden der neuen 2-Klauseln */
  neueFA2Klausel = neueFA2Klauseln0;
  for (x = naechstesVorkK(nx); x != nx; x = naechstesVorkK(x))
    {
      a = LitVk(x);
      if ((RundeL(a) != Runde) && (RundeLFA(Komp(a)) != RundeFA))
	*(neueFA2Klausel++) = Komp(a);
    }

  if (neueFA2Klausel == neueFA2Klauseln0)
    return false;
  /* eintragen der neuen 2-Klauseln */
  FastAutarkien++;
  eintragenNK();
  erzeugeFANK(kl, neueFA2Klauseln0, neueFA2Klausel);
  setzenFastautarkie(l, true);
  return true;
}

#endif

/* ---------------------------------------------------------------------------------- */



__inline__ void Filter(VAR v)

{
  Eps = Pos;

/*   Belegungen werden immer durch Literale angegeben, die auf 0 gesetzt werden. */
/*   Somit ist der erste Zweig v -> 0. */

  La_HuelleFil(Literal(v, Eps));

  switch (auswerten())  /* Auswertung des ersten Zweiges */
    {
    case 1 :

      erfuellt = true;  /* aktuelle Klm ist erfuellbar */
      Zweig = Eps;      /* zur Rekonstruktion */
      Autarkien++;      /* jede erfuellende Belegung ist Autarkie */
      return;

    case 2 :

      Autarkien++;  /* eine (echte) Autarkie wurde gefunden */

      erfuellt = false;
      reduziert = false;
      Wahl = true;
      Single = true;
      Zweig = Eps;
      return;

    case 3 :  /* nur noch 2-Klauseln? */

      Eps = Neg;

      La_HuelleFil(Literal(v, Eps));

      switch (auswerten())  /* Auswerten des zweiten Zweiges im Falle, dass der erste Zweig */
        {                   /*  nur 2-Kl. zurueckliess */
        case 1 :

          erfuellt = true;  /* aktuelle Klm ist erfuellbar */
	  Zweig = Eps;      /* zur Rekonstruktion */
	  Autarkien++;      /* jede erfuellende Belegung ist Autarkie */
          return;
	  
        case 2 :

          Autarkien++;  /* eine (echte) Autarkie wurde gefunden */

          erfuellt = false;
	  reduziert = false;
          Wahl = true;
          Single = true;
          Zweig = Eps;
          return;

        case 3 :  /* ein Quasi-Single-Knoten (mit 2-CNF in beiden(!) Zweigen) */

          erfuellt = false;
	  reduziert = false;
          Wahl = true;
          Single = false;
          Zweig = Pos;  /* willkuerlich (da unerheblich) */
          return;

        default :  /* ein Quasi-Single-Knoten */

          erfuellt = false;
	  reduziert = false;
          Wahl = true;
          Single = false;
          Zweig = (VZ) ! Eps;
          return;
        }

    case 4 :
#ifndef FASTAUTARKIE
      FastAutarkien++; /* geht ueber in den Sonst-Fall */
#else
      if ((reduziert = BehFastautarkie()))
	{
	  erfuellt = false;
	  return;
	}
#endif
    default :  /* erster Zweig hat nichts erbracht */

      Eps = Neg;

      La_HuelleFil(Literal(v, Eps));

      switch (auswerten())  /* Auswertung des zweiten Zweiges */
        {
        case 1 :

          erfuellt = true;  /* aktuelle Klm ist erfuellbar */
	  Zweig = Eps;      /* zur Rekonstruktion */
	  Autarkien++;      /* jede erfuellende Belegung ist Autarkie */
          return;

        case 2 :

          Autarkien++;  /* eine (echte) Autarkie wurde gefunden */

          erfuellt = false;
	  reduziert = false;
          Wahl = true;
          Single = true;
          Zweig = Eps;
          return;

        case 3 :  /* ein Quasi-Single-Knoten */

          erfuellt = false;
	  reduziert = false;
          Wahl = true;
          Single = false;
          Zweig = Eps;
          return;

	case 4 :
#ifndef FASTAUTARKIE
	  FastAutarkien++; /* geht ueber in den Sonst-Fall */
#else
	  if ((reduziert = BehFastautarkie()))
	    {
	      erfuellt = false;
	      return;
	    }
#endif
        default :  /* normale binaere Verzweigung */

          erfuellt = false;
	  reduziert = false;
          Wahl = false;
          return;
        }
    }
}


/* --------------------------------------------------------------------------------- */
                      /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Angelegt: 23.2.2001 */

/* Einheit: Abstand_nKaZ */

/* "Neue Klauseln mit Gewichten" */

/* Zweigauswahl: Maximiere die Wahrscheinlichkeit, dass eine feste */
/* Belegung die Klauselmenge, deren Klauseln zufaellig und unabhaengig */
/* voneinander gewaehlt werden (nur die Groessen und die Anzahlen sind */
/* gegeben) erfuellt. */

/* Fuer Gewichte G3 - G7 (fuer neue Klauseln der Laenge 3 - 7) wird das optimale  */
/* Gewicht aus Experimenten mit Zufalls-k-KNFs nahe dem Schwellenwert  */
/* (wobei k = i+1 fuer Gi) gesetzt. */

/* Fuer Gewichte G8, ... wird eine Extrapolation der Gewichte G3 - G7  */
/* verwendet. */


#include <stdlib.h>
#include <math.h>

#include "OK.h"
#include "Filter.h"
#include "Abstand.h"

const char *Abstandsname = "Abstand_nKaZ.c";

float d0, d1;
unsigned int d0S, d1S;


#define G3 0.2
#define G4 0.05
#define G5 0.01
#define G6 0.003

#define ALPHA 20.4514
#define BETA 0.218673

static float *Gewicht;

static float *GewichtZ;


/* -------------------------------------------------------------------------- */

size_t BedarfAbstandsV( void )
{
  return P * sizeof(float) +
    (P + 1) * sizeof(float);
}

void *AbstandsV(void *Z)
{
  unsigned int p;
  const float c2 = log(3.0 / 4);

  Gewicht = (float *) Z; Z = (void *) (Gewicht + P);
  GewichtZ = (float *) Z; Z = (void *) (GewichtZ + P + 1);

  for (p = 2; p < P; p++)
    switch (p)
      {
      case 2 : Gewicht[2] = 1; break;
      case 3 : Gewicht[3] = G3; break;
      case 4 : Gewicht[4] = G4; break;
      case 5 : Gewicht[5] = G5; break;
      case 6 : Gewicht[6] = G6; break;
      default : Gewicht[p] = ALPHA * pow(BETA, p);
      }

  GewichtZ[2] = 1;
  for (p = 3; p <= P; p++)
    GewichtZ[p] = log(1 - pow(2, - ((int) p))) / c2;
  
  return Z;
}


/* -------------------------------------------------------------------------- */


__inline__ void Abstand ( void )

/* d = \sum_{i=2}^{+\infty} Gewicht[i] * NeuK[i] */

/* NeuK[i] = Anzahl neuer Klauseln der Laenge i (= 0 fuer i >= aktP) */

/* Zweigauswahl: Minimiere \sum_{C \in F} log(1 - 2^(-|C|))  */

{
  unsigned int i;
  float s;

  /* Es wird vorausgesetzt, dass P >= 3 und Gewicht[2] = GewichtZ[2] = 1. */

  d0 = (NeuK[Pos][Schalter])[2];
  d1 = (NeuK[Neg][Schalter])[2];

  s = (DeltaK[Pos][Schalter])[2] - (DeltaK[Neg][Schalter])[2];

  for (i = 3; i < aktP; i++)
    {
      d0 += Gewicht[i] * (NeuK[Pos][Schalter])[i];
      d1 += Gewicht[i] * (NeuK[Neg][Schalter])[i];

      s += GewichtZ[i] * ((DeltaK[Pos][Schalter])[i] - (DeltaK[Neg][Schalter])[i]);
    }

  s += GewichtZ[aktP] * ((DeltaK[Pos][Schalter])[aktP] - (DeltaK[Neg][Schalter])[aktP]);
  
  if (s >= 0)
    Zweig = Pos;
  else
    Zweig = Neg;

  d0S = DeltaN[Pos][Schalter];
  d1S = DeltaN[Neg][Schalter];
}
/* Einheit: Projektion_Prod */

#include "OK.h"
#include "Abstand.h"
#include "Projektion.h"

const char *Projektionsname = "Projektion_Prod.c";

__inline__ double Projektion( void )
{
  return d0 * d1;
}

__inline__ unsigned int Projektion2 ( void )
{
  return d0S * d1S;
}
                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Angelegt: 19.2.2001 */

/* Einheit: Speicher */

#include <stdlib.h>
#include <stdio.h>

#include "OK.h"
#include "Ausgaben.h"
#include "Speicher.h"


void *xmalloc(size_t size)
{
  register void *value = malloc(size);
  if (value == 0)
    {
      fprintf(stderr, "%s\n", Meldung(5));
      exit(1);
    }
  return value;
}


void *xrealloc(void *ptr, size_t size)
{
  register void *value = realloc(ptr, size);
  if (value == 0)
    {
      fprintf(stderr, "%s\n", Meldung(5));
      exit(1);
    }
  return value;
}


                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */


/* Hauptprogramm */

/* Einheit: OK */


/* 21.8.1998: Ersetzen von "ErfK" durch "NeuK" */

/* 20.9.1998: Bisher wurde beim Finden einer Autarkie oder einer */
/* verallg. 1-Klauseln-Elim. ein neuer "Single-"Knoten erzeugt. */
/* Nun wird diese Belegung direkt ausgefuehrt, und die Auswahl */
/* einer Verzweigungs-Variablen neu begonnen. */

/* 13.11.1998: Resolutions-Baum-Verwaltung */

/* 31.3.2000: InitAbstand2() im Falle, dass DYNAMISCH gesetzt wurde */

/* 27.4.2000: Einlesen von OKVar.h */

/* 24.7.2000: Die Variablen LitTab und KlTab aus der Prozedure Einlesen */
/* nun mit "calloc" dynamisch erzeugt, und mit "free" dann geloescht. */

/* 25.7.2000: Ersetzen der drei calloc-Aufrufe durch einen malloc-Aufruf. */

/* 27.7.2000: Elimination der Rekursion (mittels Indices) */

/* 29.1.2001: Nun wurde die Elimination der Rekursion dynamisch durchgefuehrt, */
/* d.h., es wird nur soviel Speicherplatz fuer die lokalen Variablen angefordert, */
/* wie auch wirklich benoetigt wird. */

/* 29.1.2001: Die Speicherung der zweiten Belegung nun korrekt durchgefuehrt */
/* (mittels Dynamisierung). */

/* 30.1.2001: Elimination von OKVar.h */

/* 18.2.2001: Liste von Argumenten (mit Optionen) */

/* 20.2.2001: Ausgabe der Gesamtzeit. */

/* 27.2.2001: Realisierung der 1-Klauseln-Elimination fuer die Eingabe */

/* 28.2.2001: Freisetzen von Speicher und Datenstroemen wird nun ueber den */
/* NULL-Wert geregelt. */
/* 28.2.2001: Ein Syntax-Fehler fuehrt nun nicht mehr zum Abbruch. */
/* 28.2.2001: Senden des Signals SIGUSR1 bewirkt Ausdruck der Statistik */
/* (aber nicht in die Datei), und SIGUSR2 bewirkt Abbruch der Bearbeitung */
/* der aktuellen Formel und Ausdruck der Statistik (diesmal in die Datei, */
/* falls gewuenscht). */

/* 3.3.2001: Dynamisierung von relVarM; die Stapelzeiger nun wirklich als */
/* Zeiger; H und EK im Reduktionsmodul, EinerKl und Huelle im Filtermodul, */
/* sowie relVarM, relVarK und Pfad zusammengefasst; LITTAB als Uebersetzer- */
/* Option bewirkt, dass die Variablenmengen der Eingabeklauseln mit weniger */
/* Speicherplatz abgespeichert werden. */

/* 6.3.2001: Feld 14 (Redplus) aus der Statistikausgabe gestichen. */


/* --------------------------------------------------------- */


#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>

#ifdef SYSTIME

#include <sys/times.h>

#endif

#include "OK.h"
#include "BaumRes.h"
#include "VarLitKlm.h"
#include "Filter.h"
#include "Belegung.h"
#include "Abstand.h"
#include "Projektion.h"
#include "Einlesen.h"
#include "Parameter.h"
#include "Ausgaben.h"
#include "Speicher.h"
#include "Reduktion.h"
#ifdef LOKALLERNEN
#include "lokalesLernen.h"
#endif

/* ------------------------------------------------------------- */

unsigned int Sprache = SPRACHE;
unsigned int Standard = STANDARD;

#ifndef BELEGUNG
bool Belegung = false;
#else
bool Belegung = true;
#endif
/* Ist Belegung = true, so wird eine erfuellende Belegung ausgegen (ggfs.), */
/* und zwar auf den Standard-Ausgabe-Strom (die naechste Zeile nach der */
/* Statistik). */
/* Das Format der Darstellung der partiellen Belegung ist: */
/*  pro Zeile wird ein Paar "v e" angegeben, wobei */
/*  v die Nummer der Variablen in der Eingabe ist, */
/*  und e entweder 0 oder 1 ist. */
/* Bsp.: F = (-4,2)(4,55)(-2) */
/* ergibt */
/*     4 0 */
/*     2 0 */
/*    55 1 */

/* Schalter: */

bool Dateiausgabe = false;
bool Monitor = false;
bool nurVorreduktion = false;
bool Schranken = false;
unsigned int Zeitschranke = 0;
bool randomisiert = false;

// Setzen des voreingestellen Ausgabeformates

#ifdef DIMACSAUS
#  ifdef XMLAUS
#    error "Keine Kombination von DIMACS- und XML-Format moeglich!"
#  endif
bool spezRueckgabe = true; // Kodierung des Ergebnisses im Rueckgabewert
Ausgabeformat Format = Dimacs_Format;
#else
bool spezRueckgabe = false;
#  ifdef XMLAUS
Ausgabeformat Format = XML_Format;
#  else
Ausgabeformat Format = Tabellen_Format;
#  endif
#endif
// am 10.3.2002 fuer den SAT-Wettbewerb hinzugefuegt
// falls DIMACS_Format gesetzt ist, so wird die Ausgabe einer Belegung
// als Liste von wahr gemachten Literalen ausgegeben (in einer Zeile),
// und die Kommentarzeile wird in zwei Zeilen aufgespalten:
// der SAT-Wert (0, 1 oder 2) ergibt
// s UNSATISFIABLE (SATISFIABLE, UNKNOWN)
// und die restlichen Zahlen werden in eine Zeile beginnend mit "c "
// geschrieben.
// (Fuer den Wettbewerb muss sowohl BELEGUNG als auch DIMACSAUS
// gesetzt werden.)



unsigned int MAXN = 30000;
unsigned int MAXL = 400000;
unsigned int MAXK = 150000;


/* ------------------------------------------------------------- */


static const long EPS =  /* Einheiten pro Sekunde */
#ifndef SYSTIME
    CLOCKS_PER_SEC;
#else
    CLK_TCK;
#endif

#ifdef SYSTIME
  struct tms SysZeit;
  struct tms *Zeiger = &SysZeit;
#endif

clock_t akkVerbrauch = 0; /* akkumulierter Verbrauch */


/* ------------------------------------------------------------- */

/* Die Darstellung der Eingabe */

unsigned int P, P0, N, N0, K, K0, L, L0, aktN, aktP;

/* Hilfsvariablen fuer Reduktion und Filter */

unsigned int Runde;


/* ------------------------------------------------------------- */

/* Statistik */

unsigned long int Knoten, SingleKnoten, VerSingleKnoten, QuasiSingleKnoten, PureL, Autarkien, V1KlRed, FastAutarkien, InitEinerRed, neue2Klauseln, maxneue2K;
unsigned int Suchbaumtiefe, Ueberschreitung2, init2Klauseln;

static clock_t Verbrauch;

static char *aktName;
static char* Wurzel = NULL;

enum Ergebniswerte {SAT = 1, UNSAT = 0, Unbestimmt = 2};

static Ergebniswerte s = Unbestimmt; /* Ergebniswert */


/* ------------------------------------------------------------- */


/* Lokale Datenstrukturen und Variablen */



/* Der Stapel, der die Belegungen fuer den jeweils zweiten Zweig enthaelt */

static StapeleintragFZ zweiteBel = NULL;

static unsigned int Zeiger2;
static unsigned int Groesse2;



/* Zur Simulation der Rekursion */

enum Spruenge { SAT1, SAT2 };

struct Sammlung {
  unsigned int P2, N2, altZeiger2;
  Spruenge Ruecksprung;
  unsigned int *AnzK2;
#ifndef LOKALLERNEN
  Pfadinfo *altTiefe;
#else
  Marken *Marke;
#endif
#ifdef OUTPUTTREEDATAXML
  // clock_t start_run_time;
  unsigned long int number_2_reductions_at_new_node;
#endif
  struct Sammlung * davor;
  struct Sammlung * danach;
};

static struct Sammlung *SatVar;
static struct Sammlung *SatVar0 = NULL;
/* speichert den initialen Wert von SatVar */

/* ------------------------------------------------------------- */


/* Zur Beobachtung der SAT-Entscheidung */

static unsigned int Beobachtungsniveau = 6;

static unsigned int Rekursionstiefe;

static unsigned int *Zweiglast = NULL;
/* = 2^0, ..., 2^(Beobachtungsniveau-1) */

static unsigned int Gesamtlast; /* = 2^Beobachtungsniveau */

static unsigned int *beobachtet = NULL;
static unsigned int totalbeobachtet;

static unsigned long int altKnoten;

static FILE *fpmo = NULL; /* die aktuelle Ausgabeidatei zur Ueberwachung */


__inline__ static void Monitorausgabe(unsigned int b)
{
  if (b > totalbeobachtet)
    {
      totalbeobachtet = b;
#ifndef SYSTIME
      Verbrauch = clock() - akkVerbrauch;
#else
      times(Zeiger);
      Verbrauch = SysZeit.tms_utime - akkVerbrauch;
#endif
      printf("%3d:%6ld, %6.1f, %6.1f\n", b, Knoten - altKnoten, (double) Verbrauch / EPS, ((double) Gesamtlast / b - 1) * Verbrauch / EPS);
      if (Dateiausgabe)
        {
          fprintf(fpmo, "%3d:%6ld, %6.1f, %6.1f\n", b, Knoten - altKnoten, (double) Verbrauch / EPS, ((double) Gesamtlast / b - 1) * Verbrauch / EPS);
        }
      fflush(NULL);
      altKnoten = Knoten;
    }
  return;
}

__inline__ static void Verzweigungsliteralausgabe(const LIT x, unsigned int Tiefe) {
  VAR v; VZ e;
  v = Var(x);
  if (x == Literal(v, Pos))
    e = Pos;
  else
    e = Neg;
  printf("%d : %7s %d\n", Tiefe, Symbol(v), e);
  fprintf(fpmo, "%d : %7s %d\n", Tiefe, Symbol(v), e);
  fflush(NULL);
}

/* ------------------------------------------------------------- */


typedef enum { gleich = 0, groesser = 1, kleiner = 2} VERGL;

/* Zur Bestimmung, ob einer Gleitpunktzahl "wirklich" groesser ist als eine andere: */

static VERGL Vergleich(float a, float b)
{
  float h;
  h = b * 4 * FLT_EPSILON;
  if (a > b + h)
    return groesser;
  else if (a < b - h)
    return kleiner;
  else
    return gleich;
}


/* ------------------------------------------------------------- */



/* Prozeduren zur Speicherverwaltung */


static struct Sammlung *neuerKnoten( void )
{
  struct Sammlung *s;
  s = (struct Sammlung *) xmalloc(sizeof(struct Sammlung) + (P + 1) * sizeof(unsigned int));
  s -> AnzK2 = (unsigned int *)(s + 1);
  s -> danach = NULL;
  return s;
}

/* -------------------------------------------------------------------------------- */

/* Randomisierung */

static long unsigned int Schluessel = 1;

static long unsigned int randx;

static float Verhaeltnis = 0.2;


__inline__ static void srand_S(void)
{
  randx = Schluessel;
}

__inline__ static int rand_S(void)
{
  return(((randx = randx * 1103515245L + 12345)>>16) & 0x7fff);
}

__inline__ static float Verschmierung(double x)
{
  return (rand_S() / ((float) 0x7fff) * Verhaeltnis + 1) * x;
}



/* -------------------------------------------------------------------------------- */


/* Initialisierung */


void InitSat( void )
{
  Groesse2 = N;
  zweiteBel = (StapeleintragFZ) xmalloc(Groesse2 * sizeof(StapeleintragF));
  SatVar0 = SatVar = neuerKnoten();
  SatVar -> davor = NULL;
  
  Runde = 0; Zeiger2 = 0;

  if (Monitor && (! nurVorreduktion))
    {
      unsigned int p; unsigned int *Z;
      totalbeobachtet = 0;
      Rekursionstiefe = 0;
      altKnoten = 0;
      Zweiglast = (unsigned int *) xmalloc(Beobachtungsniveau * sizeof(unsigned int));
      for (p = 1, Z = Zweiglast + Beobachtungsniveau; Z != Zweiglast; p *= 2)
        *(--Z) = p;
      Gesamtlast = p;
      beobachtet = (unsigned int *) xmalloc(Beobachtungsniveau * sizeof(unsigned int));
      beobachtet[0] = 0;
    }

  return;
}

static void AufraeumenSat(void)
{
  struct Sammlung *Z; struct Sammlung *Z0;
  
  Knoten = SingleKnoten = VerSingleKnoten = QuasiSingleKnoten = PureL = Autarkien = V1KlRed = Suchbaumtiefe = Ueberschreitung2 = FastAutarkien = InitEinerRed = neue2Klauseln = maxneue2K = init2Klauseln = 0;
  Tiefe = NULL;

  free(zweiteBel); zweiteBel = NULL;

  Z0 = SatVar0;
  while (Z0 != NULL)
    {
      Z = Z0 -> danach;
      free(Z0);
      Z0 = Z;
    }
  SatVar0 = NULL;

  free(Zweiglast); Zweiglast = NULL;
  free(beobachtet); beobachtet = NULL;
}


/* -------------------------------------------------------------------------------- */

#ifdef OUTPUTTREEDATAXML

static char* NameTreeDataFile = NULL;
static FILE* TreeDataFile = NULL;

void BeginTreeElement() {
  fprintf(TreeDataFile, "<t l=\"%.4f\">", (V1KlRed - SatVar -> number_2_reductions_at_new_node) / EPS);
}
void EndTreeElement() {
  fprintf(TreeDataFile, "</t>");
}
void FinaliseSATPath() {
  do {
    EndTreeElement();
    SatVar = SatVar -> davor;
  } while (SatVar != NULL);
}

#endif


/* Die (rekursive) Loesungs-Prozedur */


static Ergebniswerte SATEntscheidung( void )

{
  unsigned int i;
  float opta; float a; unsigned int optaS;
  VAR v;
  VZ optZweig;
  Spruenge r;
  unsigned int DN, DN2;
  StapeleintragFZ Z;


Anfang:

  Knoten++;

#ifdef OUTPUTTREEDATAXML
  SatVar -> number_2_reductions_at_new_node = V1KlRed;
#endif  

  /* Reduktionen, die von Autarkien nicht affiziert werden */
  /* (zumindest verallgemeinerte 1-Klauseln-Elimination) */

alleReduktionen:

  switch ( Reduktion1() ) {

  case 1:
#ifdef OUTPUTTREEDATAXML
    BeginTreeElement();
    FinaliseSATPath();
#endif  
    return SAT;

  case 2:
#ifdef OUTPUTTREEDATAXML
    BeginTreeElement();
#endif
    r = SatVar -> Ruecksprung;
    SatVar = SatVar -> davor;
    if (SatVar == NULL) {
#ifdef OUTPUTTREEDATAXML
      EndTreeElement();
#endif
      return UNSAT;
    }
    if (Monitor)
      Rekursionstiefe--;
    switch (r)
      {
      case SAT1 : goto nachSAT1;
      case SAT2 : goto nachSAT2;
      }
  }


Schleife:

#ifdef DYNAMISCH

  InitAbstand2();

#endif

/* Zwar werden so die Autarkien nicht in die dynamische Gewichtsberechnung einbezogen, 
    dieser Effekt scheint mir jedoch gering, waehrend ihr Einbezug grossen Aufwand mit sich 
    braechte. So wird also nur Reduktion1() beruecksichtigt. */


  /* Reduktionen, die von Autarkien affiziert werden */
  /* (zumindest pure Literale) */

  switch ( Reduktion2() ) {
    
  case 1:
#ifdef OUTPUTTREEDATAXML
    BeginTreeElement();
    FinaliseSATPath();
#endif
    return SAT;

  case 2:
#ifdef OUTPUTTREEDATAXML
    BeginTreeElement();
#endif
    r = SatVar -> Ruecksprung;
    SatVar = SatVar -> davor;
    if (SatVar == NULL) {
#ifdef OUTPUTTREEDATAXML
      EndTreeElement();
#endif
      return UNSAT;
    }
    if (Monitor)
      Rekursionstiefe--;
    switch (r)
      {
      case SAT1 : goto nachSAT1;
      case SAT2 : goto nachSAT2;
      }
  }

  opta = 0; optaS = 0;

/*   Schleife ueber alle Variablen, die jeweils dem Filter vorgelegt werden: */
/*     Findet dieser eine Entscheidung oder eine Einer-Verzweigung, so wird eine */
/*     solche Variable ausgesucht, und die Schleife abgebrochen. */
/*     Andernfalls wird mittels "Abstand" die (Zweier-)Verzweigung bewertet, und */
/*     sie ersetzt, falls besser, die alte, bisher beste Verzweigung. */
/*     (Die Zweigauswahl wird von "Abstand" mitberechnet.) */

  for (v = ersteVar(); echteVar(v); v = naechsteVar(v))
    {
      Filter(v);
      if (erfuellt)
        {
          if (Belegung) /* Durchfuehrung der Belegung (zur Ausgabe) */
            {
              DN = DeltaN[Zweig][Schalter];
              for (i = 0, Z = Huelle[Zweig][Schalter]; i < DN; i++, Z++)
#ifndef BAUMRES
                belege(*Z);
#else
                belege(Z -> l);
#endif
            }
#ifdef OUTPUTTREEDATAXML
	  BeginTreeElement();
	  FinaliseSATPath();
#endif  
          return SAT;
        }

      if (reduziert)
        goto alleReduktionen;
      
      if (Wahl)
        {
              if (Single)  /* (zur Zeit) der (nicht-erfuellende) Autarkiefall */
            {
              /* Durchfuehrung der Belegung */
              DN = DeltaN[Zweig][Schalter];
#ifdef LOKALLERNEN
              eintragenTiefe();
#endif
              for (i = 0, Z = Huelle[Zweig][Schalter]; i < DN; i++, Z++)
#ifndef BAUMRES
                belege(*Z);
#else
                belege(Z -> l);
#endif
              
              /* Falls BAUMRES gesetzt ist: */
              /* Da der Standard-Filter nur Autarkien hier liefern kann, */
              /* die nie zur Erzeugung der leeren Klausel beitragen koennen, */
              /* muss hier in relVar nichts eingetragen werden. */
              
              aktP = LaP[Zweig][Schalter];
              aktN -= DeltaN[Zweig][Schalter];
              memcpy((void *)(aktAnzK + 2), (void *)(LaAnzK[Zweig][Schalter] + 2), (aktP - 1) * sizeof(unsigned int));
              goto Schleife;
            }
          else
            {
              QuasiSingleKnoten++;
              Schalter = ! Schalter;
              optZweig = Zweig;
              break;
            }
        }

      Abstand();
      if (randomisiert)
        a = Verschmierung(Projektion());
      else
        a = Projektion();
      switch (Vergleich(a, opta)) {
      case gleich :
        if (Projektion2() <= optaS)
          break;
      case groesser :
        opta = a; optaS = Projektion2();
        Schalter = ! Schalter;
        optZweig = Zweig;
        break;
      case kleiner :
        break;
      }
    }
  
  /* Nun ist die beste Variable gefunden, und es wird verzweigt */

#ifdef OUTPUTTREEDATAXML
  BeginTreeElement();
#endif

#ifndef LOKALLERNEN
  SatVar -> altTiefe = Tiefe;
#else
  SatVar -> Marke = Markierung();
#endif

  /* zuerst optZweig */

  DN = DeltaN[optZweig][! Schalter];
  DN2 = DeltaN[! optZweig][! Schalter];
#ifdef LOKALLERNEN
  eintragenTiefe();
#endif
#ifndef BAUMRES
  for (i = 0, Z = Huelle[optZweig][! Schalter]; i < DN; i++, Z++)
    belege(*Z);
#else
  Z = Huelle[optZweig][! Schalter];
  belege((Z++) -> l);
  for (i = 1; i < DN; i++, Z++)
    belege_VK(Z -> l, Z -> k);
#endif
  aktP = LaP[optZweig][! Schalter];
  SatVar -> P2 = LaP[! optZweig][! Schalter];
  memcpy((void *)(aktAnzK + 2), (void *)(LaAnzK[optZweig][! Schalter] + 2), (aktP - 1) * sizeof(unsigned int));
  memcpy((void *)((SatVar -> AnzK2) + 2), (void *)(LaAnzK[! optZweig][! Schalter] + 2), ((SatVar -> P2) - 1) * sizeof(unsigned int));
  SatVar -> N2 = aktN - DN2;
  aktN -= DN;
  SatVar -> altZeiger2 = Zeiger2;

/* Ist noch genug Speicher fuer die zweite Belegung?! */

  if (Zeiger2 + DN2 >= Groesse2)
    {
      Ueberschreitung2++;
      Groesse2 += N;
      zweiteBel = (StapeleintragFZ) xrealloc(zweiteBel, Groesse2 * sizeof(StapeleintragF));
    }
  memcpy((void *)(zweiteBel + Zeiger2), (void *)(Huelle[! optZweig][! Schalter]), DN2 * sizeof(StapeleintragF));
  Zeiger2 += DN2;

  if (SatVar -> danach == NULL)
    {
      struct Sammlung *Z;
      Suchbaumtiefe++;
      SatVar -> danach = (Z = neuerKnoten());
      Z -> davor = SatVar;
      SatVar = Z;
    }
  else
    SatVar = SatVar -> danach;
  SatVar -> Ruecksprung = SAT1;
  if (Monitor)
    if (++Rekursionstiefe < Beobachtungsniveau) {
      beobachtet[Rekursionstiefe] = beobachtet[Rekursionstiefe-1];
      if (Dateiausgabe)
#ifndef BAUMRES
	Verzweigungsliteralausgabe(*Huelle[optZweig][! Schalter], Rekursionstiefe - 1);
#else
        Verzweigungsliteralausgabe(Huelle[optZweig][! Schalter] -> l, Rekursionstiefe - 1);
#endif
    }
  goto Anfang;


  nachSAT1 :

#ifdef OUTPUTTREEDATAXML
    EndTreeElement();
#endif

    if (Monitor)
      if (Rekursionstiefe < Beobachtungsniveau)
        {
          beobachtet[Rekursionstiefe] += Zweiglast[Rekursionstiefe];
          Monitorausgabe(beobachtet[Rekursionstiefe]);
        }

  /* rueckgaengig machen */

#ifdef LOKALLERNEN
  Rueckgaengigmachung(SatVar -> Marke);
#else
#ifndef BAUMRES
  do
    {
      Tiefe--;
      rebelege(PfadLit());
    }
  while (Tiefe > SatVar -> altTiefe);
#else
  while (--Tiefe > SatVar -> altTiefe)
    rebelege(PfadLit());
  if (rebelege_Verz(PfadLit()))
    aktV_eintragen_relV();
  else
    {
      Zeiger2 = SatVar -> altZeiger2;
      SingleKnoten++;
      r = SatVar -> Ruecksprung;
      SatVar = SatVar -> davor;
      if (SatVar == NULL) {
#ifdef OUTPUTTREEDATAXML
	EndTreeElement();
#endif
        return UNSAT;
      }
      if (Monitor)
        Rekursionstiefe--;
      switch (r)
        {
        case SAT1 : goto nachSAT1;
        case SAT2 : goto nachSAT2;
        }
    }
#endif
#endif

  /* nun der zweite Zweig */

#ifdef LOKALLERNEN
  eintragenTiefe();
#endif
#ifndef BAUMRES
  for (i = SatVar -> altZeiger2, Z = zweiteBel + (SatVar -> altZeiger2); i < Zeiger2; i++, Z++)
    belege(*Z);
#else
  Z = zweiteBel + (SatVar -> altZeiger2);
  belege((Z++) -> l);
  for (i = SatVar -> altZeiger2 + 1; i < Zeiger2; i++, Z++)
    belege_VK(Z -> l, Z -> k);
#endif

  Zeiger2 = SatVar -> altZeiger2;
  aktP = SatVar -> P2;
  aktN = SatVar -> N2;
  memcpy((void *)(aktAnzK + 2), (void *)((SatVar -> AnzK2) + 2), (aktP - 1) * sizeof(unsigned int));

  if (SatVar -> danach == NULL)
    {
      struct Sammlung *Z;
      Suchbaumtiefe++;
      SatVar -> danach = (Z = neuerKnoten());
      Z -> davor = SatVar;
      SatVar = Z;
    }
  else
    SatVar = SatVar -> danach;
  SatVar -> Ruecksprung = SAT2;
  if (Monitor)
    if (++Rekursionstiefe < Beobachtungsniveau)
      beobachtet[Rekursionstiefe] = beobachtet[Rekursionstiefe-1];
  goto Anfang;

  nachSAT2 :

#ifdef OUTPUTTREEDATAXML
    EndTreeElement();
#endif

    if (Monitor)
      if (Rekursionstiefe < Beobachtungsniveau)
        {
          beobachtet[Rekursionstiefe] += Zweiglast[Rekursionstiefe];
          Monitorausgabe(beobachtet[Rekursionstiefe]);
        }

#ifdef LOKALLERNEN
  Rueckgaengigmachung(SatVar -> Marke);
#else
#ifndef BAUMRES
  do
  {
    Tiefe--;
    rebelege(PfadLit());
  }
  while (Tiefe > SatVar -> altTiefe);
#else
  while (--Tiefe > SatVar -> altTiefe)
    rebelege(PfadLit());
  if (rebelege_Verz(PfadLit()))
    relVMhinzufuegen();
  else
    VerSingleKnoten++;
#endif
#endif
  r = SatVar -> Ruecksprung;
  SatVar = SatVar -> davor;
  if (SatVar == NULL) {
#ifdef OUTPUTTREEDATAXML
    EndTreeElement();
#endif
    return UNSAT;
  }
  if (Monitor)
      Rekursionstiefe--;
  switch (r)
    {
    case SAT1 : goto nachSAT1;
    case SAT2 : goto nachSAT2;
    } 
}




/* -------------------------------------------------------------------------------- */


void Statistikzeile(FILE *fp)
{
  if (Format == Dimacs_Format) {
    fprintf(fp, "s ");
    switch (s) {
    case SAT :
      fprintf(fp, "SATISFIABLE"); break;
    case UNSAT :
      fprintf(fp, "UNSATISFIABLE"); break;
    case Unbestimmt :
      fprintf(fp, "UNKNOWN"); break;
    }
    fprintf(fp, "\n");
    fprintf(fp, "c sat_status=%d initial_maximal_clause_length=%d initial_number_of_variables=%d initial_number_of_clauses=%d initial_number_of_literal_occurrences=%d running_time(s)=%1.1f number_of_nodes=%ld number_of_single_nodes=%ld number_of_quasi_single_nodes=%ld number_of_2-reductions=%ld number_of_pure_literals=%ld number_of_autarkies=%ld number_of_missed_single_nodes=%ld max_tree_depth=%d number_of_table_enlargements=%d reduced_maximal_clause_length=%d reduced_number_of_variables=%d reduced_number_of_clauses=%d reduced_number_of_literal_occurrences=%d number_of_1-autarkies=%ld number_of_initial_unit-eliminations=%ld number_of_new_2-clauses=%ld maximal_number_of_added_2-clauses=%ld initial_number_of_2-clauses=%d file_name=%s\n",
	    s, P0, N0, K0, L0, (double) Verbrauch / EPS,
	    Knoten, SingleKnoten, QuasiSingleKnoten, V1KlRed, PureL, 
	    Autarkien, VerSingleKnoten, Suchbaumtiefe,
	    Ueberschreitung2, P0 - P, N0 - N, K0 - K, L0 - L,
	    FastAutarkien, InitEinerRed, neue2Klauseln, maxneue2K, init2Klauseln,
	    aktName);
  }
  else if (Format == XML_Format) {
    fprintf(fp, "<SAT-solver.output timestamp = \"%ld\" >\n", time(0));

    fprintf(fp, "  <result value = \"");
    switch (s) {
    case SAT : fprintf(fp, "satisfiable"); break;
    case UNSAT : fprintf(fp, "unsatisfiable"); break;
    case Unbestimmt : fprintf(fp, "unknown"); break;
    }
    fprintf(fp, "\" />\n");

    fprintf(fp, "  <processing-description>\n");
    fprintf(fp, "    <basic>\n");
    fprintf(fp, "      <running-time type = \"core\" sec = \"%.2f\" />\n", (double) Verbrauch / EPS);
    fprintf(fp, "      <tree-depth type = \"maximal\" count = \"%d\" />\n", Suchbaumtiefe);
    fprintf(fp, "      <autarky-reductions where = \"search-tree\" level = \"pure\" count = \"%ld\" />\n", PureL);
    fprintf(fp, "      <autarky-reductions where = \"search-tree\" level = \"non-pure\" count = \"%ld\" />\n", Autarkien);
    fprintf(fp, "      <number_of_table_enlargements count = \"%d\" />\n", Ueberschreitung2);
    fprintf(fp, "      <nodes type = \"node\" count = \"%ld\" />\n", Knoten);
    fprintf(fp, "      <nodes type = \"single\" count = \"%ld\" />\n", SingleKnoten);
    fprintf(fp, "      <nodes type = \"quasi-single\" count = \"%ld\" />\n", QuasiSingleKnoten);
    fprintf(fp, "      <nodes type = \"missed_single\" count = \"%ld\" />\n", VerSingleKnoten);
    fprintf(fp, "    </basic>\n");
    fprintf(fp, "    <extended>\n");
    fprintf(fp, "      <generalised_unit_reductions where = \"initial\" level = \"1\" count = \"%ld\" />\n", InitEinerRed);
    fprintf(fp, "      <generalised_unit_reductions where = \"search-tree\" level = \"2\" count = \"%ld\" />\n", V1KlRed);
    fprintf(fp, "      <generalised_autarky-reductions where = \"search-tree\" level = \"1\" specifier = \"total\" count = \"%ld\" />\n", FastAutarkien);
    fprintf(fp, "      <numbers_of_new_clauses list_of_clause-lengths = \"complete\" type = \"local\" specifier = \"maximum\" >\n");
    fprintf(fp, "        <nc_number length = \"2\" count = \"%ld\" />\n", maxneue2K);
    fprintf(fp, "      </numbers_of_new_clauses>\n");
    fprintf(fp, "      <numbers_of_new_clauses list_of_clause-lengths = \"complete\" type = \"local\" specifier = \"total\" >\n");
    fprintf(fp, "        <nc_number length = \"2\" count = \"%ld\" />\n", neue2Klauseln);
    fprintf(fp, "      </numbers_of_new_clauses>\n");
    fprintf(fp, "    </extended>\n");
    fprintf(fp, "  </processing-description>\n");
 
    fprintf(fp, "  <instance_specs system-name = \"%s\" >\n", aktName);

    fprintf(fp, "    <measures>\n");
    fprintf(fp, "      <reduction> <none/> </reduction>\n");
    fprintf(fp, "      <number_of_variables specifier = \"exact\" count = \"%d\" />\n", N0);
    fprintf(fp, "      <clause-length specifier = \"exact\" type = \"maximal\" value = \"%d\" />\n", P0);
    fprintf(fp, "      <total_number_of_clauses specifier = \"exact\" count = \"%d\" />\n", K0);
    fprintf(fp, "      <number_of_literal_occurrences specifier = \"exact\" count = \"%d\" />\n", L0);
    fprintf(fp, "    </measures>\n");

    fprintf(fp, "    <measures>\n");
    fprintf(fp, "      <reduction> <tautological_clauses/> <multiple_literal_occurences_in_clauses/> <unit_clause_elimination/> </reduction>\n");

    if (Knoten == 0) { // nur Vorreduktion
      if (s == UNSAT) { // Vorreduktion erzeugte die leere Klausel
	fprintf(fp, "      <numbers_of_clauses list_of_clause-lengths = \"incomplete\" >\n");
	fprintf(fp, "        <number length = \"0\" specifier = \"lower_bound\" count = \"1\" />\n");
	fprintf(fp, "      </numbers_of_clauses>\n");
      }
      else if (s == SAT) { // Vorreduktion erfuellte die Klauselmenge
	fprintf(fp, "      <total_number_of_clauses specifier = \"exact\" count = \"0\" />\n");
      }
      else { // keine Entscheidung
	fprintf(fp, "      <number_of_variables specifier = \"exact\" count = \"%d\" />\n", N);
	fprintf(fp, "      <clause-length type = \"maximal\" specifier = \"exact\" count = \"%d\" />\n", P);
	fprintf(fp, "      <total_number_of_clauses specifier = \"exact\" count = \"%d\" />\n", K);
	fprintf(fp, "      <number_of_literal_occurrences specifier = \"exact\"count = \"%d\" />\n", L);
      }
    }
    else { // nun wurde der Suchbaum aufgebaut
      fprintf(fp, "      <number_of_variables specifier = \"exact\" count = \"%d\" />\n", N);
      fprintf(fp, "      <clause-length type = \"maximal\" specifier = \"exact\" value = \"%d\" />\n", P);
      fprintf(fp, "      <total_number_of_clauses specifier = \"exact\" count = \"%d\" />\n", K);
      fprintf(fp, "      <numbers_of_clauses list_of_clause-lengths = \"complete\" >\n");
      for (unsigned int i = 0; i <= P - 2; i++)
	if (InitAnzK[i+2] != 0)
	  fprintf(fp, "        <number length = \"%d\" count = \"%d\" />\n", i+2, InitAnzK[i+2]);
      fprintf(fp, "      </numbers_of_clauses>\n");
      fprintf(fp, "      <number_of_literal_occurrences specifier = \"exact\" count = \"%d\" />\n", L);
    }
    fprintf(fp, "    </measures>\n");

    fprintf(fp, "  </instance_specs>\n");

   if (! Belegung || s != SAT || Dateiausgabe)
      fprintf(fp, "</SAT-solver.output>\n");
  }
  else
    fprintf(fp, "%1d %2d %6d %6d %6d %6.1f %7ld %6ld %6ld %8ld %8ld %5ld %4ld %4d %4d %2d %6d %6d %6d %6ld %6ld %7ld %6ld %6d %s\n",
	    s, P0, N0, K0, L0, (double) Verbrauch / EPS,
	    Knoten, SingleKnoten, QuasiSingleKnoten, V1KlRed, PureL, 
	    Autarkien, VerSingleKnoten, Suchbaumtiefe,
	    Ueberschreitung2, P0 - P, N0 - N, K0 - K, L0 - L,
	    FastAutarkien, InitEinerRed, neue2Klauseln, maxneue2K, init2Klauseln,
	    aktName);
  return;
}

char* BasisName(char* const name) {
  char* const basis = strrchr(aktName, '/');
  if (basis == NULL)
    return name;
  else
    return basis + 1;
}


/* -------------------------------------------------------------------------------- */

static FILE *fpaus = NULL; /* fuer die Ausgabe der Ergebnisse */

static void Zustandsanzeige (int signum)
{
#ifndef SYSTIME
  Verbrauch = clock() - akkVerbrauch;
#else
  times(Zeiger);
  Verbrauch = SysZeit.tms_utime - akkVerbrauch;
#endif
  Statistikzeile(stdout);
  if (Dateiausgabe) {
    Statistikzeile(fpaus);
    fflush(NULL);
  }
  signal(SIGUSR1, Zustandsanzeige);
}

jmp_buf Ausgabepunkt;

static void Abbruch (int signum)
{
  signal(SIGUSR2, Abbruch);
  signal(SIGALRM, Abbruch);
  longjmp(Ausgabepunkt, 1);
}

static FILE *fp = NULL; /* die aktuelle Eingabedatei */
static unsigned int Groesse;
static FILE *fppa = NULL; /* fuer die Ausgabe einer erfuellenden Belegung */

static unsigned int Argument;

static char *NameBel = NULL; char *NameMon = NULL;



int main(unsigned int argc, char *argv[])

{
  char *Ausgabedatei = "OKs" VERSIONSNUMMER1 "_" VERSIONSNUMMER2 "_" OPTIONENKENNUNG5 OPTIONENKENNUNG6 OPTIONENKENNUNG7 OPTIONENKENNUNG1 OPTIONENKENNUNG2 OPTIONENKENNUNG3 OPTIONENKENNUNG4".res";
  char *Version = VERSIONSNUMMER1 "." VERSIONSNUMMER2;

  signal(SIGUSR1, Zustandsanzeige);
  signal(SIGUSR2, Abbruch);
  signal(SIGALRM, Abbruch);
  if (setjmp(Ausgabepunkt))
    goto Ausgabe;

  if (Konstantenfehler())
    {
      fprintf(stderr, "%s\n", Meldung(0));
      return 1;
    }

  if (argc == 1)
    {
      fprintf(stderr, "%s\n", Meldung(3));
      return 0;
    }

  setzenStandard();

  for (Argument = 1; Argument < argc; Argument++)
    {
      if (strcmp("-V", argv[Argument]) == 0)
        {
          printf("%s %s; %s %s\n%s: %s, %s\n", Meldung(24), DATUM, Meldung(2), Version, Meldung(6), __DATE__, __TIME__);
          printf("%s", Meldung(44));
#ifdef NBAUMRES
          printf(" NBAUMRES");
#endif
#ifdef LITTAB
          printf(" LITTAB");
#endif
#ifdef SYSTIME
          printf(" SYSTIME");
#endif
#ifdef DYNAMISCH
          printf(" DYNAMISCH");
#endif
#ifdef DYNAMISCH
          printf(" FAKTOR=%.2f", FAKTOR);
#endif
#ifdef LOKALLERNEN
          printf(" LOKALLERNEN");
#endif
#ifdef NL2RED
          printf(" NL2RED");
#endif
#ifdef FASTAUTARKIE
          printf(" FASTAUTARKIE");
#endif
#ifdef KEININLINE
          printf(" KEININLINE");
#endif
#ifdef OUTPUTTREEDATAXML
	  printf(" OUTPUTTREEDATAXML");
#endif
          printf(" STANDARD=%1d", STANDARD);

          printf("\n");
          printf("%s %s\n", Meldung(45), Abstandsname);
          printf("%s %s\n", Meldung(46), Projektionsname);
        }
      else if (strcmp("-A", argv[Argument]) == 0)
        printf("%s\n", Meldung(25));
      else if (strcmp("-H", argv[Argument]) == 0)
        printf("%s\n", Meldung(26));
      else if (strcmp("-O", argv[Argument]) == 0)
        Belegung = ! Belegung;
      else if (strcmp("-F", argv[Argument]) == 0)
        Dateiausgabe = ! Dateiausgabe;
      else if (strcmp("-M", argv[Argument]) == 0)
        Monitor = ! Monitor;      
      else if (strcmp("-P", argv[Argument]) == 0)
        nurVorreduktion = ! nurVorreduktion;      
      else if (strcmp("-R", argv[Argument]) == 0)
        spezRueckgabe = ! spezRueckgabe;
      else if (strcmp("-B", argv[Argument]) == 0)
        Schranken = ! Schranken;
      else if (strcmp("-I", argv[Argument]) == 0)
        printf("%s\n%s\n", Meldung(47), Meldung(48));
      else if (strcmp("-RA", argv[Argument]) == 0)
        randomisiert = ! randomisiert;
      else if (strcmp("-DO", argv[Argument]) == 0) {
        Format = Dimacs_Format;
	spezRueckgabe = true;
      }
      else if (strcmp("-XO", argv[Argument]) == 0) {
        Format = XML_Format;
	spezRueckgabe = false;
      }
      else if (strcmp("-TO", argv[Argument]) == 0) {
        Format = Tabellen_Format;
	spezRueckgabe = false;
      }
      else if (strcmp("-specification", argv[Argument]) == 0) {
	printf("<SAT-solver.specification>\n");
	printf("  <solver-type mode = \"deterministic\">\n");
	printf("    <complete/>\n");
	printf("  </solver-type>\n");
	printf("  <solver-name base-name = \"%s\" version = \"%s\" url = \"%s\" />\n", "OKsolver", Version, "http://cs-svr1.swan.ac.uk/~csoliver/OKsolver");
	printf("  <solver-author first_name = \"Oliver\" last_name = \"Kullmann\" country = \"United Kingdom\" e-mail = \"O.Kullmann@Swansea.ac.uk\" www = \"http://cs-svr1.swan.ac.uk/~csoliver/\" />\n");
	printf("  <programming_language name = \"C++\" />\n");
	printf("  <compilation_time timestamp = \"%s %s\" />\n", __DATE__, __TIME__);
	printf("  <url for = \"executable\" value = \"");
	if (argv[0][0] == '/') // absoluter Pfadname
	  printf("%s\" />\n", argv[0]);
	else // relativer Pfadname
	  printf("%s/%s\" />\n", getenv("PWD"), argv[0]);
	printf("  <options string = \"%s\" />\n", OPTIONENKENNUNG5 OPTIONENKENNUNG6 OPTIONENKENNUNG7 OPTIONENKENNUNG1 OPTIONENKENNUNG2 OPTIONENKENNUNG3 OPTIONENKENNUNG4);
	{
	  // extern bool internal; // 28.3.2005
	  if (internal)
	    printf("  <internal/>\n");
	}
	printf("</SAT-solver.specification>\n");
      }
      else if (strncmp("-L", argv[Argument], 2) == 0)
        {
          int Nummer;
          if (sscanf(argv[Argument] + 2, "%d", &Nummer) != 1)
            {
              fprintf(stderr, "%s\n", Meldung(18));
              return 1;
            }
          if ((Nummer < 0) || (Nummer >= ANZSPRACHEN))
            {
              fprintf(stderr, "%s %2d\n", Meldung(19), ANZSPRACHEN - 1);
              return 1;
            }
          Sprache = Nummer;
        }
      else if (strncmp("-S", argv[Argument], 2) == 0)
        {
          int Nummer;
          if (sscanf(argv[Argument] + 2, "%d", &Nummer) != 1)
            {
              fprintf(stderr, "%s\n", Meldung(20));
              return 1;
            }
          if ((Nummer < 0) || (Nummer > ANZSTANDARDS))
            {
              fprintf(stderr, "%s %2d\n", Meldung(21), ANZSTANDARDS);
              return 1;
            }
          Standard = Nummer;
          setzenStandard();
        }
      else if (strncmp("-D", argv[Argument], 2) == 0)
	// -D fuer Beobachtungstiefe (depth)
	// Verzweigungsliterale werden (falls ueberhaupt Dateiausgabe gesetzt ist)
	// nur bis zu zwei Stufen unterhalb der Beobachtungsschicht ausgegeben,
	// so dass hierfuer als das Beobachtungsniveau mindestens zwei sein
	// sollte.
        {
          int Nummer;
          if (sscanf(argv[Argument] + 2, "%d", &Nummer) != 1)
            {
              fprintf(stderr, "%s\n", Meldung(31));
              return 1;
            }
          if (Nummer < 0)
            {
              fprintf(stderr, "%s\n", Meldung(32));
              return 1;
            }
          Beobachtungsniveau = Nummer;
        }
      else if (strncmp("-MAXN=", argv[Argument], 6) == 0)
        {
          int maxn;
          if (sscanf(argv[Argument] + 6, "%d", &maxn) != 1)
            {
              fprintf(stderr, "%s\n", Meldung(33));
              return 1;
            }
          if (maxn < 0)
            {
              fprintf(stderr, "%s\n", Meldung(34));
              return 1;
            }
          MAXN = maxn;
        }
      else if (strncmp("-MAXL=", argv[Argument], 6) == 0)
        {
          int maxl;
          if (sscanf(argv[Argument] + 6, "%d", &maxl) != 1)
            {
              fprintf(stderr, "%s\n", Meldung(36));
              return 1;
            }
          if (maxl < 0)
            {
              fprintf(stderr, "%s\n", Meldung(37));
              return 1;
            }
          MAXL = maxl;
        }
      else if (strncmp("-MAXK=", argv[Argument], 6) == 0)
        {
          int maxk;
          if (sscanf(argv[Argument] + 6, "%d", &maxk) != 1)
            {
              fprintf(stderr, "%s\n", Meldung(39));
              return 1;
            }
          if (maxk < 0)
            {
              fprintf(stderr, "%s\n", Meldung(40));
              return 1;
            }
          MAXK = maxk;
        }
      else if (strncmp("-T", argv[Argument], 2) == 0)
        {
          int t;
          if (sscanf(argv[Argument] + 2, "%d", &t) != 1)
            {
              fprintf(stderr, "%s\n", Meldung(42));
              return 1;
            }
          if (t < 0)
            {
              fprintf(stderr, "%s\n", Meldung(43));
              return 1;
            }
          Zeitschranke = t;
        }
      else if (strncmp("-seed=", argv[Argument], 6) == 0)
        {
          long unsigned int S;
          if (sscanf(argv[Argument] + 6, "%lud", &S) != 1)
            {
              fprintf(stderr, "%s\n", Meldung(52));
              return 1;
            }
          Schluessel = S;
        }
      else if (strncmp("-quot=", argv[Argument], 6) == 0)
        {
          double V;
          if (sscanf(argv[Argument] + 6, "%lf", &V) != 1)
            {
              fprintf(stderr, "%s\n", Meldung(53));
              return 1;
            }
          Verhaeltnis = V;
        }
      else if (strncmp("-", argv[Argument], 1) == 0)
        {
          fprintf(stderr, "%s %s\n", Meldung(22), argv[Argument]);
          return 1;
        }
      else
        {
          aktName = argv[Argument];
          s = Unbestimmt;
          alarm(Zeitschranke);
#ifndef SYSTIME
          akkVerbrauch = clock();
#else
          times(Zeiger);
          akkVerbrauch = SysZeit.tms_utime;
#endif
          if (randomisiert)
            srand_S();
          if ((fp = fopen(aktName, "r")) == NULL)
            {
              fprintf(stderr, "%s %s\n", Meldung(4), aktName);
              return 1;
            }
          {
            struct stat stbuf;
            if (stat(aktName, &stbuf) == -1)
              {
                fprintf(stderr, Meldung(7), aktName);
                return 1;
              }
            Groesse = stbuf.st_size;
          }

          if (Dateiausgabe)
            {
              if ((fpaus = fopen(Ausgabedatei, "a")) == NULL)
                {
                  fprintf(stderr, "%s %s\n", Meldung(30), Ausgabedatei);
                  return 1;
                }
              if (Belegung || (Monitor && (! nurVorreduktion)))
                {
                  Wurzel = BasisName(aktName);
                  if (Belegung)
                    {
                      NameBel = (char *) xmalloc((strlen(Wurzel) + 3 + 1) * sizeof(char));
                      strcpy(NameBel, Wurzel); strcat(NameBel, ".pa");
                    }
                  if (Monitor && (! nurVorreduktion))
                    {
                      NameMon = (char *) xmalloc((strlen(Wurzel) + 3 + 1) * sizeof(char));
                      strcpy(NameMon, Wurzel); strcat(NameMon, ".mo");
                      if ((fpmo = fopen(NameMon, "w")) == NULL)
                        {
                          fprintf(stderr, "%s %s\n", Meldung(29), NameMon);
                          return 1;
                        }
                    }
                }     
            }

#ifdef OUTPUTTREEDATAXML
	  {
	    if (not Wurzel) Wurzel = BasisName(aktName);
	    NameTreeDataFile = (char*) xmalloc((strlen(Wurzel) + 4 + 1));
	    strcpy(NameTreeDataFile, Wurzel); strcat(NameTreeDataFile, ".xml");
	    if ((TreeDataFile = fopen(NameTreeDataFile, "w")) == NULL) {
	      fprintf(stderr, "%s %s\n", Meldung(54), NameTreeDataFile);
	      return 1;
	    }
	    fprintf(TreeDataFile, "<?xml version=\"1.0\" standalone=\"yes\" ?>\n");
	    fprintf(TreeDataFile, "<!DOCTYPE t [\n");
	    fprintf(TreeDataFile, "  <!ELEMENT t (t?, t?)>\n");
	    fprintf(TreeDataFile, "  <!ATTLIST t\n");
	    fprintf(TreeDataFile, "    l NMTOKEN #REQUIRED>\n");
	    fprintf(TreeDataFile, "]>\n\n");
	    // If already in the preprocessing phase the formula is decided, then no tree-xml-element is output, and thus the file with name NameTreeDataFile does not contain correct xml.
	  }
#endif

          switch (Einlesen(fp, Groesse)) {
          case Sat : s = SAT; break;
          case Unsat : s = UNSAT; break;
          case Fehler :
            fprintf(stdout, "%s %s.\n", Meldung(17), aktName);
            if (Dateiausgabe)
              fprintf(fpaus, "%s %s.\n", Meldung(17), aktName);
            goto Aufraeumen;
          case Norm :
            if (nurVorreduktion)
              break;
            InitVarLitKlm();
            InitSat();
#ifdef LOKALLERNEN
            InitlokalesLernen();
#endif
            if (Monitor)
              {
                printf("\n%s\n %s, %4d\n", Meldung(28), aktName, Gesamtlast);
                if (Dateiausgabe)
                  fprintf(fpmo, "%s\n %s, %4d\n", Meldung(28), aktName, Gesamtlast);
              }
            
            s = SATEntscheidung();
          }

Ausgabe :

#ifndef SYSTIME
          Verbrauch = clock() - akkVerbrauch;
#else
          times(Zeiger);
          Verbrauch = SysZeit.tms_utime - akkVerbrauch;
#endif
          if (Monitor)
            printf("\n");
          Statistikzeile(stdout);
          if (Dateiausgabe)
            Statistikzeile(fpaus);

/* Achtung: Die Analyse der Ausgabe verlangt, dass das allererste */
/*             Zeichen die SAT-Zugehoerigkeit (d.h.: 0 oder 1) angibt. */

          if (Belegung && (s == SAT))
            {
              if (! Dateiausgabe)
                AusgabeBelegung(stdout);
              else
                {
                  if ((fppa = fopen(NameBel, "w")) == NULL)
                    {
                      fprintf(stderr, "%s %s\n", Meldung(27), NameBel);
                      return 1;
                    }
                  AusgabeBelegung(fppa);
                }
            }
        Aufraeumen :

        alarm(0);
        AufraeumenSat();
#ifdef BAUMRES
        AufraeumenBaumRes();
#endif
#ifdef LOKALLERNEN
        AufraeumenlokalesLernen();
#endif
        AufraeumenEinlesen(); /* zuletzt */
        free(NameBel); NameBel = NULL;
        free(NameMon); NameMon = NULL;
#ifdef OUTPUTTREEDATAXML
	fprintf(TreeDataFile, "\n");
	free(NameTreeDataFile); NameTreeDataFile = NULL;
	if (TreeDataFile != NULL) {
	  fclose(TreeDataFile); TreeDataFile = NULL;
	}
#endif
        if (fp != NULL)
            {
              fclose(fp); fp = NULL;
            }
          if (fpmo != NULL)
            {
              fclose(fpmo); fpmo = NULL;
            }
          if (fpaus != NULL)
            {
              fclose(fpaus); fpaus = NULL;
            }
          if (fppa != NULL)
            {
              fclose(fppa); fppa = NULL;
            }
        }
    }

  if (spezRueckgabe)
    switch (s) {
    case SAT : return 10;
    case UNSAT : return 20;
    case Unbestimmt : return 0;
    }
  else
    return 0;
}
                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* 19.1.2001, UofT */

/* Einheit: Ausgaben */

/* Ausgaben (Fehler, Ergebnisse und Zustandsmeldungen) */

#include <stdio.h>

#include "OK.h"
#include "Ausgaben.h"



const char *Meldungen[ANZSPRACHEN][55] =
{
  {"Fehler in der Definition der Konstanten! Abbruch.", /* 0 */
   "Kombination von BAUMRES mit LOKALLERNEN zur Zeit noch nicht moeglich.", /* 1 */
   "Version", /* 2 */
   "Verwendung von OKsolver: Optionen, Dateiname1, Optionen, Dateiname2, ...", /* 3 */
   "Die Eingabedatei existiert nicht oder ist lesegeschuetzt:", /* 4 */
   "Virtueller Speicher nicht ausreichend! Abbruch.", /* 5 */
   "Datum der Uebersetzung", /* 6 */
   "Die Groesse der Eingabe-Datei kann nicht bestimmt werden:", /* 7 */
   "Syntax-Fehler!", /* 8 */
   "Syntax-Fehler! Kommentarzeilen muessen auch am Dateiende mit dem Zeilenende-Zeichen abgeschlossen werden.", /* 9 */
   "Syntax-Fehler! Jede Klausel muss mit dem Klausel-End-Symbol abgeschlossen werden --- auch die leere Klausel am Dateiende.", /* 10 */
   "Position in der Eingabe-Datei und fehlerhaftes Zeichen:", /* 11 */
   "Zeilennummer:", /* 12 */
   "Syntax-Fehler! Der Beginn eines Variablennamens wird erwartet.", /* 13 */
   "Syntax-Fehler! Jede Klausel muss mit dem Klausel-End-Symbol abgeschlossen werden --- auch am Dateiende.", /* 14 */
   "Syntax-Fehler! Es muss ein neues Literal folgen am Dateiende.", /* 15 */
   "Syntax-Fehler! Es muss ein neues Literal folgen.", /* 16 */
   "Syntaxfehler in", /* 17 */
   "Der Option -L muss eine Nummer folgen.", /* 18 */
   "Nummern fuer Sprachen sind 0, ..,", /* 19 */
   "Der Option -S muss eine Nummer folgen.", /* 20 */
   "Nummern fuer Standards sind 1, ..,", /* 21 */
   "Die folgende Option wurde nicht erkannt:", /* 22 */
   "Position in der Zeile:", /* 23 */
   "OKsolver: 5.3.1998 -", /* 24 */
   "Oliver Kullmann (Frankfurt, Toronto, Swansea); O.Kullmann@Swansea.ac.uk", /* 25 */
   "OKsolver wird mit einer Liste von Argumenten aufgerufen, die der Reihe nach abgearbeitet werden. Neben Dateinamen (fuer Klauselmengen) koennen dies die folgenden Optionen sein:\n-V -A -H -O -F -M -P -R -B -I -RA -Li -Si -Di -MAXN= -MAXK= -MAXL= -Ti -seed= -quot=\nwobei i fuer eine natuerliche Zahl steht.", /* 26 */
   "Die folgende Datei konnte nicht zum Einschreiben der erfuellenden Belegung geoeffnet werden:", /* 27 */
   "Name der Datei und Anzahl der Beobachtungsknoten:", /* 28 */
   "Die folgende Datei konnte nicht zum Einschreiben der Ueberwachungsmeldungen geoeffnet werden:", /* 29 */
   "Die folgende Datei konnte nicht zum Eintragen der Ergebnisse geoeffnet werden:", /* 30 */
   "Der Option -D muss eine natuerliche Zahl folgen.", /* 31 */
   "Das Beobachtungsniveau kann nicht negativ sein.", /* 32 */
   "Der Option -MAXN= muss eine natuerliche Zahl folgen.", /* 33 */
   "Die maximale Variablenanzahl kann nicht negativ sein.", /* 34 */
   "Die maximale Variablenanzahl wurde ueberschritten:", /* 35 */
   "Der Option -MAXL= muss eine natuerliche Zahl folgen.", /* 36 */
   "Die maximale Anzahl von Literalvorkommen kann nicht negativ sein.", /* 37 */
   "Die maximale Anzahl von Literalvorkommen wurde ueberschritten.", /* 38 */
   "Der Option -MAXK= muss eine natuerliche Zahl folgen.", /* 39 */
   "Die maximale Klauselanzahl kann nicht negativ sein.", /* 40 */
   "Die maximale Klauselnanzahl wurde ueberschritten.", /* 41 */
   "Der Option -T muss eine natuerliche Zahl folgen.", /* 42 */
   "Zeit wird durch eine natuerliche Zahl (>=0) angegeben (Sekunden).", /* 43 */
   "Uebersetzer-Optionen:", /* 44 */
   "Datei mit Abstandsfunktion:", /* 45 */
   "Datei mit Projektionsfunktion:", /* 46 */
   "Interpretation der Ausgabezeile:", /* 47 */
   "SAT, P0, N0, K0, L0, Zeit, Knoten, Single-Knoten, Quasi-Single-Knoten, 2-Reduktionen, pure Literale, Autarkien, verpasste Single-Knoten, maximale Suchbaumtiefe, Ueberschreitung2, P0 - P, N0 - N, K0 - K, L0 - L, Fastautarkien, initiale 1-Reduktionen, neue 2-Klauseln (insgesamt), neue 2-Klauseln maximal, initiale 2-Klauseln, Dateiname", /* 48 */
   "FASTAUTARKIE setzt LOKALLERNEN voraus!", /* 49 */
   "Im Standard1 sind keine leeren Klauseln erlaubt.", /* 50 */
   "Im Standard1 muss zwischen dem durch 0 markierten Klauselende und einem folgenden Kommentar oder neuer Klausel ein Leerzeichen stehen.", /* 51 */
   "Der Option -seed= muss eine Zahl folgen.", /* 52 */
   "Der Option -quot= muss eine Zahl folgen.", /* 53 */
   "Die folgende Datei konnte nicht zum Einschreiben der Baumdaten geoeffnet werden: (XML; Makro OUTPUTTREEDATAXML definiert)", /* 54 */
  },

  {"Error in the definition of the constants! Abbort.", /* 0 */
   "Combination of BAUMRES and LOKALLERNEN yet not possible.", /* 1 */
   "version", /* 2 */
   "When calling OKsolver, exactly one parameter is needed, the name of the input clause-set.", /* 3 */
  "The input file doesn't exist or is read-protected:", /* 4 */
   "Virtual memory exhausted! Exit.", /* 5 */
   "Date of compilation", /* 6 */
   "The size of the input file can not be determined:", /* 7 */
   "Syntax error!", /* 8 */
   "Syntax error! Comment lines must be closed with the end-of-line character also at the end of the input file.", /* 9 */
   "Syntax error! Each clause must be completed with the clause-end-symbol --- also the empty clause at the end of the file.", /* 10 */
   "The position in the input file and the wrong character are:", /* 11 */
   "Line number:", /* 12 */
   "Syntax error! The begin of a name of a variable is expected.", /* 13 */
   "Syntax error! Each clause must be completed with the clause-end-symbol --- also at the end of the file.", /* 14 */
   "Syntax error! There must be another literal at the end of file.", /* 15 */
   "Syntax error! There must be another literal.", /* 16 */
   "Syntax error in", /* 17 */
   "Option -L must be followed by a number", /* 18 */
   "Numbers for languages are 0, ...,", /* 19 */
   "Option -L must be followed by a number", /* 20 */
   "Numbers for standards are 1, ...,", /* 21 */
   "The following option has not been recognized:", /* 22 */
   "position in the line:", /* 23 */
   "OKsolver: 5.3.1998 -", /* 24 */
   "Oliver Kullmann (Frankfurt, Toronto, Swansea); O.Kullmann@Swan.ac.uk", /* 25 */
   "OKsolver is called with a list of arguments, which will be processed consecutively. The arguments can be names of files (for clause-sets) as well as the following options:\n-V -A -H -O -F -M -P -R -B -I -RA -Li -Si -Di -MAXN= -MAXK= -MAXL= -Ti -seed= -quot=\nwhere i is a non-negative integer.", /* 26 */
   "The following file could not be opened for writing the satisfying partial assignment:", /* 27 */
   "Name of file and number of monitoring nodes:", /* 28 */
   "The following file could not be opened for writing the monitor messages:", /* 29 */
   "The following file could not be opened for adding the results:", /* 30 */
   "Option -D must be followed by a non-negative integer.", /* 31 */
   "The monitor level can not be negative.", /* 32 */
   "Option -MAXN must be followed by a non-negative integer.", /* 33 */
   "The maximal number of variables can not be negative.", /* 34 */
   "The maximal number of variables has been exceeded:", /* 35 */
   "Option -MAXL must be followed by a non-negative integer.", /* 36 */
   "The maximal number of literal occurrences can not be negative.", /* 37 */
   "The maximal number of literal occurrences has been exceeded:", /* 38 */
   "Option -MAXK must be followed by a non-negative integer.", /* 39 */
   "The maximal number of clauses can not be negative.", /* 40 */
   "The maximal number of clauses has been exceeded:", /* 41 */
   "Option -T must be followed by a non-negative integer.", /* 42 */
   "Time must be given by a non-negative integer (seconds).", /* 43 */
   "Compiler options:", /* 44 */
   "File with distance function:", /* 45 */
   "File with projection function:", /* 46 */
   "Interpretation of the output line:", /* 47 */
   "SAT, P0, N0, K0, L0, time, nodes, single nodes, quasi single nodes, 2-reductions, pure literals, autarkies, missed single nodes, maximal depth, Ueberschreitung2, P0 - P, N0 - N, K0 - K, L0 - L, near-autarkies, initial 1-reductions, new 2-clauses (total), new 2-clauses maximal, initial 2-clauses, file name", /* 48 */
   "FASTAUTARKIE needs LOKALLERNEN!", /* 49 */
   "No empty clauses are allowed with Standard1.", /* 50 */
   "In Standard1 after a clause-end marked by 0 there must be a space before another clause or comment.", /* 51 */
   "Option -seed= must be followed by a number.", /* 52 */
   "Option -quot= must be followed by a number.", /* 53 */
   "The following file could not be opened for writing the tree data (xml; Macro OUTPUTTREEDATAXML defined):", /* 54 */
  },
};

const char *Meldung(unsigned int i)
{
  return Meldungen[Sprache][i];
}


char Konstantenfehler ( void )
{
#ifdef BAUMRES
#ifdef LOKALLERNEN
  fprintf(stderr, "%s\n", Meldung(1));
  return 1;
#endif
#endif
#ifdef FASTAUTARKIE
#ifndef LOKALLERNEN
  fprintf(stderr, "%s\n", Meldung(49));
  return 1;
#endif
#endif
  if ((Sprache < 0) || (Sprache >= ANZSPRACHEN))
    {
      fprintf(stderr, "%s %2d\n", Meldung(19), ANZSPRACHEN - 1);
      return 1;
    }
  if ((Standard < 0) || (Sprache > ANZSTANDARDS))
    {
      fprintf(stderr, "%s %2d\n", Meldung(21), ANZSTANDARDS);
      return 1;
    }

  return 0;
}
                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* angelegt 19.1.2001, UofT */

/* 13.2.2001: Berechnung der MAX-Werte in der Prozedur "Schranken" */

/* Einheit: Einlesen */

/* Einlesen der Eingabe */


#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <string.h> /* fuer C++ (memset; 14.8.2001) */


#include "OK.h"
#include "VarLitKlm.h"
#include "anlVar.h"
#include "Parameter.h"
#include "Einlesen.h"
#include "Ausgaben.h"
#include "Speicher.h"
#include "Belegung.h"

extern bool nurVorreduktion;
extern bool Schranken;

static unsigned int maxn, maxl, maxk, maxlk;
/* maxn ist die maximale Anzahl von Variablen, */
/* maxlk die maximale Summe von Literalanzahl und Klauselanzahl */
static bool unsichermaxn;
static bool unsichermaxl;
static bool unsichermaxk;
static unsigned int M;  /* Groesse der Hashtabelle */
/* fuer die Hashtabelle, falls Schranken gesetzt: */
static const unsigned int Lastfaktor = 4;


static char *Eingabesymbole;  /* zeigt immer auf den aktuellen Symbolanfang */
static char *freiSymbole;  /* zeigt auf den ersten freien Platz */
static char *Eingabesymbole0 = NULL;

/* Zeiger auf dynamisch erzeugten Speicherplatz muesse NULL sein, */
/* wenn sie nicht auf solchen zeigen. */
  
static unsigned int *Hashtabelle = NULL;

static int *LitTab = NULL;
/* eingelesene Multiklauselmenge, Literal fuer Literal, wobei */
/* Klauselnenden durch NullLiteral angezeigt werden. */

static int *aktKlauselAnfang;
static int *aktfreies;  /* bzgl. LitTab */

static bool leereKlausel;
static bool Tautologie; /* ist die aktuelle Klausel tautologisch? */
static bool globalTautologie; /* gibt es insgesamt eine Tautologie? */
static bool EinerKlausel;

static unsigned int aktp, aktp0;

static void *Z0 = NULL; /* Zeiger auf lokale Variablen */

static void *Basis0 = NULL;
/* der Hauptteil der dynamisch erzeugten Variablen (fuer's Hauptprogramm) */


/* der Typ des Variablensymbols in der Eingabe: */

typedef char *VarSym;

/* Variablen koennen in der Eingabe beliebige Nummern haben, so ist z.B. */
/*    F = (123,-4,5)(-5,8,123) */
/* erlaubt. In VarTab[i] finden wir zur Variablen i mit 1 <= i <= N die urspruengliche */
/* Nummer, wobei sich i auf das Vorkommen bezieht: */
/*    VarTab[1] = 123, */
/*    VarTab[2] = 4 */
/*    VarTab[3] = 5, */
/*    VarTab[4] = 8. */
/* VarTab wird nur im Falle, dass eine erfuellende Belegung ausgegeben werden soll, */
/* als globale Variable gefuehrt. */

static VarSym *VarTab = NULL;  /* VarTab[v] fuer Variable v */

static unsigned int *VarTab1 = NULL;
static unsigned *VarTab2 = NULL;

static int *Pfad0 = NULL;



/* --------------------------------------------------- */


/* Syntaxerkennung */

bool Beginn[CHAR_MAX];
bool Ende[CHAR_MAX];
bool Trenner[CHAR_MAX];
bool Negator[CHAR_MAX];
bool Kommentar[CHAR_MAX];

void setzenStandard(void)
{
  int i;
  switch (Standard) {
  case 1 :
    for (i = 0; i <= CHAR_MAX; i++)
      Negator[i] = Kommentar[i] = false;
    Negator['-'] = true;
    Kommentar['c'] = Kommentar['p'] = true;
    break;
  case 2 :
    for (i = 0; i <= CHAR_MAX; i++)
      Beginn[i] = Ende[i] = Trenner[i] = Negator[i] = Kommentar[i] = false;
    Beginn['('] = true;
    Ende[')'] = true;
    Trenner[','] = true;
    Negator['-'] = true;
    Kommentar['%'] = true;
    break;
  case 3 :
    for (i = 0; i <= CHAR_MAX; i++)
      Beginn[i] = Ende[i] = Negator[i] = Kommentar[i] = false;
    Beginn['('] = true;
    Ende[')'] = true;
    Negator['-'] = true;
    Kommentar['%'] = true;
    break;
  case 4 :
    for (i = 0; i <= CHAR_MAX; i++)
      Negator[i] = Kommentar[i] = false;
    Negator['-'] = true;
    Kommentar['%'] = true;
    break;
  }
}


__inline__ bool Kommzeichen(char c)
{
  return Kommentar[(int) c];
}


__inline__ bool Sepzeichen(char c)
{
  return Trenner[(int) c];
}


__inline__ bool Negzeichen(char c)
{
  return Negator[(int) c];
}


__inline__ bool Variablenanfang(char c)
{
  if (Standard == 1)
    return (isalnum(c) && (! Kommentar[(int) c]) && (c != '0'));
  else
    return isalnum(c);
}


__inline__ bool Klauselbeginn(char c)
{
  return Beginn[(int) c];
}


__inline__ bool Klauselende(char c)
{
  return Ende[(int) c];
}

/* --------------------------------------------------- */


__inline__ bool uebernehmenLiteral(int l)
/* gibt false zurueck im Fehlerfalle */
{
  int *Lauf;

  L0++; aktp0++;
  if (Tautologie)
    return true;
  for (Lauf = aktKlauselAnfang; Lauf != aktfreies; Lauf++)
    if (*Lauf == -l)
      {
	Tautologie = globalTautologie = true;
	break;
      }
    else if (*Lauf == l)
      break;

  if (Tautologie)
    {
      aktfreies = aktKlauselAnfang;
      L -= aktp;
      return true;
    }
  else if (Lauf == aktfreies)
    {
      if (unsichermaxl && L >= maxl)
	return false;
      aktp++;
      L++;
      *(aktfreies++) = l;
    }
  return true;
}

__inline__ bool uebernehmenKlausel(void)
{
  K0++;
  if (aktp0 > P0)
    P0 = aktp0;
  if (! Tautologie)
    {
      if (unsichermaxk && K >= maxk)
	return false;
      K++;
      if (aktp > P)
	P = aktp;
      if (aktp == 1)
	EinerKlausel = true;
      *(aktfreies++) = 0;
      aktKlauselAnfang = aktfreies;
    }
  return true;
}


/* --------------------------------------------------- */

/* Hashverwaltung */

unsigned int Hash(char *Name)
{
  unsigned int h = 0, a = 127;
  for (; *Name != '\0'; Name++)
    h = (a * h + (unsigned int) *Name) % M;
  return h;
}

unsigned int eintragen(void)
/* Berechnet die Variablennummer 1, ... zum in Eingabesymbole */
/* abgespeicherten Symbol. */
/* Gibt 0 zurueck, falls Eintragen in die Hashtabelle */
/* nicht moeglich war. */
{
  unsigned int i; unsigned int v;

  i = Hash(Eingabesymbole);
  while ((v = *(Hashtabelle + i)) != 0)
    {
      if (strcmp(Eingabesymbole, *(VarTab + v)) == 0)
	{
	  freiSymbole = Eingabesymbole;
	  return v;
	}
      else
	i = (i + 1) % M;
    }
  if (unsichermaxn && (N0 >= maxn))
    return 0;
  N0++;
  *(VarTab + N0) = Eingabesymbole;
  *(Hashtabelle + i) = N0;
  Eingabesymbole = freiSymbole;
  return N0;
}


/* --------------------------------------------------- */

void Fehlerumgebung(FILE *fp, char c)
{
  long Position;
  long Zeile, p, letzteZeile;

  Position = ftell(fp);
  fprintf(stderr, "%s %2ld, ", Meldung(11), Position);
  if (isgraph(c))
    {
      fprintf(stderr, "%c\n", c);
    }
  else
    fprintf(stderr, "\\%03d\n", c);
  rewind(fp);
  Zeile = 1; letzteZeile = 0;
  for (p = 1; p < Position; p++)
    {
      c = fgetc(fp);
      if (c == '\n')
	{
	  Zeile++;
	  letzteZeile = p;
	}
    }
  fprintf(stderr, "%s %2ld; %s %2ld\n", Meldung(12), Zeile, Meldung(23), Position - letzteZeile);
  return;
}


/* --------------------------------------------------- */



TEIN Einlesen(FILE *fp, unsigned int G)

{  
  char c; unsigned int v; VZ e;

  P = P0 = N = N0 = K = K0 = L = L0 = 0;
  
  if (G == 0)
    return Sat;

  {
    unsigned int trivSchranke;
    trivSchranke = (unsigned int) ceil(G / 2.0); /* laesst Spiel von +1 zu */
    if (Schranken && (MAXN < trivSchranke))
      {
	maxn = MAXN;
	M = Lastfaktor * maxn; /* Groesse der Hashtabelle */
	unsichermaxn = true;
      }
    else
      {
	maxn = trivSchranke;
	unsichermaxn = false;
	M = trivSchranke;
	if (M % 127 == 0)
	  M++;
      }
    if (Schranken && (MAXL < trivSchranke))
      {
	maxl = MAXL;
	unsichermaxl = true;
      }
    else
      {
	maxl = trivSchranke;
	unsichermaxl = false;
      }
    if (Schranken && (MAXK < trivSchranke))
      {
	maxk = MAXK;
	unsichermaxk = true;
      }
    else
      {
	maxk = trivSchranke;
	unsichermaxk = false;
      }
	
    maxlk = maxl + maxk;
  }

  Hashtabelle = (unsigned int *) xmalloc(M * sizeof(unsigned int));
       
  aktKlauselAnfang = aktfreies = LitTab = (int *) xmalloc(maxlk * sizeof(int));

  freiSymbole = Eingabesymbole = Eingabesymbole0 = (char *) xmalloc(G * sizeof(char));

  VarTab = (VarSym *) xmalloc((maxn + 1) * sizeof(VarSym));

  memset((void *) Hashtabelle, 0, M * sizeof(unsigned int));

/*  1. Phase: Berechnung von LitTab und VarTab sowie von */
/*    P, K, L und P0, N0, K0, L0. */
/*  P0, N0, K0, L0 sind hierbei die urspuenglichen Groessen, */
/*    waehrend P, K, L die Streichung von tautologischen Klauseln  */
/*    und Mehrfachvorkommen von Literalen in Klauseln beruecksichtigen. */
/*  Format von LitTab: */
/*    Variablen sind von 1 ab durchnumeriert, das Komplement von v ist -v, */
/*    und Klauselenden werden durch 0 markiert. */

 leereKlausel = globalTautologie = Tautologie = EinerKlausel = false;
 aktp = aktp0 = 0;

 c = getc(fp);
 switch (Standard) {
 case 1 : goto S1;
 case 2 : goto S2;
 case 3 : goto S3;
 case 4 : goto S4;
 }

S1:

 ZustandS11 :

    if (c == EOF)
      goto Phase2;
    else if (isspace(c))
      {
	c = getc(fp); goto ZustandS11;
      }
    else if (Kommzeichen(c))
      {
	c = getc(fp); goto ZustandS12;
      }
    else if (Variablenanfang(c))
      {
	*(freiSymbole++) = c;
	e = Pos; Tautologie = false;
	aktp0 = aktp = 0;
	c = getc(fp); goto ZustandS15;
      }
    else if (Negzeichen(c))
      {
	e = Neg; Tautologie = false;
	aktp0 = aktp = 0;
	c = getc(fp); goto ZustandS14;
      }
    else
      {
	if (c == '0')
	  fprintf(stderr, "%s\n", Meldung(50));
	else
	  fprintf(stderr, "%s\n", Meldung(8));
	Fehlerumgebung(fp, c);
	return Fehler;
      }
 
 ZustandS12 :

   if (c == EOF)
     {
       fprintf(stderr, "%s\n", Meldung (9));
       return Fehler;
     }
   else if (c == '\n')
     {
       c = getc(fp); goto ZustandS11;
     }
   else
     {
       c = getc(fp); goto ZustandS12;
     }
 
 ZustandS14 :

   if (isspace(c))
     {
       c = getc(fp); goto ZustandS14;
     }
   else if (Variablenanfang(c))
     {
       *(freiSymbole++) = c;
       c = getc(fp); goto ZustandS15;
     }
   else
     {
       if (c == EOF)
	 fprintf(stderr, "%s\n", Meldung(14));
       else
	 {
	   fprintf(stderr, "%s\n", Meldung(13));
	   Fehlerumgebung(fp, c);
	 }
       return Fehler;
     }

 ZustandS15 :

   if (isspace(c))
     {
       *(freiSymbole++) = '\0';
       v = eintragen();
       if (v == 0)
	 {
	   fprintf(stderr, "%s %6d\n", Meldung(35), maxn);
	   return Fehler;
	 }
       if (! uebernehmenLiteral((e == Pos) ? (int) v : - (int) v))
	 {
	   fprintf(stderr, "%s %7d\n", Meldung(38), maxl);
	   return Fehler;
	 }
       c = getc(fp); goto ZustandS16;
     }
   else if (isalnum(c))
     {
       *(freiSymbole++) = c;
       c = getc(fp); goto ZustandS15;
     }
   else
     {
       if (c == EOF)
	 fprintf(stderr, "%s\n", Meldung(14));
       else
	 {
	   fprintf(stderr, "%s\n", Meldung(8));
	   Fehlerumgebung(fp, c);
	 }
       return Fehler;
     }
 
 ZustandS16 :
   
   if (c == '0')
     {
       if (! uebernehmenKlausel())
	 {
	   fprintf(stderr, "%s %6d\n", Meldung(41), maxk);
	   return Fehler;
	 }
       c = getc(fp); goto ZustandS17;
     }
   else if (isspace(c))
     {
       c = getc(fp); goto ZustandS16;
     }
   else if (Variablenanfang(c))
     {
       *(freiSymbole++) = c;
       e = Pos;
       c = getc(fp); goto ZustandS15;
     }
   else if (Negzeichen(c))
     {
       e = Neg;
       c = getc(fp); goto ZustandS14;
     }
   else
     {
       if (c == EOF)
	 fprintf(stderr, "%s\n", Meldung(14));
       else
	 {
	   fprintf(stderr, "%s\n", Meldung(8));
	   Fehlerumgebung(fp, c);
	 }
       return Fehler;
     }

 ZustandS17 :
   
   if (c == EOF)
     goto Phase2;
   else if (isspace(c))
     {
       c = getc(fp); goto ZustandS11;
     }
   else
     { 
       fprintf(stderr, "%s\n", Meldung(51));
       Fehlerumgebung(fp, c);
       return Fehler;
     }


/* ---------------------- */


S2:

 ZustandS21 :

   if (c == EOF)
     goto Phase2;
   else if (isspace(c))
     {
       c = getc(fp); goto ZustandS21;
     }
   else if (Kommzeichen(c))
     {
       c = getc(fp); goto ZustandS22;
     }
   else if (Klauselbeginn(c))
     {
       c = getc(fp); goto ZustandS23;
     }
   else
     {
       fprintf(stderr, "%s\n", Meldung(8));
       Fehlerumgebung(fp, c);
       return Fehler;
     }
 
 ZustandS22 :
   
   if (c == EOF)
     {
       fprintf(stderr, "%s\n", Meldung (9));
       return Fehler;
     }
   else if (c == '\n')
     {
       c = getc(fp); goto ZustandS21;
     }
   else
     {
       c = getc(fp); goto ZustandS22;
     }
 
 ZustandS23 :

   if (Klauselende(c))
     {
       leereKlausel = true; K0++; K++;
       c = getc(fp); goto ZustandS21;
     }
   else if (isspace(c))
     {
       c = getc(fp); goto ZustandS23;
     }
   else if (isalnum(c))
     {
       *(freiSymbole++) = c;
       e = Pos; Tautologie = false;
       aktp0 = aktp = 0;
       c = getc(fp); goto ZustandS25;
     }
   else if (Negzeichen(c))
     {
       e = Neg; Tautologie = false;
       aktp0 = aktp = 0;
       c = getc(fp); goto ZustandS24;
     }
   else
     {
       if (c == EOF)
	 fprintf(stderr, "%s\n", Meldung(10));
       else
	 {
	   fprintf(stderr, "%s\n", Meldung(8));
	   Fehlerumgebung(fp, c);
	 }
       return Fehler;
     }

 ZustandS24 :

   if (isspace(c))
     {
       c = getc(fp); goto ZustandS24;
     }
   else if (isalnum(c))
     {
       *(freiSymbole++) = c;
       c = getc(fp); goto ZustandS25;
     }
   else
     {
       if (c == EOF)
	 fprintf(stderr, "%s\n", Meldung(14));
       else
	 {
	   fprintf(stderr, "%s\n", Meldung(13));
	   Fehlerumgebung(fp, c);
	 }
       return Fehler;
     }
 
 ZustandS25 :
   
   if (Klauselende(c))
     {
       *(freiSymbole++) = '\0';
       v = eintragen();
       if (v == 0)
	 {
	   fprintf(stderr, "%s %6d\n", Meldung(35), maxn);
	   return Fehler;
	 }
       if (! uebernehmenLiteral((e == Pos) ? (int) v : - (int) v))
	 {
	   fprintf(stderr, "%s %7d\n", Meldung(38), maxl);
	   return Fehler;
	 }
       if (! uebernehmenKlausel())
	 {
	   fprintf(stderr, "%s %6d\n", Meldung(41), maxk);
	   return Fehler;
	 }	  
       c = getc(fp); goto ZustandS21;
     }
   else if (Sepzeichen(c))
     {
       *(freiSymbole++) = '\0';
       v = eintragen();
       if (v == 0)
	 {
	   fprintf(stderr, "%s %6d\n", Meldung(35), maxn);
	   return Fehler;
	 }
       if (! uebernehmenLiteral((e == Pos) ? (int) v : - (int) v))
	 {
	   fprintf(stderr, "%s %7d\n", Meldung(38), maxl);
	   return Fehler;
	 }
       c = getc(fp); goto ZustandS27;
     }
   else if (isspace(c))
     {
       *(freiSymbole++) = '\0';
       v = eintragen();
       if (v == 0)
	 {
	   fprintf(stderr, "%s %6d\n", Meldung(35), maxn);
	   return Fehler;
	 }
       if (! uebernehmenLiteral((e == Pos) ? (int) v : - (int) v))
	 {
	   fprintf(stderr, "%s %7d\n", Meldung(38), maxl);
	   return Fehler;
	 }
       c = getc(fp); goto ZustandS26;
     }
   else if (isalnum(c))
     {
       *(freiSymbole++) = c;
       c = getc(fp); goto ZustandS25;
     }
   else
     {
       if (c == EOF)
	 fprintf(stderr, "%s\n", Meldung(14));
       else
	 {
	   fprintf(stderr, "%s\n", Meldung(8));
	   Fehlerumgebung(fp, c);
	 }
       return Fehler;
     }
 
 ZustandS26 :
   
   if (Klauselende(c))
     {
       if (! uebernehmenKlausel())
	 {
	   fprintf(stderr, "%s %6d\n", Meldung(41), maxk);
	   return Fehler;
	 }
       c = getc(fp); goto ZustandS21;
     }
   else if (isspace(c))
     {
       c = getc(fp); goto ZustandS26;
     }
   else if (Sepzeichen(c))
     {
       c = getc(fp); goto ZustandS27;
     }
   else
     {
       if (c == EOF)
	 fprintf(stderr, "%s\n", Meldung(14));
       else
	 {
	   fprintf(stderr, "%s\n", Meldung(8));
	   Fehlerumgebung(fp, c);
	 }
       return Fehler;
     }
 
 ZustandS27 :
   
   if (isspace(c))
     {
       c = getc(fp); goto ZustandS27;
     }
   else if (isalnum(c))
     {
       *(freiSymbole++) = c;
       e = Pos;
       c = getc(fp); goto ZustandS25;
     }
   else if (Negzeichen(c))
     {
       e = Neg;
       c = getc(fp); goto ZustandS24;
     }
   else
     {
       if (c == EOF)
	 fprintf(stderr, "%s\n", Meldung(15));
       else
	 {
	   fprintf(stderr, "%s\n", Meldung(16));
	   Fehlerumgebung(fp, c);
	 }
       return Fehler;
     }
 

/* ---------------------- */


S3:

 ZustandS31 :

   if (c == EOF)
     goto Phase2;
   else if (isspace(c))
     {
       c = getc(fp); goto ZustandS31;
     }
   else if (Kommzeichen(c))
     {
       c = getc(fp); goto ZustandS32;
     }
   else if (Klauselbeginn(c))
     {
       c = getc(fp); goto ZustandS33;
     }
   else
     {
       fprintf(stderr, "%s\n", Meldung(8));
       Fehlerumgebung(fp, c);
       return Fehler;
     }
 
 ZustandS32 :
   
   if (c == EOF)
     {
       fprintf(stderr, "%s\n", Meldung (9));
       return Fehler;
     }
   else if (c == '\n')
     {
       c = getc(fp); goto ZustandS31;
     }
   else
     {
       c = getc(fp); goto ZustandS32;
     }
 
 ZustandS33 :
   
   if (Klauselende(c))
     {
       leereKlausel = true; K0++; K++;
       c = getc(fp); goto ZustandS31;
     }
   else if (isspace(c))
     {
       c = getc(fp); goto ZustandS33;
     }
   else if (isalnum(c))
     {
       *(freiSymbole++) = c;
       e = Pos; Tautologie = false;
       aktp0 = aktp = 0;
       c = getc(fp); goto ZustandS35;
     }
   else if (Negzeichen(c))
     {
       e = Neg; Tautologie = false;
       aktp0 = aktp = 0;
       c = getc(fp); goto ZustandS34;
     }
   else
     {
       if (c == EOF)
	 fprintf(stderr, "%s\n", Meldung(10));
       else
	 {
	   fprintf(stderr, "%s\n", Meldung(8));
	   Fehlerumgebung(fp, c);
	 }
       return Fehler;
     }
 
 ZustandS34 :
   
   if (isspace(c))
     {
       c = getc(fp); goto ZustandS34;
     }
   else if (isalnum(c))
     {
       *(freiSymbole++) = c;
       c = getc(fp); goto ZustandS35;
     }
   else
     {
       if (c == EOF)
	 fprintf(stderr, "%s\n", Meldung(14));
       else
	 {
	   fprintf(stderr, "%s\n", Meldung(13));
	   Fehlerumgebung(fp, c);
	 }
       return Fehler;
     }
 
 ZustandS35 :
   
   if (Klauselende(c))
     {
       *(freiSymbole++) = '\0';
       v = eintragen();
       if (v == 0)
	 {
	   fprintf(stderr, "%s %6d\n", Meldung(35), maxn);
	   return Fehler;
	 }
       if (! uebernehmenLiteral((e == Pos) ? (int) v : - (int) v))
	 {
	   fprintf(stderr, "%s %7d\n", Meldung(38), maxl);
	   return Fehler;
	 }
       if (! uebernehmenKlausel())
	 {
	   fprintf(stderr, "%s %6d\n", Meldung(41), maxk);
	   return Fehler;
	 }	  
       c = getc(fp); goto ZustandS31;
     }
   else if (isspace(c))
     {
       *(freiSymbole++) = '\0';
       v = eintragen();
       if (v == 0)
	 {
	   fprintf(stderr, "%s %6d\n", Meldung(35), maxn);
	   return Fehler;
	 }
       if (! uebernehmenLiteral((e == Pos) ? (int) v : - (int) v))
	 {
	   fprintf(stderr, "%s %7d\n", Meldung(38), maxl);
	   return Fehler;
	 }
       c = getc(fp); goto ZustandS36;
     }
   else if (isalnum(c))
     {
       *(freiSymbole++) = c;
       c = getc(fp); goto ZustandS35;
     }
   else
     {
       if (c == EOF)
	 fprintf(stderr, "%s\n", Meldung(14));
       else
	 {
	   fprintf(stderr, "%s\n", Meldung(8));
	   Fehlerumgebung(fp, c);
	 }
       return Fehler;
     }
 
 ZustandS36 :
   
   if (Klauselende(c))
     {
       if (! uebernehmenKlausel())
	 {
	   fprintf(stderr, "%s %6d\n", Meldung(41), maxk);
	   return Fehler;
	 }
       c = getc(fp); goto ZustandS31;
     }
   else if (isspace(c))
     {
       c = getc(fp); goto ZustandS36;
     }
   else if (isalnum(c))
     {
       *(freiSymbole++) = c;
       e = Pos;
       c = getc(fp); goto ZustandS35;
     }
   else if (Negzeichen(c))
     {
       e = Neg;
       c = getc(fp); goto ZustandS34;
     }
   else
     {
       if (c == EOF)
	 fprintf(stderr, "%s\n", Meldung(14));
       else
	 {
	   fprintf(stderr, "%s\n", Meldung(8));
	   Fehlerumgebung(fp, c);
	 }
       return Fehler;
     }
 
 
/* ---------------------- */
 


S4:

 ZustandS41 :

   if (c == EOF)
     goto Phase2;
   else if (c == '\n')
     {
       leereKlausel = true; K0++; K++;
       c = getc(fp); goto ZustandS41;
     }
   else if (isspace(c))
     {
       c = getc(fp); goto ZustandS41;
     }
   else if (Kommzeichen(c))
     {
       c = getc(fp); goto ZustandS42;
     }
   else if (isalnum(c))
     {
       *(freiSymbole++) = c;
       e = Pos; Tautologie = false;
       aktp0 = aktp = 0;
       c = getc(fp); goto ZustandS45;
     }
   else if (Negzeichen(c))
     {
       e = Neg; Tautologie = false;
       aktp0 = aktp = 0;
       c = getc(fp); goto ZustandS44;
     }
   else
     {
       fprintf(stderr, "%s\n", Meldung(8));
       Fehlerumgebung(fp, c);
       return Fehler;
     }
 
 ZustandS42 :

   if (c == EOF)
     {
       fprintf(stderr, "%s\n", Meldung (9));
       return Fehler;
     }
   else if (c == '\n')
     {
       c = getc(fp); goto ZustandS41;
     }
   else
     {
       c = getc(fp); goto ZustandS42;
     }
 
 ZustandS44 :
   
   if (isspace(c))
     {
       c = getc(fp); goto ZustandS44;
     }
   else if (isalnum(c))
     {
       *(freiSymbole++) = c;
       c = getc(fp); goto ZustandS45;
     }
   else
     {
       if (c == EOF)
	 fprintf(stderr, "%s\n", Meldung(14));
       else
	 {
	   fprintf(stderr, "%s\n", Meldung(13));
	   Fehlerumgebung(fp, c);
	 }
       return Fehler;
     }
 
 ZustandS45 :
   
   if (c == '\n')
     {
       *(freiSymbole++) = '\0';
       v = eintragen();
       if (v == 0)
	 {
	   fprintf(stderr, "%s %6d\n", Meldung(35), maxn);
	   return Fehler;
	 }
       if (! uebernehmenLiteral((e == Pos) ? (int) v : - (int) v))
	 {
	   fprintf(stderr, "%s %7d\n", Meldung(38), maxl);
	   return Fehler;
	 }
       if (! uebernehmenKlausel())
	 {
	   fprintf(stderr, "%s %6d\n", Meldung(41), maxk);
	   return Fehler;
	 }	  
       c = getc(fp); goto ZustandS41;
     }
   else if (isspace(c))
     {
       *(freiSymbole++) = '\0';
       v = eintragen();
       if (v == 0)
	 {
	   fprintf(stderr, "%s %6d\n", Meldung(35), maxn);
	   return Fehler;
	 }
       if (! uebernehmenLiteral((e == Pos) ? (int) v : - (int) v))
	 {
	   fprintf(stderr, "%s %7d\n", Meldung(38), maxl);
	   return Fehler;
	 }
       c = getc(fp); goto ZustandS46;
     }
   else if (isalnum(c))
     {
       *(freiSymbole++) = c;
       c = getc(fp); goto ZustandS45;
     }
   else
     {
       if (c == EOF)
	 fprintf(stderr, "%s\n", Meldung(14));
       else
	 {
	   fprintf(stderr, "%s\n", Meldung(8));
	   Fehlerumgebung(fp, c);
	 }
       return Fehler;
     }
 
 ZustandS46 :
   
   if (c == '\n')
     {
       if (! uebernehmenKlausel())
	 {
	   fprintf(stderr, "%s %6d\n", Meldung(41), maxk);
	   return Fehler;
	 }
       c = getc(fp); goto ZustandS41;
     }
   else if (isspace(c))
     {
       c = getc(fp); goto ZustandS46;
     }
   else if (isalnum(c))
     {
       *(freiSymbole++) = c;
       e = Pos;
       c = getc(fp); goto ZustandS45;
     }
   else if (Negzeichen(c))
     {
       e = Neg;
       c = getc(fp); goto ZustandS44;
     }
   else
     {
       if (c == EOF)
	 fprintf(stderr, "%s\n", Meldung(14));
       else
	 {
	   fprintf(stderr, "%s\n", Meldung(8));
	   Fehlerumgebung(fp, c);
	 }
       return Fehler;
     }
 

/* ------------------------------------ */

/*  Nun sind P0, N0, K0, L0 und P, K, L berechnet, und */
/*    die Eingabe ist in LitTab abgespeichert (Klauseln */
/*    getrennt durch 0). */

Phase2:

 free(Hashtabelle); Hashtabelle = NULL;
 if (! Belegung)
   {
     free(VarTab); VarTab =  NULL;
     free(Eingabesymbole0); Eingabesymbole0 = NULL;
   }

 if (K == 0)
   {
     N = 0;
     return Sat;
   }

 N = N0;
 if (globalTautologie)  
   /* es muss N neu berechnet werden */
   /* und die neuen Variablennummern neu bestimmt werden */
   {
     unsigned int m, i;
     int l; unsigned int v0, v; VZ e;
     N = 0;
     Hashtabelle = (unsigned int *) xmalloc((N0 + 1) * sizeof(unsigned int));
     memset(Hashtabelle, 0, (N0 + 1) * sizeof(unsigned int));
     if (Belegung)
       VarTab1 = (unsigned int *) xmalloc((N0 + 1) * sizeof(unsigned int));
     for (m = i = 0; m < L; m++, i++)
       {
	 l = LitTab[i];
	 if (l == 0)
	   l = LitTab[++i];
	 if (l > 0)
	   {
	     v0 = l; e = Pos;
	   }
	 else
	   {
	     v0 = -l; e = Neg;
	   }
	 v = *(Hashtabelle + v0);
	 if (v == 0)
	   {
	     N++;
	     v = *(Hashtabelle + v0) = N;
	     if (Belegung)
	       VarTab1[N] = v0;
	   }
	 LitTab[i] = (e == Pos) ? (int) v : - (int) v;
       }
     free(Hashtabelle); Hashtabelle = NULL;
   }

 if (leereKlausel)
   return Unsat;


 if (EinerKlausel)
   {
     typedef struct {
       int *Anfang;
       int *Ende;
       bool erfuellt;
     } Klauselinfo;
     typedef struct Vorkommen {
       struct Vorkommen *naechstes;
       Klauselinfo *Klausel;
     } Randvorkommen;

     unsigned int AnzahlAnker = 2 * N + 1;
     unsigned int PositionAnker = N;

     Randvorkommen *Anfangsvorkommen; Randvorkommen *freiesAnfvork;
     Randvorkommen *Endvorkommen; Randvorkommen *freiesEndvork;
     Klauselinfo *BeginnKlauseln; Klauselinfo *freieKlausel;
     bool *belegt;
     int *EinerKlauseln; int *sz;

     void *Z; unsigned int k;
     int *i; int *i0;
     Klauselinfo *Klausel;
     unsigned int aktp;

     Z0 = xmalloc( 2 * (AnzahlAnker + K) * sizeof(Randvorkommen) +
		   K * sizeof(Klauselinfo) +
		   AnzahlAnker * sizeof(bool) +
		   N * sizeof(LIT));
     Anfangsvorkommen = (Randvorkommen *) Z0; Z = (void *) (Anfangsvorkommen + AnzahlAnker); Anfangsvorkommen += PositionAnker;
     freiesAnfvork = (Randvorkommen *) Z; Z = (void *) (freiesAnfvork + K);
     Endvorkommen = (Randvorkommen *) Z; Z = (void *) (Endvorkommen + AnzahlAnker); Endvorkommen += PositionAnker;
     freiesEndvork = (Randvorkommen *) Z; Z = (void *) (freiesEndvork + K);
     BeginnKlauseln = freieKlausel = (Klauselinfo *) Z; Z = (void *) (BeginnKlauseln + K);
     belegt = (bool *) Z; Z = (void *) (belegt + AnzahlAnker);
     belegt += PositionAnker;
     EinerKlauseln = (int *) Z; sz = EinerKlauseln;

     if (Belegung)
       Pfad0 = (int *) xmalloc(N * sizeof(LIT));

     for (v = 1; v <= N; v++)
       {
	 (Anfangsvorkommen + v) -> naechstes = NULL;
	 (Anfangsvorkommen - (int) v) -> naechstes = NULL;
	 (Endvorkommen + v) -> naechstes = NULL;
	 (Endvorkommen - (int) v) -> naechstes = NULL;
	 *(belegt + v) = false;
	 *(belegt - (int) v) = false;
       }

     i = LitTab;
     for (k = 0; k < K; k++)
       {
	 int erstesLit, letztesLit;
	 
	 erstesLit = *i; i0 = i;
	 do
	   i++;
	 while (*i != 0);
	 if (i - i0 == 1)
	   {
	     if (belegt[erstesLit])
	       {
		 return Unsat;
	       }
	     if (! belegt[-erstesLit])
	       {
		 belegt[-erstesLit] = true;
		 *(sz++) = -erstesLit;
	       }
	   }
	 else
	   {
	     freiesAnfvork -> naechstes = Anfangsvorkommen[erstesLit].naechstes;
	     Anfangsvorkommen[erstesLit].naechstes = freiesAnfvork;
	     freiesAnfvork -> Klausel = freieKlausel;
	     letztesLit = *(i-1);
	     freiesEndvork -> naechstes = Endvorkommen[letztesLit].naechstes;
	     Endvorkommen[letztesLit].naechstes = freiesEndvork;
	     freiesEndvork -> Klausel =  freieKlausel;
	     freieKlausel -> erfuellt = false;
	     freieKlausel -> Anfang = i0;
	     freieKlausel -> Ende = i - 1;
	     freiesAnfvork++; freiesEndvork++; freieKlausel++;
	   }
	 i++;
       }

     do
       {
	 int l;
	 Randvorkommen *x; Randvorkommen *xn;

	 l = *(--sz);
	 if (Belegung)
	   Pfad0[InitEinerRed++] = l;
	 else
	   InitEinerRed++;
	 
	 for (x = Anfangsvorkommen[l].naechstes; x != NULL; x = xn)
	   {
	     xn = x -> naechstes;
	     Klausel = x -> Klausel;
	     if (Klausel -> erfuellt)
	       continue;
	     aktp = 0;
	     for (i = (Klausel -> Anfang) + 1; i != (Klausel -> Ende) + 1; i++)
	       {
		 if (belegt[-*i])
		   {
		     Klausel -> erfuellt = true;
		     break;
		   }
		 if (! belegt[*i])
		   {
		     if (++aktp == 1)
		       i0 = i;
		     else
		       break;
		   }
	       }
	     if (Klausel -> erfuellt)
	       continue;
	     if (aktp == 0)
	       {
		 return Unsat;
	       }
	     if (aktp == 1)
	       {
		 belegt[-*i0] = true;
		 *(sz++) = -*i0;
		 Klausel -> erfuellt = true;
	       }
	     else
	       {
		 int a;
		 
		 a = *i0;
		 x -> naechstes = Anfangsvorkommen[a].naechstes;
		 Anfangsvorkommen[a].naechstes = x;
		 Klausel -> Anfang = i0;
	       }
	   }

	 for (x = Endvorkommen[l].naechstes; x != NULL; x = xn)
	   {
	     xn = x -> naechstes;
	     Klausel = x -> Klausel;
	     if (Klausel -> erfuellt)
	       continue;
	     aktp = 0;
	     for (i = (Klausel -> Ende) - 1; i != (Klausel -> Anfang) - 1; i--)
	       {
		 if (belegt[-*i])
		   {
		     Klausel -> erfuellt = true;
		     break;
		   }
		 if (! belegt[*i])
		   {
		     if (++aktp == 1)
		       i0 = i;
		     else
		       break;
		   }
	       }
	     if (Klausel -> erfuellt)
	       continue;
	     if (aktp == 0)
	       {
		 return Unsat;
	       }
	     if (aktp == 1)
	       {
		 belegt[-*i0] = true;
		 *(sz++) = -*i0;
		 Klausel -> erfuellt = true;
	       }
	     else
	       {
		 int a;
		 
		 a = *i0;
		 x -> naechstes = Endvorkommen[a].naechstes;
		 Endvorkommen[a].naechstes = x;
		 Klausel -> Ende = i0;
	       }
	   }
       }
     while (sz != EinerKlauseln);

     Hashtabelle = (unsigned int *) xmalloc((N + 1) * sizeof(unsigned int));
     memset(Hashtabelle, 0, (N + 1) * sizeof(unsigned int));
     if (Belegung)
       VarTab2 = (unsigned int *) xmalloc((N - InitEinerRed + 1) * sizeof(unsigned int));
     P = N = K = L = 0;
     i0 = LitTab;
     for (Klausel = BeginnKlauseln; Klausel != freieKlausel; Klausel++)
       {
	 if (Klausel -> erfuellt)
	   continue;
	 for (i = Klausel -> Anfang; i != (Klausel -> Ende) + 1; i++)
	   if (belegt[-*i])
	     {
	       Klausel -> erfuellt = true;
	       break;
	     }
	 if (Klausel -> erfuellt)
	   continue;
	 aktp = 0;
	 for (i = Klausel -> Anfang; i != (Klausel -> Ende) + 1; i++)
	   if (! belegt[*i])
	     {
	       unsigned int v0, v; int l; VZ e;
	       
	       aktp++; l = *i;
	       if (l > 0)
		 {
		   v0 = l; e = Pos;
		 }
	       else
		 {
		   v0 = -l; e = Neg;
		 }
	       v = *(Hashtabelle + v0);
	       if (v == 0)
		 {
		   N++;
		   v = *(Hashtabelle + v0) = N;
		   if (Belegung)
		     VarTab2[N] = v0;
		 }
	       *(i0++) = (e == Pos) ? (int) v : - (int) v; L++;
	     }
	 if (aktp > P)
	   P = aktp;
	 K++;
	 *(i0++) = 0;
       }
     free(Hashtabelle); Hashtabelle = NULL;
     free(Z0); Z0 = NULL;

     if (K == 0)
       return Sat;
   }


 /*   Erzeugung aller Vektoren, deren Laenge von */
 /*     P, N, K oder L abhaengt */

 if (nurVorreduktion)
   return Norm;

 InitParameter();
 {
   void *Basis;

   Basis0 = Basis = xmalloc(BedarfVarLitKlmV() + BedarfBelegungV() + BedarfFilterV() + BedarfReduktionsV() + BedarfAbstandsV()
#ifdef BAUMRES
			    + BedarfBaumResV()
#endif
			    );
#ifdef BAUMRES
   Basis = BaumResV(Basis); 
/* vor VarLitKlm, da Adresse von aktrelV benoetigt wird */
#endif
   Basis = VarLitKlmV(Basis);
   Basis = BelegungV(Basis);
   Basis = FilterV(Basis);
   Basis = ReduktionsV(Basis);
   Basis = AbstandsV(Basis);
 }

/* Zweite Stufe */

/*   Nun wird die Klauselmenge hergestellt, LaenK, aktAnzK belegt, */
/*   und aktP, aktN werden initialisiert. */

  /* Durchlaufen der Literalvorkommen in LitTab */
  /* Belegen der Literalvorkommen und von LaenK, aktAnzK */
 
 {
   unsigned int k;
   int *j;
   j = LitTab;
   for (k = 0; k < K; k++)
     {
       Klauselanfangen();
       while (*j != 0)
	 {
	   Literaleintragen(*j);
	   j++;
	 }
       Klauselbeenden();
       j++;
     }
 }

 if (Belegung)
   {
     unsigned int v, vi;
     for (v = 1; v <= N; v++)
       {
	 vi = v;
	 if (VarTab2 != NULL)
	   vi = VarTab2[vi];
	 if (VarTab1 != NULL)
	   vi = VarTab1[vi];
	 Symboleintragen(v, VarTab[vi]);
       }
   }

 free(LitTab); LitTab = NULL;
 free(VarTab2); VarTab2 = NULL;
 if (! Belegung)
   {
     free(VarTab1); VarTab1 = NULL;
     free(VarTab); VarTab = NULL;
   }
 
 aktP = P; /* Initialisieren von aktP */
 aktN = N; /* Initialisieren von aktN */

 init2Klauseln = aktAnzK[2];

 extern Ausgabeformat Format;
 if (Format == XML_Format)
   for (unsigned int i = 0; i <= P - 2; InitAnzK[i+2] = aktAnzK[i+2], i++);
 // i wird von 0 gestartet, um Korrektheit auch im Falle von
 // P = std::numeric_limits<unsigned int>::max() zu garantieren

 return Norm;

}

/* ------------------------------------------------------------- */


void AufraeumenEinlesen(void)
{
  free(Hashtabelle); Hashtabelle = NULL;
  free(LitTab); LitTab = NULL;
  free(Eingabesymbole0); Eingabesymbole0 = NULL;
  free(VarTab); VarTab = NULL;
  free(VarTab1); VarTab1 = NULL;
  free(VarTab2); VarTab2 = NULL;
  free(Pfad0); Pfad0 = NULL;
  free(Basis0); Basis0 = NULL;
  free(Z0); Z0 = NULL;
}

/* ------------------------------------------------------------- */


__inline__ static char *Symbol1(unsigned int v)
{
  if (VarTab1 != NULL)
    v = VarTab1[v];
  return VarTab[v];
}


void AusgabeBelegung(FILE *fp)
{
  extern Ausgabeformat Format;
  const char *Einrueckung;

  if (Format == Dimacs_Format)
    fprintf(fp, "v");
  else if (Format == XML_Format) {
    extern bool Dateiausgabe;
    if (! Dateiausgabe)
      Einrueckung = "  ";
    else
      Einrueckung = "";
    fprintf(fp, "%s<solution>\n", Einrueckung);
  }

  if (EinerKlausel)
    {
      unsigned int i; unsigned int v; VZ e;
      for (i = 0; i < InitEinerRed; i++)
	{
	  if (Pfad0[i] > 0)
	    {
	      v = Pfad0[i]; e = Pos;
	    }
	  else
	    {
	      v = -Pfad0[i]; e = Neg;
	    }
	  if (Format == Dimacs_Format) {
	    if (e == Neg)
	      fprintf(fp, " %s", Symbol1(v));
	    else
	      fprintf(fp, " -%s", Symbol1(v));
	  }
	  else if (Format == XML_Format)
	    fprintf(fp, "%s  <value var = \"%s\"> %d </value>\n", Einrueckung, Symbol1(v), e);
	  else
	    fprintf(fp, "%7s %d\n", Symbol1(v), e);
	}
    }
  {
    Pfadinfo *Z; VAR v; LIT l; VZ e;
    Z = Tiefe;
    for (Tiefe = Pfad; Tiefe < Z; Tiefe++)
      {
	l = PfadLit(); v = Var(l);
	if (l == Literal(v, Pos))
	  e = Pos;
	else
	  e = Neg;
	if (Format == Dimacs_Format) {
	  if (e == Neg)
	    fprintf(fp, " %s", Symbol(v));
	  else
	    fprintf(fp, " -%s", Symbol(v));
	}
	else if (Format == XML_Format)
	  fprintf(fp, "%s  <value var = \"%s\"> %d </value>\n", Einrueckung, Symbol(v), e);
	else
	  fprintf(fp, "%7s %d\n", Symbol(v), e);
      }
    Tiefe = Z;
  }

  if (Format == Dimacs_Format)
    fprintf(fp, " 0\n");
  else if (Format == XML_Format) {
    fprintf(fp, "%s</solution>\n", Einrueckung);
    extern bool Dateiausgabe;
    if (! Dateiausgabe)
      fprintf(fp, "</SAT-Solver.output>\n");
  }
}


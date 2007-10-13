// Oliver Kullmann, 14.3.2001 (Toronto)
/* Copyright 2001 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKsolver/SAT2002/VarLitKlm.h
  \brief The interface for variables, literals and clause-sets

  The central data types for variables, literals, literal-occurrences and
  clause-occurrences are:
  <ol>
   <li> VAR is the datatype for variables (implemented as a pointer to
   a variable-structure var). </li>
   <li> LIT is the datatype for literals (implemented as a pointer to
   a literal-structure lit). </li>
   <li> LITV is the datatype for literal-occurrences (implemented as a
   pointer to a literal-occurrence-structure litv). </li>
   <li> KLN is the datatype for clause-orrurrences (implemented as a
   pointer to a clause-occurrence-structure Klauselv). </li>
  </ol>
  One has to distinguish between variables and literals on the one hand,
  and literal- and clause-<em>occurrences</em> on the other hand:
  <ul>
   <li> If the input has n variables, then we have n different var-objects and 2n
   different lit-objects, each containing basic data and statistics about
   the current state of the variable resp. literal. </li>
   <li> If the length of the input is L (the sum of the length of the clauses),
   then we have L different litv-objects, and if it has K clauses, then
   we have K different Klauselv-objects. </li>
  </ul>


  <h2> %Variables, literals, and their occurrences </h2>

  The basic operations for literals and variables are:
  <ul>
   <li> %Literals are constructed by <code>Literal(v,e)</code> for a variable v and
   a sign e. </li>
   <li> %Literals are complemented by <code>Komp(l)</code>. </li>
   <li> The underlying variable of a literal is <code>Var(l)</code>. </li>
  </ul>

  After processing the input, the double-linked list of variables is constructed,
  which shrinks when applying a partial assignment (removal of those variables which
  have been assigned a value), and which grows again when backtracking (undoing the
  removal):
  <ul>
   <li> Running through the list of current variables is done via a loop
   \code
for (VAR v = ersteVar(); echteVar(v); v = naechsteVar(v)) { ... }
   \endcode
   where
    <ol>
     <li> <code>ersteVar()</code> returns the current first variable (potentially
     singular, if there is no variable left), </li>
     <li> <code>echteVar(v)</code> is true if the variable is a
     real variable (not singular), </li>
     <li> while <code>naechsteVar(v)</code> for a non-singular variable
     v returns the next current variable in the list. </li>
    </ol>
   </li>
   <li> Eliminating a variable v from the current list is done by
   <code>loeseV(v)</code> (relinking the predecessor with the successor). </li>
   <li> Re-inserting a variable into the current list is done by
   <code>bindeV(v)</code>. </li>
  </ul>

  A literal-occurrence object is an element of two "orthogonal" double-linked
  lists, one for all the occurrences of the underlying literal, the other
  for all the literal occurrence is the clause associated with this
  literal-occurrence:
  <ul>
   <li> Removing literal-occurrence x from the list of all occurrences of the
   underlying literal <code>LitVk(x)</code> is done by <code>loeseLv(x)</code>,
   re-insertion by <code>bindeLv(x)</code>. </li>
   <li> Removing literal-occurrence x from its clause <code>KlnVk(x)</code>
   is is done by <code>loeseLK(x)</code>, re-insertion by <code>bindeLK(x)</code>.
   </li>
  </ul>

  Running through all literal occurrences of a literal l happens by
  \code
for (LITV x = erstesVork(l); echtesVork(x); x = naechstesVork(x)) { ... }
  \endcode
  For the literals in a clause there is no "begin" and "end", but just a
  ring-list, where the next literal-occurrence in the clause is
  <code>naechstesVorkK(x)</code>.


  <h2> %Clauses, and tree pruning </h2>

  A clause-occurrence (and object of type Klauselv) contains the following
  information:
  <ul>
   <li> the current length; </li>
   <li> for look-ahead-purposes the "look-ahead length" together with
   a round-counter; </li>
   <li> for tree-pruning-purposes access to the set of variables
   of this clause. </li>
  </ul>
  Variable-sets are represented as follows:
  <ul>
   <li> If macro LITTAB (for saving memory) is not set, then there is just one
   representation of sets of variables, namely a binary representation of
   length n (actually, GroesseVarMenge). </li>
   <li> Otherwise the input clauses of length k are represented by k consecutive
   structs of the VarMaske, each containing a counter for the word-position
   and a binary mask for the bit inside a word. </li>
  </ul>

*/


#ifndef GELESENVARLITKLM_jJqp6
#define GELESENVARLITKLM_jJqp6

#include "OK.h"

#ifdef BAUMRES
//! block of bits, representing a part of a variable-set
typedef unsigned long int Wort;
//! pointer to a part of a variable-set
typedef Wort* ZWort;
//! pointer to the first word of a variable-set
typedef Wort* VarMenge;
# ifdef LITTAB
//! for a variable its word-position and mask
typedef struct {
  ZWort Position;
  Wort Maske;
} VarMaske;
# endif
#endif

/* ------------------------------------------------- */


/* Die vier zentralen abstrakten Datentypen VAR, LIT, KLN, LITV */

#ifdef LOKALLERNEN
typedef struct neuK* NKL;
#endif
//! the "abstract" type for variables
typedef struct var* VAR;
//! the "abstract" type for literals
typedef struct lit* LIT;
//! the "abstract" type for literal-occurrences
typedef struct litv* LITV;
//! the "abstract" type for clause-occurrences
typedef struct Klauselv* KLN;

//! structure identifying one clause-occurrence
struct Klauselv {
  KLL Laenge;
  KLL LaLaenge;
  /* LaLaenK[k] == 0 heisst, dass die Klausel schon erfuellt wurde. */
  unsigned int RundeK;
#ifdef BAUMRES
# ifndef LITTAB
  VarMenge VarM;
# else
  unsigned int UrLaenge;
  VarMaske* ersteV;
# endif
#endif
};

//! structure for one literal-occurrence
struct litv {
  struct litv* nLv;
  struct litv* lLv;
  struct litv* nLK;
  struct litv* lLK;
  struct lit* lit;
  struct Klauselv* kln;
};

#ifdef LOKALLERNEN
//! new binary clause for local learning of clauses
typedef struct neuK {
  struct neuK* naechstes;
  struct neuK* vorheriges;
  struct Klauselv Klausel;
  struct litv x1;
  struct litv x2;
# ifdef FASTAUTARKIE
  LIT l; /* falls l != NullLiteral, so ist dies das "Fast-Autarkie-Literal" */
# endif
};
//! size of struct neuK
const unsigned int sizeneuK = sizeof(neuK);
#endif

//! the null-literal
extern const LIT NullLiteral;


/* ------------------------------------------------- */

//! the current number of clauses
extern unsigned int* aktAnzK;
/* extern unsigned int aktAnzK [ MAXP + 1 ]; */
//! initial number of clauses (in case of xml-output)
extern unsigned int* InitAnzK;

/* ------------------------------------------------- */

//! initialisation of variable-sets for original clause in case of not LITTAB
extern void InitVarLitKlm(void);

//! literals from variables and signs
extern LIT Literal(VAR v, VZ e);
//! the underlying variable of a literal
extern VAR Var(LIT l);
//! the complement of a literal
extern LIT Komp(LIT l);
#ifdef BAUMRES
//! the word-index of a variable
extern ZWort VPosition(VAR v);
//! the mask of a variable w.r.t. its word
extern Wort VMaske(VAR v);
#endif
//! is variable assigned?
extern bool belegt(VAR v);
//! set truth-value for assigned-test
extern void setzenbelegt(VAR v, bool T);
//! the original name (in the input-file, in extended DIMACS-format) of a variable
extern const char* Symbol(VAR v);

//! remove a variable from the current list
extern void loeseV(VAR v);
//! re-insert the variable to the current list
extern void bindeV(VAR v);

//! begin of the variable-list
extern VAR ersteVar( void );
//! variable is not after-the-end?
extern bool echteVar(VAR v);
//! next variable in the list
extern VAR naechsteVar(VAR v);

//! round of a literal
unsigned int RundeL(LIT l);
//! set round of literal to the current round
void setzenRundeL(LIT l);
//! reset round of all literals
void NullsetzenRL( void );

#ifdef FASTAUTARKIE

bool Fastautarkie(LIT l);
void setzenFastautarkie(LIT l, bool T);

unsigned int RundeLFA(LIT l);
void setzenRundeLFA(LIT l);
void NullsetzenRLFA(void);

#endif

//! remove literal-occurrence from list of all occurrences
extern void loeseLv(LITV x);
//! re-insert literal-occurrence into list of all occurrences
extern void bindeLv(LITV x);
//! remove literal-occurrence from clause-list
extern void loeseLK(LITV x);
//! re-insert literal-occurrence into clause-list
extern void bindeLK(LITV x);

//! begin of list of literal-occurrences for literal l
extern LITV erstesVork(LIT l);
//! literal-occurrence is not past-the-end?
extern bool echtesVork(LITV x, LIT l);
//! next literal-occurrence for literal l
extern LITV naechstesVork(LITV x);

//! next literal-occurrence in clause
extern LITV naechstesVorkK(LITV x);

//! literal of literal-occurrence
extern LIT LitVk(LITV x);
//! clause-identifier of literal-occurrence
extern KLN KlnVk(LITV x);

//! current length of clause
extern KLL Laenge(KLN k);
//! set length of clause
extern void ZuwLaenge(KLN k, KLL m);
//! current length, with post-decrement
extern KLL LaengeM1(KLN k);
//! current length, with post-increment
extern KLL LaengeP1(KLN k);
//! current look-ahead-length of clause
extern KLL LaLaenge(KLN k);
//! set look-ahead-length of clause
extern void ZuwLaLaenge(KLN k, KLL m);
//! current look-ahead-length of clause with pre-decrement
extern KLL M1LaLaenge(KLN k);

//! current round-counter-value of clause
extern unsigned int RundeK(KLN k);
//! set round-counter of clause
extern void setzenRundeK(KLN k);
//! reset all clause-round-counters
extern void NullsetzenRK( void );
#ifdef BAUMRES
# ifndef LITTAB
//! the full variable-set of a clause
extern VarMenge VarK(KLN k);
# else
//! original length of clause
extern unsigned int UrLaenge(KLN k);
//! variable mask for first variable (others follow)
extern VarMaske* ersteVK(KLN k);
# endif
#endif

#ifdef LOKALLERNEN
//! reset round-counter of a single clause
extern void setzen0RundeK(KLN K);
#endif

//! open a new clause (for input)
extern void Klauselanfangen(void);
//! enter literal into a clause (for input)
extern void Literaleintragen(int l);
//! close a clause (for input)
extern void Klauselbeenden(void);
//! set name of variable (for input)
void Symboleintragen(unsigned int v, const char* S);

//! set first literal-occurrence for a literal
void setzenerstesV(LIT l, LITV x);
//! set literal for a literal-occurrence
void setzenLit(LITV x, LIT l);
//! set clause for a literal-occurrence
void setzenKln(LITV x, KLN k);
//! set next literal occurrence for literal-occurrence x
void setzennLv(LITV x, LITV y);
//! set previous literal occurrence for literal-occurrence x
void setzenlLv(LITV x, LITV y);
//! set next literal-occurrence in clause
void setzennLK(LITV x, LITV y);
//! set previous literal-occurrence in clause
void setzenlLK(LITV x, LITV y);


#endif

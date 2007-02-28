// Matthew Lewsey, 2.2.2007 (Swansea)

#include "ubcsat.h"
#include <stdio.h>

#include <cassert>
 
/*!
  \file AutarkySearch/AnalyseTotalAssignment.cpp
  \brief Supplementing UBCSAT by analysing total assignments for
  contained autarkies.

  \bug Shouldn't the above inclusion of "ubcsat.h" be replaced
  by inclusion of the file in the OKlibrary ? We should never have
  inclusions like that (with path-information).

  \bug Since this is C++, instead of stdio.h cstdio should be used.

  \bug What is the status of the following bug?? IF SOMETHING HAPPENED
  TO A BUG (OR ANY OTHER TODO) THEN IT MUST BE CLEARLY FLAGGED.

  \bug Program is faulty, and produces a segmentation fault for simple
  3-CNF input. Valgrind-output:

==32686== Conditional jump or move depends on uninitialised value(s)
==32686==    at 0x40727A: ReadCNF (in /home/csoliver/SAT-Algorithmen/OKplatform/system_directories/bin/AnalyseTotalAssignment)
==32686==    by 0x400FD9: Initialise (AnalyseTotalAssignment.c:33)
==32686==    by 0x4016D5: main (AnalyseTotalAssignment.c:147)

  Further information:
  <ol>
   <li> Running

cat example.cnf | valgrind ~/SAT-Algorithmen/OKplatform/ExternalSources/Ubcsat/1-0-0/bin/ubcsat -alg adaptnovelty+

  also yields a segmentation fault. If the usage of ubcsat is correct, then
  either it is miscompiled or faulty (or both). The first
  "Conditional jump or move" happens again in ReadCNF. </li>

   <li> Perhaps the problems is with the wrong definitions in ubcsat-types.h:

#ifndef UINT32
#define UINT32 unsigned long
#endif

#ifndef SINT32
#define SINT32 signed long
#endif

   since unsigned long is for example 64 bit on OK's workstation. These definitions
   have to be corrected using the appropriate fixed-size C99 types. (Also
   BOOL there should be changed; the maximal exponent of double is
   1E308, so the definition there for FLOAT should be alright. PROBABILITY
   is doubtful.) </li>

   <li> Proposal to change ubcsat-types.h:
    Replace <p>

#define FLOAT double
#define FLOATMAX (1E+300)

#define PROBABILITY unsigned long

#ifndef BOOL
#define BOOL unsigned long
#endif

#ifndef UINT32
#define UINT32 unsigned long
#endif

#ifndef SINT32
#define SINT32 signed long
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

    </p> by <p>

#ifdef BOOL
# error "BOOL predefined, which might conflict with the C99 definition of bool"
#endif
#ifdef TRUE
# error "TRUE predefined, which might conflict with the C99 definition of true"
#endif
#ifdef FALSE
# error "FALSE predefined, which might conflict with the C99 definition of false"
#endif
#include <stdbool.h>
#define BOOL bool
#define TRUE true
#define FALSE false

#include <float.h>
#if DBL_MAX_10_EXP < 308
# error "Maximal decimal exponent for double is only " DBL_MAX_10_EXP ", but should be at least 308"
#endif
#define FLOAT double
#define FLOATMAX (1E+300)

#include <stdint.h>
#ifdef UINT32
# error "UINT32 predefined, which might conflict with the C99 definition of uint32_t"
#endif
#define UINT32 uint32_t
#ifdef SINT32
# error "SINT32 predefined, which might conflict with the C99 definition of int32_t"
#endif
#define SINT32 int32_t

#define PROBABILITY UINT32

    </p> </li>
  </ol>

  \bug Code is not proper C++.

  \todo Create Doxygen documentation.
*/

void Initialise();
void AnalyseTotalAssignment();

void Initialise()
{
  // call the relevant UBCSAT library procedures to detach this function from being inside the UBCSAT system
  void ReadCNF();
  ReadCNF();
  void CreateLitOccurence();
  CreateLitOccurence(); // allows useage of aNumLitOcc, pLitClause
  void CreateCandidateList();
  CreateCandidateList(); // allows useage of aCandidateList
  void CreateDefaultStateInfo();
  CreateDefaultStateInfo(); // allows the usage of aVarValue array once initialised
  void CreateFalseClauseList();
  CreateFalseClauseList(); // allows useage of the new false clause list after variable flip

  // UBCSAT uses aVarInit to determine how variable i is to be assigned (2 = random)
  assert(iNumVars >= 0);
  for(UINT32 i=1; i <= UINT32(iNumVars); ++i)
    aVarInit[i] = 2;

  void DefaultInitVars();
  DefaultInitVars(); // initially assigns ALL variables randomnly (randomnly due to above loop)
  void InitDefaultStateInfo();
  InitDefaultStateInfo(); // necessary to get aNumTrueLit working
  void InitFalseClauseList();
  InitFalseClauseList(); // necessary to get aFalseList working
}


void AnalyseTotalAssignment()

{
  BOOL bOneLitTrue = FALSE, bNotDone = TRUE;
  BOOL *aCurrAutarky;
  UINT32 *aStack, iStackSize, iCurrStackVar;
  UINT32 iCurrClauseNum, iCurrVar, iCurrLit,  iCurrFlipVarLitValue, iCurrAutarkySize, iClauseLen;
  UINT32 *aLitClauses;

  aCurrAutarky = (BOOL *)malloc((iNumVars+1) * sizeof(BOOL));
  iStackSize = 0;
  aStack = (UINT32 *)malloc((iNumVars) * sizeof(UINT32));

    for(UINT32 j=0; j <= iNumVars; ++j)
      aCurrAutarky[j] = TRUE;

    for(UINT32 j=0; j < iNumFalse; ++j) {
      iCurrClauseNum = aFalseList[j];
      iClauseLen = aClauseLen[iCurrClauseNum];
      for(UINT32 k=0; k < iClauseLen; ++k) {
	iCurrVar = pClauseLits[iCurrClauseNum][k-aClauseLen[iCurrClauseNum]] / 2;
	aCurrAutarky[iCurrVar] = FALSE;
      }
    }

    for(UINT32 j=1; j <= iNumVars; ++j) {
      if(aCurrAutarky[j] == FALSE) continue;
      iCurrFlipVarLitValue = aVarValue[j] ? (j*2)+1 : j*2;
      aLitClauses = pLitClause[iCurrFlipVarLitValue];
      for(UINT32 k=0; k < aNumLitOcc[iCurrFlipVarLitValue]; ++k) {
	iCurrClauseNum = aLitClauses[k];
	iClauseLen = aClauseLen[iCurrClauseNum];
	for(UINT32 m=0; m < iClauseLen*2; ++m) {
	  if(m >= iClauseLen){
	    iCurrVar = pClauseLits[iCurrClauseNum][m - iClauseLen] / 2;
	    if(aCurrAutarky[iCurrVar]) {
	      aStack[iStackSize] = iCurrVar;
	      iStackSize++;
	      aCurrAutarky[iCurrVar] = FALSE;
	    }
	  } else {
	    iCurrLit = pClauseLits[iCurrClauseNum][m];
	    iCurrVar = iCurrLit / 2;
	    bOneLitTrue = aCurrAutarky[iCurrVar] && ((aVarValue[iCurrVar] && iCurrLit % 2 == 0)  || (!aVarValue[iCurrVar] && iCurrLit % 2 == 1 ));
	    if(bOneLitTrue) break;
	  }
	}
	if(! bOneLitTrue) { 
	  while(bNotDone) {
	    iCurrStackVar = aStack[iStackSize - 1];
	    --iStackSize;
	    iCurrFlipVarLitValue = aVarValue[iCurrStackVar] ? (j*2)+1 : j*2;
	    aLitClauses = pLitClause[iCurrFlipVarLitValue];
	    for(UINT32 m=0; m < aNumLitOcc[iCurrFlipVarLitValue]; ++m) {
	      iCurrClauseNum = aLitClauses[m];
	      iClauseLen = aClauseLen[iCurrClauseNum];
	      for(UINT32 n=0; n < iClauseLen; ++n) {
		iCurrLit = pClauseLits[iCurrClauseNum][n];
		iCurrVar = iCurrLit / 2;
		if(aCurrAutarky[iCurrVar] == TRUE) {
		  aStack[iStackSize] = iCurrVar;
		  ++iStackSize;
		  aCurrAutarky[iCurrVar] = FALSE;
		}
	      }
	    }
	    bNotDone = iStackSize > 0;
	  }
	}
      }
    }

    iCurrAutarkySize = 0;
    for(UINT32 j = 1; j <= iNumVars; ++j) {
      if(aCurrAutarky[j]) ++iCurrAutarkySize;
    }
    printf("iCurrAutarkySize=%u\n",iCurrAutarkySize);
 
  void DefaultFlip();
  DefaultFlip();
  void CheckTermination();
  CheckTermination();
  if(bSolutionFound) printf("solution found\n"); else printf("solution not found yet\n");
  // Output function needs to be added
  void UpdateFalseClauseList();
  UpdateFalseClauseList();
}

int main() 
{
  sFilenameIn = "sample.cnf";
  Initialise();
  AnalyseTotalAssignment();
}


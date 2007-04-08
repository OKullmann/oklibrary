/*

      ##  ##  #####    #####   $$$$$   $$$$   $$$$$$    
      ##  ##  ##  ##  ##      $$      $$  $$    $$      
      ##  ##  #####   ##       $$$$   $$$$$$    $$      
      ##  ##  ##  ##  ##          $$  $$  $$    $$      
       ####   #####    #####  $$$$$   $$  $$    $$      
  ======================================================
  SLS SAT Solver from The University of British Columbia
  ======================================================
  ...Developed by Dave Tompkins (davet [@] cs.ubc.ca)...
  ------------------------------------------------------
  .......consult legal.txt for legal information........
  ......consult revisions.txt for revision history......
  ------------------------------------------------------
  ... project website: http://www.satlib.org/ubcsat ....
  ------------------------------------------------------
  .....e-mail ubcsat-help [@] cs.ubc.ca for support.....
  ------------------------------------------------------

*/

#include "ubcsat.h"

PROBABILITY iNovNoise;

void PickNovelty();
void PickNoveltyPlus();
void PickNoveltyW();
void PickNoveltyPlusW();


void AddNovelty() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("novelty","",FALSE,
    "Novelty",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickNovelty",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange",
    "default","default");
  
  AddParmProbability(&pCurAlg->parmList,"-novnoise","Novelty Noise","novelty noise set to [novnoise]","",&iNovNoise,0.50);

  CreateTrigger("PickNovelty",ChooseCandidate,PickNovelty,"","");


  pCurAlg = CreateAlgorithm("novelty","",TRUE,
    "Novelty: (weighted)",
    "McAllester, Selman, Kautz [AAAI 97]",
    "PickNoveltyW",
    "DefaultProceduresW,Flip+FalseClauseListW,VarLastChange",
    "wdefault","default");
  
  CopyParameters(pCurAlg,"novelty","",FALSE);

  CreateTrigger("PickNoveltyW",ChooseCandidate,PickNoveltyW,"","");

}

void AddNoveltyPlus() {
  
  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("novelty+","",FALSE,
    "Novelty+: Novelty with random walk",
    "Hoos [AAAI 99]",
    "PickNoveltyPlus",
    "DefaultProcedures,Flip+FalseClauseList,VarLastChange",
    "default","default");
  
  CopyParameters(pCurAlg,"novelty","",FALSE);

  AddParmProbability(&pCurAlg->parmList,"-wp","walk probability","Choose random variable from random false clause with prob [wp]","",&iWp,0.01);

  CreateTrigger("PickNoveltyPlus",ChooseCandidate,PickNoveltyPlus,"","");


  pCurAlg = CreateAlgorithm("novelty+","",1,
    "Novelty+: Novelty with random walk (weighted)",
    "Hoos [AAAI 99]",
    "PickNoveltyPlusW",
    "DefaultProceduresW,Flip+FalseClauseListW,VarLastChange",
    "wdefault","default");
  
  CopyParameters(pCurAlg,"novelty+","",FALSE);

  CreateTrigger("PickNoveltyPlusW",ChooseCandidate,PickNoveltyPlusW,"","");

 
}

void PickNovelty() {
 
  UINT32 i;
  UINT32 j;
  SINT32 iScore;
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE *pLit;
  LITTYPE *pClause;
  UINT32 iNumOcc;
  UINT32 iVar;

  UINT32 iYoungestVar;

  SINT32 iSecondBestScore;
  
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;

  iBestScore = iNumClauses;
  iSecondBestScore = iNumClauses;

  /* select an unsatisfied clause uniformly at random */

  if (iNumFalse) {
    iClause = aFalseList[RandomInt(iNumFalse)];
    iClauseLen = aClauseLen[iClause];
  } else {
    iFlipCandidate = 0;
    return;
  }

  pLit = pClauseLits[iClause];

  iYoungestVar = GetVarFromLit(*pLit);

  for (j=0;j<iClauseLen;j++) {

    /* for WalkSAT variants, it's faster to calculate the
       score for each literal than to cache the values */

    iScore = 0;

    iVar = GetVarFromLit(*pLit);

    iNumOcc = aNumLitOcc[*pLit];
    pClause = pLitClause[*pLit];
    
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause++]==0) {
        iScore--;
      }
    }

    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];
    
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause++]==1) {
        iScore++;
      }
    }

    /* keep track of which literal was the 'youngest' */

    if (aVarLastChange[iVar] > aVarLastChange[iYoungestVar])
      iYoungestVar = iVar;

    /* keep track of the 'best' and the 'second best' variables,
       breaking ties by selecting the younger variables */

    if ((iScore < iBestScore) || ((iScore == iBestScore) && (aVarLastChange[iVar] < aVarLastChange[iBestVar]))) {
      iSecondBestVar = iBestVar;
      iBestVar = iVar;
      iSecondBestScore = iBestScore;
      iBestScore = iScore;
    } else if ((iScore < iSecondBestScore) || ((iScore == iSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestVar]))) {
      iSecondBestVar = iVar;
      iSecondBestScore = iScore;
    }

    pLit++;
  }
  
  iFlipCandidate = iBestVar;

  /* if the best is the youngest, select it */

  if (iFlipCandidate != iYoungestVar)
    return;

  /* otherwise, choose the second best with probability (novnoise) */

  if (RandomProb(iNovNoise))
    iFlipCandidate = iSecondBestVar;
}

void PickNoveltyPlus()
{
 
  UINT32 iClause;
  UINT32 iClauseLen;

  LITTYPE litPick;

  /* with probability (iWp) uniformly choose an unsatisfied clause,
     and then uniformly choose a literal from that clause */

  if (RandomProb(iWp)) {
    if (iNumFalse) {
      iClause = aFalseList[RandomInt(iNumFalse)];
      iClauseLen = aClauseLen[iClause];
      litPick = (pClauseLits[iClause][RandomInt(iClauseLen)]);
      iFlipCandidate = GetVarFromLit(litPick);
    } else {
      iFlipCandidate = 0;
    }
  } else {

    /* otherwise, use regular novelty */

    PickNovelty();
  }
}

void PickNoveltyW() {

  /* for general comments, review the unweighted version */
 
  UINT32 i;
  UINT32 j;
  FLOAT fScore;
  UINT32 iClause;
  UINT32 iClauseLen;
  LITTYPE *pLit;
  LITTYPE *pClause;
  UINT32 iNumOcc;
  UINT32 iVar;

  UINT32 iYoungestVar;

  FLOAT fSecondBestScore;
  
  UINT32 iBestVar=0;
  UINT32 iSecondBestVar=0;

  fBestScore = fTotalWeight;
  fSecondBestScore = fTotalWeight;

  /* select the clause according to a weighted scheme */

  if (iNumFalse) {
    iClause = PickClauseWCS();
    iClauseLen = aClauseLen[iClause];
  } else {
    iFlipCandidate = 0;
    return;
  }

  pLit = pClauseLits[iClause];

  iYoungestVar = GetVarFromLit(*pLit);

  for (j=0;j<iClauseLen;j++) {
    fScore = 0.0f;

    iVar = GetVarFromLit(*pLit);

    iNumOcc = aNumLitOcc[*pLit];
    pClause = pLitClause[*pLit];
    
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause++]==0) {
        fScore -= aClauseWeight[*pClause];
      }
    }

    iNumOcc = aNumLitOcc[GetNegatedLit(*pLit)];
    pClause = pLitClause[GetNegatedLit(*pLit)];
    
    for (i=0;i<iNumOcc;i++) {
      if (aNumTrueLit[*pClause++]==1) {
        fScore += aClauseWeight[*pClause];
      }
    }

    if (aVarLastChange[iVar] > aVarLastChange[iYoungestVar])
      iYoungestVar = iVar;

    if ((fScore < fBestScore) || ((fScore == fBestScore) && (aVarLastChange[iVar] < aVarLastChange[iBestVar]))) {
      iSecondBestVar = iBestVar;
      iBestVar = iVar;
      fSecondBestScore = fBestScore;
      fBestScore = fScore;
    } else if ((fScore < fSecondBestScore) || ((fScore == fSecondBestScore) && (aVarLastChange[iVar] < aVarLastChange[iSecondBestVar]))) {
      iSecondBestVar = iVar;
      fSecondBestScore = fScore;
    }

    pLit++;
  }
  
  iFlipCandidate = iBestVar;

  if (iFlipCandidate != iYoungestVar)
    return;

  if (RandomProb(iNovNoise))
    iFlipCandidate = iSecondBestVar;
}

void PickNoveltyPlusW()
{

   /* for comments, review the unweighted version */
 
  UINT32 iClause;
  UINT32 iClauseLen;

  LITTYPE litPick;

  if (RandomProb(iWp)) {
    if (iNumFalse) {
      iClause = aFalseList[RandomInt(iNumFalse)];
      iClauseLen = aClauseLen[iClause];
      litPick = (pClauseLits[iClause][RandomInt(iClauseLen)]);
      iFlipCandidate = GetVarFromLit(litPick);
    } else {
      iFlipCandidate = 0;
    }
  } else {
    PickNoveltyW();
  }
}


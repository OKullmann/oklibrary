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

UINT32 iTabuTenureMedian;
UINT32 iTabuTenureInterval;
UINT32 iTabuTenureLow;
UINT32 iTabuTenureHigh;

void InitRoTS();
void PickRoTS();
void PickRoTSW();

void AddRoTS() {

  ALGORITHM *pCurAlg;

  pCurAlg = CreateAlgorithm("rots","",FALSE,
    "RoTS: Robust TABU Search",
    "Taillard [Parallel Computing 1991], based on implementation by Stuetzle",
    "PickRoTS",
    "DefaultProcedures,Flip+VarScore",
    "default","default");
  
  AddParmUInt(&pCurAlg->parmList,"-tabu","target tabu tenure","median tabu tenure length","",&iTabuTenureMedian,10);
  AddParmUInt(&pCurAlg->parmList,"-tabuinterval","tabu interval","tabu +/- interval size as a percent of tabu tenure","",&iTabuTenureInterval,25);

  CreateTrigger("InitRoTS",PreRun,InitRoTS,"","");
  CreateTrigger("PickRoTS",ChooseCandidate,PickRoTS,"InitRoTS,VarLastChange,BestFalse","");

  pCurAlg = CreateAlgorithm("rots","",1,
    "RoTS: Robust TABU Search (weighted)",
    "Taillard [Parallel Computing 1991], based on implementation by Stuetzle",
    "PickRoTSW",
    "DefaultProceduresW,Flip+VarScoreW",
    "wdefault","default");
  
  CopyParameters(pCurAlg,"rots","",FALSE);
  CreateTrigger("PickRoTSW",ChooseCandidate,PickRoTSW,"InitRoTS,VarLastChange,BestFalse","");

}

void InitRoTS() {

  /* use the iTabuTenureInterval to set the low & high range for the tabu tenure */
  iTabuTenure = iTabuTenureMedian;
  iTabuTenureLow = iTabuTenure - (iTabuTenureInterval * iTabuTenure) / 100;
  iTabuTenureHigh = iTabuTenure + (iTabuTenureInterval * iTabuTenure) / 100;
}

void PickRoTS() {
  
  UINT32 j;
  SINT32 iScore;
  UINT32 iTabuCutoff;

  /* every N steps, choose the tabu tenure uniformly from the tabu range */

  if (iTabuTenureLow != iTabuTenureHigh) {
    if ((iStep % iNumVars)==0) {
      iTabuTenure = iTabuTenureLow + RandomInt(iTabuTenureHigh - iTabuTenureLow);
    }
  }

  /* calculation of tabu cutoff */
  
  if (iStep > iTabuTenure) {
    iTabuCutoff = iStep - iTabuTenure;
    if (iVarLastChangeReset > iTabuCutoff) {
      iTabuCutoff = iVarLastChangeReset;
    }
  } else {
    iTabuCutoff = 1;
  }

  iNumCandidates = 0;
  iBestScore = iNumClauses;

  /* check all variables */

  for (j=1;j<=iNumVars;j++) {

    /* use cached value of score */    
    
    iScore = aVarScore[j];

    if (aVarLastChange[j] >= iTabuCutoff) { 

      /* check for aspiration (early exit) */

      if ((iNumFalse + iScore) < iBestNumFalse) {
        iFlipCandidate = j;
	      return;
      }

    } else if ((iStep - aVarLastChange[j]) > (iNumVars * 10)) {

      /* check for variable stagnation  (early exit) */

      iFlipCandidate = j;
	    return;

    } else { 

      /* build candidate list of best vars */

      if (iScore <= iBestScore) {
        if (iScore < iBestScore) {
          iNumCandidates=0;
          iBestScore = iScore;
        }
        aCandidateList[iNumCandidates++] = j;
      }
    }
  }

  /* select flip candidate uniformly from candidate list */
  
  if (iNumCandidates > 1)
    iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
  else
    iFlipCandidate = aCandidateList[0];

}

void PickRoTSW() {
  
  UINT32 j;
  FLOAT fScore;
  UINT32 iTabuCutoff;

  /* every N steps, choose the tabu tenure uniformly from the tabu range */

  if (iTabuTenureLow != iTabuTenureHigh) {
    if ((iStep % iNumVars)==0) {
      iTabuTenure = iTabuTenureLow + RandomInt(iTabuTenureHigh - iTabuTenureLow);
    }
  }

  /* calculation of tabu cutoff */
  
  if (iStep > iTabuTenure) {
    iTabuCutoff = iStep - iTabuTenure;
    if (iVarLastChangeReset > iTabuCutoff) {
      iTabuCutoff = iVarLastChangeReset;
    }
  } else {
    iTabuCutoff = 1;
  }

  iNumCandidates = 0;
  fBestScore = fTotalWeight;

  /* check all variables */

  for (j=1;j<=iNumVars;j++) {

    /* use cached value of weighted score */

    fScore = aVarScoreW[j];

    if (aVarLastChange[j] >= iTabuCutoff) { 

      /* check for aspiration (early exit) */

      if ((fSumFalseW + fScore) < fBestSumFalseW) {
        iFlipCandidate = j;
	      return;
      }
    } else if ((iStep - aVarLastChange[j]) > (iNumVars * 10)) {
      
      /* check for variable stagnation  (early exit) */

      iFlipCandidate = j;
	    return;

    } else { 

      /* build candidate list of best vars */

      if (fScore <= fBestScore) {
        if (fScore < fBestScore) {
          iNumCandidates=0;
          fBestScore = fScore;
        }
        aCandidateList[iNumCandidates++] = j;
      }
    }
  }

  /* select flip candidate uniformly from candidate list */
  
  if (iNumCandidates > 1)
    iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
  else
    iFlipCandidate = aCandidateList[0];

}




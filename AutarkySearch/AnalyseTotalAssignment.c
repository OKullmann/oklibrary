// Who created this file, and when?

/*!
  \file AutarkySearch/AnalyseTotalAssignment.c
  \brief Supplementing UBCSAT by analysing total assignments for
  contained autarkies.

  \todo Add file creation date (first file line).
  \todo Create Doxygen documentation.
  \todo Improve C usage (local declaration of variables, local
  loop variables etc.).
*/

#include <stdio.h>

#include <ubcsat.h>
 

void Initialise();
void AnalyseTotalAssignment();

void Initialise()
{
  // call the relevant UBCSAT library procedures to detach this function from being inside the UBCSAT system
  UINT32 i= 1;
  void ReadCNF();
  ReadCNF();
  void CreateLitOccurence(); // allows useage of aNumLitOcc, pLitClause
  CreateLitOccurence();
  void CreateCandidateList(); // allows useage of aCandidateList
  CreateCandidateList();
  void CreateDefaultStateInfo(); // allows the usage of aVarValue array once initialised
  CreateDefaultStateInfo();
  void CreateFalseClauseList(); // allows useage of the new false clause list after variable flip
  CreateFalseClauseList();

  // UBCSAT uses aVarInit to determine how variable i is to be assigned (2 = random)
  while(i <= iNumVars) {
    aVarInit[i++] = 2;
  } 

  void DefaultInitVars();
  DefaultInitVars(); // initially assigns ALL variables randomnly (randomnly due to above loop)
  void InitDefaultStateInfo();
  InitDefaultStateInfo(); // necessary to get aNumTrueLit working
  void InitFalseClauseList();
  InitFalseClauseList(); // necessary to get aFalseList working
}


void AnalyseTotalAssignment()
{
  BOOL bRemoveVars = FALSE, bNegLitSat, bPosLitSat, bOneLitTrue = FALSE;
  BOOL *aCurrAutarky;
  BOOL *aTmpCurrAutarky;
  UINT32 iiVar, iiVarLitValue, iiFlipVarLitValue, iCurrClauseNum, iCurrVar, iCurrVarLitValue, iCurrFlipVarLitValue, iCurrLit, iCurrAutarkySize;
  UINT32 *aLitClauses;
  UINT32 i, j, k, m;

  sFilenameIn = "sample.cnf";
  Initialise();

  aCurrAutarky = (BOOL *)malloc((iNumVars+1) * sizeof(BOOL));
  for(i=1;i<=iNumVars;i++) {
    iiVar = i;
    for(j=1;j<=iNumVars;j++) {
      aCurrAutarky[j] = TRUE;
    }

    iiVarLitValue = aVarValue[i] ? i*2 : (i*2) + 1; 
    iiFlipVarLitValue = aVarValue[i] ? iiVarLitValue + 1  : iiVarLitValue - 1;
    aLitClauses = pLitClause[iiVarLitValue];

    for(j=0;j<aNumLitOcc[iiVarLitValue];j++) {
      if(aNumTrueLit[aLitClauses[j]] == 1) {
        for(k=0;k<aClauseLen[aLitClauses[j]];k++) {
          iCurrVar = pClauseLits[aLitClauses[j]][k] / 2;
          aCurrAutarky[iCurrVar] = FALSE;  
        }
      }
    }
    for(j=0;j<iNumFalse;j++) {
      iCurrClauseNum = aFalseList[j];
      bRemoveVars = TRUE;
      aTmpCurrAutarky = aCurrAutarky;    
      for(k=0;k<aClauseLen[iCurrClauseNum];k++) {
        iCurrVar = pClauseLits[iCurrClauseNum][k] / 2;
        if(iCurrVar == iiVar) {
          bRemoveVars = FALSE;
          break;
        } else {
          aTmpCurrAutarky[iCurrVar] = FALSE;
        }             
      }
      if(bRemoveVars) aCurrAutarky = aTmpCurrAutarky;
    }
    // this loop needs to be tested, above it works
    for(j=1;j<=iNumVars;j++) {
      if(aCurrAutarky[j]) {
        if(j == iiVar) {
	  iCurrVarLitValue = iiFlipVarLitValue;
          iCurrFlipVarLitValue = aVarValue[j] ? j*2 : (j*2)+1;
	} else {
	  iCurrVarLitValue = aVarValue[j] ? j*2 : (j*2)+1;
          iCurrFlipVarLitValue = aVarValue[j] ? (j*2)+1 : j*2;
	}
        aLitClauses = pLitClause[iCurrFlipVarLitValue];
        for(k=0;k < aNumLitOcc[iCurrFlipVarLitValue];k++) {
          bOneLitTrue = FALSE;
          aTmpCurrAutarky = aCurrAutarky;
          for(m=0;m < aClauseLen[aLitClauses[k]];m++) {
            iCurrLit = pClauseLits[aLitClauses[k]][m];
            iCurrVar = iCurrLit / 2;
            aTmpCurrAutarky[iCurrVar] = FALSE;
            if(iCurrVar == iiVar) {
               bNegLitSat = aVarValue[iCurrVar] && (iCurrLit == (iCurrVar*2)+1);
               bPosLitSat = (!aVarValue[iCurrVar]) && (iCurrLit == iCurrVar*2);
            } else {
              bNegLitSat = (!aVarValue[iCurrVar]) && (iCurrLit == (iCurrVar*2)+1);
              bPosLitSat = aVarValue[iCurrVar] && (iCurrLit == iCurrVar*2);
            }
            bOneLitTrue = aCurrAutarky[iCurrVar] && (bNegLitSat || bPosLitSat) ? TRUE : FALSE;
            if(bOneLitTrue)  break;  
          }
          if(!bOneLitTrue) aCurrAutarky = aTmpCurrAutarky;
        } 
      }
    }
    iCurrAutarkySize = 0;
    for(j = 1;j <= iNumVars;j++) {
      if(aCurrAutarky[j]) iCurrAutarkySize++;
    }
    if(i == 1) {
      iBestScore = iCurrAutarkySize;
      iNumCandidates = 1;
      aCandidateList[0] = i;
    } else {
      if(iCurrAutarkySize <= iBestScore) {
        if(iCurrAutarkySize == iBestScore) {
          aCandidateList[iNumCandidates++] = i;
        } else {
          iBestScore = iCurrAutarkySize;
          iNumCandidates = 1;
          aCandidateList[0] = i;
        }
      }
    }
  }
  if (iNumCandidates > 1) {
    iFlipCandidate = aCandidateList[RandomInt(iNumCandidates)];
  } else {
    iFlipCandidate = aCandidateList[0];
  }
  void DefaultFlip();
  DefaultFlip();
  void CheckTermination();
  CheckTermination();
  // Output function needs to be added
  void UpdateFalseClauseList();
  UpdateFalseClauseList();
}




//Matthew Lewsey, 2.2.2007 (Swansea)

#include "ubcsat.h"
#include <stdio.h>
 
/*!
  \file AutarkySearch/AnalyseTotalAssignment.c
  \brief Supplementing UBCSAT by analysing total assignments for
  contained autarkies.

  \todo Add file creation date (first file line).
  \todo Create Doxygen documentation.
  \todo Improve C usage (local declaration of variables, local
  loop variables etc.).
*/

void Initialise();
void AnalyseTotalAssignment();

void Initialise()
{
  //call the relevant UBCSAT library procedures to detach this function from being inside the UBCSAT system
  void ReadCNF();
  ReadCNF();
  void CreatLitOccurence();
  CreateLitOccurence(); //allows useage of aNumLitOcc, pLitClause
  void CreateCandidateList();
  CreateCandidateList(); //allows useage of aCandidateList
  void CreateDefaultStateInfo();
  CreateDefaultStateInfo(); //allows the usage of aVarValue array once initialised
  void CreateFalseClauseList();
  CreateFalseClauseList(); //allows useage of the new false clause list after variable flip

  //UBCSAT uses aVarInit to determine how variable i is to be assigned (2 = random)
  for(UINT32 i=1;i<=iNumVars;i++) {
    aVarInit[i] = 2;
  } 

  void DefaultInitVars();
  DefaultInitVars(); //initially assigns ALL variables randomnly (randomnly due to above loop)
  void DefaultStateInto();
  InitDefaultStateInfo(); //necessary to get aNumTrueLit working
  void InitFalseClauseList();
  InitFalseClauseList(); //necessary to get aFalseList working
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

    for(UINT32 j=0;j<=iNumVars;j++) {
      aCurrAutarky[j] = TRUE;
    }

    for(UINT32 j=0;j<iNumFalse;j++) {
      iCurrClauseNum = aFalseList[j];
      iClauseLen = aClauseLen[iCurrClauseNum];
      for(UINT32 k=0;k<iClauseLen;k++) {
	iCurrVar = pClauseLits[iCurrClauseNum][k-aClauseLen[iCurrClauseNum]] / 2;
	aCurrAutarky[iCurrVar] = FALSE;
      }
    }

    for(UINT32 j=1;j<=iNumVars;j++) {
      if(aCurrAutarky[j] == FALSE) continue;
      iCurrFlipVarLitValue = aVarValue[j] ? (j*2)+1 : j*2;
      aLitClauses = pLitClause[iCurrFlipVarLitValue];
      for(UINT32 k=0;k<aNumLitOcc[iCurrFlipVarLitValue];k++) {
	iCurrClauseNum = aLitClauses[k];
	iClauseLen = aClauseLen[iCurrClauseNum];
	for(UINT32 m=0;m<iClauseLen*2;m++) {
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
	if(!bOneLitTrue) { 
	  while(bNotDone) {
	    iCurrStackVar = aStack[iStackSize - 1];
	    iStackSize--;
	    iCurrFlipVarLitValue = aVarValue[iCurrStackVar] ? (j*2)+1 : j*2;
	    aLitClauses = pLitClause[iCurrFlipVarLitValue];
	    for(UINT32 m=0;m<aNumLitOcc[iCurrFlipVarLitValue];m++) {
	      iCurrClauseNum = aLitClauses[m];
	      iClauseLen = aClauseLen[iCurrClauseNum];
	      for(UINT32 n=0;n<iClauseLen;n++) {
		iCurrLit = pClauseLits[iCurrClauseNum][n];
		iCurrVar = iCurrLit / 2;
		if(aCurrAutarky[iCurrVar] == TRUE) {
		  aStack[iStackSize] = iCurrVar;
		  iStackSize++;
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
    for(UINT32 j = 1;j <= iNumVars;j++) {
      if(aCurrAutarky[j]) iCurrAutarkySize++;
    }
    printf("iCurrAutarkySize=%d\n",iCurrAutarkySize);
 
  void DefaultFlip();
  DefaultFlip();
  void CheckTermination();
  CheckTermination();
  if(bSolutionFound) printf("solution found\n"); else printf("solution not found yet\n");
  //Output function needs to be added
  void UpdateFalseClauseList();
  UpdateFalseClauseList();
}

int main() 
{
  sFilenameIn = "sample.cnf";
  Initialise();
  AnalyseTotalAssignment();
}


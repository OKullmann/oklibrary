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

void AddDataTriggers();


/***** Trigger ReadCNF *****/
/* 
    iNumVars              # of variables
    iNumClauses           # of clauses
    iNumLits              # of literals

    aClauseLen[j]         length of clause[j]
    aClauseLits[j][k]     literal [k] of clause[j]           

    aClauseWeight         (if weighted algorithm) weight of clause[j]
    fTotalWeight          sum of all clause weights
*/

extern UINT32 iNumVars;
extern UINT32 iNumClauses;
extern UINT32 iNumLits;

extern UINT32 *aClauseLen;
extern LITTYPE **pClauseLits;

extern FLOAT *aClauseWeight;
extern FLOAT fTotalWeight;

extern UINT32 iVARSTATELen;


/***** Trigger LitOccurence *****/
/* 
    aNumLitOcc[j]         # of times literal j occurs
    pLitClause[j]         pointer to list of clauses literal j appears in
*/

extern UINT32 *aNumLitOcc;
extern UINT32 *aLitOccData;
extern UINT32 **pLitClause;


/***** Trigger CandidateList *****/
/* 
    aCandidateList        array used by algorithms to store 'best' flip candidates
    iNumCandidates        number of 'best' flip candidates
*/

extern UINT32 *aCandidateList;
extern UINT32 iNumCandidates;


/***** Trigger InitVarsFromFile *****/
/***** Trigger DefaultInitVars *****/
/*
    aVarInit[j]           how variable[j] is initialised each run (0,1,2 => False,True,Random)
    iInitVarFlip          # of variables to be 're-flipped' after initialisation
    bVarInitGreedy        if true, then greedily init vars that have a biased # of pos/neg literals
*/
  
extern UINT32 *aVarInit;
extern UINT32 iInitVarFlip;
extern BOOL bVarInitGreedy;


/***** Trigger DefaultStateInfo *****/
/*
    iNumFalse             # of currently unsatisfied (false) clauses
    fSumFalseW            sum of the weights of currently unsatisfied (false) clauses
    aNumTrueLit[j]        # of true literals occuring in clause[j] (i.e.: 0 -> false clause)
    aVarValue[j]          current value of variable[j] (0,1 => False,True)
*/

extern UINT32 *aNumTrueLit;
extern UINT32 *aVarValue;
extern UINT32 iNumFalse;
extern FLOAT fSumFalseW;


/***** Trigger DefaultFlip[W] *****/


/***** Trigger CheckTermination *****/


/***** Trigger FalseClauseList *****/
/***** Trigger Flip+FalseClauseList[W] *****/
/*
    aFalseList[j]         clause # for the jth false clause
    aFalseListPos[j]      for clause[j], position it occurs in aFalseList
    iNumFalseList         # entries in aFalseList (the same as iNumFalse, but updated separately)
*/

extern UINT32 *aFalseList;
extern UINT32 *aFalseListPos;
extern UINT32 iNumFalseList;


/***** Trigger VarScore[W] *****/
/***** Trigger Flip+VarScore[W] *****/
/*
    aVarScore[j]          score for variable[j]... Change in # false clauses if variable[j] is flipped
    aVarScoreW[j]         weighted score for variable[j]... Change in sum of false clause weights if variable[j] is flipped
*/

extern SINT32 *aVarScore;
extern FLOAT *aVarScoreW;


/***** Trigger MakeBreak[W] *****/
/***** Trigger Flip+MakeBreak[W] *****/
/*
    aMakeCount[k]         # of clauses that flipping variable[j] will 'make'
    aBreakCount[k]        # of clauses that flipping variable[j] will 'break'
    aCritSat[k]           critical variable for clause[k] if clause has only one true literal
    aMakeCountW[k]        (same as aMakeCount, but as a sum of clause weights)
    aBreakCountW[k]       (same as aBreakCount, but as a sum of clause weights)

*/

extern UINT32 *aBreakCount;
extern UINT32 *aMakeCount;
extern UINT32 *aCritSat;
extern FLOAT *aBreakCountW;
extern FLOAT *aMakeCountW;


/***** Trigger VarInFalse *****/
/***** Trigger Flip+VarInFalse *****/
/*
    aVarInFalseList[j]    variable # for the jth variable that appears in false clauses
    aVarInFalseListPos[j] for variable[j], position it occurs in aVarInFalseList
    iNumVarsInFalseList   # variables that appear in false clauses 
*/

extern UINT32 iNumVarsInFalseList;
extern UINT32 *aVarInFalseList;
extern UINT32 *aVarInFalseListPos;


/***** Trigger VarLastChange *****/
/*
    aVarLastChange[j]     the step # of the most recent time variable[j] was flipped
    iVarLastChangeReset   the step # of the last time all aVarLastChange values were reset
*/

void UpdateVarLastChange();
extern UINT32 *aVarLastChange;
extern UINT32 iVarLastChangeReset;


/***** Trigger TrackChanges *****/
/*
    iNumChanges           # of changes to aVarScore[] values this step
    aChangeList[j]        variable # of the jth variable changed this step
    aChangeOldScore[j]    the previous score of variable[j]
    aChangeLastStep[j]    the step of the last change for variable[j]
*/

extern UINT32 iNumChanges;
extern UINT32 *aChangeList;
extern SINT32 *aChangeOldScore;
extern UINT32 *aChangeLastStep;


/***** Trigger BestScoreList *****/
/*
    iNumBestScoreList       # of variables tied for the 'best' aVarScore
    *aBestScoreList[j]      variable # of the jth variable with the best score
    *aBestScoreListPos[j]   the location of variable[j] in aBestScoreList
*/

extern UINT32 iNumBestScoreList;
extern UINT32 *aBestScoreList;
extern UINT32 *aBestScoreListPos;


/***** Trigger ClausePenalty *****/
/*
    aClausePenalty[j]       dynamic clause penalty for clause[j]
    bClausePenaltyCreated   boolean to indicate clause penalties exist
    fBasePenalty            value of smallest possible penalty
    fTotalPenalty           sum of all penalties
*/

extern FLOAT *aClausePenalty;
extern BOOL bClausePenaltyCreated;
extern FLOAT fBasePenalty;
extern FLOAT fTotalPenalty;


/***** Trigger MakeBreakPenalty *****/
/*
    aMakePenalty[j]       (same as aMakeCount, but as a sum of dynaimc clause penalties)  
    aBreakPenalty[j]      (same as aBreakCount, but as a sum of dynaimc clause penalties)  
*/

extern FLOAT *aMakePenalty;
extern FLOAT *aBreakPenalty;


/***** Trigger NullFlips *****/
/*
    iNumNullFlip          number of null flips (steps where iFlipCandidate == 0)
*/

extern UINT32 iNumNullFlips;

 
/***** Trigger VarStateBackup *****/
/*    
    aVarStateBackup[j]    backup value of variable[j]...used with BackupVarState() & RestoreVarState

    BackupVarState()      routine to back up current variable state
    RestoreVarState()     routine to restore backed up variable state
*/

extern void BackupVarState();
extern void RestoreVarState();

extern UINT32 *aVarStateBackup;


/***** Trigger LogDist *****/
/*
    aLogDistValues[j]         array from 10...iCutoff of log-distributed values
    iNumLogDistValues         size of aLogDistValues[]
    iLogDistStepsPerDecade    # of elements in aLogDistValues per decade
*/

extern UINT32 *aLogDistValues;
extern UINT32 iNumLogDistValues;
extern UINT32 iLogDistStepsPerDecade;


/***** Trigger BestFalse *****/
/*
    iBestNumFalse             best value of iNumFalse this run
    iBestStepNumFalse         step where iBestNumFalse occured
    fBestSumFalseW            best weighted sum of false clauses seen this run
    iBestStepSumFalseW        step where iBestSumFalseW occured
*/

extern UINT32 iBestNumFalse;
extern UINT32 iBestStepNumFalse;
extern FLOAT fBestSumFalseW;
extern UINT32 iBestStepSumFalseW;


/***** Trigger SaveBest *****/
/*
    aVarStateBest[j]          the candidate solution for the best solution quality seen this run   
*/
extern UINT32 *aVarStateBest;


/***** Trigger NoImprove *****/
/*
    iNoImprove            terminate this run if no improvement in iNoImprove steps
*/

extern UINT32 iNoImprove;


/***** Trigger StartSeed *****/
/*
    iStartSeed            starting seed required to duplicate the current run
*/

extern UINT32 iStartSeed;


/***** Trigger CountRandom *****/
/*
    iNumRandomCalls       # of calls to random number generator this run

    (note that code is in ubcsat-io.c)
*/


/***** Trigger CheckTimeout *****/


/***** Trigger CheckForRestarts *****/


/***** Trigger FlipCounts *****/
/*
    aFlipCounts[j]        # of times that variable[j] has been flipped this run
*/

extern UINT32 *aFlipCounts;


/***** Trigger UnsatCounts *****/
/*
    aUnsatCounts[j]       # of steps that clause[j] has been unsatisfied
*/

extern UINT32 *aUnsatCounts;


/***** Trigger ClauseLast *****/
/*
    aClauseLast[j]        last step # for which clause[j] was unsatisfied
*/

extern UINT32 *aClauseLast;


/***** Trigger ClauseLast *****/
/*
    aSQGrid[j][k]         for run [k], solution quality at step aLogDistValues[j]
    aSQGridW[j][k]        for run [k], solution quality at step aLogDistValues[j]
*/

extern FLOAT *aSQGridW;
extern UINT32 *aSQGrid;


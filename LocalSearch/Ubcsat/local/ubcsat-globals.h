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

/***** UBCSAT GLOBAL VARIABLES *****/
/*
    sAlgName              name of current algorithm
    sVarName              variant name of current algorithm
    bWeighted             does the current algorithm use weighted clauses?

    pActiveAlgorithm      the active algorithm for this session of UBCSAT

    iNumRuns              total number of runs
    iCutoff               step cutoff for each run
    iTimeOut              timeout for all runs in seconds
    iSeed                 initial seed for the system

    iTarget               target solution quality (# of false clauses)
    fTargetW              weighted target solution quality (sum of false clause weights)

    iFlipCandidate        current variable to flip

    iFind                 total number of solutions to find
    iNumSolutionsFound    number of solutions found so far

    iPeriodicRestart      restart each run every iPeriodicRestart steps
    iProbRestart          restart with a probability
    iStagnateRestart      restart if no improvement in iStagnateRestart steps

    bRestart              flag to restart the current run

    iRun                  current run number
    iStep                 current step number

    bTerminateAllRuns     flag to terminate all runs
    bSolutionFound        flag to indicate target solution quality found this run 
    bTerminateRun         flag to terminate this run

    bSolveMode            flag to indicate "solve mode"

    sFilenameIn           file name of instance
    sFilenameParms        file name of current parameter file (note, can be multiple files)
    sFilenameVarInit      file name of variable initialisation file

    bReportEcho           flag to set all file output to screen
    bReportClean          flag to remove headers from output

    iBestScore            value of best score improvement this step
    fBestScore            value of best weighted score improvement this step
*/

extern char sNull;

extern char *sAlgName;
extern char *sVarName;
extern BOOL bWeighted;

ALGORITHM *pActiveAlgorithm;

extern UINT32 iNumRuns;
extern UINT32 iCutoff;
extern UINT32 iTimeOut;
extern UINT32 iSeed;

extern UINT32 iTarget;
extern FLOAT fTargetW;

extern UINT32 iFlipCandidate;

extern UINT32 iFind;
extern UINT32 iNumSolutionsFound;
extern UINT32 iPeriodicRestart;
extern PROBABILITY iProbRestart;
extern UINT32 iStagnateRestart;

extern BOOL bRestart;

extern UINT32 iRun;
extern UINT32 iStep;

extern BOOL bTerminateAllRuns;
extern BOOL bSolutionFound;
extern BOOL bTerminateRun;

extern BOOL bSolveMode;

extern char *sFilenameIn;
extern char *sFilenameParms;
extern char *sFilenameVarInit;

extern BOOL bReportEcho;
extern BOOL bReportClean;

extern SINT32 iBestScore;
extern FLOAT fBestScore;


/***** UBCSAT GLOBAL ROUTINES *****/

/*
    CreateAlgorithm()     add a new algorithm to the UBCSAT system
*/

ALGORITHM *CreateAlgorithm (const char *sName, const char *sVariant, BOOL bWeighted, 
                            const char *sDescription, 
                            const char *sAuthors,
                            const char *sHeuristicTriggers,
                            const char *sDataTriggers,
                            const char *sDefaultOutput,
                            const char *sDefaultStats);


/*
    CopyParameters()      copy the parameters from one algorithm to another
*/

void CopyParameters(ALGORITHM *pDest, const char *sName, const char *sVar, BOOL bWeighted);

/*
    InheritDataTriggers()   copy the data triggers from one algorithm to another
*/

void InheritDataTriggers(ALGORITHM *pDest, const char *sName, const char *sVar, BOOL bWeighted);

/*
    CreateTrigger()       add a new trigger to the UBCSAT system
*/

void CreateTrigger(const char *sID,
                   enum EVENTPOINT eEventPoint,
                   FXNPTR pProcedure,
                   char *sDependencyList,
                   char *sDeactivateList);

/*
    CreateContainerTrigger()    add a new container trigger to the UBCSAT system
*/

void CreateContainerTrigger(const char *sID, const char *sList);

/*
    AddParm????()         adds a parameter to an algorithm (many different types)
*/


void AddParmProbability(ALGPARMLIST *pParmList,
                  const char *sSwitch, 
                  const char *sName, 
                  const char *sDescription,
                  const char *sTriggers,
                  PROBABILITY *pProb,
                  FLOAT fProb);

void AddParmUInt(ALGPARMLIST *pParmList, 
                  const char *sSwitch, 
                  const char *sName, 
                  const char *sDescription,
                  const char *sTriggers,
                  UINT32 *pInt,
                  UINT32 iDefInt);

void AddParmSInt(ALGPARMLIST *pParmList, 
                  const char *sSwitch, 
                  const char *sName, 
                  const char *sDescription,
                  const char *sTriggers,
                  SINT32 *pSInt,
                  SINT32 iDefSInt);

void AddParmBool(ALGPARMLIST *pParmList, 
                  const char *sSwitch, 
                  const char *sName, 
                  const char *sDescription,
                  const char *sTriggers,
                  UINT32 *pBool,
                  BOOL bDefBool);

void AddParmFloat(ALGPARMLIST *pParmList, 
                  const char *sSwitch, 
                  const char *sName, 
                  const char *sDescription,
                  const char *sTriggers,
                  FLOAT *pFloat,
                  FLOAT fDefFloat);

void AddParmString(ALGPARMLIST *pParmList, 
                  const char *sSwitch, 
                  const char *sName, 
                  const char *sDescription,
                  const char *sTriggers,
                  char **pString,
                  char *sDefString);

                  
/*
    CreateReport()        add a new report to the system
*/

REPORT *CreateReport(const char *sID, 
                     const char *sDescription, 
                     const char *sOutputFile, 
                     const char *sTriggers);

/*
    AddReportParm???()    add a parameter to a report
*/

void AddReportParmUInt(REPORT *pRep, const char *sParmName, UINT32 *pDefault);
void AddReportParmFloat(REPORT *pRep, const char *sParmName, FLOAT *pDefault);
void AddReportParmString(REPORT *pRep, const char *sParmName, const char *pDefault);

/*
    AddColumn????()       add a column of data for output & rtd reports
*/

void AddColumnUInt(const char *sID, 
                   const char *sDescription, 
                   char *sHeader1,  
                   char *sHeader2,  
                   char *sHeader3, 
                   char *sPrintFormat, 
                   UINT32 *pCurValue,
                   char *sTriggers,
                   enum COLTYPE eColType);

void AddColumnFloat(const char *sID, 
                    const char *sDescription, 
                    char *sHeader1,  
                    char *sHeader2,  
                    char *sHeader3, 
                    char *sPrintFormat,
                    FLOAT *pCurValue,
                    char *sTriggers,
                    enum COLTYPE eColType);

void AddColumnComposite(const char *sID, 
                        const char *sList);

/*
    AddStat()       add a statistic, calculated on a column of data
*/

void AddStat(const char *sID, 
             const char *sDescription, 
             const char *sBaseDescription, 
             const char *sDefParm,
             const char *sRequiredCols,
             const char *sDataColumn,
             const char *sTriggers,
             BOOL bSortByStep);

void AddContainerStat(const char *sID, 
                      const char *sList);

/*
    AddStatCustom()     add a custom statistic, which can be calculated via triggers
*/

void AddStatCustom(const char *sID, 
                   const char *sDescription, 
                   const char *sPrintCustomFormat,
                   void *pCurValue,
                   enum CDATATYPE eCustomType,
                   const char *sRequiredCols,
                   const char *sTriggers);


/*
    IsLocalMinimum()      returns TRUE if currently in a local minimum
*/

BOOL IsLocalMinimum();


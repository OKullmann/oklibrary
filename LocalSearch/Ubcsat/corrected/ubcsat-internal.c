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


/*  
    This file is the kitchen sink of ubcsat... 
    ideally, you shouldn't have to ever look at any of this code.
*/

char *sValidStatCodes[NUMVALIDSTATCODES] = {"all","mean","stddev","cv","median","min","max","q05","q10","q25","q75","q90","q95","q98","qr75/25","qr90/10","qr95/05"};

UINT32 aActiveCalcColumns[MAXITEMLIST];
FXNPTR aActiveProcedures[NUMEVENTPOINTS][MAXFXNLIST];
ALGORITHM aAlgorithms[MAXNUMALG];
REPORTCOL aColumns[MAXITEMLIST];
UINT32 aNumActiveProcedures[NUMEVENTPOINTS];
UINT32 aOutputColumns[MAXITEMLIST];
REPORT aReports[MAXREPORTS];
UINT32 aRTDColumns[MAXITEMLIST];
REPORTSTAT aStats[MAXITEMLIST];
char **aTotalParms;
TRIGGER aTriggers[MAXITEMLIST];
BOOL bReportClean;
BOOL bReportEcho;
BOOL bRestart;
BOOL bSolutionFound = FALSE;
BOOL bSolveMode = FALSE;
BOOL bTerminateAllRuns = FALSE;
BOOL bTerminateRun = FALSE;
BOOL bValidArgument[MAXTOTALPARMS];
BOOL bWeighted = 0;
FLOAT fDummy = 0.0;
FLOAT fFlipsPerSecond;
FLOAT fBestScore;
FLOAT fTargetW;
SINT32 iBestScore;
UINT32 iCutoff;
UINT32 iFind;
UINT32 iFlipCandidate;
UINT32 iNumActiveCalcColumns = 0;
UINT32 iNumAlg = 0;
UINT32 iNumOutputColumns = 0;
UINT32 iNumReports = 0;
UINT32 iNumRTDColumns = 0;
UINT32 iNumRuns;
UINT32 iNumSolutionsFound;
UINT32 iNumStatsActive = 0;
UINT32 iNumTotalParms;
UINT32 iPeriodicRestart;
PROBABILITY iProbRestart;
UINT32 iRun;
UINT32 iRunProceduresLoop;
UINT32 iRunProceduresLoop2;
UINT32 iSeed;
UINT32 iStagnateRestart;
UINT32 iStep;
UINT32 iTarget;
UINT32 iTimeOut;
ITEMLIST listColumns = { 0 };
ITEMLIST listStats = { 0 };
ITEMLIST listTriggers = { 0 };
ALGORITHM *pActiveAlgorithm = NULL;
ALGPARMLIST parmAlg;
ALGPARMLIST parmHelp;
ALGPARMLIST parmIO;
ALGPARMLIST parmUBCSAT;
char *sAlgName = &sNull;
char sDefaultComment[] = "#";
char *sCommentString = sDefaultComment;
char *sFilenameIn = &sNull;
char *sFilenameParms;
char *sFilenameVarInit = &sNull;
char sMasterString[MAXITEMLISTSTRINGLENGTH];
char sNull = 0;
char sParmLine[MAXPARMLINELEN];
char sStringParm[MAXPARMLINELEN];
char *sVarName = &sNull;

BOOL bReportStateLMOnly = 0;
FLOAT fReportStateQuality = -1.0f;
BOOL bReportStateQuality;
UINT32 iReportStateQuality;

void AddContainerItem(ITEMLIST *pList,const char *sID, const char *sList);
void AddItem(ITEMLIST *pList,const char *sID);
ALGPARM *AddParmCommon(ALGPARMLIST *pParmList,const char *sSwitch,const char *sName,const char *sDescription,const char *sTriggers);
void AddReportParmCommon(REPORT *pRep, const char *sParmName);
void DeactivateTrigger(UINT32 iFxnID, const char *sItem);
ALGORITHM *FindAlgorithm(const char *sFindName, const char *sFindVar, BOOL bFindWeighted);
UINT32 MatchParameter(char *sSwitch,char *sParm);
void ParseParameters(ALGPARMLIST *pParmList);
void SetAlgorithmDefaultReports();
void SetDefaultParms(ALGPARMLIST *pParmList);
void VerifyStatsParms (char *sStatsParms);

void ActivateAlgorithmTriggers() {
  ParseItemList(&listTriggers,pActiveAlgorithm->sDataTriggers,ActivateTrigger);
  ParseItemList(&listTriggers,pActiveAlgorithm->sHeuristicTriggers,ActivateTrigger);
}

void ActivateColumn(UINT32 iColID, const char *sItem) {
  
  REPORTCOL *pCol;
  NOREF(sItem);
 
  pCol = &aColumns[iColID];

  if (pCol->bActive == 0) {
  
    pCol->bActive = 1;

    ParseItemList(&listTriggers,pCol->sTriggers,ActivateTrigger);

    switch (pCol->eColType) {
      case ColTypeMean:
      case ColTypeStddev:
      case ColTypeCV:
      case ColTypeFinalDivStep:
      case ColTypeFinalDivStep100:
        pCol->pfColumnData = AllocateRAM(iNumRuns * sizeof(FLOAT));
        memset(pCol->pfColumnData,0,(iNumRuns)*sizeof(FLOAT));
      break;

      case ColTypeFinal:
      case ColTypeMin:
      case ColTypeMax:
        switch (pCol->eDataType) {
          case DTypeUInt:
            pCol->puiColumnData = AllocateRAM(iNumRuns * sizeof(UINT32));
            memset(pCol->puiColumnData,0,(iNumRuns)*sizeof(UINT32));
            break;
          case DTypeSInt:
            pCol->psiColumnData = AllocateRAM(iNumRuns * sizeof(SINT32));
            memset(pCol->psiColumnData,0,(iNumRuns)*sizeof(SINT32));
            break;
          case DTypeFloat:
            pCol->pfColumnData = AllocateRAM(iNumRuns * sizeof(FLOAT));
            memset(pCol->pfColumnData,0,(iNumRuns)*sizeof(FLOAT));
            break;
        }
        break;
    }

    switch (pCol->eColType) {
      case ColTypeMean:
      case ColTypeStddev:
      case ColTypeCV:
      case ColTypeMin:
      case ColTypeMax:
        ParseItemList(&listTriggers,"ColumnStepCalculation",ActivateTrigger);
        aActiveCalcColumns[iNumActiveCalcColumns++] = iColID;
      case ColTypeFinal:
      case ColTypeFinalDivStep:
      case ColTypeFinalDivStep100:
        ParseItemList(&listTriggers,"ColumnRunCalculation",ActivateTrigger);
        break;
    }
  }
}  

void ActivateStat(UINT32 iStatID, const char *sItem) {

  REPORTSTAT *pStat;
  char *pPos;
  char *pPos2;

  pStat = &aStats[iStatID];

  if (pStat->bActive == 0) {
    pStat->bActive = 1;
    pStat->iActiveID = iNumStatsActive++;

    pPos = strchr(sItem,'[');
    if (pPos) {
      pPos++;
      pPos2 = strchr(pPos,']');
      if (pPos2) {
        *(pPos2) = 0;
        SetString(&pStat->sStatParms,pPos);
      }
    }

    if ((pStat->bCustomField==0)&&(pStat->bCustomField==0)) {
      VerifyStatsParms(pStat->sStatParms);    
    }
  
    ParseItemList(&listColumns,pStat->sRequiredCols,ActivateColumn);
    ParseItemList(&listTriggers,pStat->sTriggers,ActivateTrigger);
  }
}

void ActivateTrigger(UINT32 iFxnID, const char *sItem) {
  
  UINT32 j;
  BOOL bAlready;
  enum EVENTPOINT eEventPoint;
  TRIGGER *pTrigger;

  NOREF(sItem);

  pTrigger = &aTriggers[iFxnID];

  if (pTrigger->bActive == TRUE) {
    return;
  }

  pTrigger->bActive = TRUE;

  if (pTrigger->bDisabled == TRUE) {
    ReportPrint(pRepErr,"Warning! Disabled trigger has been reactivated.  Please check trigger ordering\n");
    return;
  }

  ParseItemList(&listTriggers,pTrigger->sDependencyList,ActivateTrigger);
  ParseItemList(&listTriggers,pTrigger->sDeactivateList,DeactivateTrigger);

  eEventPoint = pTrigger->eEventPoint;

  bAlready = 0;
  for (j=0;j<aNumActiveProcedures[eEventPoint];j++) {
    if (aActiveProcedures[eEventPoint][j]==pTrigger->pProcedure) {
      bAlready = 1;
      break;
    }
  }

  if (bAlready==0) {
    aActiveProcedures[eEventPoint][aNumActiveProcedures[eEventPoint]] = pTrigger->pProcedure;
    aNumActiveProcedures[eEventPoint]++;
    if (aNumActiveProcedures[eEventPoint] == MAXFXNLIST) {
      ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXFXNLIST [%u]\n",MAXFXNLIST);
      AbnormalExit();
    }
  }

}

void AddColumnComposite(const char *sID, 
                        const char *sList)
{
  AddContainerItem(&listColumns,sID,sList);
}

void AddColumnFloat(const char *sID, 
                  const char *sDescription, 
                  char *sHeader1,  
                  char *sHeader2,  
                  char *sHeader3, 
                  char *sPrintFormat,
                  FLOAT *pCurValue,
                  char *sTriggers,
                  enum COLTYPE eColType
                  )
{

  REPORTCOL *pCol;
  
  pCol = &aColumns[listColumns.iNumItems];

  pCol->bActive = 0;

  SetString(&pCol->sDescription,sDescription);
  SetString(&pCol->sHeader1,sHeader1);
  SetString(&pCol->sHeader2,sHeader2);
  SetString(&pCol->sHeader3,sHeader3);
  SetString(&pCol->sPrintFormat,sPrintFormat);
  
  pCol->pfCurValue = pCurValue;

  pCol->eDataType = DTypeFloat;

  SetString(&pCol->sTriggers,sTriggers);

  pCol->pfColumnData = 0;

  pCol->eColType = eColType;

  AddItem(&listColumns,sID);

}

void AddColumnUInt(const char *sID, 
                  const char *sDescription, 
                  char *sHeader1,  
                  char *sHeader2,  
                  char *sHeader3, 
                  char *sPrintFormat, 
                  UINT32 *pCurValue,
                  char *sTriggers,
                  enum COLTYPE eColType
                  )
{

  REPORTCOL *pCol;
  
  pCol = &aColumns[listColumns.iNumItems];

  pCol->bActive = 0;

  SetString(&pCol->sDescription,sDescription);
  SetString(&pCol->sHeader1,sHeader1);
  SetString(&pCol->sHeader2,sHeader2);
  SetString(&pCol->sHeader3,sHeader3);
  SetString(&pCol->sPrintFormat,sPrintFormat);
  pCol->puiCurValue = pCurValue;

  pCol->eDataType = DTypeUInt;

  SetString(&pCol->sTriggers,sTriggers);
  
  pCol->puiColumnData = 0;
  pCol->pfColumnData = 0;

  pCol->eColType = eColType;

  AddItem(&listColumns,sID);
}

void AddContainerItem(ITEMLIST *pList,const char *sID, const char *sList) {
  SetString(&pList->aItems[pList->iNumItems].sID,sID);
  pList->aItems[pList->iNumItems].bContainer = 1;
  SetString(&pList->aItems[pList->iNumItems].sContainerList,sList);
  pList->iNumItems++;
  if (pList->iNumItems==MAXITEMLIST) {
    ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXITEMLIST [%u] \n",MAXITEMLIST);
    AbnormalExit();
  }
}

void AddItem(ITEMLIST *pList,const char *sID) {
  SetString(&pList->aItems[pList->iNumItems].sID,sID);
  pList->aItems[pList->iNumItems].bContainer = 0;
  pList->iNumItems++;
  if (pList->iNumItems==MAXITEMLIST) {
    ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXITEMLIST [%u] \n",MAXITEMLIST);
    AbnormalExit();
  }
}

void AddOutputColumn(UINT32 j, const char *sItem) {
  NOREF(sItem);
  aOutputColumns[iNumOutputColumns++] = j;
}

void AddParmBool(ALGPARMLIST *pParmList, 
                  const char *sSwitch, 
                  const char *sName, 
                  const char *sDescription,
                  const char *sTriggers,
                  UINT32 *pBool,
                  BOOL bDefBool)
{
  ALGPARM *p;
  p = AddParmCommon(pParmList,sSwitch,sName,sDescription,sTriggers);
  p->pParmValue = (void *) pBool;
  p->defDefault.bBool = bDefBool;
  p->eType = PTypeBool;
}

void AddParmFloat(ALGPARMLIST *pParmList, 
                  const char *sSwitch, 
                  const char *sName, 
                  const char *sDescription,
                  const char *sTriggers,
                  FLOAT *pFloat,
                  FLOAT fDefFloat)
{
  ALGPARM *p;
  p = AddParmCommon(pParmList,sSwitch,sName,sDescription,sTriggers);
  p->pParmValue = (void *) pFloat;
  p->defDefault.fFloat = fDefFloat;
  p->eType = PTypeFloat;
}

ALGPARM *AddParmCommon(ALGPARMLIST *pParmList,
                  const char *sSwitch, 
                  const char *sName, 
                  const char *sDescription,
                  const char *sTriggers)
{
  ALGPARM *p;
  
  if (pParmList->iNumParms >= MAXALGPARMS) {
    ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXALGPARMS [%u]\n",MAXALGPARMS);
    AbnormalExit();
  }

  p = &pParmList->aParms[pParmList->iNumParms++];

  SetString(&p->sSwitch,sSwitch);
  SetString(&p->sName,sName);
  SetString(&p->sDescription,sDescription);
  SetString(&p->sTriggers,sTriggers);
  p->bSpecified = 0;
  return(p);
}

void AddParmUInt(ALGPARMLIST *pParmList, 
                  const char *sSwitch, 
                  const char *sName, 
                  const char *sDescription,
                  const char *sTriggers,
                  UINT32 *pInt,
                  UINT32 iDefInt)
{
  ALGPARM *p;
  p = AddParmCommon(pParmList,sSwitch,sName,sDescription,sTriggers);
  p->pParmValue = (void *) pInt;
  p->defDefault.iUInt = iDefInt;
  p->eType = PTypeUInt;
}

void AddParmProbability(ALGPARMLIST *pParmList,
                  const char *sSwitch, 
                  const char *sName, 
                  const char *sDescription,
                  const char *sTriggers,
                  PROBABILITY *pProb,
                  FLOAT fProb)
{
  ALGPARM *p;
  p = AddParmCommon(pParmList,sSwitch,sName,sDescription,sTriggers);
  p->pParmValue = (void *) pProb;
  p->defDefault.iProb = FloatToProb(fProb);
  p->eType = PTypeProbability;
}

void AddParmReport(ALGPARMLIST *pParmList,
                  const char *sSwitch, 
                  const char *sName, 
                  const char *sDescription,
                  const char *sTriggers
                  )
{
  ALGPARM *p;
  p = AddParmCommon(pParmList,sSwitch,sName,sDescription,sTriggers);
  p->eType = PTypeReport;
}

void AddParmSInt(ALGPARMLIST *pParmList, 
                  const char *sSwitch, 
                  const char *sName, 
                  const char *sDescription,
                  const char *sTriggers,
                  SINT32 *pSInt,
                  SINT32 iDefSInt)
{
  ALGPARM *p;
  p = AddParmCommon(pParmList,sSwitch,sName,sDescription,sTriggers);
  p->pParmValue = (void *) pSInt;
  p->defDefault.iSInt = iDefSInt;
  p->eType = PTypeSInt;
}

void AddParmString(ALGPARMLIST *pParmList, 
                  const char *sSwitch, 
                  const char *sName, 
                  const char *sDescription,
                  const char *sTriggers,
                  char **pString,
                  char *sDefString)
{
  ALGPARM *p;
  p = AddParmCommon(pParmList,sSwitch,sName,sDescription,sTriggers);
  p->pParmValue = (void *) pString;
  SetString(&p->defDefault.sString,sDefString);
  p->eType = PTypeString;
}

void AddReportParmCommon(REPORT *pRep, const char *sParmName) {
  SetString(&pRep->aParmName[pRep->iNumParms],sParmName);
  pRep->iNumParms++;
  if (pRep->iNumParms==MAXREPORTPARMS) {
    ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXREPORTPARMS [%u]\n",MAXREPORTPARMS);
    AbnormalExit();
  }
}

void AddReportParmFloat(REPORT *pRep, const char *sParmName, FLOAT *pDefault) {
  pRep->aParmTypes[pRep->iNumParms] = PTypeFloat;
  pRep->aParameters[pRep->iNumParms] = (void *) pDefault;
  AddReportParmCommon(pRep,sParmName);
}

void AddReportParmString(REPORT *pRep, const char *sParmName, const char *pDefault) {
  pRep->aParmTypes[pRep->iNumParms] = PTypeString;
  SetString((char **) &pRep->aParameters[pRep->iNumParms],pDefault);
  AddReportParmCommon(pRep,sParmName);
}

void AddReportParmUInt(REPORT *pRep, const char *sParmName, UINT32 *pDefault) {
  pRep->aParmTypes[pRep->iNumParms] = PTypeUInt;
  pRep->aParameters[pRep->iNumParms] = (void *) pDefault;
  AddReportParmCommon(pRep,sParmName);
}

void AddRTDColumn(UINT32 j, const char *sItem) {
  NOREF(sItem);
  aRTDColumns[iNumRTDColumns++] = j;
}

void AddStat(const char *sID, 
                  const char *sDescription, 
                  const char *sBaseDescription, 
                  const char *sDefParm,
                  const char *sRequiredCols,
                  const char *sDataColumn,
                  const char *sTriggers,
                  BOOL bSortByStep)
{

  REPORTSTAT *pStat;

  pStat = &aStats[listStats.iNumItems];

  pStat->bActive = 0;

  SetString(&pStat->sDescription,sDescription);
  
  pStat->bCustomField = 0;

  SetString(&pStat->sBaseDescription,sBaseDescription);
  
  SetString(&pStat->sRequiredCols,sRequiredCols);
  SetString(&pStat->sDataColumn,sDataColumn);
  SetString(&pStat->sStatParms,sDefParm);

  SetString(&pStat->sTriggers,sTriggers);

  pStat->bSortByStep = bSortByStep;

  AddItem(&listStats,sID);
}

void AddContainerStat(const char *sID, 
                      const char *sList) {

  AddContainerItem(&listStats,sID,sList);
}

void AddStatCustom(const char *sID, 
                  const char *sDescription, 
                  const char *sPrintCustomFormat,
                  void *pCurValue,
                  enum CDATATYPE eCustomType,
                  const char *sRequiredCols,
                  const char *sTriggers
                )
{

  REPORTSTAT *pStat;

  pStat = &aStats[listStats.iNumItems];

  pStat->bActive = 0;

  SetString(&pStat->sDescription,sDescription);
  
  pStat->bCustomField = 1;

  SetString(&pStat->sPrintCustomFormat,sPrintCustomFormat);
  
  pStat->pCustomValue = pCurValue;
  pStat->eCustomType = eCustomType;

  SetString(&pStat->sRequiredCols,sRequiredCols);

  SetString(&pStat->sTriggers,sTriggers);
  
  AddItem(&listStats,sID);

}

void CheckInvalidParamters() {

  UINT32 j;

  for (j=0;j<iNumTotalParms;j++) {
    if (!bValidArgument[j]) {
      HelpBadParm(aTotalParms[j]);
    }
  }
}

void CheckParamterFile(int iCommandLineCount,char **aCommandLineArgs) {
  int j;
  char *sParameterFilename = 0;
  FILE *filParm;
  char *pStart;
  char *pPos;

  UINT32 iNumParmFiles = 0;

  for (j=0;j<(iCommandLineCount - 1);j++) {
    if (MatchParameter("-param|-fp",aCommandLineArgs[j])) {
      iNumParmFiles++;
    }
  }

  if (iNumParmFiles==0) {
    iNumTotalParms = iCommandLineCount - 1;
    if (iNumTotalParms==MAXTOTALPARMS) {
      ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXTOTALPARMS [%u]\n",MAXTOTALPARMS);
      AbnormalExit();
    }
    aTotalParms = &aCommandLineArgs[1];
    return;
  }

  aTotalParms = AllocateRAM(MAXTOTALPARMS * sizeof(char *));

  for (j=0;j<(iCommandLineCount - 1);j++) {
    if (MatchParameter("-param|-fp",aCommandLineArgs[j])) {
      
      sParameterFilename = aCommandLineArgs[j+1];

      SetupFile(&filParm,"r",sParameterFilename,stdin,0);

      while (!feof(filParm)) {
        if (fgets(sParmLine,MAXPARMLINELEN,filParm)) {
          if (strlen(sParmLine)==MAXPARMLINELEN-1) {
            ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXPARMLINELEN [%u]\n",MAXPARMLINELEN);
            AbnormalExit();
          }
          if ((*sParmLine)&&(*sParmLine != '#')) {
            pStart = sParmLine;
            pPos = strchr(pStart,' ');
            while (pPos) {
              if (pPos==pStart) {
                pStart++;
              } else {
                *pPos++=0;
                SetString(&aTotalParms[iNumTotalParms++],pStart);
                if (iNumTotalParms==MAXTOTALPARMS) {
                  ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXTOTALPARMS [%u]\n",MAXTOTALPARMS);
                  AbnormalExit();
                }
                pStart = pPos;
              }
              pPos = strchr(pStart,' ');
            }
            pPos = strchr(pStart,10);
            if (pPos) 
              *pPos = 0;
            pPos = strchr(pStart,13);
            if (pPos) 
              *pPos = 0;
            if (strlen(pStart)) {
              SetString(&aTotalParms[iNumTotalParms++],pStart);
              if (iNumTotalParms==MAXTOTALPARMS) {
                ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXTOTALPARMS [%u]\n",MAXTOTALPARMS);
                AbnormalExit();
              }
            }
          }
        }
      }

      CloseSingleFile(filParm);

    }
  }

  for (j=1;j<(iCommandLineCount);j++) {
    SetString(&aTotalParms[iNumTotalParms++],aCommandLineArgs[j]);;
    if (iNumTotalParms==MAXTOTALPARMS) {
      ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXTOTALPARMS [%u]\n",MAXTOTALPARMS);
      AbnormalExit();
    }
  }

}

void ClearActiveProcedures() {
  memset(aNumActiveProcedures,0,sizeof(UINT32) * NUMEVENTPOINTS);
}

void CopyParameters(ALGORITHM *pDest, const char *sName, const char *sVar, BOOL bWeighted) {
  UINT32 j;
  ALGPARMLIST *pParmList;
  ALGORITHM *pSrc = FindAlgorithm(sName,sVar,bWeighted);
  if (pSrc==0) {
    ReportPrint1(pRepErr,"Unexpected Error: Can't find algorithm %s\n",(char *) sName);
    AbnormalExit();
  }
  pParmList = &pSrc->parmList;
  pDest->parmList.iNumParms = pParmList->iNumParms;
  for (j=0;j<pParmList->iNumParms;j++) {
    pDest->parmList.aParms[j] = pParmList->aParms[j];
  }
}

ALGORITHM *CreateAlgorithm (const char *sName, const char *sVariant, BOOL bWeighted, 
                            const char *sDescription, 
                            const char *sAuthors,
                            const char *sHeuristicTriggers,
                            const char *sDataTriggers,
                            const char *sDefaultOutput,
                            const char *sDefaultStats) {
  
  ALGORITHM *pCurAlg;

  pCurAlg = &aAlgorithms[iNumAlg];

  SetString(&pCurAlg->sName,sName);
  SetString(&pCurAlg->sVariant,sVariant);
  pCurAlg->bWeighted = bWeighted;

  SetString(&pCurAlg->sDescription,sDescription);
  SetString(&pCurAlg->sAuthors,sAuthors);

  SetString(&pCurAlg->sHeuristicTriggers,sHeuristicTriggers);
  SetString(&pCurAlg->sDataTriggers,sDataTriggers);

  SetString(&pCurAlg->sDefaultOutput,sDefaultOutput);
  SetString(&pCurAlg->sDefaultStats,sDefaultStats);

  pCurAlg->parmList.iNumParms = 0;

  iNumAlg++;

  if (iNumAlg==MAXNUMALG) {
    ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXNUMALG [%u]\n",MAXNUMALG);
    AbnormalExit();
  }

  return(pCurAlg);
}


void CreateContainerTrigger(const char *sID, const char *sList) {
  AddContainerItem(&listTriggers,sID,sList);
}

REPORT *CreateReport(const char *sID, const char *sDescription, const char *sOutputFile, const char *sTriggers) {
  REPORT *pRep;

  pRep = &aReports[iNumReports++];

  if (iNumReports==MAXREPORTS) {
    ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXREPORTS [%u] \n",MAXREPORTS);
    AbnormalExit();
  }


  SetString(&pRep->sID,sID);
  pRep->bActive = 0;
  pRep->fileOut = stdout;
  SetString(&pRep->sOutputFile,sOutputFile);
  pRep->iNumParms = 0;
  SetString(&pRep->sTriggers,sTriggers);
  SetString(&pRep->sDescription,sDescription);

  return (pRep);
}

void CreateTrigger(const char *sID, enum EVENTPOINT eEventPoint, FXNPTR pProcedure, char *sDependencyList, char *sDeactivateList) {
  TRIGGER *pNewTrig;

  pNewTrig = &aTriggers[listTriggers.iNumItems];

  pNewTrig->eEventPoint = eEventPoint;
  pNewTrig->pProcedure = pProcedure;
  SetString(&pNewTrig->sDependencyList,sDependencyList);
  SetString(&pNewTrig->sDeactivateList,sDeactivateList);

  AddItem(&listTriggers,sID);
}

void DeactivateTrigger(UINT32 iFxnID, const char *sItem) {
  
  UINT32 j;
  UINT32 k;
  enum EVENTPOINT eEventPoint;
  TRIGGER *pTrigger;

  NOREF(sItem);

  pTrigger = &aTriggers[iFxnID];

  pTrigger->bDisabled = TRUE;

  if (pTrigger->bActive == TRUE) {

    eEventPoint = pTrigger->eEventPoint;

    for (j=0;j<aNumActiveProcedures[eEventPoint];j++) {
      if (aActiveProcedures[eEventPoint][j]==pTrigger->pProcedure) {
        for (k=j;k<aNumActiveProcedures[eEventPoint]-1;k++) {
          aActiveProcedures[eEventPoint][k] = aActiveProcedures[eEventPoint][k+1];
        }
        aNumActiveProcedures[eEventPoint]--;
        break;
      }
    }
  }
}

ALGORITHM *FindAlgorithm(const char *sFindName, const char *sFindVar, BOOL bFindWeighted) {
  UINT32 j;
  if (*sFindName != 0) {
    for (j=0;j<iNumAlg;j++) {
      if (strcmp(sFindName,aAlgorithms[j].sName)==0) {
        if (strcmp(sFindVar,aAlgorithms[j].sVariant)==0) {
          if (aAlgorithms[j].bWeighted == bFindWeighted) {
            return(&aAlgorithms[j]);
          }
        }
      }
    }
  }
  return(0);
}

UINT32 FindItem(ITEMLIST *pList,char *sID) {
  SINT32 j;
  char *pPos;
  char *pPos2;
  UINT32 iLen;

  pPos = strchr(sID,'[');
  if (pPos) {
    pPos2 = strchr(sID,']');
    if (pPos2) {
      iLen = pPos - sID;
      for (j=0;j<(SINT32)pList->iNumItems;j++) {
        if (strlen(pList->aItems[j].sID) == iLen) {
          if (strncmp(sID,pList->aItems[j].sID,iLen)==0) {
            return(j);
          }
        }
      }
    } else {
      ReportPrint1(pRepErr,"Error: unbalanced [] in (%s)\n",sID);
    }
  } else {
    for (j=0;j<(SINT32)pList->iNumItems;j++) {
        if (strcmp(sID,pList->aItems[j].sID)==0) {
          return(j);
        }
    }
  }
  ReportPrint1(pRepErr,"Error: reference to (%s) is unknown\n",sID);
  AbnormalExit();
  return(0);
}

ALGPARM *FindParm(ALGPARMLIST *pParmList, char *sSwitch) {

  UINT32 j;

  for (j=0;j<pParmList->iNumParms;j++) {
    if (MatchParameter(sSwitch,pParmList->aParms[j].sSwitch))
      return(&pParmList->aParms[j]);
  }
  return(0);
}

ALGORITHM *GetAlgorithm() {
  UINT32 j;
  if (*sAlgName) {
    for (j=0;j<iNumAlg;j++) {
      if (strcmp(sAlgName,aAlgorithms[j].sName)==0) {
        if (strcmp(sVarName,aAlgorithms[j].sVariant)==0) {
          if (aAlgorithms[j].bWeighted == bWeighted) {
            return(&aAlgorithms[j]);
          }
        }
      }
    }
  }
  return(0);
}

void InheritDataTriggers(ALGORITHM *pDest, const char *sName, const char *sVar, BOOL bWeighted) {
  ALGORITHM *pSrc = FindAlgorithm(sName,sVar,bWeighted);
  if (pSrc==0) {
    ReportPrint1(pRepErr,"Unexpected Error: Can't find algorithm %s\n",(char *) sName);
    AbnormalExit();
  }
  if (pDest->sDataTriggers) {
    *sMasterString = 0;
    strcat(sMasterString,pDest->sDataTriggers);
    strcat(sMasterString,",");
    strcat(sMasterString,pSrc->sDataTriggers);
    SetString(&pDest->sDataTriggers,sMasterString);
  } else {
    SetString(&pDest->sDataTriggers,pSrc->sDataTriggers);
  }
}

BOOL IsLocalMinimum() {
  UINT32 j,k;
  UINT32 iNumOcc;
  UINT32 *pClause;
  LITTYPE litCur;
  SINT32 iScore;

  for (j=1;j<=iNumVars;j++) {

    iScore = 0;
    
    litCur = GetFalseLit(j);

    iNumOcc = aNumLitOcc[litCur];
    pClause = pLitClause[litCur];
  
    for (k=0;k<iNumOcc;k++) {
      if (aNumTrueLit[*pClause++]==0) {
        iScore--;
      }
    }

    iNumOcc = aNumLitOcc[GetNegatedLit(litCur)];
    pClause = pLitClause[GetNegatedLit(litCur)];
  
    for (k=0;k<iNumOcc;k++) {
      if (aNumTrueLit[*pClause++]==1) {
        iScore++;
      }
    }

    if (iScore < 0)
      return(FALSE);
  }
  return(TRUE);
}

UINT32 MatchParameter(char *sSwitch,char *sParm) {
  
  char *pPos;
  char *pEndParm;
  
  pPos = strstr(sSwitch,sParm);
  while (pPos) {
    pEndParm = pPos + strlen(sParm);

    if ((*pEndParm==0)||(*pEndParm=='|')) {
      if (pPos==sSwitch) {
        return(1);
      } else {
        if (*(pPos-1)=='|')
          return(1);
      }
    }
    
    pPos = strstr(pPos + 1,sParm);
  }
  return(0);
}

void ParseAllParameters(int argc, char *argv[]) {

  CheckParamterFile(argc,argv);

  ParseParameters(&parmHelp);
  ParseParameters(&parmAlg);
  
  pActiveAlgorithm = GetAlgorithm();

  CheckPrintHelp();
  
  SetAlgorithmDefaultReports();

  ParseParameters(&parmUBCSAT);
  ParseParameters(&parmIO);
  ParseParameters(&(pActiveAlgorithm->parmList));

  CheckInvalidParamters();
}

void ParseItemList(ITEMLIST *pList, char *sItems, CALLBACKPTR ItemFunction) {

  char *pPos;
  SINT32 j;

  if (*sItems==0)
    return;

  pPos = strchr(sItems,',');
  if (pPos) {
    if (strlen(sItems) > MAXITEMLISTSTRINGLENGTH-1) {
      ReportPrint1(pRepErr,"Unexpected Error: increase constant MAXITEMLISTSTRINGLENGTH [%u] \n",MAXITEMLISTSTRINGLENGTH);
      AbnormalExit();
    }
    strcpy(sMasterString,sItems);
    sMasterString[pPos-sItems] = 0;
    pPos++;
    ParseItemList(pList,sMasterString,ItemFunction);
    ParseItemList(pList,pPos,ItemFunction);
    return;
  }
  j = FindItem(pList,sItems);
  if (pList->aItems[j].bContainer) {
    ParseItemList(pList,pList->aItems[j].sContainerList,ItemFunction);
  } else {
    ItemFunction((UINT32) j,sItems);
  }
}

void ParseParameters(ALGPARMLIST *pParmList) {

  UINT32 j;

  UINT32 iCurParm = 0;
  SINT32 iCurReport;
  REPORT *pRep;
  UINT32 iNumRepParms;
  ALGPARM *pParm;

  
  float fTemp;
  float fTemp2 = 100.0;
  BOOL bRatioParm;

  SetDefaultParms(pParmList);

  while (iCurParm < iNumTotalParms) {
    pParm = 0;
    for (j=0;j<pParmList->iNumParms;j++) {
      
      if (MatchParameter(pParmList->aParms[j].sSwitch,aTotalParms[iCurParm])) {
        pParm = &pParmList->aParms[j];
        bValidArgument[iCurParm++] = 1;
        break;
      }
    }

    if (pParm) {
      switch (pParm->eType)
      {
        case PTypeUInt:
          if (iCurParm == iNumTotalParms) {
            HelpBadParm(aTotalParms[iCurParm-1]);
          }
          if (sscanf(aTotalParms[iCurParm],"%u",pParm->pParmValue)==0) {
            HelpBadParm(aTotalParms[iCurParm-1]);
          }
          bValidArgument[iCurParm++] = 1;
          break;
        case PTypeSInt:
          if (iCurParm == iNumTotalParms) {
            HelpBadParm(aTotalParms[iCurParm-1]);
          }
          if (sscanf(aTotalParms[iCurParm],"%d",pParm->pParmValue)==0) {
            HelpBadParm(aTotalParms[iCurParm-1]);
          }
          bValidArgument[iCurParm++] = 1;
          break;
        case PTypeBool:
          *((UINT32 *)pParm->pParmValue) = 1;
          if (iCurParm < iNumTotalParms) {
            if (sscanf(aTotalParms[iCurParm],"%i",pParm->pParmValue)) {
              bValidArgument[iCurParm++] = 1;
            }
          }
          break;

        case PTypeProbability:
          if (iCurParm == iNumTotalParms) {
            HelpBadParm(aTotalParms[iCurParm-1]);
          }
          
          if (sscanf(aTotalParms[iCurParm],"%f",&fTemp)==0) {
            HelpBadParm(aTotalParms[iCurParm-1]);
          }

          bRatioParm = 0;
          bValidArgument[iCurParm++] = 1;
          if (iCurParm < iNumTotalParms) {
            if (sscanf(aTotalParms[iCurParm],"%f",&fTemp2)) {
              bRatioParm = 1;
              bValidArgument[iCurParm++] = 1;
              if (fTemp2 == 0.000000) {
                ReportPrint(pRepErr,"\n\nProbability invalid: The 2nd parameter to a Probability can not be 0\n\n");
                HelpBadParm(aTotalParms[iCurParm-3]);
                AbnormalExit();
              }
            }
          }
          
          if (bRatioParm == 0) {
            if (strcmp("1",aTotalParms[iCurParm-1])==0) {
              ReportPrint(pRepErr,"\n\nProbability ambiguous: 1/100 or 1.0? specify 0.01 or 1 100 or 1.0\n\n");
              HelpBadParm(aTotalParms[iCurParm-2]);
              AbnormalExit();
            }
            if (fTemp > 1.0) {
              fTemp /= 100;
              if (fTemp > 1.0) {
                ReportPrint(pRepErr,"\n\nProbability invalid: specify range 0.0-1.0 or 0-100\n\n");
                HelpBadParm(aTotalParms[iCurParm-2]);
                AbnormalExit();
              }
            }
          } else {
            fTemp = fTemp / fTemp2;
            if (fTemp > 1.0) {
              ReportPrint(pRepErr,"\n\nProbability ratio invalid: numerator > denominator \n\n");
              HelpBadParm(aTotalParms[iCurParm-3]);
              AbnormalExit();
            }
          }

          *((PROBABILITY *)pParm->pParmValue) = FloatToProb (fTemp);

          break;

        case PTypeString:
          if (iCurParm == iNumTotalParms) {
            HelpBadParm(aTotalParms[iCurParm-1]);
          }
          SetString((char **) pParm->pParmValue,aTotalParms[iCurParm]);
          bValidArgument[iCurParm++] = 1;
          break;

        case PTypeFloat:
          if (iCurParm == iNumTotalParms) {
            HelpBadParm(aTotalParms[iCurParm-1]);
          }
          if (sscanf(aTotalParms[iCurParm],"%f",&fTemp)==0) {
            HelpBadParm(aTotalParms[iCurParm-1]);
          }
          *((FLOAT *)pParm->pParmValue) = (FLOAT) fTemp;
          bValidArgument[iCurParm++] = 1;
          break;

        case PTypeReport:
          if (iCurParm == iNumTotalParms) {
            HelpBadParm(aTotalParms[iCurParm-1]);
          }
          iCurReport = -1;          
          for (j=0;j<iNumReports;j++) {
            if (strcmp(aTotalParms[iCurParm],aReports[j].sID)==0) {
              iCurReport = j;
              break;
            }
          }
          if (iCurReport==-1) {
             HelpBadParm(aTotalParms[iCurParm-1]);
          }
          pRep = &aReports[iCurReport];
          
          pRep->bActive = 1;
          bValidArgument[iCurParm++] = 1;

          if (iCurParm < iNumTotalParms) {
            if (*aTotalParms[iCurParm] != '-') {
              SetString(&pRep->sOutputFile,aTotalParms[iCurParm]);
              bValidArgument[iCurParm++] = 1;
            }
            iNumRepParms = 0;
            while (iNumRepParms < pRep->iNumParms) {
              if (iCurParm < iNumTotalParms) {
                if (*aTotalParms[iCurParm] != '-') {
                  switch (pRep->aParmTypes[iNumRepParms])
                  {
                    case PTypeUInt:
                      if (sscanf(aTotalParms[iCurParm],"%i",pRep->aParameters[iNumRepParms])==0) {
                        HelpBadParm(aTotalParms[iCurParm]);
                      }
                      bValidArgument[iCurParm++] = 1;
                      break;
                    case PTypeFloat:
                      if (sscanf(aTotalParms[iCurParm],"%f",&fTemp)==0) {
                        HelpBadParm(aTotalParms[iCurParm]);
                      }
                      *((FLOAT *)pRep->aParameters[iNumRepParms]) = (FLOAT) fTemp;
                      bValidArgument[iCurParm++] = 1;
                      break;
                    case PTypeString:
                      SetString((char **) &pRep->aParameters[iNumRepParms],aTotalParms[iCurParm]);
                      bValidArgument[iCurParm++] = 1;
                      break;
                  }
                }
              }
              iNumRepParms++;
            }
          }
          break;      
      }
      
      pParm->bSpecified = 1;

      ParseItemList(&listTriggers,pParm->sTriggers,ActivateTrigger);
      
    } else {
      iCurParm++;
    }
  }
}

void PrintAlgParmSettings(REPORT *pRep, ALGPARMLIST *pParmList) {
  
  UINT32 j;
  ALGPARM *pCurParm;

  for (j=0;j<pParmList->iNumParms;j++) {
    pCurParm = &pParmList->aParms[j];
    ReportHdrPrefix(pRep);
    ReportHdrPrint1(pRep," %s ",pCurParm->sSwitch);
    switch(pCurParm->eType)
    {
      case PTypeUInt:
        ReportHdrPrint1(pRep,"%u ", *(UINT32 *)pCurParm->pParmValue);
        break;
      case PTypeSInt:
        ReportHdrPrint1(pRep,"%d ", *(int *)pCurParm->pParmValue);
        break;
      case PTypeProbability:
        ReportHdrPrint1(pRep,"%f ", ProbToFloat(*(PROBABILITY *)pCurParm->pParmValue));
        break;
      case PTypeString:
        if (**(char **)pCurParm->pParmValue ==0) {
          ReportHdrPrint1(pRep,"%s ","[null]");
        } else {
          ReportHdrPrint1(pRep,"%s ", *(char **)pCurParm->pParmValue);
        }
        break;
      case PTypeFloat:
        ReportHdrPrint1(pRep,"%f ", *(FLOAT *)pCurParm->pParmValue);
        break;
      case PTypeBool:
        ReportHdrPrint1(pRep,"%u ", *(UINT32 *)pCurParm->pParmValue);
        break;
      case PTypeReport:
        break;
    }  
    ReportHdrPrint(pRep,"\n");
  }
}

void PrintFullStat(REPORTSTAT *pStat, const char *sStatID, char *sPrintID, FLOAT fValue) {
  if ((strstr(pStat->sStatParms,sStatID))||(strstr(pStat->sStatParms,"all"))) { 
    ReportPrint3(pRepStats,"%s_%s = %f\n",pStat->sBaseDescription,sPrintID,fValue);
  }
}

void SetAlgorithmDefaultReports() {
  SetString((char **) &pRepOut->aParameters[0],pActiveAlgorithm->sDefaultOutput);
  SetString((char **) &pRepStats->aParameters[0],pActiveAlgorithm->sDefaultStats);
}

void SetDefaultParms(ALGPARMLIST *pParmList) {
  UINT32 j;
  ALGPARM *pParm;

  for (j=0;j<pParmList->iNumParms;j++) {
    pParm = &pParmList->aParms[j];

    switch (pParm->eType)
    {
      case PTypeUInt:
        *((UINT32 *)pParm->pParmValue) = pParm->defDefault.iUInt;
        break;
      case PTypeSInt:
        *((SINT32 *)pParm->pParmValue) = pParm->defDefault.iSInt;
	      break;
      case PTypeBool:
        *((UINT32 *)pParm->pParmValue) = pParm->defDefault.bBool;
        break;
      case PTypeProbability:
        *((PROBABILITY *)pParm->pParmValue) = pParm->defDefault.iProb;
        break;
      case PTypeString:
        SetString((char **) pParm->pParmValue,pParm->defDefault.sString);
        break;
      case PTypeFloat:
        *((FLOAT *)pParm->pParmValue) = pParm->defDefault.fFloat;
        break;
      case PTypeReport:
        break;
    }
  }
}


void SetupUBCSAT() {

  pRepErr = CreateReport("err","Error Report -- All errors that are encountered (Defaults to stderr)","stderr","");
  pRepErr->bActive = 1;
  pRepErr->fileOut = stderr;

  ClearActiveProcedures();

}
void VerifyStatsParms (char *sStatsParms) {

  UINT32 j;
  char *pPos;
  char *pPos2;
  UINT32 iLen;
  UINT32 bValid;
  
  pPos = sStatsParms;

  while (*pPos != 0) {
    bValid = 0;
    pPos2 = strstr(pPos,"+");

    if (pPos2) {
      iLen = pPos2 - pPos;
    } else {
      iLen = strlen(pPos);
    }
    
    if (iLen) {
      for (j=0;j<NUMVALIDSTATCODES;j++) {
        if (strlen(sValidStatCodes[j])==iLen) {
          if (strncmp(pPos,sValidStatCodes[j],iLen)==0) {
            bValid = 1;  
          }
        }
      }
      if (bValid==0) {
        ReportPrint1(pRepErr,"Error: reference to (%s) is unknown\n",pPos);        
        AbnormalExit();
      }
    }

    pPos += iLen;
    if (pPos2)
      pPos++;
  }
}


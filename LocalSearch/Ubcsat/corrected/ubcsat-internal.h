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


#define NOFXN 0
#define NOREF(A) (A)=(A)

#define RunProcedures(A) {if (aNumActiveProcedures[A]) {for (iRunProceduresLoop=0;iRunProceduresLoop<aNumActiveProcedures[A];iRunProceduresLoop++) aActiveProcedures[A][iRunProceduresLoop]();} }
#define RunProcedures2(A) {if (aNumActiveProcedures[A]) {for (iRunProceduresLoop2=0;iRunProceduresLoop2<aNumActiveProcedures[A];iRunProceduresLoop2++) aActiveProcedures[A][iRunProceduresLoop2]();} }
/* Use RunProcedures2 when calling from outside of the main loop in ubcsat.c */

#define NUMVALIDSTATCODES 17
extern char *sValidStatCodes[];

extern ALGORITHM aAlgorithms[];

extern ALGPARMLIST parmAlg;
extern ALGPARMLIST parmHelp;
extern ALGPARMLIST parmIO;
extern ALGPARMLIST parmUBCSAT;

extern BOOL bShowHelp;
extern BOOL bShowHelpA;
extern BOOL bShowHelpV;
extern BOOL bShowHelpT;
extern BOOL bShowHelpR;
extern BOOL bShowHelpC;
extern BOOL bShowHelpS;

extern char sVersion[];
extern char *sCommentString;
extern FXNPTR aActiveProcedures[][MAXFXNLIST];

extern BOOL bReportOutputSupress;

extern BOOL bReportStateLMOnly;
extern FLOAT fReportStateQuality;
extern BOOL bReportStateQuality;
extern UINT32 iReportStateQuality;

extern FLOAT fDummy;
extern FLOAT fFlipsPerSecond;
extern FLOAT fPercentSuccess;

extern ITEMLIST listColumns;
extern ITEMLIST listStats;
extern ITEMLIST listTriggers;

extern REPORT aReports[];
extern REPORTCOL aColumns[];
extern REPORTSTAT aStats[];
extern TRIGGER aTriggers[];

extern UINT32 aNumActiveProcedures[];

extern UINT32 aActiveCalcColumns[];
extern UINT32 aOutputColumns[];
extern UINT32 aRTDColumns[];

extern UINT32 iNumActiveCalcColumns;
extern UINT32 iNumAlg;
extern UINT32 iNumOutputColumns;
extern UINT32 iNumReports;
extern UINT32 iNumRTDColumns;
extern UINT32 iNumStatsActive;
extern UINT32 iRunProceduresLoop;
extern UINT32 iRunProceduresLoop2;


void ActivateAlgorithmTriggers();
void ActivateColumn(UINT32 iColID, const char *sItem);
void ActivateStat(UINT32 iStatID, const char *sItem);
void ActivateTrigger(UINT32 iFxnID, const char *sItem);

void AddOutputColumn(UINT32 j, const char *sItem);
void AddParameters();
void AddParmReport(ALGPARMLIST *pParmList,const char *sSwitch,const char *sName,const char *sDescription,const char *sTriggers);
void AddReportTriggers();
void AddRTDColumn(UINT32 j, const char *sItem);
void CheckPrintHelp();
UINT32 FindItem(ITEMLIST *pList,char *sID);
void HelpBadParm();
void ParseAllParameters(int argc, char *argv[]);
void PrintAlgParmSettings(REPORT *pRep, ALGPARMLIST *pParmList);
void PrintFullStat(REPORTSTAT *pStat, const char *sStatID, char *sPrintID, FLOAT fValue);
void ParseItemList(ITEMLIST *pList, char *sItems, CALLBACKPTR ItemFunction);
void PrintUBCSATHeader(REPORT *pRep);
void SetupUBCSAT();


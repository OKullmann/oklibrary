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
  This file contains routines for displaying help 
*/

BOOL bShowHelp;
BOOL bShowHelpA;
BOOL bShowHelpV;
BOOL bShowHelpT;
BOOL bShowHelpR;
BOOL bShowHelpC;
BOOL bShowHelpS;

void HelpNoAlgorithm();
void HelpBadReport();
void HelpPrintAlgorithms();
void HelpShow();
void HelpShowVerbose();
void HelpShowTerse();
void HelpPrintReports();
void HelpPrintColumns();
void HelpPrintStats();

void CheckPrintHelp() {
  if (bShowHelpA) HelpPrintAlgorithms();
  if (bShowHelpV) HelpShowVerbose();
  if (bShowHelpT) HelpShowTerse();
  if (bShowHelpR) HelpPrintReports();
  if (bShowHelpC) HelpPrintColumns();
  if (bShowHelpS) HelpPrintStats();
  if (bShowHelp)  HelpShow();
  if (!pActiveAlgorithm) HelpNoAlgorithm();
}

void PrintUBCSATHeader(REPORT *pRep) {
  ReportHdrPrefix(pRep);
  ReportHdrPrint(pRep,"\n");
  ReportHdrPrefix(pRep);
  ReportHdrPrint1(pRep,"UBCSAT version %s\n",sVersion);
  ReportHdrPrefix(pRep);
  ReportHdrPrint(pRep,"\n");
  ReportHdrPrefix(pRep);
  ReportHdrPrint(pRep,"\n");
  ReportHdrPrefix(pRep);
  ReportHdrPrint(pRep,"ubcsat -h for help\n");
  ReportHdrPrefix(pRep);
  ReportHdrPrint(pRep,"\n");
}

void HelpPrintAlgorithms() {
  UINT32 j;

  PrintUBCSATHeader(pRepOut);
  ReportPrint(pRepOut,"\nSupported algorithms:\n\n");

  for (j=0;j<iNumAlg;j++) {
    ReportPrint1(pRepOut," -alg %s",aAlgorithms[j].sName);
    if (*aAlgorithms[j].sVariant) {
      ReportPrint1(pRepOut," -v %s",aAlgorithms[j].sVariant);
    }
    if (aAlgorithms[j].bWeighted) {
      ReportPrint(pRepOut," -w");
    }
    ReportPrint(pRepOut,"\n");
    ReportPrint1(pRepOut,"    %s\n",aAlgorithms[j].sDescription);
    ReportPrint1(pRepOut,"    %s\n\n",aAlgorithms[j].sAuthors);
  }
  AbnormalExit();
}

void HelpPrintReports() {
  UINT32 j,k;

  PrintUBCSATHeader(pRepOut);
  ReportPrint(pRepOut,"\nPrinting Reports in UBCSAT:\n");
  ReportPrint(pRepOut,"--------------------------\n\n");

  ReportPrint(pRepOut,"To specify a report use the following syntax:\n");
  ReportPrint(pRepOut,"  -r reportname [filename [paramater(s)]] \n\n");
  
  ReportPrint(pRepOut,"If you do not specify a filename or parameter(s), defaults will be used.\n\n");
  ReportPrint(pRepOut,"For filename, you may alternatively specify the following special keywords:\n");
  ReportPrint(pRepOut,"  stdout (default), stderr, or null\n\n");

  ReportPrint(pRepOut,"REPORTS:  [default parameters in square brackets]\n\n");
  
  for (j=0;j<iNumReports;j++) {
    ReportPrint1(pRepOut, "  -r %s\n",aReports[j].sID);
    ReportPrint1(pRepOut,"    %s\n",aReports[j].sDescription);
  
    if (aReports[j].iNumParms) {

      for (k=0;k<aReports[j].iNumParms;k++) {
        ReportPrint2(pRepOut,"      Parameter %2d: %s",k+1,aReports[j].aParmName[k]);
        switch (aReports[j].aParmTypes[k]) {
          {
          case PTypeUInt:
            ReportPrint1(pRepOut," [%u] \n",*(int *)aReports[j].aParameters[k]);
            break;
          case PTypeFloat:
            ReportPrint1(pRepOut," [%f] \n",*(FLOAT *)aReports[j].aParameters[k]);
            break;
          case PTypeString:
            ReportPrint1(pRepOut," [%s] \n",(char *)aReports[j].aParameters[k]);          
            break;
          }
        }
      }
    }
    ReportPrint(pRepOut,"\n");
  }

  AbnormalExit();
}

void HelpPrintColumns() {
  UINT32 j;

  PrintUBCSATHeader(pRepOut);
  ReportPrint(pRepOut,"\nPrinting Reports with Columns in UBCSAT:\n");
  ReportPrint(pRepOut,  "---------------------------------------\n\n");

  ReportPrint(pRepOut,"For Output & RTD reports, you may customize the report by specifying\n");
  ReportPrint(pRepOut,"which columns you wish to display.  For example:\n\n");
  ReportPrint(pRepOut,"  -r out stdout found,beststep\n\n");
  ReportPrint(pRepOut,"displays only the output columns found & beststep to the console (stdout) \n");
  ReportPrint(pRepOut,"if you wish to write the output to a file, specify a filename instead of stdout\n");

  ReportPrint(pRepOut,"\n\nOutput (-r out) & RTD (-r rtd) Columns available:\n\n");

  for (j=0;j<listColumns.iNumItems;j++) {
    if (!listColumns.aItems[j].bContainer) {
      ReportPrint1(pRepOut,"%15s -  ",listColumns.aItems[j].sID);
      ReportPrint1(pRepOut,"%s\n",aColumns[j].sDescription);
    }
  }

  ReportPrint(pRepOut,"\nContainer Columns available:\n\n");

  for (j=0;j<listColumns.iNumItems;j++) {
    if (listColumns.aItems[j].bContainer) {
      ReportPrint1(pRepOut,"%15s -  ",listColumns.aItems[j].sID);
      ReportPrint1(pRepOut,"%s\n",listColumns.aItems[j].sContainerList);
    }
  }


  AbnormalExit();
}

void HelpPrintStats() {
  UINT32 j;

  PrintUBCSATHeader(pRepOut);
  ReportPrint(pRepOut,"\nPrinting Statistics Reports in UBCSAT:\n");
  ReportPrint(pRepOut,  "-------------------------------------\n\n");

  ReportPrint(pRepOut,"For statistics reports, you may customize the report by specifying\n");
  ReportPrint(pRepOut,"which statistics you wish to display.  For example:\n\n");
  ReportPrint(pRepOut,"  -r stats mystats.stats percentsolve,steps[mean+median+max],numlits\n\n");
  ReportPrint(pRepOut,"writes to file mystats.stats the following lines:\n\n");
  
  ReportPrint(pRepOut,"PercentSuccess = x.x\n");
  ReportPrint(pRepOut,"Steps_Mean = x.x\n");
  ReportPrint(pRepOut,"Steps_Median = x.x\n");
  ReportPrint(pRepOut,"Steps_Max = x.x\n");
  ReportPrint(pRepOut,"TotalLiterals = x\n\n");

  ReportPrint(pRepOut,"For full statistics, you can specify which fields to print from:\n");
  ReportPrint(pRepOut,"  [");
  for (j=0;j<NUMVALIDSTATCODES;j++) {
    if (j>0)
      ReportPrint(pRepOut,"+");
    ReportPrint1(pRepOut,"%s",sValidStatCodes[j]);
  }
  ReportPrint(pRepOut,"]\n\n");

  ReportPrint(pRepOut,"Full Statistics available: [default fields shown in square brackets]\n\n");
  for (j=0;j<listStats.iNumItems;j++) {
    if (!listStats.aItems[j].bContainer) {
      if (!aStats[j].bCustomField) {
        ReportPrint3(pRepOut,"%15s -  %s  [%s]\n",listStats.aItems[j].sID,aStats[j].sDescription,aStats[j].sStatParms);
      }
    }
  }

  ReportPrint(pRepOut,"\n\nSingle Statistics available:\n\n");

  for (j=0;j<listStats.iNumItems;j++) {
    if (!listStats.aItems[j].bContainer) {
      if (aStats[j].bCustomField) {
        ReportPrint2(pRepOut,"%15s -  %s\n",listStats.aItems[j].sID,aStats[j].sDescription);
      }
    }
  }


  ReportPrint(pRepOut,"\nContainer Statistics available:\n\n");

  for (j=0;j<listStats.iNumItems;j++) {
    if (listStats.aItems[j].bContainer) {
      ReportPrint2(pRepOut,"%15s -  %s\n",listStats.aItems[j].sID,listStats.aItems[j].sContainerList);
    }
  }


  AbnormalExit();
}

void HelpNoAlgorithm() {

  PrintUBCSATHeader(pRepErr);
  ReportPrint(pRepErr,"\n");
  ReportPrint(pRepErr,"FATAL ERROR ENCOUNTERED: \n");

  if (*sAlgName) {
    ReportPrint(pRepErr,"   Invalid algorithm specified\n");
  } else {
    ReportPrint(pRepErr,"   UBCSAT requires you to specify an algorithm with -alg\n");
  }
  ReportPrint(pRepErr,"   ubcsat -ha for a list of valid algorithms\n");
  AbnormalExit();
}


void HelpPrintParameters(ALGPARMLIST *pParmList) {
  
  UINT32 j;
  ALGPARM *pCurParm;

  if (pParmList->iNumParms==0) {
    ReportPrint(pRepOut,"  No Paramaters\n\n");
  } else {
    for (j=0;j<pParmList->iNumParms;j++) {
      pCurParm = &pParmList->aParms[j];
      ReportPrint1(pRepOut,"  %s: \n",pCurParm->sName);
      ReportPrint1(pRepOut,"    %s\n",pCurParm->sDescription);
      ReportPrint1(pRepOut,"    %s ",pCurParm->sSwitch);
      switch(pCurParm->eType)
      {
        case PTypeUInt:
          if (strcmp(pCurParm->sName,"seed")==0) {
            ReportPrint1(pRepOut,"N (default based on system time) ",pCurParm->defDefault.iUInt);
          } else {
            ReportPrint1(pRepOut,"N (default %u) ",pCurParm->defDefault.iUInt);
          }
          break;
        case PTypeSInt:
          ReportPrint1(pRepOut,"N (default %d) ",pCurParm->defDefault.iSInt);
          break;
        case PTypeProbability:
          ReportPrint1(pRepOut,"0.nn (default %3.2f)",ProbToFloat(pCurParm->defDefault.iProb));
          break;
        case PTypeString:
          ReportPrint(pRepOut,"name ");
          break;
        case PTypeFloat:
          ReportPrint1(pRepOut,"n.nn (default %f) ",pCurParm->defDefault.fFloat);
          break;
        case PTypeReport:
          ReportPrint(pRepOut,"reportname [filename [parameters]]");
          break;
      }  
      ReportPrint(pRepOut,"\n");
    }
    ReportPrint(pRepOut,"\n\n");
  }
}

void HelpPrintParametersTerse(ALGPARMLIST *pParmList) {
  
  UINT32 j;
  ALGPARM *pCurParm;

  ReportPrint(pRepOut,"  ");
  for (j=0;j<pParmList->iNumParms;j++) {
    pCurParm = &pParmList->aParms[j];
    ReportPrint1(pRepOut,"%s ",pCurParm->sSwitch);
  }
  ReportPrint(pRepOut,"\n");
}



void HelpPrintAlgParameters(ALGORITHM *pCurAlg) {

  if (pCurAlg) {
    ReportPrint1(pRepOut,"[-alg %s",pCurAlg->sName);
    if (*pCurAlg->sVariant != 0)
      ReportPrint1(pRepOut," -v %s",pCurAlg->sVariant);
    if (pCurAlg->bWeighted)
      ReportPrint(pRepOut," -w");
    ReportPrint(pRepOut,"]\n");

    ReportPrint1(pRepOut,"%s\n",pCurAlg->sDescription);
    ReportPrint1(pRepOut,"%s\n\n",pCurAlg->sAuthors);
    HelpPrintParameters(&pCurAlg->parmList);
  }
}


void HelpPrintAlgParametersTerse(ALGORITHM *pCurAlg) {

  if (strcmp(pCurAlg->sName,"default")!=0) {
    ReportPrint1(pRepOut,"[-alg %s",pCurAlg->sName);
    if (*pCurAlg->sVariant != 0)
      ReportPrint1(pRepOut," -v %s",pCurAlg->sVariant);
    if (pCurAlg->bWeighted)
      ReportPrint(pRepOut," -w");
    ReportPrint(pRepOut,"]\n");
  }
  HelpPrintParametersTerse(&pCurAlg->parmList);
}

void HelpPrintSpecialParameters() {

  ReportPrint(pRepOut,"Help Parameters:\n");
  HelpPrintParameters(&parmHelp);

  ReportPrint(pRepOut,"Algorithm Specification Parameters:\n");
  HelpPrintParameters(&parmAlg);
  
  ReportPrint(pRepOut,"UBCSAT Reporting & File I/O Parameters:\n");
  HelpPrintParameters(&parmIO);  

  ReportPrint(pRepOut,"UBCSAT Parameters:\n");
  HelpPrintParameters(&parmUBCSAT);

}

void HelpPrintSpecialParametersTerse() {

  ReportPrint(pRepOut,"Help Parameters:\n");
  HelpPrintParametersTerse(&parmHelp);

  ReportPrint(pRepOut,"Algorithm Specification Parameters:\n");
  HelpPrintParametersTerse(&parmAlg);
  
  ReportPrint(pRepOut,"UBCSAT Parameters:\n");
  HelpPrintParametersTerse(&parmUBCSAT);

  ReportPrint(pRepOut,"UBCSAT Reporting & File I/O Parameters:\n");
  HelpPrintParametersTerse(&parmIO);

}


void HelpShow() {

  HelpPrintSpecialParameters();
  HelpPrintAlgParameters(pActiveAlgorithm);

  AbnormalExit();
}

void HelpShowVerbose() {
  UINT32 j;
  HelpPrintSpecialParameters();
  for (j=0;j<iNumAlg;j++)
    HelpPrintAlgParameters(&aAlgorithms[j]);
  AbnormalExit();
}

void HelpShowTerse() {
  UINT32 j;
  HelpPrintSpecialParametersTerse();
  for (j=0;j<iNumAlg;j++)
    HelpPrintAlgParametersTerse(&aAlgorithms[j]);
  AbnormalExit();
}

void HelpBadParm(char *sParm) {
  PrintUBCSATHeader(pRepErr);
  ReportPrint(pRepErr,"\n");
  ReportPrint(pRepErr,"FATAL ERROR ENCOUNTERED: \n");
  ReportPrint1(pRepErr,"   Parameter [%s] is unrecognized or invalid\n",sParm);
  ReportPrint(pRepErr,"\n");
  AbnormalExit();
}


void HelpBadReport(char *sParm) {
  PrintUBCSATHeader(pRepErr);
  ReportPrint(pRepErr,"\n");
  ReportPrint(pRepErr,"FATAL ERROR ENCOUNTERED: \n");
  ReportPrint1(pRepErr,"   Report Parameter [%s] is unrecognized\n",sParm);
  ReportPrint(pRepErr,"   ubcsat -hr for help on report parameters  \n");
  ReportPrint(pRepErr,"\n");
  AbnormalExit();
}






